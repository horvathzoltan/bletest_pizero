#include "updater.h"

#include "helpers/logger.h"
#include "helpers/processhelper.h"
#include <QStringList>
#include <QDir>

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
    zInfo("msg:"+ msg);
    return !out.exitCode;
}

