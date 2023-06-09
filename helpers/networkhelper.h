#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QHostAddress>
#include <QString>

class NetworkHelper
{
public:
    static QString hwid_wlan();
    static QString hwid_ethernet();
    static QString hwip(const QString &hwid);
    static QList<QHostAddress> GetLocalAddresses();
};

#endif // NETWORKHELPER_H
