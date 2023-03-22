#ifndef DOWORK_H
#define DOWORK_H

#include "bleapi.h"

#include <QByteArray>


class DoWork
{
private:
    static bool _inited;
    static BleApi* _bleApi;
public:
    static bool init(BleApi* bleApi);

    // adatváltozás teszthez
    static QByteArray maki();
    static QByteArray miki();

    static QByteArray commands(); //0x18
    static QByteArray bommands(); //0x19
    static QByteArray lasterr(); //0x51

    static QByteArray type();
    static QByteArray version();
    static QByteArray datalength();
    static QByteArray data();

    // beállítások, kulcs-érték pár mentése
    //static bool settings1(const QByteArray& data);

};

#endif // DOWORK_H
