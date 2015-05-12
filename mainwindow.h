#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>

#include <iostream>
#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_calcularUmbralBT_clicked();

    void on_abrirImagenBT_clicked();

    void on_aperturaBT_clicked();

    void on_cierreBT_clicked();

    void on_GO_clicked();

    //FUNCIONES PARA PROCESAMIENTO
    Mat  umbralAutomatico( Mat sourceImage);
    Mat  morphImage( Mat sourceImage);
    Mat  skeleton( Mat sourceImage);

private:
    Mat srcImage;
    QString imageFile;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
