#include "controlficheros.h"

ControlFicheros::ControlFicheros()
{

}

ControlFicheros::~ControlFicheros()
{

}

void ControlFicheros::abrirFichero(const path& dirPath,QStringList& imagesDirectory)
{
 if(is_directory(dirPath))
 {
    cout<<"En el fichero: "<<dirPath<<endl;
    directory_iterator endIter;
    for(directory_iterator dirIter(dirPath);dirIter != endIter;++dirIter)
    {
        try
        {
            if(is_regular_file(dirIter->status()))
            {
                imagesDirectory.push_back(QString::fromStdString(dirIter->path().filename().string()));
            }
        }
        catch(const exception & ex)
        {

        }
    }
 }

}

