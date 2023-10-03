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

    static QByteArray commands(const QString& data);
    static QByteArray bommands(const QString& data);
    static QByteArray lasterr(const QString& data);
    static QByteArray hwinfo(const QString& data);
    static QByteArray swinfo(const QString& data);
    //static QByteArray instance();

    static QByteArray datalength(const QString& data);
    static QByteArray data(const QString& data);
    static QByteArray data10(const QString& data);
    static QByteArray battery(const QString& data);
    //static QByteArray updateasync();

    //static QByteArray updatestatus();

    static QByteArray update(const QString& data);
    static QByteArray upload(const QString& data);
    static QByteArray upload2(const QString& data);
    static QByteArray uploadm(const QString& data);
    static QByteArray aaa(const QString& data);
    static QByteArray aab(const QString& data);

    //static QByteArray checkupdate(const QString& data);
    static QByteArray restart(const QString& data);    
    //static QByteArray reboot(const QString& data);

    static QByteArray getip(const QString& data);
    // beállítások, kulcs-érték pár mentése
    //static bool settings1(const QByteArray& data);
    static void Test();
};

#endif // DOWORK_H
