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
    //adaptiveThreshold(sourceImage, umbralImage, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV,3,5);
    threshold(sourceImage , umbralImage, 170 ,255,CV_THRESH_BINARY);
    return umbralImage;
}

Mat  ControlPreprocesamiento::morphImage( Mat sourceImage){

    Mat morphedImage;// = morphImage( thresholdImage );
    Mat BStructElement = getStructuringElement(CV_SHAPE_RECT,Size(3,3));

    morphologyEx( sourceImage, morphedImage, CV_MOP_ERODE, BStructElement );
    threshold(morphedImage, morphedImage, 100,255, CV_THRESH_BINARY_INV);

    return morphedImage;




}

Mat  ControlPreprocesamiento::skeleton( Mat sourceImage){
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

vector<vector<Point> > ControlPreprocesamiento::getContornos( Mat src_gray){
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int thresh = 100;

    /// Detect edges using canny
    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

   /*
    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    RNG rng(12345);
    for( int i = 0; i< contours.size(); i++ )
       {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
         //circle( drawing, mc[i], 4, color, -1, 8, 0 );
       }

    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
*/
    return contours;
}

/**
 * @brief ControlPreprocesamiento::getContornosImage Retorna una imagen con los contornos dibujados.
 * @param src_gray Imagen de referencia para obtener el tamaño de la imágen de salida
 * @param contours Vector con los contornos.
 * @return
 */
Mat ControlPreprocesamiento::getContornosImage(Mat src_gray, vector<vector<Point> > contours){
    Mat drawing = Mat::zeros( src_gray.size(), CV_8UC3 );
    RNG rng(12345);
    for( int i = 0; i< contours.size(); i++ )
       {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         //drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
         drawContours( drawing, contours, i, color, 1, 8, noArray(), 0, Point() );
         //circle( drawing, mc[i], 4, color, -1, 8, 0 );
       }
    return drawing;
}


vector<vector<double> > ControlPreprocesamiento::getHuMoments( vector<vector<Point> >  contours){

    vector<vector<double> > HuMoments;
    HuMoments.resize(contours.size());

    for( int i = 0; i< contours.size(); i++ )
       {
        // From the example you posted, you already know how to compute the contours
        // so let's take just the first one and compute the moments
        cv::Moments mom = cv::moments(contours[i]);
        double hu[7];
        cv::HuMoments(mom, hu); // now in hu are your 7 Hu-Moments

        //resize Vector and copy hu Moments Values to the vector
        cout<<"Contourn "<<i<<"\n";
        HuMoments[i].resize(7);
        for(int c = 0; c < 7; c++ ){
            HuMoments[i][c] = hu[c];
            cout<<"\tHu["<<c<<"]="<<hu[c]<<"\n";
        }
       }

    return HuMoments;
}

