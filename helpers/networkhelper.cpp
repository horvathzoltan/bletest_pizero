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

QList<QHostAddress> NetworkHelper::GetLocalAddresses()
{
    QList<QHostAddress> result;
    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();

    for(auto&eth:allInterfaces) {
        QList<QNetworkAddressEntry> allEntries = eth.addressEntries();
        for(auto&entry:allEntries) {
            auto ip = entry.ip();
            if (ip.protocol() != QAbstractSocket::IPv4Protocol) continue;
            if(ip == QHostAddress(QHostAddress::LocalHost)) continue;
            if(entry.netmask().isNull()) continue;
            result.append(ip);
        }
    }
    return result;
}
