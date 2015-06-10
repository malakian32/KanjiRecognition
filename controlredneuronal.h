#ifndef CONTROLREDNEURONAL_H
#define CONTROLREDNEURONAL_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/opencv.hpp"         // opencv general include file
#include "opencv2/ml/ml.hpp"          // opencv machine learning include file

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;


#define TRAINING_SAMPLES 1196    //Number of samples in training dataset
#define ATTRIBUTES 20  		  //Number of hu moments
#define TEST_SAMPLES 732       //Number of samples in test dataset
#define CLASSES 12             //Number of distinct labels. 1 or 0
#define HIDDEN_NEURONES 30       //NUMBER OF HIDDEN NEURONES


class controlredneuronal
{
public:
    controlredneuronal();
    controlredneuronal(char *srcNetworkFile);
    //FUNCIONES PARA RED NEURONAL
    int predict( cv::Mat data );
    static int train(char *DataSet, char *TestDataSet, char *dstNetworkFile);




private:

    CvANN_MLP NeuralNetwork;// Red Neuronal

    static void read_trainingDataset( char *filename, cv::Mat &data, cv::Mat &classes, int total_samples );
    static int  returnValue( cv::Mat outNetwork);

};

#endif // CONTROLREDNEURONAL_H
