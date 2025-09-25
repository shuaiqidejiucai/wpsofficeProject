#ifndef WPSMANAGER_H
#define WPSMANAGER_H

#include <QObject>
#include "wppapi.h"

class WpsManager : public QObject
{
    Q_OBJECT
public:
    static WpsManager *Instance();

 private:
     explicit WpsManager(QObject *parent = nullptr);
     void initWpsApplication();


     IKRpcClient *pWpsRpcClient;
};

#endif // WPSMANAGER_H
