#include "dowork.h"
#include "global.h"
#include "helpers/logger.h"
#include "helpers/textfilehelper.h"
#include "instance.h"
#include "bi/hwinfo.h"

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
    // hwinfo_b827eb96cabf.csv
    // b8:27:eb:96:ca:bf;1010;logger_2v0
    // /home/pi/bletest_pizero/bin/hwinfo.csv
    // scp hwinfo_b827eb96cabf.csv pi@10.10.10.102:bletest_pizero/bin/hwinfo.csv
    // "B8:27:EB:E3:CC:41"
    auto hwinfoFn = QDir(appdir).filePath("hwinfo.csv");
    QStringList lines; // fájltartalom
    QByteArray a; // ez lesz a response
    bool ok = TextFileHelper::LoadLines(hwinfoFn, &lines);
    if(ok){
        if(!lines.isEmpty()) {
            auto tokens = lines.at(0).split(';');
            if(!tokens.isEmpty()){                
                if(0==tokens.at(0).compare(HwInfo::_value, Qt::CaseInsensitive)){
                    a = lines.at(0).toUtf8();
                    status.set(Status::OK);
                } else{
                    status.set(Status::Err, "wrong hwid in record");
                }
            } else{
                status.set(Status::Err, "invalid hwinfo record");
            }
        } else{
            status.set(Status::Err, "no hwinfo record");
        }

    } else {
        status.set(Status::Err, TextFileHelper::LastError());
    }

    return a;
}

QByteArray DoWork::swinfo()
{
    QString a = QCoreApplication::applicationName();
    QString b = QCoreApplication::applicationVersion();

    auto c = (a+':'+b).toUtf8();
    return c;
}

QByteArray DoWork::instance()
{
    QString a = QCoreApplication::applicationName();
    QString b = Instance::_value;

    auto c = (a+':'+b).toUtf8();
    return c;
}

QByteArray DoWork::datalength()
{
    QByteArray a = QStringLiteral("datalength").toUtf8();

    return a;
}
