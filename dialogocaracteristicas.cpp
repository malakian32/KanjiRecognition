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
    ControlObtencionCaracteristicas::buscarPuntos(dstImagenFinal,endPoints, insersectPoints);
    if(insersectPoints.empty())cout<<"1 empty"<<endl;
    if(endPoints.empty())cout<<"2 empty"<<endl;

    cvtColor(dstImagenFinal,dstImagenFinalEndPoints,CV_GRAY2BGR);
    for(int i=0; i<endPoints.size();i++)
    {
        circle(dstImagenFinalEndPoints,endPoints.at(i),2,Scalar(0,0,255),2);
    }

    for(int i=0; i<insersectPoints.size();i++)
    {
        circle(dstImagenFinalEndPoints,insersectPoints.at(i),2,Scalar(255,0,0),2);
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
    int cuadEndPoints0 = 0,cuadEndPoints1 = 0, cuadEndPoints2 = 0, cuadEndPoints3 = 0,
            cuadEndPoints4 = 0, cuadEndPoints5 = 0, cuadEndPoints6 = 0, cuadEndPoints7 = 0, cuadEndPoints8 = 0;
    int mitadX1 = int(dstImagenFinal.cols/3);
    int mitadX2 = int(2*dstImagenFinal.cols/3);
    int mitadY1 = int(dstImagenFinal.rows/3);
    int mitadY2 = int(2*dstImagenFinal.rows/3);
    Point p;

    for(unsigned c = 0; c< endPoints.size();c++){
        p = endPoints.at(c);
        //0
        if( p.x < mitadX1 && p.y <  mitadY1 ){
            cuadEndPoints0++;
            continue;}
        //1
        if( p.x <  mitadX2 && p.y <  mitadY1 ){
            cuadEndPoints1++;
            continue;}
        //2
        if( p.x >=  mitadX2 && p.y < mitadY1 ){
            cuadEndPoints2++;
            continue;}
        //3
        if( p.x < mitadX1 && p.y < mitadY2 ){
            cuadEndPoints3++;
            continue;}
        //4
        if( p.x < mitadX2 && p.y < mitadY2 ){
            cuadEndPoints4++;
            continue;}
        //5
        if( p.x >= mitadX2 && p.y < mitadY2 ){
            cuadEndPoints5++;
            continue;}
        //6
        if( p.x < mitadX1 && p.y >= mitadY2 ){
            cuadEndPoints6++;
            continue;}
        //7
        if( p.x < mitadX2 && p.y >= mitadY2 ){
            cuadEndPoints7++;
            continue;}
        //8
        if( p.x >= mitadX2 && p.y >= mitadY2 ){
            cuadEndPoints8++;
            continue;}


    }

    int cuadInterPoints0 = 0,cuadInterPoints1 = 0, cuadInterPoints2 = 0, cuadInterPoints3 = 0,
            cuadInterPoints4 = 0, cuadInterPoints5 = 0, cuadInterPoints6 = 0, cuadInterPoints7 = 0, cuadInterPoints8 = 0;


    for(unsigned z = 0; z< insersectPoints.size();z++)
    {
        p = insersectPoints.at(z);

        //0
        if( p.x < mitadX1 && p.y <  mitadY1 ){
            cuadInterPoints0=1;
            continue;}
        //1
        if( p.x <  mitadX2 && p.y <  mitadY1 ){
            cuadInterPoints1=1;
            continue;}
        //2
        if( p.x >=  mitadX2 && p.y < mitadY1 ){
            cuadInterPoints2=1;
            continue;}
        //3
        if( p.x < mitadX1 && p.y < mitadY2 ){
            cuadInterPoints3=1;
            continue;}
        //4
        if( p.x < mitadX2 && p.y < mitadY2 ){
            cuadInterPoints4=1;;
            continue;}
        //5
        if( p.x >= mitadX2 && p.y < mitadY2 ){
            cuadInterPoints5=1;;
            continue;}
        //6
        if( p.x < mitadX1 && p.y >= mitadY2 ){
            cuadInterPoints6=1;;
            continue;}
        //7
        if( p.x < mitadX2 && p.y >= mitadY2 ){
            cuadInterPoints7=1;;
            continue;}
        //8
        if( p.x >= mitadX2 && p.y >= mitadY2 ){
            cuadInterPoints8=1;;
            continue;}
    }
    cout<<cuadEndPoints0<<cuadEndPoints1<<cuadEndPoints2<<cuadEndPoints3<<cuadEndPoints4<<cuadEndPoints5<<cuadEndPoints6<<cuadEndPoints7<<cuadEndPoints8<<endl;
    cout<<cuadInterPoints0<<cuadInterPoints1<<cuadInterPoints2<<cuadInterPoints3<<cuadInterPoints4<<cuadInterPoints5<<cuadInterPoints6<<cuadInterPoints7<<cuadInterPoints8<<endl;


    ///
    controlredneuronal red = controlredneuronal("./NeuralNetwork.xml");
    cv::Mat caracteristicas(1,19,CV_32F);


    caracteristicas.at<float>(0, 0) = momentosHu.at(0).at(0);
    caracteristicas.at<float>(0, 1) = momentosHu.at(0).at(1);
    caracteristicas.at<float>(0, 2) = momentosHu.at(0).at(2);
    caracteristicas.at<float>(0, 3) = momentosHu.at(0).at(3);
    caracteristicas.at<float>(0, 4) = momentosHu.at(0).at(4);
    caracteristicas.at<float>(0, 5) = momentosHu.at(0).at(5);
    caracteristicas.at<float>(0, 6) = momentosHu.at(0).at(6);
    caracteristicas.at<float>(0, 7) = relacionAnchoAlto;
    caracteristicas.at<float>(0, 8) = endPoints.size();
    caracteristicas.at<float>(0, 9) = cuadEndPoints0;
    caracteristicas.at<float>(0, 10) = cuadEndPoints1;
    caracteristicas.at<float>(0, 11) = cuadEndPoints2;
    caracteristicas.at<float>(0, 12) = cuadEndPoints3;
    caracteristicas.at<float>(0, 13) = cuadEndPoints4;
    caracteristicas.at<float>(0, 14) = cuadEndPoints5;
    caracteristicas.at<float>(0, 15) = cuadEndPoints6;
    caracteristicas.at<float>(0, 16) = cuadEndPoints7;
    caracteristicas.at<float>(0, 17) = cuadEndPoints8;
//    caracteristicas.at<float>(0, 18) = cuadInterPoints0;
//    caracteristicas.at<float>(0, 19) = cuadInterPoints1;
//    caracteristicas.at<float>(0, 20) = cuadInterPoints2;
//    caracteristicas.at<float>(0, 21) = cuadInterPoints3;
//    caracteristicas.at<float>(0, 22) = cuadInterPoints4;
//    caracteristicas.at<float>(0, 23) = cuadInterPoints5;
//    caracteristicas.at<float>(0, 24) = cuadInterPoints6;
//    caracteristicas.at<float>(0, 25) = cuadInterPoints7;
//    caracteristicas.at<float>(0, 26) = cuadInterPoints8;

    caracteristicas.at<float>(0, 18) = contornos.size();


    valorPredecido = red.predict( caracteristicas );
    cout<<"THE NUMBER IS: "<<valorPredecido<<endl;

    ostringstream valorPredOS;
    valorPredOS<<valorPredecido;

    string imagenPredecida = "./resources/images/Resultados/" + valorPredOS.str() + ".png  ";
    cout<<imagenPredecida<<endl;

    map<int,string> clases=map_list_of(1,"ICHI")
                            (2,"NI")
                            (3,"SAN")
                            (4,"SHI")
                            (5,"GO")
                            (6,"ROKU")
                            (7,"SHICHI")
                            (8,"HACHI")
                            (9,"KU")
                            (10,"JUU")
                            (11,"HYAKU")
                            (12,"SEN");

//    Mat imageKanjiPredecido = imread(imagenPredecida,0);
//    imshow("pas",imageKanjiPredecido);
//    Mat imageKanjiResize;
//    cv::resize(imageKanjiPredecido,imageKanjiResize,Size(70,70),0,0,INTER_CUBIC);

//    QImage imagePredecida = MainWindow::Mat2QImage(imageKanjiPredecido);

  //  ui->kanjiImagenLB->setPixmap(QPixmap::fromImage(imagePredecida));
    string temp = clases.find(valorPredecido)->second;
    ui->kanjiReconocidoLB->setText(QString::fromStdString(valorPredOS.str() + " - " + temp));
}

