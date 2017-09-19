#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
//include tcp sockets
#include <QTcpSocket>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    //socket is a connection with IP and port
    //need tcp socket
    //makes one socket called socket
    QTcpSocket *socket;
//only this class can manage hence, private slots
// private slots are normal C++ fuctions that can be called
//normally but have signals connected to them
private slots:
    //on_<objectname>_<signal name>(<signal parameters>);
    void on_pushButtonConnect_clicked();

    void on_pushButtonSend_clicked();


    void connected();
        void disconnected();
        void bytesWritten(qint64 bytes);
        void readyRead();

private slots:
    void displayError (QAbstractSocket::SocketError socketError);



private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
