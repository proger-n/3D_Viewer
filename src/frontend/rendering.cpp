#include "rendering.h"

#include <math.h>
#include <stdio.h>

Rendering::Rendering(QWidget *parent) : QOpenGLWidget(parent) {
  loadSettings();
}

Rendering::~Rendering() {
  saveSettings();
  remove_matrix(&V);
  remove_array(&F);
}

void Rendering::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();
}

void Rendering::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void Rendering::paintGL() {
  glClearColor(style.backRed, style.backGreen, style.backBlue, style.backAlfa);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  setProjection();
  glRotatef(currentXRot, 1, 0, 0);
  glRotatef(currentYRot, 0, 1, 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, V.matrix);
  drawLines();
  drawPoints();
  glDisableClientState(GL_VERTEX_ARRAY);
  update();
}

void Rendering::clearRender() {
  if (V.rows) remove_matrix(&V);
  if (F.count) remove_array(&F);
  update();
}

void Rendering::start_read_file(char *str) {
  read_file(str, &V, &F);
  initializeGL();
  update();
}

void Rendering::scale(double scl) {
  s21_scale(&V, scl);
  update();
}

void Rendering::rotate(double x, double y, double z) {
  rotate_x(&V, x);
  rotate_y(&V, y);
  rotate_z(&V, z);
}

void Rendering::mousePressEvent(QMouseEvent *mo) { mPos = mo->pos(); }

void Rendering::mouseMoveEvent(QMouseEvent *mo) {
  float deltaX = mo->x() - mPos.x();
  float deltaY = mo->y() - mPos.y();
  if (mo->buttons() & Qt::LeftButton) {
    currentXRot += 0.25f * deltaY;
    currentYRot += 0.25f * deltaX;
    update();
    mPos = mo->pos();
  } else if (mo->buttons() & Qt::RightButton) {
    XRot = 0.25f * deltaY;
    YRot = 0.25f * deltaX;
    move(XRot, YRot, 0);
    mPos = mo->pos();
    update();
  }
}

void Rendering::wheelEvent(QWheelEvent *event) {
  float step = event->angleDelta().y() / 100;
  float scaleF = 1.0 + (step * 0.1);
  scale(scaleF);
  update();
}

void Rendering::saveSettings() {
  QSettings settings("Setting", "3D_Viewer");

  settings.setValue("coordinateX", currentXRot);
  settings.setValue("coordinateY", currentYRot);
  settings.setValue("backAlfa", style.backAlfa);
  settings.setValue("backBlue", style.backBlue);
  settings.setValue("backGreen", style.backGreen);
  settings.setValue("backRed", style.backRed);
  settings.setValue("edgedAlfa", style.edgeAlfa);
  settings.setValue("edgedBlue", style.edgeBlue);
  settings.setValue("edgedGreen", style.edgeGreen);
  settings.setValue("edgedRed", style.edgeRed);
  settings.setValue("vertexBlue", style.vertexBlue);
  settings.setValue("vertexGreen", style.vertexGreen);
  settings.setValue("vertexRed", style.vertexRed);
  settings.setValue("vertexAlfa", style.vertexAlfa);
  settings.setValue("vertexWidth", style.vertexWidth);
  settings.setValue("lineWidth", style.lineWidth);
  settings.setValue("projectionType", style.projectionType);
  settings.setValue("vertexType", style.vertexType);
  settings.setValue("edgeType", style.edgeType);
  settings.sync();
}

void Rendering::loadSettings() {
  QSettings settings("Setting", "3D_Viewer");

  currentXRot = settings.value("coordinateX", 0.0f).toFloat();
  currentYRot = settings.value("coordinateY", 0.0f).toFloat();
  style.backAlfa = settings.value("backAlfa", 1.0f).toFloat();
  style.backBlue = settings.value("backBlue", 0.451f).toFloat();
  style.backGreen = settings.value("backGreen", 0.451f).toFloat();
  style.backRed = settings.value("backRed", 0.451f).toFloat();
  style.edgeAlfa = settings.value("edgedAlfa", 0.0f).toFloat();
  style.edgeBlue = settings.value("edgedBlue", 0.0f).toFloat();
  style.edgeGreen = settings.value("edgedGreen", 0.0f).toFloat();
  style.edgeRed = settings.value("edgedRed", 0.0f).toFloat();
  style.vertexBlue = settings.value("vertexBlue", 0.0f).toFloat();
  style.vertexGreen = settings.value("vertexGreen", 0.0f).toFloat();
  style.vertexRed = settings.value("vertexRed", 0.0f).toFloat();
  style.vertexAlfa = settings.value("vertexAlfa", 0.0f).toDouble();
  style.vertexWidth = settings.value("vertexWidth", 0.0f).toDouble();
  style.lineWidth = settings.value("lineWidth", 0.0f).toFloat();
  style.projectionType = settings.value("projectionType", 0.0f).toInt();
  style.vertexType = settings.value("vertexType", 0.0f).toInt();
  style.edgeType = settings.value("edgeType", 0.0f).toInt();
}

void Rendering::setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double drawRange = V.max_vertex * 2;
  if (!style.projectionType) {
    glOrtho(-drawRange, drawRange, -drawRange, drawRange, -drawRange,
            drawRange * 5);
  } else {
    float fov = 60.0 * M_PI / 180;
    float heapHeight = drawRange / (2 * tan(fov / 2));
    glFrustum(-drawRange, drawRange, -drawRange, drawRange, heapHeight,
              drawRange * 5);
    glTranslated(0, 0, -heapHeight * 3);
  }
}

void Rendering::drawLines() {
  glColor4f(style.edgeRed, style.edgeGreen, style.edgeBlue, style.edgeAlfa);
  glEnable(GL_LINE_STIPPLE);
  if (style.edgeType) {
    glLineStipple(1, 0x00ff);
  } else {
    glLineStipple(0, 0xffff);
  }
  glLineWidth(style.lineWidth);
  glDrawElements(GL_LINES, F.count, GL_UNSIGNED_INT, F.array);
  glDisable(GL_LINE_STIPPLE);
}

void Rendering::drawPoints() {
  if (style.vertexType)
    glPointSize(style.vertexWidth);
  else
    glPointSize(0.00000001);
  glColor4f(style.vertexRed, style.vertexGreen, style.vertexBlue,
            style.vertexAlfa);
  if (style.vertexType == 1) glEnable(GL_POINT_SMOOTH);
  glDrawArrays(GL_POINTS, 0, V.rows);
  if (style.vertexType == 1) glDisable(GL_POINT_SMOOTH);
}

void Rendering::move(double value_x, double value_y, double value_z) {
  s21_move_3d(&V, value_x, value_y, value_z);
  update();
}
