#pragma once

#include "common/display.h"
#include <QtOpenGL/QGLWidget>
#include <memory>

namespace QtFrontend {
class DisplayWidget : public QGLWidget, public Display
{
  Q_OBJECT

public:
  DisplayWidget(QWidget* parent);
  ~DisplayWidget();

  void ResizeDisplay(uint32 width = 0, uint32 height = 0) override;
  void ResizeFramebuffer(uint32 width, uint32 height) override;
  void DisplayFramebuffer() override;

  virtual QSize sizeHint() const override;

  void moveGLContextToThread(QThread* thread);

Q_SIGNALS:
  void onKeyPressed(QKeyEvent* event);
  void onKeyReleased(QKeyEvent* event);

protected:
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void keyReleaseEvent(QKeyEvent* event) override;
  virtual void paintEvent(QPaintEvent* e) override;
  virtual void resizeEvent(QResizeEvent* e) override;
  virtual void initializeGL() override;
  virtual void resizeGL(int w, int h) override;
  virtual void paintGL() override;

private:
  GLuint m_framebuffer_texture = 0;
  std::unique_ptr<byte[]> m_framebuffer_texture_buffer;
};
} // namespace QtFrontend