#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controlpreprocesamiento.h"
#include "controlsegmentacion.h"
#include "controlobtencioncaracteristicas.h"
#include "dialogocaracteristicas.h"
#include "controlficheros.h"
#include "controlredneuronal.h"


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

    void on_EntrenarBT_clicked();

    void generarDataSet(char* fileName,vector<pair<string,int> > imagesList);

    void on_testPathBT_clicked();

    void on_EntrenarBT_2_clicked();

    int getNumber();
private:
    Mat srcImage;
    Mat srcImageEqualizada;
    Mat dstImageThreshold;
    Mat dstImageThresholdAdaptative;
    Mat dstImageOpening;
    Mat dstImageClose;
    Mat dstImageAdelgazada;
    Mat dstImageSegmentacion;
    Mat dstImageRectanguloEnvolvente;
    Mat dstImageContornos;
    Mat dstImageFinal;
    Rect dstRectanguloEnvolvente;

    QString imageFile;
    QString ficheroAbierto;
    //QStringList imagesDirectory;
    vector<pair<string,int> > imagesDirectory;
    vector<pair<string,int> > testImagesDirectory;
    bool ImagenAbierta;
    Ui::MainWindow *ui;

    int number;
    //char *srcNetworkFile = "/home/snipercat/Desktop/NeuralNetwork.xml";
    //char *srcDataSetFile = "/home/snipercat/Desktop/ArchivoCaracteristicasDataSet.csv";

};

#endif // MAINWINDOW_H
