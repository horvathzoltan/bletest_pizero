#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QString>

class NetworkHelper
{
public:
    static QString hwid_wlan();
    static QString hwid_ethernet();
    static QString hwip(const QString &hwid);
};

#endif // NETWORKHELPER_H
