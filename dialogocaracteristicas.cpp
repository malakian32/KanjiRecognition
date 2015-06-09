#include "dialogocaracteristicas.h"
#include "ui_dialogocaracteristicas.h"

DialogoCaracteristicas::DialogoCaracteristicas(QWidget *parent,QMainWindow* parentWindow, Mat srcImage, Mat dstImagePreprocesada, Rect ROI,vector<vector<Point> > contornos) :
    QDialog(parent),
    ui(new Ui::DialogoCaracteristicas)
{
    this->srcImage = srcImage;
    this->dstImagePreprocesada = dstImagePreprocesada;
    this->ROI = ROI;
    this->dstImagenFinal = dstImagePreprocesada(ROI).clone();
    cout<<"x " <<ROI.x<<endl;
    cout<<"y " <<ROI.y<<endl;
    this->parentWindow = parentWindow;
    this->contornos = contornos;
    ui->setupUi(this);

    inicializarImagenes();
}

DialogoCaracteristicas::~DialogoCaracteristicas()
{
    delete ui;
}

void DialogoCaracteristicas::inicializarImagenes()
{
    QImage originalImage = MainWindow::Mat2QImage(this->srcImage);
    ui->ImagenOriginalLB->setPixmap(QPixmap::fromImage(originalImage));

    QImage preprocesadaImage = MainWindow::Mat2QImage(this->dstImagePreprocesada);
    ui->ImagenPreprocesadaLB->setPixmap(QPixmap::fromImage(preprocesadaImage));


}

void DialogoCaracteristicas::on_CalcularCaracteristicasBT_clicked()
{
    cout<<dstImagePreprocesada.cols<<endl;
    cout<<dstImagenFinal.cols<<endl;
    relacionAnchoAlto = (double)dstImagenFinal.cols/dstImagenFinal.rows;
    ostringstream relAAOS;
    relAAOS<<relacionAnchoAlto;
    ui->RelacionLB->setText(QString::fromStdString(relAAOS.str()));

    vector<Point> endPoints;
    vector<Point> insersectPoints;
    ControlObtencionCaracteristicas::buscarEndPoints(dstImagenFinal,endPoints, insersectPoints);


    cvtColor(dstImagenFinal,dstImagenFinalEndPoints,CV_GRAY2BGR);
    for(int i=0; i<endPoints.size();i++)
    {
        circle(dstImagenFinalEndPoints,endPoints.at(i),2,Scalar(0,0,255),2);
    }

    this->momentosHu = ControlObtencionCaracteristicas::getHuMoments(contornos);
    //cout<<momentosHu.size()<<endl;

    QImage endPointsImage = MainWindow::Mat2QImage(this->dstImagenFinalEndPoints);
    ui->ImagenEndPointsLB->setPixmap(QPixmap::fromImage(endPointsImage));

    ostringstream endPOS;
    cout<<endPoints.size()<<endl;
    endPOS<<endPoints.size();
    ui->EndPointsLB->setText(QString::fromStdString(endPOS.str()));


 ///Determina a cual cuadrante pertenece cada punto
    int cuadrante1 = 0;
    int cuadrante2 = 0;
    int cuadrante3 = 0;
    int cuadrante4 = 0;
    int mitadX = int(dstImagenFinal.cols/2);
    int mitadY = int(dstImagenFinal.rows/2);
    Point p;

    for(unsigned c = 0; c< endPoints.size();c++){
        p = endPoints.at(c);
        if( p.x >= mitadX && p.y <  mitadY ){ cuadrante1++; }
        if( p.x <  mitadX && p.y <  mitadY ){ cuadrante2++; }
        if( p.x <  mitadX && p.y >= mitadY ){ cuadrante3++; }
        if( p.x >= mitadX && p.y >= mitadY ){ cuadrante4++; }
    }

    ///
    controlredneuronal red = controlredneuronal("/home/snipercat/Desktop/NeuralNetwork.xml");
    cv::Mat caracteristicas(1,14,CV_32F);


    caracteristicas.at<float>(0, 0) = momentosHu.at(0).at(0);
    caracteristicas.at<float>(0, 1) = momentosHu.at(0).at(1);
    caracteristicas.at<float>(0, 2) = momentosHu.at(0).at(2);
    caracteristicas.at<float>(0, 3) = momentosHu.at(0).at(3);
    caracteristicas.at<float>(0, 4) = momentosHu.at(0).at(4);
    caracteristicas.at<float>(0, 5) = momentosHu.at(0).at(5);
    caracteristicas.at<float>(0, 6) = momentosHu.at(0).at(6);
    caracteristicas.at<float>(0, 7) = relacionAnchoAlto;
    caracteristicas.at<float>(0, 8) = endPoints.size();
    caracteristicas.at<float>(0, 9) = cuadrante1;
    caracteristicas.at<float>(0, 10) = cuadrante2;
    caracteristicas.at<float>(0, 11) = cuadrante3;
    caracteristicas.at<float>(0, 12) = cuadrante4;
    caracteristicas.at<float>(0, 13) = contornos.size();


    int number = red.predict( caracteristicas );
    cout<<"THE NUMBER IS: "<<number<<endl;

}

