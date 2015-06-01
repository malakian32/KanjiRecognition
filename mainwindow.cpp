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



    this->srcImage = imread(imageFile.toStdString().data(),0);
        cv::equalizeHist(this->srcImage,  this->srcImage );
    this->ImagenAbierta = true;
}

void MainWindow::on_calcularUmbralBT_clicked()
{
    this->dstImageTreshold = ControlPreprocesamiento::umbralAutomaticoAdaptativo(srcImage);


    QImage umbralImage = Mat2QImage(dstImageTreshold);
    ui->UmbralizacionLB->setPixmap(QPixmap::fromImage(umbralImage));

    ui->AperturaBT->setEnabled(true);

}


void MainWindow::on_AperturaBT_clicked()
{
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(2,2));
    //erode(dstImageTreshold,dstImageOpening,Mat(),cv::Point(-1,-1),2);
    morphologyEx(this->dstImageTreshold, this->dstImageClose, CV_MOP_DILATE, BStructElement,Point(-1,-1) ,2 );
    //morphologyEx(this->dstImageClose, this->dstImageOpening, CV_MOP_OPEN, BStructElement );
    dstImageClose = ControlPreprocesamiento::morphImage(this->dstImageTreshold);
   // imshow("APERTURA",this->dstImageOpening);
    QImage filtradoImage = Mat2QImage(dstImageClose);
    ui->FiltradoMorfologicoLB->setPixmap(QPixmap::fromImage(filtradoImage));

    ui->SegmentacionBT->setEnabled(true);

}

void MainWindow::on_cierreBT_clicked()
{
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(3,3));

    morphologyEx(this->dstImageOpening, this->dstImageClose, CV_MOP_CLOSE, BStructElement );

    imshow("CIERRE",this->dstImageClose);
}


/**
 * @brief MainWindow::on_GO_clicked REALIZA EL PROCESAMIENTO AUMÁTICO PARA UNA IMAGEN
 */

void MainWindow::on_GO_clicked()
{
/*

    //Path de la imágen
    String path = "/home/snipercat/Desktop/ETL8/images/GO/000.png";

    this->srcImage = imread( path , CV_LOAD_IMAGE_GRAYSCALE );// cargamos la imagen en Blanco y negros

    /*Cambiar el Tamaño de la imagen en caso de que sea muy grande
     * int maxheight = ui->maxHeight->value();
    if(maxheight !=0 && imagenBW->height > maxheight){
        IplImage *resizedImage = cvCreateImage( cvSize( maxheight * (imagenBW->width/(imagenBW->height)) , maxheight) , imagenBW->depth , imagenBW->nChannels);
        cvResize(imagenBW, resizedImage, CV_INTER_LINEAR);
        imagenBW = resizedImage;
    }


    //Colocar la imágen original en el recuadro
    QImage originalImage = Mat2QImage(this->srcImage);
    ui->ImagenOriginalLB->setPixmap(QPixmap::fromImage(originalImage));


    //Equalize
        Mat equalizedImage;
        cv::equalizeHist(this->srcImage, equalizedImage);

        //imshow( "Equalize", equalizedImage ); // representamos la imagen en la ventana

    //Threshold
        this->dstImageTreshold = ControlPreprocesamiento::umbralAutomatico(equalizedImage);
            //Colocar la imágen en el recuadro
            QImage thresholdlImage = Mat2QImage(this->dstImageTreshold);
            ui->UmbralizacionLB->setPixmap(QPixmap::fromImage(thresholdlImage));

            //imshow( "Binarizada", dstImageTreshold ); // representamos la imagen en la ventana

    //Morphologia
        this->dstImageClose =  ControlPreprocesamiento::morphImage(this->dstImageTreshold);
            //Colocar la imágen en el recuadro
            QImage morphedImage = Mat2QImage(this->dstImageClose);
            ui->FiltradoMorfologicoLB->setPixmap(QPixmap::fromImage(morphedImage));
            //imshow( "Morphed", this->dstImageClose ); // representamos la imagen en la ventana

    //Bordes
        vector<vector<Point> > contours = ControlPreprocesamiento::getContornos( this->dstImageClose);
        this->dstImageContornos= ControlPreprocesamiento::getContornosImage(this->dstImageClose,contours);
            //Colocar la imágen en el recuadro
                QImage contoursImage = Mat2QImage( this->dstImageContornos);
                ui->ContornosLB->setPixmap(QPixmap::fromImage(contoursImage));
    //cout<<"Contornos "<<contours.size()<<"\n";

    //MOMENTOS DE HU
        vector<vector<double> > HuMoments = ControlPreprocesamiento::getHuMoments(contours);


/////////////////////////
   ///SEGMENTACION
        Mat src = imread(path);
            ControlSegmentacion::encontrarSegmentos(src,dstImageClose,dstImageSegmentacion,dstRectanguloEnvolvente);

            QImage segmentacionImage = Mat2QImage(this->dstImageSegmentacion);
            ui->SegmentacionLB->setPixmap(QPixmap::fromImage(segmentacionImage));
            ui->CaracteristicasBT->setEnabled(true);
    ///ADELGAZAMIENTO
            dstImageAdelgazada = Mat::zeros(dstImageClose.size(), CV_8UC1);
            dstImageClose.copyTo(dstImageAdelgazada);
            ControlPreprocesamiento::adelgazamiento(dstImageAdelgazada);
            //Colocar la imágen en el recuadro
                QImage adelgazamientoImage = Mat2QImage(dstImageAdelgazada);
                ui->AdelgazamientoLB->setPixmap(QPixmap::fromImage(adelgazamientoImage));

//////////////////////
*/
}


void MainWindow::on_AdelgazamientoBT_clicked()
{
    dstImageAdelgazada = Mat::zeros(dstImageClose.size(), CV_8UC1);
    dstImageClose.copyTo(dstImageAdelgazada);

    ControlPreprocesamiento::adelgazamiento(dstImageAdelgazada);

    dstImageAdelgazada.copyTo(dstImageRectanguloEnvolvente);
    //Se suman 3 pixeles de distancia a las medidas del rectangulo para darle espacio
    //alalgoritmo de busqueda de end-points
    dstRectanguloEnvolvente.height += 6;
    dstRectanguloEnvolvente.width += 6;
    dstRectanguloEnvolvente.x -= 3;
    dstRectanguloEnvolvente.y -= 3;

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
/*
void MainWindow::on_CaracteristicasBT_clicked()
{

    //Bordes
        vector<vector<Point> > contours;
        contours = ControlPreprocesamiento::getContornos(this->dstImageClose);
    //MOMENTOS DE HU
        vector<vector<double> > HuMoments = ControlPreprocesamiento::getHuMoments(contours);
        cout<<"HU MOMENTS END;";

}
*/

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
    DialogoCaracteristicas* dialogoCaracteristicas = new DialogoCaracteristicas(this,this,srcImage,dstImageAdelgazada,dstRectanguloEnvolvente);

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

    ui->URLFicherLB->setText(ficheroAbierto);

}
