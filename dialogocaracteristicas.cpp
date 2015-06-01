#include "dialogocaracteristicas.h"
#include "ui_dialogocaracteristicas.h"

DialogoCaracteristicas::DialogoCaracteristicas(QWidget *parent,QMainWindow* parentWindow, Mat srcImage, Mat dstImagePreprocesada, Rect ROI) :
    QDialog(parent),
    ui(new Ui::DialogoCaracteristicas)
{
    this->srcImage = srcImage;
    this->dstImagePreprocesada = dstImagePreprocesada;
    this->ROI = ROI;
    this->dstImagenFinal = dstImagePreprocesada(ROI).clone();
    this->parentWindow = parentWindow;
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

    QImage endPointsImage = MainWindow::Mat2QImage(this->dstImagenFinalEndPoints);
    ui->ImagenEndPointsLB->setPixmap(QPixmap::fromImage(endPointsImage));

    ostringstream endPOS;
    cout<<endPoints.size()<<endl;
    endPOS<<endPoints.size();
    ui->EndPointsLB->setText(QString::fromStdString(endPOS.str()));

}

