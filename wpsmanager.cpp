#include "wpsmanager.h"
#include <QDebug>
WpsManager::WpsManager(QObject *parent)
    : QObject{parent},
    pWpsRpcClient(nullptr)
{

}

void WpsManager::initWpsApplication()
{

}


WpsManager* WpsManager::Instance()
{
    static WpsManager manager;
    return &manager;
}
