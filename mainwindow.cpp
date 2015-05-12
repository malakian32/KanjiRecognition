#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calcularUmbralBT_clicked()
{
    int umbral = ui->umbralSP->value();
    cout<<umbral<<endl;


    Mat umbralImage;

    threshold(srcImage,umbralImage,umbral,255,CV_THRESH_BINARY);

    imshow("UMBRAL",umbralImage);
    imshow("ORIGINAL",srcImage);


}

//When Clic on Abrir Imagen
void MainWindow::on_abrirImagenBT_clicked()
{
    this->imageFile = QFileDialog::getOpenFileName(this,tr("Open Image"),"./resources",tr("Image Files(*.png *.jpg *.bmp)"));

    this->ui->filePathLE->setText(imageFile);
    QPixmap imagePixel(imageFile);
    ui->imageLabelLB->setPixmap(imagePixel);

    ui->calcularUmbralBT->setEnabled(true);
    ui->umbralSP->setEnabled(true);

    this->srcImage = imread(imageFile.toStdString().data(),0);

}

void MainWindow::on_aperturaBT_clicked()
{
    int size = 3;
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(3,3));
    Mat dstOpening;

    erode(srcImage,dstOpening,BStructElement);

    imshow("DILATE",dstOpening);
}

void MainWindow::on_cierreBT_clicked()
{

}



void MainWindow::on_GO_clicked()
{



    String path = "/home/snipercat/Desktop/ETL8/images/GO/GO.ITSUT_000.png";
    //const char* image = ;
    Mat imagenBW = imread( path , CV_LOAD_IMAGE_GRAYSCALE );// cargamos la imagen en Blanco y negros

    /* //Cambiar el Tamaño de la imagen en caso de que sea muy grande
     * int maxheight = ui->maxHeight->value();
    if(maxheight !=0 && imagenBW->height > maxheight){
        IplImage *resizedImage = cvCreateImage( cvSize( maxheight * (imagenBW->width/(imagenBW->height)) , maxheight) , imagenBW->depth , imagenBW->nChannels);
        cvResize(imagenBW, resizedImage, CV_INTER_LINEAR);
        imagenBW = resizedImage;
    }
    */

    imshow("ORIGINAL",imagenBW);

//Threshold
    Mat thresholdImage = umbralAutomatico(imagenBW);
    imshow( "Binarizada", thresholdImage ); // representamos la imagen en la ventana

//Morphologia
    Mat morphedImage =  morphImage(thresholdImage);
    imshow( "Morphed", morphedImage ); // representamos la imagen en la ventana

//Esqueletización
    Mat skeletonImage = skeleton(morphedImage);
    imshow( "skeleton", skeletonImage ); // representamos la imagen en la ventana


}


/***************** FUNCIONES PARA EL PROCESAMIENTO DE IMAGENES ******************************/
Mat  MainWindow::umbralAutomatico( Mat sourceImage){
    Mat umbralImage;
    adaptiveThreshold(sourceImage, umbralImage, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,3,5);
    //threshold(sourceImage , umbralImage, 170 ,255,CV_THRESH_BINARY);
    return umbralImage;
}

Mat  MainWindow::morphImage( Mat sourceImage){

    Mat morphedImage;// = morphImage( thresholdImage );
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(3,3));

    morphologyEx( sourceImage, morphedImage, CV_MOP_ERODE, BStructElement );
    threshold(morphedImage, morphedImage, 100,255, CV_THRESH_BINARY_INV);

    return morphedImage;
}

Mat  MainWindow::skeleton( Mat sourceImage){
    Mat img( sourceImage);

    cv::Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
    cv::Mat temp(img.size(), CV_8UC1);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

    bool done;
    do
    {
      cv::morphologyEx(img, temp, cv::MORPH_OPEN, element);
      cv::bitwise_not(temp, temp);
      cv::bitwise_and(img, temp, temp);
      cv::bitwise_or(skel, temp, skel);
      cv::erode(img, img, element);

      double max;
      cv::minMaxLoc(img, 0, &max);
      done = (max == 0);
    } while (!done);


    return skel;

}
