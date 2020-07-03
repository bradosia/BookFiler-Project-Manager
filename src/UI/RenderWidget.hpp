/*
 * @name BookFiler Recognize
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Text recognition viewer, editor, and analyzer.

 * Icons and images owned by their respective owners
 */

#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

// C++
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

/* QT 5.13.2-1
 * License: LGPLv3
 */
#include <QClipboard>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QWidget>

// Local Project
#include "BookfilerRenderWidget.hpp"

/* RenderWidget
 * Wrapper for graphic widgets.
 * Only renders when input is captured to save energy.
 */
class RenderWidget : public QOpenGLWidget {
  Q_OBJECT

public:
  RenderWidget(QWidget *parent = nullptr);
  ~RenderWidget();
  // override events
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

  // bundle events
  void keyEvent(QKeyEvent *event);
  void mouseEvent(QMouseEvent *event);

  bool updateFn();

  std::function<bool(std::shared_ptr<bookfiler::WidgetData>)> renderFunction;
  std::function<bool(std::shared_ptr<bookfiler::WidgetData>)>
      initGraphicsFunction;
  std::shared_ptr<bookfiler::WidgetData> widgetData;
  std::function<bool()> updateSlot;

protected:
  void paintEvent(QPaintEvent *event) override;
  void initializeGL() override;

private:
  std::chrono::steady_clock::time_point renderStart, renderStop;
  std::unordered_map<int, int> keyMap;
  QSurface *surface;
  bool initGraphics = false;
};
#endif // RENDER_WIDGET_H
