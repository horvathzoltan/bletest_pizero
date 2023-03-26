#include "networkhelper.h"
//#include "helpers/logger.h"

#include <QNetworkInterface>

QString NetworkHelper::hwid_wlan(){
    auto a= QNetworkInterface::allInterfaces();
    for(auto&i:a){
        if(i.type()==QNetworkInterface::Wifi) return i.hardwareAddress();
    }
    return QString();
}

QString NetworkHelper::hwid_ethernet(){
    auto a= QNetworkInterface::allInterfaces();
    for(auto&i:a)
    {
        if(i.type()==QNetworkInterface::Ethernet) return i.hardwareAddress();
    }
    return QString();
}

QString NetworkHelper::hwip(const QString& hwid){
    auto a= QNetworkInterface::allInterfaces();
    for(auto&i:a)
    {
        if(i.hardwareAddress()==hwid){
            auto addrs = i.allAddresses();
            for(auto&j:addrs){
                if(j.isLoopback()) continue;
                if(j.protocol()==QAbstractSocket::IPv4Protocol) return j.toString();
            }
        }
    }
    return QString();
}

