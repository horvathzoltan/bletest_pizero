#include "dowork.h"
#include "global.h"
#include "helpers/logger.h"
#include <QString>
#include "buildnumber.h"

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
    auto a = QStringLiteral("hwinfo").toUtf8();
    return a;
}

QByteArray DoWork::swinfo()
{
    auto a = QStringLiteral("swinfo").toUtf8();
    return a;
}

QByteArray DoWork::buildnum()
{
    auto a = Buildnumber::toString().toUtf8();
    return a;
}
