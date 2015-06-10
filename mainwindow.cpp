#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ImagenAbierta = false;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//When Clic on Abrir Imagen
void MainWindow::on_abrirImagenBT_clicked()
{
    if(ImagenAbierta)
    {
        QMessageBox msgBox;
        msgBox.setText("Una Imagen esta siendo procesada en este momento.");
        msgBox.setInformativeText("¿Desea abrir una nueva imagen?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Yes:
            resetWidgets();
            buscarArchivo();
            break;
        case QMessageBox::Discard:

            break;
        default:
            // should never be reached
            break;
        }
    }
    else
    {
        buscarArchivo();
    }
}

void MainWindow::buscarArchivo()
{

    this->imageFile = QFileDialog::getOpenFileName(this,tr("Open Image"),"./resources",tr("Image Files(*.png *.jpg *.bmp)"));

    this->ui->filePathLE->setText(imageFile);
    QPixmap imagePixel(imageFile);
    ui->ImagenOriginalLB->setPixmap(imagePixel);

    ui->calcularUmbralBT->setEnabled(true);


    cout<<imageFile.toStdString().data()<<endl;
    number = getNumber();
    cout<<"NUMBER IS: "<<number<<endl;
    this->srcImage = imread(imageFile.toStdString().data(),0);
    line(srcImage,Point(0,0),Point(0,srcImage.rows),Scalar(255),20);
    line(srcImage,Point(0,0),Point(srcImage.cols,0),Scalar(255),20);
    line(srcImage,Point(srcImage.cols,srcImage.rows),Point(srcImage.cols,0),Scalar(255),20);
    line(srcImage,Point(srcImage.cols,srcImage.rows),Point(0,srcImage.rows),Scalar(255),20);

    equalizeHist(this->srcImage,  this->srcImageEqualizada );
    this->ImagenAbierta = true;
}

void MainWindow::on_calcularUmbralBT_clicked()
{
    this->dstImageThresholdAdaptative = ControlPreprocesamiento::umbralAutomaticoAdaptativo(srcImage);
    this->dstImageThreshold = ControlPreprocesamiento::umbralAutomatico(srcImageEqualizada);

    QImage umbralImage = Mat2QImage(dstImageThresholdAdaptative);
    ui->UmbralizacionLB->setPixmap(QPixmap::fromImage(umbralImage));

    ui->AperturaBT->setEnabled(true);

}


void MainWindow::on_AperturaBT_clicked()
{
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(2,2));

    morphologyEx(this->dstImageThresholdAdaptative, this->dstImageClose, CV_MOP_CLOSE, BStructElement,Point(-1,-1) ,2 );


    QImage filtradoImage = Mat2QImage(dstImageClose);
    ui->FiltradoMorfologicoLB->setPixmap(QPixmap::fromImage(filtradoImage));

    ui->SegmentacionBT->setEnabled(true);

}

void MainWindow::on_cierreBT_clicked()
{
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(2,2));

    morphologyEx(this->dstImageOpening, this->dstImageClose, CV_MOP_DILATE, BStructElement );

    imshow("CIERRE",this->dstImageClose);
}


/**
 * @brief MainWindow::on_GO_clicked REALIZA EL PROCESAMIENTO AUMÁTICO PARA UNA IMAGEN
 */

void MainWindow::on_GO_clicked()
{

}


