#include "updater.h"

#include "helpers/logger.h"
#include "helpers/processhelper.h"
#include "helpers/textfilehelper.h"
#include <QStringList>
#include <QDir>

/*
 *
cat /etc/wpa_supplicant/wpa_supplicant.conf

ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=HU

network={
    ssid="MANUALM99"
    psk="akeraker"
    id_str="mestercipo"
}
*/
bool Updater::UpdateAsync(const QString& version)
{
    return true;
}

bool Updater::Update(const QString& version)
{
    if(version.isEmpty()) return false;
    ProcessHelper::Model cmd {
        .cmd = "updater.sh",
        .args ={version, "restart"},
        .timeout = -1,
        .showStdErr = false,
        .path = QDir().absolutePath()
    };
    auto out = ProcessHelper::Execute3(cmd);
    QString msg = out.ToString();
    zInfo("msg:"+ msg);
    return !out.exitCode;
}

//updating...\ngreater version NOT available\n
bool Updater::CheckUpdate(const QString& version, QString* buildnum)
{
    if(version.isEmpty()) return false;
    ProcessHelper::Model cmd {
        .cmd = "updater.sh",
        .args ={version, "checkonly"},
        .timeout = -1,
        .showStdErr = false,
        .path = QDir().absolutePath()
    };
    auto out = ProcessHelper::Execute3(cmd);
    QString msg = out.ToString();
    zInfo("msg:"+ msg);

    auto lines = out.stdOut.split('\n');
    if(! lines.isEmpty()){
        QString lastLine = GetLine(lines, "version");
        auto tokens = lastLine.split(' ');
        if(tokens.length()>=3){
            if(buildnum != nullptr){
                *buildnum = tokens[1];
            }
        }
    }
    return !out.exitCode;
}

QString Updater::GetLine(const QStringList& lines, const QString& startToken){
    for(auto &line:lines)
    {
        if(line.startsWith(startToken)) return line;
    }
    return {};
}

QString Updater::GetStatus(){
    QStringList e;
    bool ok = TextFileHelper::LoadLines("~/status.txt", &e);

    if(!ok) return "NONE";
    if(e.isEmpty()) return "NONE";

    return e.last();
}

//bool Updater::Reboot(const QString& version)
//{
//    if(version.isEmpty()) return false;
//    ProcessHelper::Model cmd {
//        .cmd = "updater.sh",
//        .args ={version, "restart"},
//        .timeout = -1,
//        .showStdErr = false,
//        .path = QDir().absolutePath()
//    };
//    auto out = ProcessHelper::Execute3(cmd);
//    QString msg = out.ToString();
//    zInfo("msg:"+ msg);
//    return !out.exitCode;
//}

/*
pi@raspberrypi:~ $ cat status.txt
start
download_dir: 0K 100% 158M=0s
download: 0K 2% 857K 2s
download: 50K 5% 184M 1s
download: 100K 8% 123M 1s
download: 150K 10% 274M 1s
download: 200K 13% 109M 1s
download: 250K 16% 165M 1s
download: 300K 19% 216M 1s
download: 350K 21% 191M 1s
download: 400K 24% 361M 1s
download: 450K 27% 323M 1s
download: 500K 29% 255M 1s
download: 550K 32% 177M 1s
download: 600K 35% 122M 1s
download: 650K 38% 176M 1s
download: 700K 40% 282M 1s
download: 750K 43% 230M 1s
download: 800K 46% 343M 1s
download: 850K 48% 258M 1s
download: 900K 51% 374M 0s
download: 950K 54% 684M 0s
download: 1000K 57% 666M 0s
download: 1050K 59% 694M 0s
download: 1100K 62% 132M 0s
download: 1150K 65% 120M 0s
download: 1200K 67% 338M 0s
download: 1250K 70% 263M 0s
download: 1300K 73% 612M 0s
download: 1350K 76% 687M 0s
download: 1400K 78% 706M 0s
download: 1450K 81% 688M 0s
download: 1500K 84% 585M 0s
download: 1550K 87% 693M 0s
download: 1600K 89% 703M 0s
download: 1650K 92% 691M 0s
download: 1700K 95% 613M 0s
download: 1750K 97% 690M 0s
download: 1800K 100% 631M=07s
failed: setup FAILED
pi@raspberrypi:~ $
*/
