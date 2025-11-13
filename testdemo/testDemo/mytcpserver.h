#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer(QObject *parent = nullptr);
    void start(quint16 port);

private slots:
    void onNewConnection();
};

#endif // MYTCPSERVER_H
