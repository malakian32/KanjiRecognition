#include "controlpreprocesamiento.h"

ControlPreprocesamiento::ControlPreprocesamiento()
{

}

ControlPreprocesamiento::~ControlPreprocesamiento()
{

}

void ControlPreprocesamiento::iteracionAdelgazamiento(Mat& srcImage, int iteracion )
{

    Mat marker = Mat::zeros(srcImage.size(), CV_8UC1);

    for (int i = 1; i < srcImage.rows-1; i++)
    {
        for (int j = 1; j < srcImage.cols-1; j++)
        {
            uchar p2 = srcImage.at<uchar>(i-1, j);
            uchar p3 = srcImage.at<uchar>(i-1, j+1);
            uchar p4 = srcImage.at<uchar>(i, j+1);
            uchar p5 = srcImage.at<uchar>(i+1, j+1);
            uchar p6 = srcImage.at<uchar>(i+1, j);
            uchar p7 = srcImage.at<uchar>(i+1, j-1);
            uchar p8 = srcImage.at<uchar>(i, j-1);
            uchar p9 = srcImage.at<uchar>(i-1, j-1);

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iteracion == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iteracion == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    srcImage &= ~marker;
}

void ControlPreprocesamiento::adelgazamiento(Mat& srcImage)
{
    srcImage /= 255;

    Mat prev = Mat::zeros(srcImage.size(), CV_8UC1);
    Mat diff;

    do {
        iteracionAdelgazamiento(srcImage, 0);
        iteracionAdelgazamiento(srcImage, 1);
        cv::absdiff(srcImage, prev, diff); //Calcula la diferencia entre la imagen fuente luego de dos iteraciones y su version previa
        srcImage.copyTo(prev);
    }
    while (cv::countNonZero(diff) > 0); //Evalua que el ciclo se repita siempre que se adelgace en la iteración

    srcImage *= 255;
}

/***************** FUNCIONES PARA EL PROCESAMIENTO DE IMAGENES ******************************/
Mat  ControlPreprocesamiento::umbralAutomatico( Mat sourceImage){
    Mat umbralImage;
    adaptiveThreshold(sourceImage, umbralImage, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV,3,5);
    //threshold(sourceImage , umbralImage, 0 ,255,CV_THRESH_BINARY_INV);
    return umbralImage;
}

Mat  ControlPreprocesamiento::morphImage( Mat sourceImage){

    Mat morphedImage;// = morphImage( thresholdImage );
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(3,3));

    morphologyEx( sourceImage, morphedImage, CV_MOP_ERODE, BStructElement );
    threshold(morphedImage, morphedImage, 100,255, CV_THRESH_BINARY_INV);

    return morphedImage;
}
