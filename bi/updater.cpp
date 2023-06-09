#include "updater.h"

#include "helpers/logger.h"
#include "helpers/processhelper.h"
#include <QStringList>
#include <QDir>

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
