#ifndef RENDERING_H
#define RENDERING_H

#define GL_SILENCE_DEPRECATION

#include <QtOpenGLWidgets/qopenglwidget.h>

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFunctions>
#include <QSettings>

extern "C" {
#include "../backend/parsing.h"
};

typedef struct look {
  float backRed, backGreen, backBlue, backAlfa;
  float vertexRed, vertexGreen, vertexBlue, vertexAlfa;
  float edgeRed, edgeGreen, edgeBlue, edgeAlfa;
  double vertexWidth, lineWidth;
  int projectionType, vertexType, edgeType;
} Look;

class Rendering : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit Rendering(QWidget *parent = nullptr);
  ~Rendering();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 public:
  void start_read_file(char *str);
  VerticesMatrix V = {0, 0, 0};
  IndicesArray F = {0, 0};
  Look style = {0.451, 0.451, 0.451, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  void scale(double scl);
  void rotate(double x, double y, double z);
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *event) override;
  float xRot, yRot, zRot;
  QPoint mPos;
  void saveSettings();
  void loadSettings();
  void clearRender();
  void setProjection();
  void drawLines();
  void drawPoints();
  void move(double value_x, double value_y, double value_z);

 private:
  float currentXRot = 0.0f;
  float currentYRot = 0.0f;
  float XRot = 0.0f;
  float YRot = 0.0f;
};

#endif  // RENDERING_H
