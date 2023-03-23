#include "dowork.h"
#include "global.h"
#include "helpers/logger.h"
#include "helpers/textfilehelper.h"
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

/**/

QByteArray DoWork::hwinfo()
{    
    auto appdir = QCoreApplication::applicationDirPath();
    auto hwinfoFn = QDir(appdir).filePath("hwinfo.csv"); //b8:27:eb:96:ca:bf;1010;logger_2v0
    QString hwinfo;
    bool ok = TextFileHelper::Load(hwinfoFn, &hwinfo);
    QByteArray a;
    if(ok){
        a = hwinfo.toUtf8();
        status.set(Status::OK);
    } else {
        status.set(Status::Err, TextFileHelper::LastError());
    }

    return a;
}

QByteArray DoWork::swinfo()
{
    QString d = QCoreApplication::applicationName();
    QString b = QCoreApplication::applicationVersion();

    auto a = (d+':'+b).toUtf8();
    return a;
}

QByteArray DoWork::instance()
{
    QByteArray a = QStringLiteral("instance").toUtf8();

    return a;
}

