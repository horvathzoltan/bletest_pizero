#ifndef DOWORK_H
#define DOWORK_H

#include "ble/bleapi.h"

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

    static QByteArray commands();
    static QByteArray bommands();
    static QByteArray lasterr();
    static QByteArray hwinfo();
    static QByteArray swinfo();
    static QByteArray instance();

    static QByteArray datalength();
    static QByteArray data();
    static QByteArray battery();

    // beállítások, kulcs-érték pár mentése
    //static bool settings1(const QByteArray& data);
    static void Test();
};

#endif // DOWORK_H
