#ifndef UPDATER_H
#define UPDATER_H

#include <QString>


class Updater
{
public:
    static bool Update(const QString& version);
};

#endif // UPDATER_H
