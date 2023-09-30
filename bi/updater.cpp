#include "updater.h"

#include "helpers/logger.h"
#include "helpers/processhelper.h"
#include <QStringList>
#include <QDir>
#include <helpers/textfilehelper.h>

bool Updater::Update(const QString& version)
{
    if(version.isEmpty()) return false;
    auto path = QDir::home().absolutePath();

    zInfo("path:"+ path);

    ProcessHelper::Model cmd {
        .cmd = "updater.sh",
        .args ={version, "restart"},
        .timeout = -1,
        .showStdErr = false,
        .path = path
    };
    auto out = ProcessHelper::Execute3(cmd);
    QString msg = out.ToString();    

    if(_verbose){
        zInfo("msg:"+ msg);
        //TextFileHelper::Save(msg, "\\home\\pi\\updater.log", false);
    }

    return !out.exitCode;
}

