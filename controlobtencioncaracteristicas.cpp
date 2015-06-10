#include "controlobtencioncaracteristicas.h"

ControlObtencionCaracteristicas::ControlObtencionCaracteristicas()
{

}

ControlObtencionCaracteristicas::~ControlObtencionCaracteristicas()
{

}


/**
 * @brief ControlPreprocesamiento::getContornosImage Retorna una imagen con los contornos dibujados.
 * @param src_gray Imagen de referencia para obtener el tamaño de la imágen de salida
 * @param contours Vector con los contornos.
 * @return
 */
Mat ControlObtencionCaracteristicas::getContornosImage(Mat src_gray, vector<vector<Point> > contours){
    Mat drawing = Mat::zeros( src_gray.size(), CV_8UC3 );
    RNG rng(12345);
    for( unsigned i = 0; i< contours.size(); i++ )
       {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         //drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
         drawContours( drawing, contours, i, color, 1, 8, noArray(), 0, Point() );
         //circle( drawing, mc[i], 4, color, -1, 8, 0 );
       }
    return drawing;
}


vector<vector<double> > ControlObtencionCaracteristicas::getHuMoments( vector<vector<Point> >  contours){

    vector<vector<double> > HuMoments;
    HuMoments.resize(contours.size());

    for( unsigned i = 0; i< contours.size(); i++ )
       {
        // From the example you posted, you already know how to compute the contours
        // so let's take just the first one and compute the moments
        cv::Moments mom = cv::moments(contours[i]);
        double hu[7];
        cv::HuMoments(mom, hu); // now in hu are your 7 Hu-Moments

        //resize Vector and copy hu Moments Values to the vector
        //cout<<"Contourn "<<i<<"\n";
        HuMoments[i].resize(7);
        for(int c = 0; c < 7; c++ ){
            HuMoments[i][c] = hu[c];
            //cout<<"\tHu["<<c<<"]="<<hu[c]<<"\n";
        }
       }

    return HuMoments;
}


void ControlObtencionCaracteristicas::buscarPuntos(Mat srcImage, vector<Point> &endPoints, vector<Point> &intersectPoints)
{



    Mat srcImageNormalizada = srcImage/255;
    vector<Point> endPointsEncontrados;
    vector<Point> intersectPointsEncontrados;
    int tamanoVecindad = 9;
    for(int i = 0; i < srcImageNormalizada.rows-2; i++)
    {
        for(int j =0; j < srcImageNormalizada.cols-2; j++)
        {
            uchar vecindad9[tamanoVecindad];
            vecindad9[0] = srcImageNormalizada.at<uchar>(i,j);
            vecindad9[1] = srcImageNormalizada.at<uchar>(i,j+1);
            vecindad9[2] = srcImageNormalizada.at<uchar>(i,j+2);
            vecindad9[3] = srcImageNormalizada.at<uchar>(i+1,j);
            vecindad9[4] = srcImageNormalizada.at<uchar>(i+1,j+1);
            vecindad9[5] = srcImageNormalizada.at<uchar>(i+1,j+2);
            vecindad9[6] = srcImageNormalizada.at<uchar>(i+2,j);
            vecindad9[7] = srcImageNormalizada.at<uchar>(i+2,j+1);
            vecindad9[8] = srcImageNormalizada.at<uchar>(i+2,j+2);

            int valorAbsoluto = 0;

            for(int k=0; k < tamanoVecindad; k++)
            {
                valorAbsoluto += vecindad9[k];
            }

            if(vecindad9[4] == 1){



                //END POINTS
                if(valorAbsoluto==2)
                {
                    endPointsEncontrados.push_back(Point(j+1, i+1));
                    //             i+=2;
                }

                //INTERSECCIONES
                if(valorAbsoluto == 5 && vecindad9[0]== 0 && vecindad9[2]== 0 && vecindad9[6]== 0 && vecindad9[8]== 0 ){
                    intersectPointsEncontrados.push_back(Point(j+1, i+1) );
                }

//                if(valorAbsoluto >= 4)
//                {
//                    intersectPointsEncontrados.push_back(Point(j+1, i+1) );

//                }

                if(valorAbsoluto == 4 &&
                        ((vecindad9[3]==1 && vecindad9[1] == 1 && (vecindad9[5] == 1 || vecindad9[7] ==1 || vecindad9[8] ==1)) ||
                         (vecindad9[1]==1 && vecindad9[5] == 1 && (vecindad9[3] == 1 || vecindad9[6] ==1 || vecindad9[7] ==1)) ||
                         (vecindad9[3]==1 && vecindad9[7] == 1 && (vecindad9[1] == 1 || vecindad9[2] ==1 || vecindad9[5] ==1)) ||
                         (vecindad9[7]==1 && vecindad9[5] == 1 && (vecindad9[0] == 1 || vecindad9[1] ==1 || vecindad9[3] ==1)) ||
                         //diagonales
                         (vecindad9[6]==1 && vecindad9[8] == 1 && (vecindad9[0] == 1 || vecindad9[1] ==1 || vecindad9[2] ==1)) ||
                         (vecindad9[8]==1 && vecindad9[2] == 1 && (vecindad9[0] == 1 || vecindad9[3] ==1 || vecindad9[6] ==1)) ||
                         (vecindad9[2]==1 && vecindad9[0] == 1 && (vecindad9[6] == 1 || vecindad9[7] ==1 || vecindad9[8] ==1)) ||
                         (vecindad9[0]==1 && vecindad9[6] == 1 && (vecindad9[2] == 1 || vecindad9[5] ==1 || vecindad9[8] ==1))

                         ))
                {
                    intersectPointsEncontrados.push_back(Point(j+1, i+1) );

                }

//                //ESQUINAS
//                if(valorAbsoluto == 3 &&
//                        ((vecindad9[1]==1 && vecindad9[3] == 1) ||
//                         (vecindad9[3]==1 && vecindad9[7] == 1) ||
//                         (vecindad9[7]==1 && vecindad9[5] == 1) ||
//                         (vecindad9[5]==1 && vecindad9[1] == 1)))


//                {
//                    cornerPointsEncontrados.push_back(Point(j+1,i+1));
//                }
            }
            else  continue;

        }
    }


    endPoints = endPointsEncontrados;
    intersectPoints = intersectPointsEncontrados;

}


