#include "dowork.h"
#include "global.h"
#include "helpers/logger.h"
#include "helpers/networkhelper.h"
#include "helpers/uploadhelper.h"
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

QByteArray DoWork::commands(const QString& data)
{
    Q_UNUSED(data)

    if(!_inited) return{};
    QString c = _params.bleApi->Requests().join(',');
    QByteArray a = c.toUtf8();

    return a;
}


QByteArray DoWork::bommands(const QString& data)
{
    Q_UNUSED(data)

    if(!_inited) return{};
    QString c = _params.bleApi->BRequests().join(',');
    QByteArray a = c.toUtf8();

    return a;
}

QByteArray DoWork::lasterr(const QString& data)
{
    Q_UNUSED(data)

    QByteArray a = status.ToString().toUtf8();

    return a;
}

// -t 0,123|uploadm*12345678

void DoWork::Test()
{
    if(!_inited) return;
    auto commands = _params.test.split(';');
    for(auto&command:commands){

        if(command.startsWith('\"') && command.endsWith('\"')){
            command = command.right(command.length()-2);
        }

        int ix = command.lastIndexOf('|');

        QString key, value, valueData;
        if(ix>0){
            key = command.left(ix); // szeparátortól balra a kulcs, jobbra az érték ami amúgy a command is
            value = command.right(command.length()-ix-1);
        } else{
            key = QString(); // egyébként a kulcs üres és csak command van
            value = command;
        }

        int ix2 = value.indexOf('*');
        if(ix2>0){
            valueData = value.right(value.length()-ix2-1);
            value = value.left(ix2);
        }

        QByteArray response = _params.bleApi->Execute(value, valueData);
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

QByteArray DoWork::hwinfo(const QString& data)
{
    Q_UNUSED(data)

    auto a = HwInfo::Value().toUtf8();
    return a;
}

QByteArray DoWork::swinfo(const QString& data)
{
    Q_UNUSED(data)

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

QByteArray DoWork::datalength(const QString& data)
{
    Q_UNUSED(data)

    QByteArray a = QString::number(WiringPiHelper::DataLength()).toUtf8();
    return a;
}

QByteArray DoWork::battery(const QString& data)
{
    Q_UNUSED(data)

    QByteArray a = QString::number(WiringPiHelper::ReadBattery()).toUtf8();
    return a;
}

QByteArray DoWork::data(const QString& data)
{
    Q_UNUSED(data)

    auto values = McpReader::GetValues();

    QString e;
    for(auto&v:values){
        if(!e.isEmpty())e+=';';
        e+=QString::number(v);
    }
    QByteArray a=e.toUtf8();
    return a;
}

QByteArray DoWork::data10(const QString& _data)
{
    return data(_data);
}

// elvileg az updaternek nem kellene újraindítania semmit, hanem
// ha a run-ig ok volt minden kellene egy exit55 hívás
// annak kellene a programból kilépnie exit 55-el - az unit újraindul magától

QByteArray DoWork::update(const QString& data)
{
    bool ok = Updater::Update(data);
    QByteArray a = (ok?QStringLiteral("Ok"):QStringLiteral("ERR")).toUtf8();
    return a;
}

//QByteArray DoWork::updatestatus()
//{
//    QString status = Updater::GetStatus();
//    QByteArray a = status.toUtf8();
//}

QByteArray DoWork::restart(const QString& data)
{
    Q_UNUSED(data)

    QCoreApplication::exit(55);
    return QStringLiteral("Ok").toUtf8();
}

//QByteArray DoWork::reboot(const QString& data)
//{
//    Q_UNUSED(data)

//    QCoreApplication::exit(55);
//    return QStringLiteral("Ok").toUtf8();
//}

// ip címek megszerzése - wlan0, eth0

QByteArray DoWork::getip(const QString& data)
{
    Q_UNUSED(data)

    auto hostaddresses = NetworkHelper::GetLocalAddresses();

    QString e;
    for (QHostAddress &a : hostaddresses) {
        if(!e.isEmpty()) e+=':';

        e+=a.toString();
    }
    QByteArray a = e.toUtf8();
    return a;
}


// data: fileid: byte
// ez bejegyzi egy id alatt
// -t "0,123|uploadm*file1.txt,30"
QByteArray DoWork::uploadm(const QString& data){

    UploadHelper::MetaData m = UploadHelper::MetaData::Parse(data);
    UploadHelper::UploadResponseModel um = UploadHelper::AddUpload(m);

    zInfo("uploadm:"+um.toString())
    QByteArray a = um.toString().toUtf8();
    return a;
}

//
QByteArray DoWork::upload(const QString& data){
    UploadHelper::UploadModel m = UploadHelper::UploadModel::Parse(data);
    UploadHelper::UploadResponseModel um = UploadHelper::Upload(m, false);

    zInfo("upload:"+um.toString())
    QByteArray a = um.toString().toUtf8();
    return a;
}

QByteArray DoWork::upload2(const QString& data){
    UploadHelper::UploadModel m = UploadHelper::UploadModel::Parse(data);
    UploadHelper::UploadResponseModel um = UploadHelper::Upload(m, true);

    zInfo("upload2:"+um.toString())
    QByteArray a = um.toString().toUtf8();
    return a;
}

QByteArray DoWork::aaa(const QString &data)
{
    int l = data.length();
    QByteArray a = QString::number(l).toUtf8();
    return a;
}

QByteArray DoWork::aab(const QString &data)
{
    QString str = QStringLiteral("abcdefghijklmnokqrstuvwzxyzABCDEFGHIJKLMNOKQRSTUVWZXYZ");
    QByteArray a = str.toUtf8();
    return a;
}


//QByteArray DoWork::upload(const QString& data){
//    zInfo("hutty2");

//    QString e = "bbb";
//    QByteArray a = e.toUtf8();
//    return a;
//}
