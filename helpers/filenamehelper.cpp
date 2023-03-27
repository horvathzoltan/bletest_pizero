#include "filenamehelper.h"

#include <QCoreApplication>
#include <QDir>

bool FileNameHelper::_inited= false;
QString FileNameHelper::_appDir;

bool FileNameHelper::Init(const QString &appDir)
{
    _inited = false;
    _appDir = appDir;
    _inited = true;
    return true;
}

QString FileNameHelper::HwInfFileName()
{
    if(!_inited) return {};
    auto hwInfFileName = QDir(_appDir).filePath("hwinfo.csv");
    return hwInfFileName;
}
