#include "controlficheros.h"

ControlFicheros::ControlFicheros()
{

}

ControlFicheros::~ControlFicheros()
{

}

void ControlFicheros::abrirFichero(const path& dirPath,vector<pair<string,int> >& imagesDirectory)
{
    map<string,int> clases=map_list_of("ICHI",1)
                            ("NI",2)
                            ("SAN",3)
                            ("SHI",4)
                            ("GO",5)
                            ("ROKU",6)
                            ("SHICHI",7)
                            ("HACHI",8)
                            ("KU",9)
                            ("JUU",10)
                            ("HYAKU",11)
                            ("SEN",12);

    if(is_directory(dirPath))
    {
        cout<<"En el fichero: "<<dirPath<<endl;
        recursive_directory_iterator endIter;
        for(recursive_directory_iterator dirIter(dirPath);dirIter != endIter;++dirIter)
        {

            if(is_regular_file(dirIter->status()))
            {
                string archivo = dirIter->path().string();
                string carpeta = dirIter->path().parent_path().filename().string();
                int clase = clases.find(carpeta)->second;

                //cout<<carpeta+" "+QString::number(clase).toStdString()<<endl;

                imagesDirectory.push_back(make_pair(archivo,clase));
            }
        }

    }
}





