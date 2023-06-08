#ifndef PROCESSHELPER_H
#define PROCESSHELPER_H

#include <QString>

class ProcessHelper
{
public:
    struct Output
    {
        QString stdOut;
        QString stdErr;
        int exitCode;
        QString ToString();
    };
    static Output Execute(const QString& cmd, const QStringList& args, int timeout = -1);
    static bool Wget(const QString &url, const QString &filename);
};

#endif // PROCESSHELPER_H
