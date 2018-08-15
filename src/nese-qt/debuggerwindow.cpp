#include "debuggerwindow.h"
#include "debuggermodels.h"
#include "nese/cpu_debug_interface.h"
#include "ui_debuggerwindow.h"
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace QtFrontend {

DebuggerWindow::DebuggerWindow(CPUDebugInterface* debugger_interface, QWidget* parent /* = nullptr */)
  : QMainWindow(parent), m_debugger_interface(debugger_interface)
{
  m_ui = std::make_unique<Ui::DebuggerWindow>();
  m_ui->setupUi(this);
  connectSignals();
  createModels();
  onExecutionStopped();
}

DebuggerWindow::~DebuggerWindow() {}

void DebuggerWindow::onExecutionContinued()
{
  setMonitorUIState(false);
}

void DebuggerWindow::onExecutionStopped()
{
  setMonitorUIState(true);
  refreshAll();
}

void DebuggerWindow::refreshAll()
{
  m_registers_model->invalidateView();
  m_stack_model->invalidateView();
  int row = m_code_model->updateInstructionPointer();
  if (row >= 0)
    m_ui->codeView->scrollTo(m_code_model->index(row, 0));
}

void DebuggerWindow::onCloseActionTriggered() {}

void DebuggerWindow::onRunActionTriggered(bool checked)
{
#if 0
  if (m_debugger_interface->IsStepping() == !checked)
    return;

  if (checked)
  {
    m_debugger_interface->SetStepping(false);
    onExecutionContinued();
  }
  else
  {
    m_debugger_interface->SetStepping(true);
    onExecutionStopped();
  }
#endif
}

void DebuggerWindow::onSingleStepActionTriggered()
{
  // m_debugger_interface->SetStepping(true, 1);
}

void DebuggerWindow::connectSignals()
{
  connect(m_ui->actionPause_Continue, SIGNAL(triggered(bool)), this, SLOT(onRunActionTriggered(bool)));
  connect(m_ui->actionStep_Into, SIGNAL(triggered()), this, SLOT(onSingleStepActionTriggered()));
  connect(m_ui->action_Close, SIGNAL(triggered()), this, SLOT(onCloseActionTriggered()));
}

void DebuggerWindow::createModels()
{
  m_code_model = std::make_unique<DebuggerCodeModel>(m_debugger_interface);
  m_ui->codeView->setModel(m_code_model.get());

  m_registers_model = std::make_unique<DebuggerRegistersModel>(m_debugger_interface);
  m_ui->registerView->setModel(m_registers_model.get());
  // m_ui->registerView->resizeRowsToContents();

  m_stack_model = std::make_unique<DebuggerStackModel>(m_debugger_interface);
  m_ui->stackView->setModel(m_stack_model.get());
}

void DebuggerWindow::setMonitorUIState(bool enabled)
{
  // Disable all UI elements that depend on execution state
  m_ui->actionPause_Continue->setChecked(!enabled);
  m_ui->codeView->setDisabled(!enabled);
  m_ui->registerView->setDisabled(!enabled);
  m_ui->stackView->setDisabled(!enabled);
  // m_ui->tabMemoryView
}

} // namespace QtFrontend