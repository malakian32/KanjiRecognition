#include "controlobtencioncaracteristicas.h"

ControlObtencionCaracteristicas::ControlObtencionCaracteristicas()
{

}

ControlObtencionCaracteristicas::~ControlObtencionCaracteristicas()
{

}

void ControlObtencionCaracteristicas::buscarEndPoints(Mat srcImage, vector<Point> &endPoints)
{
    Mat srcImageNormalizada = srcImage/255;
    vector<Point> endPointsEncontrados;
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

            if(valorAbsoluto==2 && vecindad9[4]==1)
            {
                endPointsEncontrados.push_back(Point(j+1, i+1));
   //             i+=2;
            }
            else continue;

        }
    }


//    for(int i = 0; i < srcImage.rows-1; i++)
//    {
//        for(int j = 0; j < srcImage.cols-1;j++)
//        {
//            uchar vecindad4[4];
//            vecindad4[0] = srcImageNormalizada.at<uchar>(i,j);
//            vecindad4[1] = srcImageNormalizada.at<uchar>(i,j+1);
//            vecindad4[2] = srcImageNormalizada.at<uchar>(i+1,j);
//            vecindad4[3] = srcImageNormalizada.at<uchar>(i+1,j+1);

//            int valorAbsoluto = vecindad4[0]+vecindad4[1]+vecindad4[2]+vecindad4[3];
//            //cout<<valorAbsoluto<<endl;
//            if(valorAbsoluto == 1)
//            {
//                uchar endPoint = 0;
//                int posicion = 0;

//                while (endPoint == 0)
//                {
//                    endPoint += vecindad4[posicion];
//                    posicion++;
//                }

//                posicion--;
//                endPointsEncontrados.push_back(Point(j+(posicion & 0x01), i+((posicion & 0x02)>>1)));
//                j++;

//            }
//            else continue;

//        }
//    }

    endPoints = endPointsEncontrados;

}
