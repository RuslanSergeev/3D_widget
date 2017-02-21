#ifndef SX_3D_CLIENT_H
#define SX_3D_CLIENT_H

#include <QWidget>

namespace Ui {
class SX_3D_Client;
}

class SX_3D_Client : public QWidget
{
    Q_OBJECT

public:
    explicit SX_3D_Client(QWidget *parent = 0);
    ~SX_3D_Client();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::SX_3D_Client *ui;
};

#endif // SX_3D_CLIENT_H
