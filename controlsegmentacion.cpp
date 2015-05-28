 #include "controlsegmentacion.h"

ControlSegmentacion::ControlSegmentacion()
{

}

ControlSegmentacion::~ControlSegmentacion()
{

}

void ControlSegmentacion::encontrarSegmentos(Mat srcImage, Mat srcImagePreprocesada, Mat& dstImage, Rect& calcRect)
{
    Mat dist;

    //Se calcula la transformada de la distancia,
    //para encontrar los picos que seran las semillas para el algoritmo watershed
    distanceTransform(srcImagePreprocesada,dist,CV_DIST_L2,3);
    normalize(dist,dist,0,1.,NORM_MINMAX );
    threshold(dist,dist,.5,1.,CV_THRESH_BINARY);

    /*contornos:
    Creamos la version CV_8U de la imagen dist y encontramos los contornos
    Estas son las semillas para los Kanjis
    */
    Mat dist8U;
    dist.convertTo(dist8U,CV_8U);

    vector<vector <Point> > contours;
    findContours(dist8U, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    int nComp = contours.size();

    /* Ahora generamos una semilla para los objetos encontrados por los contornos,
    suponiendo que el kanji esta en el centro creamos la semilla 5 pixeles cerca al borde para el fondo
    */

    Mat markers = Mat::zeros(dist.size(),CV_32SC1);
    for(int i = 0; i < nComp; i++){
        drawContours(markers,contours,i,Scalar::all(i+1),-1 );
    }

    circle(markers, Point(5,5), 3, CV_RGB(255,255,255),-1);

    //Watershed:

    watershed(srcImage,markers);

    std::vector<cv::Vec3b> colors;
    for (int i = 0; i < nComp; i++)
    {
        int b = cv::theRNG().uniform(0, 255);
        int g = cv::theRNG().uniform(0, 255);
        int r = cv::theRNG().uniform(0, 255);

        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // Create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);

    // F
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= nComp)
                dst.at<cv::Vec3b>(i,j) = colors[index-1];
            else
                dst.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
        }
    }
    cout<<contours.size()<<endl;

    vector<Point> puntosRectEnvolvente;
    for(unsigned int k = 0; k < contours.size();k++){
        for(unsigned int l = 0; l < contours[k].size();l++)
        {
            puntosRectEnvolvente.push_back(contours[k][l]);
        }
    }
    calcRect = boundingRect(puntosRectEnvolvente);

    dstImage = dst;

}
