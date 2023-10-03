#ifndef FILENAMEHELPER_H
#define FILENAMEHELPER_H

#include <QString>

class FileNameHelper
{
    static bool _inited;
    static QString _appDir;
public:
    static bool Init(const QString& appDir);
    static QString HwInfFileName();
    static QString UploadFileName(const QString &fn);
    static QString UploadDir();
    static QString UploadMetaFileName();
    static bool CreateDir(const QString& udPath);
    static bool zInfo(const char *);
};

#endif // FILENAMEHELPER_H
