#include "dowork.h"
#include "global.h"
#include "helpers/logger.h"
#include "helpers/networkhelper.h"
#include "instance.h"
#include "bi/hwinfo.h"
#include "bi/mcpreader.h"
#include "bi/updater.h"
#include <QCoreApplication>
#include <QDir>
#include <QString>

extern Status status;

bool DoWork::_inited = false;
DoWork::Params DoWork::_params = {};

bool DoWork::init(Params p)
{
    _inited = false;
    _params = p;
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
    if(!_inited) return{};
    QString c = _params.bleApi->Requests().join(',');
    QByteArray a = c.toUtf8();

    return a;
}


QByteArray DoWork::bommands()
{
    if(!_inited) return{};
    QString c = _params.bleApi->BRequests().join(',');
    QByteArray a = c.toUtf8();

    return a;
}

QByteArray DoWork::lasterr()
{    
    QByteArray a = status.ToString().toUtf8();

    return a;
}

void DoWork::Test()
{
    if(!_inited) return;
    auto commands = _params.test.split(',');
    for(auto&command:commands){
        QByteArray response = _params.bleApi->Execute(command);
        bool ok = !response.isEmpty();
        QString responseStr = ok?QString(response):QStringLiteral("no response");
        zInfo("command:"+command+'\n'+responseStr);
        zInfo("status:"+status.ToString());
    }
}

// hwinfo_b827eb96cabf.csv
// b8:27:eb:96:ca:bf;1010;logger_2v0
// /home/pi/bletest_pizero/bin/hwinfo.csv
// scp hwinfo_b827eb96cabf.csv pi@10.10.10.102:bletest_pizero/bin/hwinfo.csv
// "B8:27:EB:E3:CC:41"

QByteArray DoWork::hwinfo()
{    
    auto a = HwInfo::Value().toUtf8();
    return a;
}

QByteArray DoWork::swinfo()
{
    QString a = QCoreApplication::applicationName();
    QString b = QCoreApplication::applicationVersion();
    QString i = Instance::_value;

    auto c = (a+':'+b+':'+i).toUtf8();
    return c;
}

//QByteArray DoWork::instance()
//{
//    QString a = QCoreApplication::applicationName();
//    QString b = Instance::_value;

//    auto c = (a+':'+b).toUtf8();
//    return c;
//}

QByteArray DoWork::datalength()
{
    QByteArray a = QString::number(WiringPiHelper::DataLength()).toUtf8();
    return a;
}

QByteArray DoWork::battery()
{
    QByteArray a = QString::number(WiringPiHelper::ReadBattery()).toUtf8();
    return a;
}

QByteArray DoWork::data()
{
    auto values = McpReader::GetValues();

    QString e;
    for(auto&v:values){
        if(!e.isEmpty())e+=';';
        e+=QString::number(v);
    }
    QByteArray a=e.toUtf8();
    return a;
}

QByteArray DoWork::data10()
{
    return data();
}

// elvileg az updaternek nem kellene újraindítania semmit, hanem
// ha a run-ig ok volt minden kellene egy exit55 hívás
// annak kellene a programból kilépnie exit 55-el - az unit újraindul magától

QByteArray DoWork::update()
{
    QString b = QCoreApplication::applicationVersion();
    bool ok = Updater::Update(b);
    QByteArray a = (ok?QStringLiteral("Ok"):QStringLiteral("ERR")).toUtf8();
    return a;
}

QByteArray DoWork::checkupdate()
{
    QString b = QCoreApplication::applicationVersion();
    QString buildnum;
    bool ok = Updater::CheckUpdate(b, &buildnum);
    QByteArray a = (ok?buildnum:QStringLiteral("ERR")).toUtf8();
    return a;
}

QByteArray DoWork::restart()
{
    QCoreApplication::exit(55);
    return QStringLiteral("Ok").toUtf8();
}

// ip címek megszerzése - wlan0, eth0

QByteArray DoWork::getip()
{
    auto hostaddresses = NetworkHelper::GetLocalAddresses();

    QString e;
    for (QHostAddress &a : hostaddresses) {
        if(!e.isEmpty()) e+=':';

        e+=a.toString();
    }
    QByteArray a = e.toUtf8();
    return a;
}

