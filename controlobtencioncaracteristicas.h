#ifndef CONTROLOBTENCIONCARACTERISTICAS_H
#define CONTROLOBTENCIONCARACTERISTICAS_H



#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

class ControlObtencionCaracteristicas
{
public:
    ControlObtencionCaracteristicas();
    ~ControlObtencionCaracteristicas();
    static void buscarEndPoints(Mat srcImage, vector<Point>& endPoints);

};

#endif // CONTROLOBTENCIONCARACTERISTICAS_H
