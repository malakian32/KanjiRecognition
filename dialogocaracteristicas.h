#ifndef DIALOGOCARACTERISTICAS_H
#define DIALOGOCARACTERISTICAS_H

#include <QDialog>
#include <QMainWindow>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "mainwindow.h"
#include "controlobtencioncaracteristicas.h"
#include <iostream>
using namespace cv;
using namespace std;


namespace Ui {
class DialogoCaracteristicas;
}

class DialogoCaracteristicas : public QDialog
{
    Q_OBJECT

public:
    explicit DialogoCaracteristicas(QWidget *parent = 0,QMainWindow* parentWindow=0,Mat srcImage = Mat(), Mat dstImagePreprocesada = Mat(),Rect ROI = Rect(),vector<vector<Point> > contornos = vector<vector<Point> >());
    ~DialogoCaracteristicas();

private slots:
    void on_CalcularCaracteristicasBT_clicked();

private:
    Ui::DialogoCaracteristicas *ui;

    Mat srcImage;
    Mat dstImagePreprocesada;
    Mat dstImagenFinal;
    Mat dstImagenFinalEndPoints;
    Rect ROI;
    vector<vector<double> > momentosHu;
    vector<vector<Point> > contornos;
    double relacionAnchoAlto;
    QMainWindow* parentWindow;
    void inicializarImagenes();

    char *srcNetworkFile = "/home/snipercat/Desktop/NeuralNetwork.xml";

};

#endif // DIALOGOCARACTERISTICAS_H
