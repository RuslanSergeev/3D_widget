#include "SX_3D_Client.h"
#include "SX_3D_Widget.h"
#include <QApplication>
#include <QFile>
#include <QByteArray>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SX_3D_Client client;
    client.show();

    return a.exec();
}
