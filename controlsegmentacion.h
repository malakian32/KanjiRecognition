#ifndef CONTROLSEGMENTACION_H
#define CONTROLSEGMENTACION_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;



class ControlSegmentacion
{
public:
    static void encontrarSegmentos(Mat srcImage, Mat srcImagePreprocesada, Mat& dstImage, Rect& calcRect);
    ControlSegmentacion();
    ~ControlSegmentacion();
};

#endif // CONTROLSEGMENTACION_H