vector<vector<Point> > ControlObtencionCaracteristicas::getContornos( Mat src_gray){
   Mat canny_output;
   vector<vector<Point> > contours;
   vector<Vec4i> hierarchy;
   vector<vector<Point> > ret;
   ret.resize(1);
   //int size = 0;
   int thresh = 100;

   /// Detect edges using canny
   Canny( src_gray, canny_output, thresh, thresh*2, 3 );
   /// Find contours
   findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
/*
   //Add the contours to only one vector.
   for( int i = 0; i< contours.size(); i++){
      ret[0].insert(ret[0].end(), contours[i].begin(), contours[i].end());
      //cout<<"Size Vector: "<<i<<" IS " << contours[i].size()<<"\n";
      //size += contours[i].size();
   }

   //cout<<"Size Total Vector: IS "<< ret[0].size()<<" and the total size IS "<<size<<"\n";

   return ret;
   */
   return contours;
}


/**
 * @brief ControlObtencionCaracteristicas::getEnvolvingPolygon Returns a convez polygon evolving all the points
 * @param contours
 * @return
 */
vector<Point > ControlObtencionCaracteristicas::getEnvolvingPolygon( vector<vector< Point > >  contours){


  //Colocar los puntos del contorno en un solo vector
    vector<Point> contoursPoints;
    for(unsigned int k = 0; k < contours.size();k++){
        for(unsigned int l = 0; l < contours[k].size();l++)
        {
            contoursPoints.push_back(contours[k][l]);
        }
    }

    vector<Point> approxCurve;
    convexHull( contoursPoints,  approxCurve);

    return approxCurve;

    ///////////////////////////////////////
}

/**
 * @brief ControlObtencionCaracteristicas::getEnvolvingPolygonImage: returns a Black image with the white plygon
 * formed by the points
 * @param src_gray
 * @param points
 * @return
 */
Mat ControlObtencionCaracteristicas::getEnvolvingPolygonImage(Mat src_gray, vector<Point > points){
    Mat drawing = Mat::zeros( src_gray.size(), CV_8UC3 );

    fillConvexPoly(drawing,points,255);

    return drawing;

}


