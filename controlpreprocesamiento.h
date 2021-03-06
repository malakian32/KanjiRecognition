#ifndef CONTROLPREPROCESAMIENTO_H
#define CONTROLPREPROCESAMIENTO_H


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;


class ControlPreprocesamiento
{
public:
    ControlPreprocesamiento();
    ~ControlPreprocesamiento();

    //FUNCIONES PARA PROCESAMIENTO
    static Mat umbralAutomatico( Mat sourceImage);
    static Mat umbralAutomaticoAdaptativo(Mat sourceImage);
    static Mat morphImage( Mat sourceImage);
    static void adelgazamiento(Mat& srcImage);


private:
    static void iteracionAdelgazamiento(Mat& srcImage, int iteracion );

};

#endif // CONTROLPREPROCESAMIENTO_H
