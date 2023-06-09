#ifndef UPDATER_H
#define UPDATER_H

#include <QString>


class Updater
{
public:
    static bool Update(const QString& version);
    static bool CheckUpdate(const QString& version, QString* buildnum);
    static QString GetLine(const QStringList &lines, const QString &startToken);
};

#endif // UPDATER_H