void MainWindow::on_AdelgazamientoBT_clicked()
{
    dstImageAdelgazada = Mat::zeros(dstImageClose.size(), CV_8UC1);
    dstImageClose.copyTo(dstImageAdelgazada);

    ControlPreprocesamiento::adelgazamiento(dstImageAdelgazada);

    dstImageAdelgazada.copyTo(dstImageRectanguloEnvolvente);
    //Se suman 3 pixeles de distancia a las medidas del rectangulo para darle espacio
    //alalgoritmo de busqueda de end-points

//    Rect rect = boundingRect(dstImageAdelgazada);

//    if(dstRectanguloEnvolvente.x <= 5 || dstRectanguloEnvolvente.y <= 5 ) continue;
    dstRectanguloEnvolvente.height += 10;
    dstRectanguloEnvolvente.width += 10;
    dstRectanguloEnvolvente.x -= 5;
    dstRectanguloEnvolvente.y -= 5;

    rectangle(dstImageRectanguloEnvolvente,dstRectanguloEnvolvente,Scalar(255));

    //Convertir imagenes y transformarlas para mostrar en la UI
    QImage RectanguloEnvolventeImage = Mat2QImage(dstImageRectanguloEnvolvente);
    ui->RectanguloEnvolventeLB->setPixmap(QPixmap::fromImage(RectanguloEnvolventeImage));

    QImage adelgazamientoImage = Mat2QImage(dstImageAdelgazada);
    ui->AdelgazamientoLB->setPixmap(QPixmap::fromImage(adelgazamientoImage));

    //Habilitar el boton de Caracteristicas
    ui->CaracteristicasBT->setEnabled(true);

}

void MainWindow::on_SegmentacionBT_clicked()
{
    Mat src = imread(this->imageFile.toStdString());
    ControlSegmentacion::encontrarSegmentos(src,dstImageClose,dstImageSegmentacion,dstRectanguloEnvolvente);

    QImage segmentacionImage = Mat2QImage(this->dstImageSegmentacion);
    ui->SegmentacionLB->setPixmap(QPixmap::fromImage(segmentacionImage));
    ui->AdelgazamientoBT->setEnabled(true);
}


QImage MainWindow::Mat2QImage(const Mat& srcImage)
{
    Mat tempImage;
    switch (srcImage.type()) {//Se convierte el archivo Mat a RGB para poderlo mostrar en pantalla
         case CV_8UC1:
             cvtColor(srcImage,tempImage, CV_GRAY2RGB);
             break;
         case CV_8UC3:
             cvtColor(srcImage, tempImage, CV_BGR2RGB);
             break;
         }

    return QImage((uchar*)tempImage.data, tempImage.cols, tempImage.rows,tempImage.cols*3
                      ,QImage::Format_RGB888).copy();
}


void MainWindow::on_CaracteristicasBT_clicked()
{

    Mat dstImageMorph = ControlPreprocesamiento::morphImage(dstImageThreshold);
    vector<vector<Point> > contornos;
    contornos = ControlObtencionCaracteristicas::getContornos(dstImageMorph(dstRectanguloEnvolvente).clone());


    DialogoCaracteristicas* dialogoCaracteristicas = new DialogoCaracteristicas(this,this,srcImage,dstImageAdelgazada,dstRectanguloEnvolvente,contornos, number);

    dialogoCaracteristicas->show();
}

void MainWindow::resetWidgets()
{
    ui->calcularUmbralBT->setEnabled(false);
    ui->AdelgazamientoBT->setEnabled(false);
    ui->AperturaBT->setEnabled(false);
    ui->SegmentacionBT->setEnabled(false);
    ui->CaracteristicasBT->setEnabled(false);

    ui->ImagenOriginalLB->clear();
    ui->AdelgazamientoLB->clear();
    ui->FiltradoMorfologicoLB->clear();
    ui->RectanguloEnvolventeLB->clear();
    ui->SegmentacionLB->clear();
    ui->UmbralizacionLB->clear();

}



//Boton para abrir un fichero completo
void MainWindow::on_abrirFicheroBT_clicked()

