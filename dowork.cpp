#include "dowork.h"
#include "global.h"
#include <QString>

extern Status status;

bool DoWork::_inited = false;
BleApi* DoWork::_bleApi = nullptr;

bool DoWork::init(BleApi* bleApi)
{
    _inited = false;
    _bleApi = bleApi;
    _inited = true;
    return _inited;;
}

QByteArray DoWork::maki()
{
    QByteArray a = QString("mAki_result").toUtf8();

    return a;
}

QByteArray DoWork::miki()
{
    QByteArray a = QString("mIki_result").toUtf8();

    return a;
}

QByteArray DoWork::commands()
{
    QString c = _bleApi->Requests().join(',');
    QByteArray a = c.toUtf8();

    return a;
}

QByteArray DoWork::bommands()
{
    QString c = _bleApi->BRequests().join(',');
    QByteArray a = c.toUtf8();

    return a;
}

QByteArray DoWork::lasterr()
{
    QByteArray a = status.ToString().toUtf8();

    return a;
}
