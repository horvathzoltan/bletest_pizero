#include "updater.h"

#include "helpers/processhelper.h"
#include <QStringList>

bool Updater::Update(const QString& version)
{
    if(version.isEmpty()) return false;
    auto out = ProcessHelper::Execute("update.sh", {version, "norestart"}, -1);
    return !out.exitCode;
}