{
    this->ficheroAbierto = QFileDialog::getExistingDirectory(this,tr("Abrir Directorio"),"./resources",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ControlFicheros::abrirFichero(path(ficheroAbierto.toStdString()),imagesDirectory);
    string nombreFichero = path(ficheroAbierto.toStdString()).filename().c_str();
    ostringstream numeroArchivos;
    numeroArchivos<<imagesDirectory.size();

    ui->URLFicherLB->setText(ficheroAbierto);
    ui->NombreFicheroLB->setText(QString::fromStdString(nombreFichero));
    ui->SizeFicheroLB->setText(QString::fromStdString(numeroArchivos.str()));
    ui->EntrenarBT->setEnabled(true
                               );


}

//Botón para abrir path de pruebas
void MainWindow::on_testPathBT_clicked()
{
    this->ficheroAbierto = QFileDialog::getExistingDirectory(this,tr("Abrir Directorio"),"./resources",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ControlFicheros::abrirFichero(path(ficheroAbierto.toStdString()),testImagesDirectory);
    string nombreFichero = path(ficheroAbierto.toStdString()).filename().c_str();
    ostringstream numeroArchivos;
    numeroArchivos<<testImagesDirectory.size();

    ui->URLFicherPruebaLB->setText(ficheroAbierto);
    ui->NombreFicheroPruebaLB->setText(QString::fromStdString(nombreFichero));
    ui->SizeFicheroPruebaLB->setText(QString::fromStdString(numeroArchivos.str()));
    ui->EntrenarBT->setEnabled(true
                               );
}

void MainWindow::on_EntrenarBT_clicked()
{

    ui->EntrenarBT_2->setEnabled(false);
    //generar DataSet de Entrenamiento
    generarDataSet("./ArchivoCaracteristicasDataSet.csv",imagesDirectory);

    //generar DataSet de Prueba
    generarDataSet("./ArchivoCaracteristicasTestSet.csv",testImagesDirectory);

    ui->EntrenarBT_2->setEnabled(true);
}


void MainWindow::on_EntrenarBT_2_clicked()
{
    ui->EntrenarBT_2->setEnabled(false);
    int efficiency = controlredneuronal::train( "./ArchivoCaracteristicasDataSet.csv", "./ArchivoCaracteristicasTestSet.csv",  "./NeuralNetwork.xml"  );
    ui->eficacia_ValueLB->setText( QString("%1 %").arg(efficiency));
    ui->EntrenarBT_2->setEnabled(true);
}


void MainWindow::generarDataSet(char* fileName,vector<pair<string,int> > imagesList){

    FILE* archivoCaracteristicas = fopen(fileName,"w");

    for(unsigned i = 0; i<imagesList.size();i++)
    {

        cout<<"Archivo numero:"<<i<<" --- " <<imagesList.at(i).first.data()<<endl;
        //ABRIR IMAGEN
        srcImage = imread(imagesList.at(i).first.data(),0);
        line(srcImage,Point(0,0),Point(0,srcImage.rows),Scalar(255),20);
        line(srcImage,Point(0,0),Point(srcImage.cols,0),Scalar(255),20);
        line(srcImage,Point(srcImage.cols,srcImage.rows),Point(srcImage.cols,0),Scalar(255),20);
        line(srcImage,Point(srcImage.cols,srcImage.rows),Point(0,srcImage.rows),Scalar(255),20);

        equalizeHist(srcImage,srcImageEqualizada);
        //CALCULAR THRESHOLD
        this->dstImageThresholdAdaptative = ControlPreprocesamiento::umbralAutomaticoAdaptativo(srcImage);
        this->dstImageThreshold = ControlPreprocesamiento::umbralAutomatico(srcImageEqualizada);

        //FILTRADO
        Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(2,2));
        morphologyEx(this->dstImageThresholdAdaptative, this->dstImageClose, CV_MOP_CLOSE, BStructElement,Point(-1,-1) ,2 );

        //SEGMENTACION
        Mat src = imread(imagesList.at(i).first);
        ControlSegmentacion::encontrarSegmentos(src,dstImageClose,dstImageSegmentacion,dstRectanguloEnvolvente);

        //ADELGAZAMIENTOsrcImage
        dstImageAdelgazada = Mat::zeros(dstImageClose.size(), CV_8UC1);
        dstImageClose.copyTo(dstImageAdelgazada);

        ControlPreprocesamiento::adelgazamiento(dstImageAdelgazada);

        dstImageAdelgazada.copyTo(dstImageRectanguloEnvolvente);
        //Se suman 5 pixeles de distancia a las medidas del rectangulo para darle espacio
        //alalgoritmo de busqueda de end-points
        if(dstRectanguloEnvolvente.x <= 5 || dstRectanguloEnvolvente.y <= 5 ) continue;
        dstRectanguloEnvolvente.height += 10;
        dstRectanguloEnvolvente.width += 10;
        dstRectanguloEnvolvente.x -= 5;
        dstRectanguloEnvolvente.y -= 5;

        rectangle(dstImageRectanguloEnvolvente,dstRectanguloEnvolvente,Scalar(255));

        //CALCULO CARACTERISTICAS
        cout<<"ancho "<<dstRectanguloEnvolvente.width<<endl;
        cout<<"alto "<<dstRectanguloEnvolvente.height<<endl;
        cout<<"x "<<dstRectanguloEnvolvente.x<<endl;
        cout<<"y "<<dstRectanguloEnvolvente.y<<endl;
        cout<<dstImageAdelgazada.rows<<endl;

        dstImageFinal = dstImageAdelgazada(dstRectanguloEnvolvente).clone();
        double relacionAnchoAlto = (double)dstImageFinal.cols/dstImageFinal.rows;

        vector<Point> endPoints;
        vector<Point> insersectPoints;
        ControlObtencionCaracteristicas::buscarPuntos(dstImageFinal,endPoints, insersectPoints);
        cout<<endPoints.size()<<endl;

        Mat dstImageMorph = ControlPreprocesamiento::morphImage(dstImageThreshold);
        vector<vector<Point> > contornos;

        contornos = ControlObtencionCaracteristicas::getContornos(dstImageMorph);
        vector<vector<double> > momentosHu = ControlObtencionCaracteristicas::getHuMoments(contornos);

    // POLIGONO ENVOLVENTE
       vector<Point > poligono = ControlObtencionCaracteristicas::getEnvolvingPolygon( contornos);
       Mat poligonoimagen = ControlObtencionCaracteristicas::getEnvolvingPolygonImage(srcImage, poligono);
       vector<vector<Point > > contornoPoligono = ControlObtencionCaracteristicas::getContornos(poligonoimagen);
           momentosHu = ControlObtencionCaracteristicas::getHuMoments(contornoPoligono);

     /*   cout<<momentosHu.at(0).at(0)<<","
            <<momentosHu.at(0).at(1)<<","
            <<momentosHu.at(0).at(2)<<","
            <<momentosHu.at(0).at(3)<<","
            <<momentosHu.at(0).at(4)<<","
            <<momentosHu.at(0).at(5)<<","
            <<momentosHu.at(0).at(6)<2<","
            <<endl;*/
        ///Determina a cual cuadrante pertenece cada punto
           int cuadEndPoints0 = 0,cuadEndPoints1 = 0, cuadEndPoints2 = 0, cuadEndPoints3 = 0,
                   cuadEndPoints4 = 0, cuadEndPoints5 = 0, cuadEndPoints6 = 0, cuadEndPoints7 = 0, cuadEndPoints8 = 0;
           int mitadX1 = int(dstImageFinal.cols/3);
           int mitadX2 = int(2*dstImageFinal.cols/3);
           int mitadY1 = int(dstImageFinal.rows/3);
           int mitadY2 = int(2*dstImageFinal.rows/3);
           Point p;

           //Calcular cuantos endPoints hay en cada cuadrante
           for(unsigned c = 0; c< endPoints.size();c++){
               p = endPoints.at(c);
               //0
               if( p.x <  mitadX1 && p.y <  mitadY1 ){ cuadEndPoints0++; continue;}
               //1
               if( p.x <  mitadX2 && p.y <  mitadY1 ){ cuadEndPoints1++; continue;}
               //2
               if( p.x >= mitadX2 && p.y <  mitadY1 ){ cuadEndPoints2++; continue;}
               //3
               if( p.x <  mitadX1 && p.y <  mitadY2 ){ cuadEndPoints3++; continue;}
               //4
               if( p.x <  mitadX2 && p.y <  mitadY2 ){ cuadEndPoints4++; continue;}
               //5
               if( p.x >= mitadX2 && p.y <  mitadY2 ){ cuadEndPoints5++; continue;}
               //6
               if( p.x <  mitadX1 && p.y >= mitadY2 ){ cuadEndPoints6++; continue;}
               //7
               if( p.x <  mitadX2 && p.y >= mitadY2 ){ cuadEndPoints7++; continue;}
               //8
               if( p.x >= mitadX2 && p.y >= mitadY2 ){ cuadEndPoints8++; continue;}
           }

           int cuadInterPoints0 = 0,cuadInterPoints1 = 0, cuadInterPoints2 = 0, cuadInterPoints3 = 0,
                   cuadInterPoints4 = 0, cuadInterPoints5 = 0, cuadInterPoints6 = 0, cuadInterPoints7 = 0, cuadInterPoints8 = 0;


           for(unsigned z = 0; z< insersectPoints.size();z++)
           {
               p = insersectPoints.at(z);
               //0
               if( p.x <  mitadX1 && p.y <  mitadY1 ){ cuadInterPoints0=1; continue;}
               //1
               if( p.x <  mitadX2 && p.y <  mitadY1 ){ cuadInterPoints1=1; continue;}
               //2
               if( p.x >= mitadX2 && p.y <  mitadY1 ){ cuadInterPoints2=1; continue;}
               //3
               if( p.x <  mitadX1 && p.y <  mitadY2 ){ cuadInterPoints3=1; continue;}
               //4
               if( p.x <  mitadX2 && p.y <  mitadY2 ){ cuadInterPoints4=1; continue;}
               //5
               if( p.x >= mitadX2 && p.y <  mitadY2 ){ cuadInterPoints5=1; continue;}
               //6
               if( p.x <  mitadX1 && p.y >= mitadY2 ){ cuadInterPoints6=1; continue;}
               //7
               if( p.x <  mitadX2 && p.y >= mitadY2 ){ cuadInterPoints7=1; continue;}
               //8
               if( p.x >= mitadX2 && p.y >= mitadY2 ){ cuadInterPoints8=1; continue;}
           }


        fprintf(archivoCaracteristicas,"%f;%f;%f;%f;%f;%f;%f;%f;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n",
              momentosHu.at(0).at(0),
              momentosHu.at(0).at(1),
              momentosHu.at(0).at(2),
              momentosHu.at(0).at(3),
              momentosHu.at(0).at(4),
              momentosHu.at(0).at(5),
              momentosHu.at(0).at(6),
              relacionAnchoAlto,
              endPoints.size(),//
              cuadEndPoints0,
              cuadEndPoints1,
              cuadEndPoints2,
              cuadEndPoints3,
              cuadEndPoints4,
              cuadEndPoints5,
              cuadEndPoints6,
              cuadEndPoints7,
              cuadEndPoints8,
//              cuadInterPoints0,
//              cuadInterPoints1,
//              cuadInterPoints2,
//              cuadInterPoints3,
//              cuadInterPoints4,
//              cuadInterPoints5,
//              cuadInterPoints6,
//              cuadInterPoints7,
//              cuadInterPoints8,
              contornos.size(),
              poligono.size(),
              imagesList.at(i).second);

    }

    fclose(archivoCaracteristicas);
}


/**
 * @brief DialogoCaracteristicas::getNumber
 * @param srcImage
 * @return
 */
int MainWindow::getNumber(){
    srcImage = imread(imageFile.toStdString().data(),0);
    line(srcImage,Point(0,0),Point(0,srcImage.rows),Scalar(255),20);
    line(srcImage,Point(0,0),Point(srcImage.cols,0),Scalar(255),20);
    line(srcImage,Point(srcImage.cols,srcImage.rows),Point(srcImage.cols,0),Scalar(255),20);
    line(srcImage,Point(srcImage.cols,srcImage.rows),Point(0,srcImage.rows),Scalar(255),20);

    equalizeHist(srcImage,srcImageEqualizada);
    //CALCULAR THRESHOLD
    this->dstImageThresholdAdaptative = ControlPreprocesamiento::umbralAutomaticoAdaptativo(srcImage);
    this->dstImageThreshold = ControlPreprocesamiento::umbralAutomatico(srcImageEqualizada);

    //FILTRADO
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(2,2));
    morphologyEx(this->dstImageThresholdAdaptative, this->dstImageClose, CV_MOP_CLOSE, BStructElement,Point(-1,-1) ,2 );

    //SEGMENTACION
    Mat src = imread(imageFile.toStdString().data());
    ControlSegmentacion::encontrarSegmentos(src,dstImageClose,dstImageSegmentacion,dstRectanguloEnvolvente);

    //ADELGAZAMIENTOsrcImage
    dstImageAdelgazada = Mat::zeros(dstImageClose.size(), CV_8UC1);
    dstImageClose.copyTo(dstImageAdelgazada);

    ControlPreprocesamiento::adelgazamiento(dstImageAdelgazada);

    dstImageAdelgazada.copyTo(dstImageRectanguloEnvolvente);
    //Se suman 5 pixeles de distancia a las medidas del rectangulo para darle espacio
    //alalgoritmo de busqueda de end-points
    //if(dstRectanguloEnvolvente.x <= 5 || dstRectanguloEnvolvente.y <= 5 ) continue;
    dstRectanguloEnvolvente.height += 10;
    dstRectanguloEnvolvente.width += 10;
    dstRectanguloEnvolvente.x -= 5;
    dstRectanguloEnvolvente.y -= 5;

    rectangle(dstImageRectanguloEnvolvente,dstRectanguloEnvolvente,Scalar(255));

    //CALCULO CARACTERISTICAS
    cout<<"ancho "<<dstRectanguloEnvolvente.width<<endl;
    cout<<"alto "<<dstRectanguloEnvolvente.height<<endl;
    cout<<"x "<<dstRectanguloEnvolvente.x<<endl;
    cout<<"y "<<dstRectanguloEnvolvente.y<<endl;
    cout<<dstImageAdelgazada.rows<<endl;

    dstImageFinal = dstImageAdelgazada(dstRectanguloEnvolvente).clone();
    double relacionAnchoAlto = (double)dstImageFinal.cols/dstImageFinal.rows;

    vector<Point> endPoints;
    vector<Point> insersectPoints;
    ControlObtencionCaracteristicas::buscarPuntos(dstImageFinal,endPoints, insersectPoints);
    cout<<endPoints.size()<<endl;

    Mat dstImageMorph = ControlPreprocesamiento::morphImage(dstImageThreshold);
    vector<vector<Point> > contornos;

    contornos = ControlObtencionCaracteristicas::getContornos(dstImageMorph);
    vector<vector<double> > momentosHu = ControlObtencionCaracteristicas::getHuMoments(contornos);

// POLIGONO ENVOLVENTE
   vector<Point > poligono = ControlObtencionCaracteristicas::getEnvolvingPolygon( contornos);
   Mat poligonoimagen = ControlObtencionCaracteristicas::getEnvolvingPolygonImage(srcImage, poligono);
   vector<vector<Point > > contornoPoligono = ControlObtencionCaracteristicas::getContornos(poligonoimagen);
       momentosHu = ControlObtencionCaracteristicas::getHuMoments(contornoPoligono);

 /*   cout<<momentosHu.at(0).at(0)<<","
        <<momentosHu.at(0).at(1)<<","
        <<momentosHu.at(0).at(2)<<","
        <<momentosHu.at(0).at(3)<<","
        <<momentosHu.at(0).at(4)<<","
        <<momentosHu.at(0).at(5)<<","
        <<momentosHu.at(0).at(6)<2<","
        <<endl;*/
    ///Determina a cual cuadrante pertenece cada punto
       int cuadEndPoints0 = 0,cuadEndPoints1 = 0, cuadEndPoints2 = 0, cuadEndPoints3 = 0,
               cuadEndPoints4 = 0, cuadEndPoints5 = 0, cuadEndPoints6 = 0, cuadEndPoints7 = 0, cuadEndPoints8 = 0;
       int mitadX1 = int(dstImageFinal.cols/3);
       int mitadX2 = int(2*dstImageFinal.cols/3);
       int mitadY1 = int(dstImageFinal.rows/3);
       int mitadY2 = int(2*dstImageFinal.rows/3);
       Point p;

       //Calcular cuantos endPoints hay en cada cuadrante
       for(unsigned c = 0; c< endPoints.size();c++){
           p = endPoints.at(c);
           //0
           if( p.x <  mitadX1 && p.y <  mitadY1 ){ cuadEndPoints0++; continue;}
           //1
           if( p.x <  mitadX2 && p.y <  mitadY1 ){ cuadEndPoints1++; continue;}
           //2
           if( p.x >= mitadX2 && p.y <  mitadY1 ){ cuadEndPoints2++; continue;}
           //3
           if( p.x <  mitadX1 && p.y <  mitadY2 ){ cuadEndPoints3++; continue;}
           //4
           if( p.x <  mitadX2 && p.y <  mitadY2 ){ cuadEndPoints4++; continue;}
           //5
           if( p.x >= mitadX2 && p.y <  mitadY2 ){ cuadEndPoints5++; continue;}
           //6
           if( p.x <  mitadX1 && p.y >= mitadY2 ){ cuadEndPoints6++; continue;}
           //7
           if( p.x <  mitadX2 && p.y >= mitadY2 ){ cuadEndPoints7++; continue;}
           //8
           if( p.x >= mitadX2 && p.y >= mitadY2 ){ cuadEndPoints8++; continue;}
       }

       int cuadInterPoints0 = 0,cuadInterPoints1 = 0, cuadInterPoints2 = 0, cuadInterPoints3 = 0,
               cuadInterPoints4 = 0, cuadInterPoints5 = 0, cuadInterPoints6 = 0, cuadInterPoints7 = 0, cuadInterPoints8 = 0;


       for(unsigned z = 0; z< insersectPoints.size();z++)
       {
           p = insersectPoints.at(z);
           //0
           if( p.x <  mitadX1 && p.y <  mitadY1 ){ cuadInterPoints0=1; continue;}
           //1
           if( p.x <  mitadX2 && p.y <  mitadY1 ){ cuadInterPoints1=1; continue;}
           //2
           if( p.x >= mitadX2 && p.y <  mitadY1 ){ cuadInterPoints2=1; continue;}
           //3
           if( p.x <  mitadX1 && p.y <  mitadY2 ){ cuadInterPoints3=1; continue;}
           //4
           if( p.x <  mitadX2 && p.y <  mitadY2 ){ cuadInterPoints4=1; continue;}
           //5
           if( p.x >= mitadX2 && p.y <  mitadY2 ){ cuadInterPoints5=1; continue;}
           //6
           if( p.x <  mitadX1 && p.y >= mitadY2 ){ cuadInterPoints6=1; continue;}
           //7
           if( p.x <  mitadX2 && p.y >= mitadY2 ){ cuadInterPoints7=1; continue;}
           //8
           if( p.x >= mitadX2 && p.y >= mitadY2 ){ cuadInterPoints8=1; continue;}
       }


       ///CREAR RED NEURONAL Y ENVIAR VECTOR DE CARACTERISTICAS
       controlredneuronal red = controlredneuronal("./NeuralNetwork.xml");
       cv::Mat caracteristicas(1,20,CV_32F);


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
       caracteristicas.at<float>(0, 18) = contornos.size();
       caracteristicas.at<float>(0, 19) = poligono.size();

       return red.predict( caracteristicas );
}

