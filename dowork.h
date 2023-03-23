#ifndef DOWORK_H
#define DOWORK_H

#include "bleapi.h"

#include <QByteArray>

class DoWork
{
public:
    struct Params{
        BleApi* bleApi;
        QString test;
    };
private:
    static bool _inited;
    static Params _params;
public:        
    static bool init(Params p);

    static bool isTest(){ return !_params.test.isEmpty();}

    // adatváltozás teszthez
    static QByteArray maki();
    static QByteArray miki();

    static QByteArray commands(); //0x18
    static QByteArray bommands(); //0x19
    static QByteArray lasterr(); //0x17
    static QByteArray hwinfo(); //0x51
    static QByteArray swinfo(); //0x52
    static QByteArray buildnum(); //0x53

    static QByteArray type();
    static QByteArray version();
    static QByteArray datalength();
    static QByteArray data();

    // beállítások, kulcs-érték pár mentése
    //static bool settings1(const QByteArray& data);
    static void Test();
};

#endif // DOWORK_H
