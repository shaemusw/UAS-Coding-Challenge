#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    //The QTcpSocket class provides a TCP socket.
    //TCP ensures all message are recieved
    //UDP dont check errors
    //socket is the name of a new tcp socket
    socket = new QTcpSocket(this);
    ui->setupUi(this);

    // Connect button signal to appropriate slot

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT (on_pushButtonConnect_clicked()));
    //send data to server through socket calls method takes sring from textbox and puts t into right fucntion to send to socket
    connect(ui->pushButtonSend, SIGNAL(clicked(bool)), this, SLOT (on_pushButtonSend_clicked()));
    //every time server sends data socket reads this and puts it on text box
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //displays connected
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    //displays disconnected
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    //tells how many bytes written
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    //any misconnections results in error
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(displayError(QAbstractSocket::SocketError)));
    //slot is an action a method
    //

}

Dialog::~Dialog()
{
    delete ui;

}

//function to connect when button is clicked
void Dialog::on_pushButtonConnect_clicked()
{
    // connectToHost: Attempts to make a connection to hostName on the given port.
    //toplaintext: Returns the text of the text edit as plain text.
    //toint converts port text in unsigned bits to an integer of 32 bits
    //quint converts int of 32 to 16 bits
    socket->connectToHost(ui->textEditIP->toPlainText(),quint16(ui->textEditPort->toPlainText().toInt()));

}


void Dialog::connected()
{
    qDebug() << "Connected!";


}

void Dialog::disconnected()
{
    qDebug() << "Disconnected!";
}

void Dialog::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes;
}

//this just proves that i am getting messages from the server which means i am connected
void Dialog::readyRead()
{
    //like cout
    qDebug() << "Reading...";
    //Reads all remaining data from the device, and returns it as a byte array.
    QByteArray Data = socket->readAll();

    qDebug() << " Data in: " << Data;

    QString DataAsString = Data.data();
    //"welcome"
    ui->serverclientChat->setText(ui->serverclientChat->toPlainText()+"Server: "+DataAsString);

}




void Dialog::on_pushButtonSend_clicked()
{   //collects message from message box

    QString message = ui->textEditMessage->toPlainText();

    //textEditMessage name of box
    // Only send the text to the chat server if it's not empty1

    //if message is not empty
    if(!message.isEmpty())
    {
        //write a message
        //if connected start enter this if loop
        if(socket->state() == QAbstractSocket::ConnectedState){
            socket->write(QString(message).toUtf8());
            //clears eveerything out the memory
            socket->flush();
            //wait until bytes arrive
            socket->waitForBytesWritten();

            ui->serverclientChat->setText(ui->serverclientChat->toPlainText()+ QString(message+'\n'));
            //ui->serverclientChat->toPlainText()+ adds memory of previous messages
        }
    }


    // Clear out the input box so they can type something else:
    ui->textEditMessage->clear();

    // Put the focus back into the input box so they can type again:
    ui->textEditMessage->setFocus();

}
void Dialog::displayError ( QAbstractSocket::SocketError socketError )
{

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
      /*  QMessageBox::information(this, tr("Info"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));*/
         qDebug() << "The host was not found. Please check the "
                     "host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Info"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Info"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }

}

