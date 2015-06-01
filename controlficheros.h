#ifndef CONTROLFICHEROS_H
#define CONTROLFICHEROS_H

#include <QStringList>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>
using namespace std;
using namespace boost::filesystem;

class ControlFicheros
{

private:

    QStringList archivosFichero;



public:
    static void abrirFichero(const path& dirPath, QStringList& imagesDirectory );
    ControlFicheros();
    ~ControlFicheros();
};

#endif // CONTROLFICHEROS_H
