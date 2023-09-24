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

QString FileNameHelper::UploadDir()
{
    return QDir::home().filePath("upload");
}

QString FileNameHelper::UploadMetaFileName()
{
    return QDir::home().filePath("uploadMeta.ini");
}

QString FileNameHelper::UploadFileName(const QString &fn)
{
    if(!_inited) return {};
    auto upd = UploadDir();

    auto uploadFn = QDir(upd).filePath(fn);
    return uploadFn;
}
