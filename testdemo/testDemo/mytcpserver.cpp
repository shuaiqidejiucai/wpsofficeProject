#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer(parent)
{
    connect(this, &QTcpServer::newConnection, this, &MyTcpServer::onNewConnection);
}

void MyTcpServer::start(quint16 port)
{
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Server failed to start:" << this->errorString();
    }
}

void MyTcpServer::onNewConnection()
{
    QTcpSocket *client = this->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, [client]() {
        QByteArray data = client->readAll();
        qDebug() << "Received:" << data;
        client->write("Echo: " + data); // 回显
    });
    connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
    qDebug() << "New client connected:" << client->peerAddress().toString();
}
