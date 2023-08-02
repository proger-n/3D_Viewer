#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  frame_count = 0;
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::gif_creator);
  this->setWindowTitle("3DViewer");
  if (!ui->widget->style.projectionType)
    ui->parallelRButtons->setChecked(true);
  else
    ui->centralRButton->setChecked(true);
  if (!ui->widget->style.vertexType)
    ui->noneInVertices->setChecked(true);
  else if (ui->widget->style.vertexType == 1)
    ui->circleInVertices->setChecked(true);
  else
    ui->squareInVertices->setChecked(true);
  if (!ui->widget->style.edgeType)
    ui->solidType->setChecked(true);
  else
    ui->dashedType->setChecked(true);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pathButton_clicked() {
  ui->widget->clearRender();
  setlocale(LC_ALL, "C");          // setting local
  QString filter = "file(*.obj)";  // tr()
  QString path = QFileDialog::getOpenFileName(NULL, "Open a file", "", filter);
  if (path.isEmpty()) {
    ui->label->setText("Choose .obj file");
    QMessageBox::information(NULL, "Error", "Вы не выбрали файл");
  } else {
    ui->label->setText(path);
  }
  QByteArray bytes = path.toLocal8Bit();
  char *cstr = bytes.data();
  ui->widget->start_read_file(cstr);
  int number_of_vertices = ui->widget->V.rows;
  int number_of_edges = ui->widget->F.count;
  ui->vertices->setText(QString::number(number_of_vertices, 'g', 15));
  ui->edges->setText(QString::number(number_of_edges, 'g', 15));
}

void MainWindow::getColorPicker(int *red, int *green, int *blue, int *alpha) {
  QColorDialog dialog(this);
  dialog.setOption(QColorDialog::ShowAlphaChannel);
  QColor color = dialog.getColor();
  color.getRgb(red, green, blue, alpha);
}

void MainWindow::on_backgroundColorButton_clicked() {
  int red = 0, green = 0, blue = 0, alpha = 0;
  getColorPicker(&red, &green, &blue, &alpha);
  ui->widget->style.backRed = red / 255.0f;
  ui->widget->style.backGreen = green / 255.0f;
  ui->widget->style.backBlue = blue / 255.0f;
  ui->widget->style.backAlfa = alpha / 255.0f;
}

void MainWindow::on_verticesColorButton_clicked() {
  int red = 0, green = 0, blue = 0, alpha = 0;
  getColorPicker(&red, &green, &blue, &alpha);
  ui->widget->style.vertexRed = red / 255.0f;
  ui->widget->style.vertexGreen = green / 255.0f;
  ui->widget->style.vertexBlue = blue / 255.0f;
  ui->widget->style.vertexAlfa = alpha / 255.0f;
}

void MainWindow::on_edgesColorButton_clicked() {
  int red = 0, green = 0, blue = 0, alpha = 0;
  getColorPicker(&red, &green, &blue, &alpha);
  ui->widget->style.edgeRed = red / 255.0f;
  ui->widget->style.edgeGreen = green / 255.0f;
  ui->widget->style.edgeBlue = blue / 255.0f;
  ui->widget->style.edgeAlfa = alpha / 255.0f;
}

void MainWindow::on_verticesWidth_valueChanged(double arg1) {
  ui->widget->style.vertexWidth = arg1;
}

void MainWindow::on_edgesWidth_valueChanged(double arg1) {
  ui->widget->style.lineWidth = arg1;
}

void MainWindow::on_parallelRButtons_clicked() {
  ui->widget->style.projectionType = 0;
}

void MainWindow::on_centralRButton_clicked() {
  ui->widget->style.projectionType = 1;
}

void MainWindow::on_noneInVertices_clicked() {
  ui->widget->style.vertexType = 0;
}

void MainWindow::on_circleInVertices_clicked() {
  ui->widget->style.vertexType = 1;
}

void MainWindow::on_squareInVertices_clicked() {
  ui->widget->style.vertexType = 2;
}

void MainWindow::on_solidType_clicked() { ui->widget->style.edgeType = 0; }

void MainWindow::on_dashedType_clicked() { ui->widget->style.edgeType = 1; }

void MainWindow::gif_creator() {
  if (frame_count < 30) {
    image = ui->widget->grabFramebuffer();
    gif->addFrame(image);
    frame_count++;
  } else {
    frame_count = 0;
    timer->stop();

    QFileDialog file_dialog_gif(this);
    QString f_name_gif = file_dialog_gif.getSaveFileName(this, tr("Save a gif"),
                                                         "", tr("gif (*.gif)"));
    gif->save(f_name_gif);
  }
}
void MainWindow::on_recordButton_clicked() {
  gif = new QGifImage;
  timer->start(100);
}

void MainWindow::on_screenButton_clicked() {
  QString path = QFileDialog::getSaveFileName(NULL, "Save Screenshot", "",
                                              "BMP (*.bmp);;JPEG (*.jpg)");
  if (!path.isEmpty()) {
    ui->widget->grabFramebuffer().save(path);
  }
}

void MainWindow::on_lowerScaleButton_clicked() { ui->widget->scale(0.9); }

void MainWindow::on_hiegherScaleButton_clicked() { ui->widget->scale(1.1); }

void MainWindow::on_setupRotationButton_clicked() {
  if (ui->widget->V.rows) {
    double value_x = ui->rotateX->value();
    double value_y = ui->rotateY->value();
    double value_z = ui->rotateZ->value();
    if (value_z || value_y || value_x) {
      ui->widget->rotate(value_x, value_y, value_z);
      update();
    }
  }
}

void MainWindow::on_setupTranslateButton_clicked() {
  if (ui->widget->V.rows) {
    double move_x = ui->moveX->value();
    double move_y = ui->moveY->value();
    double move_z = ui->moveZ->value();
    if (move_x || move_y || move_z) {
      ui->widget->move(move_x, move_y, move_z);
      update();
    }
  }
}
