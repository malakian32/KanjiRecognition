#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controlpreprocesamiento.h"
#include "controlsegmentacion.h"
#include "dialogocaracteristicas.h"
#include "controlficheros.h"


#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

#include <iostream>
#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "boost/filesystem.hpp"

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
    static QImage Mat2QImage(const Mat& srcImage);

private slots:
    void on_calcularUmbralBT_clicked();
    void on_abrirImagenBT_clicked();
    void on_cierreBT_clicked();
    void on_GO_clicked();
    void on_AdelgazamientoBT_clicked();
    void on_SegmentacionBT_clicked();
    void on_AperturaBT_clicked();
    void resetWidgets();
    void buscarArchivo();
    void on_CaracteristicasBT_clicked();


    void on_abrirFicheroBT_clicked();

private:
    Mat srcImage;
    Mat dstImageTreshold;
    Mat dstImageOpening;
    Mat dstImageClose;
    Mat dstImageAdelgazada;
    Mat dstImageSegmentacion;
    Mat dstImageRectanguloEnvolvente;
    Mat dstImageContornos;

    Rect dstRectanguloEnvolvente;

    QString imageFile;
    QString ficheroAbierto;
    QStringList imagesDirectory;
    bool ImagenAbierta;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
