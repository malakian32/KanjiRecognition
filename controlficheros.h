#ifndef CONTROLFICHEROS_H
#define CONTROLFICHEROS_H

#include <QStringList>
#include <boost/filesystem.hpp>
#include <boost/assign.hpp>
#include <iostream>
using namespace std;
using namespace boost::filesystem;
using namespace boost::assign;
class ControlFicheros
{

private:

    QStringList archivosFichero;


public:
    static void abrirFichero(const path& dirPath,vector<pair<string,int> >& imageDirectory);
    ControlFicheros();
    ~ControlFicheros();
};

#endif // CONTROLFICHEROS_H
