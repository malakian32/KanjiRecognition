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
    static void buscarPuntos(Mat srcImage, vector<Point>& endPoints, vector<Point> &intersectPoints);

    static vector<vector<Point> > getContornos( Mat src_gray);
    static vector<vector<double> > getHuMoments( vector<vector<Point> >  contours);
    static Mat  getContornosImage(Mat src_gray, vector<vector<Point> > contours);

    static vector<Point >  getEnvolvingPolygon( vector<vector< Point > >  contours);
    static Mat getEnvolvingPolygonImage(Mat src_gray, vector<Point > points);
};

#endif // CONTROLOBTENCIONCARACTERISTICAS_H
