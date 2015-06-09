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
    ControlObtencionCaracteristicas::buscarEndPoints(dstImagenFinal,endPoints);


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

    ///
    controlredneuronal red = controlredneuronal(srcNetworkFile);
    cv::Mat caracteristicas(1,10,CV_32F);


    caracteristicas.at<float>(0, 0) = momentosHu.at(0).at(0);
    caracteristicas.at<float>(0, 1) = momentosHu.at(0).at(1);
    caracteristicas.at<float>(0, 2) = momentosHu.at(0).at(2);
    caracteristicas.at<float>(0, 3) = momentosHu.at(0).at(3);
    caracteristicas.at<float>(0, 4) = momentosHu.at(0).at(4);
    caracteristicas.at<float>(0, 5) = momentosHu.at(0).at(5);
    caracteristicas.at<float>(0, 6) = momentosHu.at(0).at(6);
    caracteristicas.at<float>(0, 7) = relacionAnchoAlto;
    caracteristicas.at<float>(0, 8) = endPoints.size();
    caracteristicas.at<float>(0, 9) = contornos.size();

     cv::Mat caracteristica(1,3,CV_32F);
     caracteristica.at<float>(0, 0) = relacionAnchoAlto;
     caracteristica.at<float>(0, 1) = endPoints.size();
     caracteristica.at<float>(0, 2) = contornos.size();


    int number = red.predict( caracteristica );
    cout<<"THE NUMBER IS: "<<number<<endl;

}

