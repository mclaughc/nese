#include "debuggermodels.h"
#include "YBaseLib/Assert.h"
#include <QtGui/QColor>

namespace QtFrontend {

const int MAX_CODE_ROWS = 1024;
const CPUDebugInterface::ProgramCounterType MAX_CODE_DISTANCE = 4096;
const int NUM_COLUMNS = 3;

DebuggerCodeModel::DebuggerCodeModel(CPUDebugInterface* intf, QObject* parent /*= nullptr*/)
  : QAbstractTableModel(parent), m_interface(intf)

{
  resetCodeView(0);
}

DebuggerCodeModel::~DebuggerCodeModel() {}

int DebuggerCodeModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
  // TODO: Don't make this fixed..
  return MAX_CODE_ROWS;
}

int DebuggerCodeModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
  return NUM_COLUMNS;
}

QVariant DebuggerCodeModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
  uint32 reg_index = static_cast<uint32>(index.row());
  if (index.column() < 0 || index.column() >= NUM_COLUMNS)
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    CPUDebugInterface::ProgramCounterType address;
    if (!getAddressForRow(&address, index.row()))
      return "<invalid>";

    switch (index.column())
    {
      case 0:
      {
        // Address
        return QVariant(QString::asprintf("0x%08X", address));
      }

      case 1:
      {
#if 0
        // Bytes
        uint32 instruction_length;
        if (!m_interface->DisassembleCode(address, nullptr, &instruction_length))
          return "<invalid>";

        SmallString value;
        for (uint32 i = 0; i < instruction_length; i++)
        {
          uint8 byte_value;
          if (!m_interface->ReadMemoryByte(address + i, &byte_value))
            break;
          value.AppendFormattedString("%s%02X", (i == 0) ? "" : " ", ZeroExtend32(byte_value));
        }
        return value.GetCharArray();
#else
        return "<invalid>";
#endif
      }

      case 2:
      {
#if 0
        // Instruction
        String instruction;
        if (!m_interface->DisassembleCode(address, &instruction, nullptr))
          return "<invalid>";

        return instruction.GetCharArray();
#else
        return "<invalid>";
#endif
      }

      default:
        return QVariant();
    }
  }
  else if (role == Qt::BackgroundColorRole)
  {
    int row = index.row();
    CPUDebugInterface::ProgramCounterType address;
    if (!getAddressForRow(&address, row))
      return QVariant();

    // breakpoint
    // return QVariant(QColor(171, 97, 107));
    if (address == m_last_instruction_pointer)
      return QVariant(QColor(255, 241, 129));
    else
      return QVariant();
  }
  else
  {
    return QVariant();
  }
}

QVariant DebuggerCodeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
  if (orientation != Qt::Horizontal)
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  static const char* header_names[] = {"Address", "Bytes", "Instruction"};
  if (section < 0 || section >= countof(header_names))
    return QVariant();

  return header_names[section];
}

void DebuggerCodeModel::resetCodeView(CPUDebugInterface::ProgramCounterType start_address)
{
  beginResetModel();
  m_start_instruction_pointer = start_address;
  m_row_ips.clear();
  m_row_ips[0] = start_address;
  endResetModel();
}

int DebuggerCodeModel::getRowForAddress(CPUDebugInterface::ProgramCounterType address)
{
  // We can move forward, but not backwards
  if (address < m_start_instruction_pointer)
  {
    resetCodeView(address);
    return 0;
  }

  // Find row with matching address
  int row = 0;
  auto iter = m_row_ips.begin();
  for (; iter != m_row_ips.end(); iter++)
  {
    // Do we already have this row cached?
    if (iter->second == address)
      return iter->first;

    // Keep moving forward while we are ahead of the current cached instruction
    if (address > iter->second)
    {
      row = iter->first;
      continue;
    }

    // If this row is behind the current instruction, use the last row instead
    break;
  }

  // Add row entries until we hit the instruction, or go past it
  iter = m_row_ips.find(row);
  DebugAssert(iter != m_row_ips.end());
  CPUDebugInterface::ProgramCounterType current_address = iter->second;
  while (current_address < address)
  {
    // Make sure we don't try to create too many rows
    if (row > MAX_CODE_ROWS)
    {
      resetCodeView(address);
      return 0;
    }

    // Get the instruction length
    uint32 instruction_size = 1;
#if 0
    if (!m_interface->DisassembleCode(current_address, nullptr, &instruction_size))
    {
      resetCodeView(address);
      return 0;
    }
#endif

    row++;
    current_address += instruction_size;
    m_row_ips[row] = current_address;
    if (current_address == address)
    {
      // Found the row
      return row;
    }
  }

  // Somehow we skipped over the address. This can happen when we jump to the middle of an instruction.
  resetCodeView(address);
  return 0;
}

bool DebuggerCodeModel::getAddressForRow(CPUDebugInterface::ProgramCounterType* address, int row) const
{
  Assert(row >= 0);
  auto iter = m_row_ips.find(row);
  if (iter != m_row_ips.end())
  {
    *address = iter->second;
    return true;
  }

  // iter = m_row_ips.lower_bound(row);
  iter = m_row_ips.begin();
  while (iter != m_row_ips.end() && iter->first < row)
  {
    auto prev_iter = iter++;
    if (iter == m_row_ips.end())
    {
      iter = prev_iter;
      break;
    }
  }
  if (iter == m_row_ips.end())
    return false;

  // Work forward until we match the row
  int last_row = iter->first;
  CPUDebugInterface::ProgramCounterType last_address = iter->second;
  while (last_row < row)
  {
    // Get the instruction length
    uint32 instruction_size = 1;
#if 0
    if (!m_interface->DisassembleCode(last_address, nullptr, &instruction_size))
      return false;
#endif

    last_address += static_cast<CPUDebugInterface::ProgramCounterType>(instruction_size);
    last_row++;
    m_row_ips[last_row] = last_address;
  }

  *address = last_address;
  return true;
}

