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
        .startTimeout = -1, // akármeddig várunk
        .runTimeout = -1, // akármeddig várunk
        .showStdErr = false,
        .path = path,
        .detached = true
    };

    ProcessHelper::Output out = ProcessHelper::Execute3(cmd);
    QString msg = out.ToString();    

    if(_verbose) zInfo("msg:"+ msg);


    return out.isStarted;
}

