#include "controlredneuronal.h"

controlredneuronal::controlredneuronal(char *srcNetworkFile)
{
    /**
     * @brief storage Upload the information of the trained neural network
     */
    CvFileStorage* storage = cvOpenFileStorage( srcNetworkFile, 0, CV_STORAGE_READ );
        CvFileNode *n = cvGetFileNodeByName(storage,0,"NeuralNetwork");
        NeuralNetwork.read(storage,n);
        cvReleaseFileStorage(&storage);
}

/**
 * @brief train: Will TRain the Neural Network with the information in the filename path
 * and will save the configuration of the resultant Neural Network in the file fileStorage_Path/
 */
void controlredneuronal::train( char *DataSet, char *dstNetworkFile ){
    //matrix to hold the training sample
    cv::Mat training_set(TRAINING_SAMPLES, ATTRIBUTES, CV_32F);
    //matrix to hold the labels of each taining sample
    cv::Mat training_set_classifications(TRAINING_SAMPLES, CLASSES, CV_32F);
    //matrix to hold the test samples
    cv::Mat test_set(TEST_SAMPLES, ATTRIBUTES, CV_32F);
    //matrix to hold the test labels.
    cv::Mat test_set_classifications(TEST_SAMPLES, CLASSES, CV_32F);

    //load the training data sets.
   read_trainingDataset(DataSet, training_set, training_set_classifications, TRAINING_SAMPLES);


    // define the structure for the neural network (MLP)
    cv::Mat layers(3, 1, CV_32S);
    layers.at<int>(0, 0) = ATTRIBUTES;//input layer
    layers.at<int>(1, 0) = HIDDEN_NEURONES;//hidden layer
    layers.at<int>(2, 0) = CLASSES;//output layer

    //create the neural network.
    //CvANN_MLP nnetwork((const Mat& layerSizes, int activateFunc=CvANN_MLP::SIGMOID_SYM, double Alfa=0, double Beta=0 );
    CvANN_MLP nnetwork(layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1);

    CvANN_MLP_TrainParams params	(

        // terminate the training after either 1000
        // iterations or a very small change in the
        // network wieghts below the specified value
        cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.000001),
        // use backpropogation for training
        CvANN_MLP_TrainParams::BACKPROP,
        // co-efficents for backpropogation training
        // recommended values taken from http://docs.opencv.org/modules/ml/doc/neural_networks.html#cvann-mlp-trainparams
        0.1, //bp_dw_scale Strength of the weight gradient term. The recommended value is about 0.1.
        0.1); //bp_moment_scale Strength of the momentum term (the difference between weights on the 2 previous iterations). This parameter provides some inertia to smooth the random fluctuations of the weights. It can vary from 0 (the feature is disabled) to 1 and beyond. The value 0.1 or so is good enough

    // train the neural network (using training data)
    cout << ("\nUsing training dataset\n");
    int iterations = nnetwork.train(training_set, training_set_classifications, cv::Mat(), cv::Mat(), params);
    cout << ("Training iterations: %i \n\n", iterations);

    // Save the model generated into an xml file.
        CvFileStorage* storage = cvOpenFileStorage( dstNetworkFile, 0, CV_STORAGE_WRITE );
        nnetwork.write(storage,"NeuralNetwork");
        cvReleaseFileStorage(&storage);

    training_set.release();
    training_set_classifications.release();

    ///TEST THE NEURAL NETWORK
    ///Carga la información del archivo de prueba y crea una red neuronal con la imformación
    /// guardada anteriormente, Luego realiza una evaluación de los datos en el archivo de prueba
    /// y registra cuantos número fueron reconocidos correctamente
    //test_set, test_set_classifications,
        read_trainingDataset("/home/snipercat/Desktop/ArchivoCaracteristicasTestSet.csv", test_set, test_set_classifications, TEST_SAMPLES);
        FILE* test = fopen("/home/snipercat/Desktop/test.csv","w");
        int expected;
        int predicted;
        int correct = 0;
        cv::Mat test_sample;
        cv::Mat test_sample_expected;
        //controlredneuronal red = controlredneuronal(dstNetworkFile);
        controlredneuronal red = controlredneuronal("/home/snipercat/Desktop/NeuralNetwork.xml");

        for( int c = 0; c <  TEST_SAMPLES ;c++ ){
            //get Expected Value
                test_sample_expected = test_set_classifications.row(c);
                expected = controlredneuronal::returnValue(test_sample_expected);
            //PREDICT
               test_sample = test_set.row(c);
               predicted = red.predict(test_sample);
            //Write result to file
               if(expected == predicted)
                   correct++;
               fprintf(test,"%d;%d;%d\n",
                     c,
                     expected,
                     predicted
                     );
        }
        cout<<"PREDICTION ACCURACY= "<<correct*100/TEST_SAMPLES<<"%";
        test_set.release();
        test_set_classifications.release();
        fclose(test);




}


/**
 * @brief controlredneuronal::predict : Predict the class of a image using a neural network
 * @param data Features Vector of the Image
 * @return
 */
int controlredneuronal::predict( cv::Mat data ){


    //output of the Neural Network
    cv::Mat classOut(1,CLASSES,CV_32F);

    //prediction
    NeuralNetwork.predict(data, classOut);
    return controlredneuronal::returnValue(classOut);

}

/**
 * @brief returnValue Return the Corresponding Value of the Class
 * @param out
 * @return
 */
int  controlredneuronal::returnValue( cv::Mat outNetwork)
{
    int maxIndex = 0;
    //Get the index of the higest value
    float value;
    float maxValue=outNetwork.at<float>(0,0);
    for(int index=1;index<CLASSES;index++)
    {   value = outNetwork.at<float>(0,index);
            if(value>maxValue)
            {   maxValue = value;
                maxIndex=index;
            }
    }
    //cout<<"MAX VALUES = "<<maxValue<<" FOR INDEX "<<maxIndex<<endl;
    //decode the index into the corresponding number
    if(maxIndex == 10){
        return 100;
    }
    if(maxIndex == 11){
        return 1000;
    }

    return maxIndex + 1;

}


/********************************************************************************
This function will read the csv files(training and test dataset) and convert them
into two matrices. classes matrix have 12 columns, one column for each class label. If the label of nth row in data matrix
is, lets say 5 then the value of classes[n][5] = 1.
first
********************************************************************************/

void controlredneuronal::read_trainingDataset(char *filename, cv::Mat &data, cv::Mat &classes, int total_samples)
{

    int label;
    float moment;
    //open the file
    FILE* inputfile = fopen(filename, "r");

    //read each row of the csv file
    for (int row = 0; row < total_samples; row++)
    {
        //for each attribute in the row
        for (int col = 0; col <= ATTRIBUTES; col++)
        {
            //if its the pixel value.
            if (col < ATTRIBUTES) {

                fscanf(inputfile, "%f;", &moment);
                data.at<float>(row, col) = moment;

            }//if its the label
            else if (col == ATTRIBUTES) {
                //make the value of label column in that row as 1.
                fscanf(inputfile, "%d", &label);
                classes.at<float>(row, label-1) = 1.0;
            }
        }

       /* for( int c=0; c< ATTRIBUTES ;  c++){
            cout<<data.at<float>(row, c)<<";";
        }
*/
        //cout<<";[";
        for( int c=0; c< CLASSES ;  c++){
            if( classes.at<float>(row, c) != 1 ){
                classes.at<float>(row, c) = 0;
            }
          //  cout<<classes.at<float>(row, c)<<" , ";
        }
        //cout<<"];"<<label<<endl;
    }

    fclose(inputfile);

}