int DebuggerCodeModel::updateInstructionPointer()
{
  CPUDebugInterface::ProgramCounterType ip = 0; // m_interface->GetInstructionPointer();
  int new_row = getRowForAddress(ip);
  if (m_last_instruction_pointer == ip)
    return new_row;

  m_last_instruction_pointer = ip;

  int old_row = getRowForAddress(m_last_instruction_pointer);
  if (old_row >= 0)
    emit dataChanged(index(old_row, 0), index(old_row, NUM_COLUMNS - 1));

  if (new_row >= 0)
    emit dataChanged(index(new_row, 0), index(new_row, NUM_COLUMNS - 1));

  return new_row;
}

DebuggerRegistersModel::DebuggerRegistersModel(CPUDebugInterface* intf, QObject* parent /*= nullptr*/)
  : QAbstractListModel(parent), m_interface(intf)
{
}

DebuggerRegistersModel::~DebuggerRegistersModel() {}

int DebuggerRegistersModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
  // return static_cast<int>(m_interface->GetRegisterCount());
  return 4;
}

int DebuggerRegistersModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
  return 2;
}

QVariant DebuggerRegistersModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
  uint32 reg_index = static_cast<uint32>(index.row());
  // if (reg_index > m_interface->GetRegisterCount())
  if (reg_index > 4)
    return QVariant();

  if (index.column() < 0 || index.column() > 1)
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  if (index.column() == 0)
  {
    // return m_interface->GetRegisterName(reg_index);
    return "<reg>";
  }
  else
  {
#if 0
    DebuggerInterface::RegisterType type = m_interface->GetRegisterType(reg_index);
    DebuggerInterface::RegisterValue value = m_interface->GetRegisterValue(reg_index);
    switch (type)
    {
      case DebuggerInterface::RegisterType::Byte:
        return QString::asprintf("0x%02X", ZeroExtend32(value.val_byte));
      case DebuggerInterface::RegisterType::Word:
        return QString::asprintf("0x%04X", ZeroExtend32(value.val_word));
      case DebuggerInterface::RegisterType::DWord:
        return QString::asprintf("0x%08X", value.val_dword);
      default:
        return QString();
    }
#endif
    return QString();
  }
}

QVariant DebuggerRegistersModel::headerData(int section, Qt::Orientation orientation,
                                            int role /*= Qt::DisplayRole*/) const
{
  if (orientation != Qt::Horizontal)
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  static const char* header_names[] = {"Register", "Value"};
  if (section < 0 || section >= countof(header_names))
    return QVariant();

  return header_names[section];
}

void DebuggerRegistersModel::invalidateView()
{
  beginResetModel();
  endResetModel();
}

DebuggerMemoryModel::DebuggerMemoryModel(CPUDebugInterface* intf, QObject* parent /*= nullptr*/)
  : QAbstractTableModel(parent), m_interface(intf)
{
}

DebuggerMemoryModel::~DebuggerMemoryModel() {}

int DebuggerMemoryModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
  return 0;
}

int DebuggerMemoryModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
  return 0;
}

QVariant DebuggerMemoryModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
  return {};
}

QVariant DebuggerMemoryModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
  return {};
}

void DebuggerMemoryModel::invalidateView()
{
  beginResetModel();
  endResetModel();
}

DebuggerStackModel::DebuggerStackModel(CPUDebugInterface* intf, QObject* parent /*= nullptr*/)
  : QAbstractListModel(parent), m_interface(intf)
{
}

DebuggerStackModel::~DebuggerStackModel() {}

int DebuggerStackModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
#if 0
  LinearMemoryAddress stack_bottom = m_interface->GetStackBottom();
  LinearMemoryAddress stack_top = m_interface->GetStackTop();
  if (stack_bottom < stack_top)
    return 0;

  DebuggerInterface::RegisterType width = m_interface->GetStackValueType();
  LinearMemoryAddress value_size = (width == DebuggerInterface::RegisterType::Word) ? 2 : 4;
  int num_rows = static_cast<int>((stack_bottom - stack_top) / value_size) + 1;
  return std::min(num_rows, 1024);
#else
  return 0;
#endif
}

int DebuggerStackModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
  return 2;
}

QVariant DebuggerStackModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
  if (index.column() < 0 || index.column() > 1)
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

#if 0
  DebuggerInterface::RegisterType width = m_interface->GetStackValueType();
  LinearMemoryAddress value_size = (width == DebuggerInterface::RegisterType::Word) ? 2 : 4;
  LinearMemoryAddress address = m_interface->GetStackTop() + static_cast<LinearMemoryAddress>(index.row()) * value_size;

  if (index.column() == 0)
    return QString::asprintf("0x%08X", address);

  if (width == DebuggerInterface::RegisterType::Word)
  {
    uint16 value;
    if (!m_interface->ReadMemoryWord(address, &value))
      return QVariant();
    return QString::asprintf("0x%04X", ZeroExtend32(value));
  }
  else
  {
    uint32 value;
    if (!m_interface->ReadMemoryDWord(address, &value))
      return QVariant();
    return QString::asprintf("0x%08X", ZeroExtend32(value));
  }
#endif
  return QString();
}

QVariant DebuggerStackModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
  if (orientation != Qt::Horizontal)
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  static const char* header_names[] = {"Address", "Value"};
  if (section < 0 || section >= countof(header_names))
    return QVariant();

  return header_names[section];
}

void DebuggerStackModel::invalidateView()
{
  beginResetModel();
  endResetModel();
}

} // namespace QtFrontend