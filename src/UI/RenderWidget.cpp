/*
 * @name BookFiler Recognize
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Text recognition viewer, editor, and analyzer.

 * Icons and images owned by their respective owners
 */

// Local Project
#include "RenderWidget.hpp"

RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent) {
  renderStart = std::chrono::steady_clock::now();
  widgetData = std::make_shared<bookfiler::WidgetData>();
  // clipboard callbacks
  widgetData->setClipboardTextFn = [](void *user_data, const char *text) {
    Q_UNUSED(user_data);
    QGuiApplication::clipboard()->setText(text);
  };
  widgetData->getClipboardTextFn = [](void *user_data) {
    Q_UNUSED(user_data);
    return (const char *)QGuiApplication::clipboard()->text().toUtf8().data();
  };
  // Key Map
  keyMap = {{Qt::Key_Tab, BOOKFILER_IO_VK_TAB},
            {Qt::Key_Left, BOOKFILER_IO_VK_LEFT},
            {Qt::Key_Right, BOOKFILER_IO_VK_RIGHT},
            {Qt::Key_Up, BOOKFILER_IO_VK_UP},
            {Qt::Key_Down, BOOKFILER_IO_VK_DOWN},
            {Qt::Key_PageUp, BOOKFILER_IO_VK_PRIOR},
            {Qt::Key_PageDown, BOOKFILER_IO_VK_NEXT},
            {Qt::Key_Home, BOOKFILER_IO_VK_HOME},
            {Qt::Key_End, BOOKFILER_IO_VK_END},
            {Qt::Key_Delete, BOOKFILER_IO_VK_DELETE},
            {Qt::Key_Backspace, BOOKFILER_IO_VK_BACK},
            {Qt::Key_Enter, BOOKFILER_IO_VK_RETURN},
            {Qt::Key_Escape, BOOKFILER_IO_VK_ESCAPE},
            {Qt::Key_A, 'A'},
            {Qt::Key_C, 'C'},
            {Qt::Key_V, 'V'},
            {Qt::Key_X, 'X'},
            {Qt::Key_Y, 'Y'},
            {Qt::Key_Z, 'Z'},
            {Qt::Key_Z, BOOKFILER_IO_VK_SPACE}};
  // Native Window
  // WId = quintptr
  widgetData->winId = (void *)winId();
  // signals and slots
  updateSlot = std::bind(&RenderWidget::updateFn, this);
}

RenderWidget::~RenderWidget() {}

void RenderWidget::initializeGL() {
  initGraphics = initGraphicsFunction(widgetData);
  surface = context()->surface();
}

void RenderWidget::keyPressEvent(QKeyEvent *event) { keyEvent(event); }
void RenderWidget::keyReleaseEvent(QKeyEvent *event) { keyEvent(event); }
void RenderWidget::keyEvent(QKeyEvent *event) {
  auto keyFound = keyMap.find(event->key());
  if (keyFound != keyMap.end()) {
    widgetData->keysDown[keyMap[event->key()]] =
        event->type() == QEvent::KeyPress;
  }

  if (event->type() == QEvent::KeyPress) {
    QString text = event->text();
    if (text.size() == 1) {
      widgetData->addInputCharacter = text.at(0).unicode();
    }
  }

#ifdef Q_OS_MAC
  widgetData->keyCtrl = event->modifiers() & Qt::MetaModifier;
  widgetData->keyShift = event->modifiers() & Qt::ShiftModifier;
  widgetData->keyAlt = event->modifiers() & Qt::AltModifier;
  widgetData->keySuper =
      event->modifiers() & Qt::ControlModifier; // Command key
#else
  widgetData->keyCtrl = event->modifiers() & Qt::ControlModifier;
  widgetData->keyShift = event->modifiers() & Qt::ShiftModifier;
  widgetData->keyAlt = event->modifiers() & Qt::AltModifier;
  widgetData->keySuper = event->modifiers() & Qt::MetaModifier;
#endif

  std::cout << "key" << std::endl;
  update();
}

void RenderWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  mouseEvent(event);
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event) {
  std::cout << "move" << std::endl;
  mouseEvent(event);
  update();
}

void RenderWidget::mousePressEvent(QMouseEvent *event) { mouseEvent(event); }

void RenderWidget::mouseReleaseEvent(QMouseEvent *event) { mouseEvent(event); }

void RenderWidget::mouseEvent(QMouseEvent *event) {
  std::cout << "mouse" << std::endl;
  widgetData->mouseDown[0] = event->buttons() & Qt::LeftButton;
  widgetData->mouseDown[1] = event->buttons() & Qt::RightButton;
  widgetData->mouseDown[2] = event->buttons() & Qt::MiddleButton;
  setFocus();
  update();
}

void RenderWidget::wheelEvent(QWheelEvent *event) {
  widgetData->mouseWheelX = event->angleDelta().x();
  widgetData->mouseWheelY = event->angleDelta().y();
  update();
}

void RenderWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  std::cout << "render " << std::endl;
  if (!initGraphics) {
    return;
  }
  widgetData->windowFocused = isActiveWindow();
  widgetData->displaySizeX = size().width();
  widgetData->displaySizeY = size().height();
  widgetData->displayFramebufferScale = devicePixelRatio();
  // Translates the global screen coordinate pos to widget coordinates.
  auto pos = mapFromGlobal(QCursor::pos());
  widgetData->mousePosX = pos.x();
  widgetData->mousePosY = pos.y();
  // in milliseconds
  std::chrono::duration<double, std::milli> processDuration =
      renderStop - renderStart;
  processDuration = std::chrono::duration<double, std::milli>(
      std::ceil(processDuration.count()));
  widgetData->deltaTime = processDuration.count();
  // render
  context()->makeCurrent(surface);
  renderStart = std::chrono::steady_clock::now();
  renderFunction(widgetData);
  renderStop = std::chrono::steady_clock::now();
  context()->swapBuffers(surface);
  context()->doneCurrent();
}

bool RenderWidget::updateFn() {
  update();
  return true;
}
