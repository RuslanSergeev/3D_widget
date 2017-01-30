#include "SX_3D_Client.h"
#include "SX_3D_Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SX_3D_Client client;
    client.show();

    return a.exec();
}
