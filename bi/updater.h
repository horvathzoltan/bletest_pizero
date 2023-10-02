#ifndef UPDATER_H
#define UPDATER_H

#include <QString>


class Updater
{
private:
    static bool _verbose;
public:
    static void SetVerbose(bool v){_verbose=v;};

    static bool Update(const QString& version);
};

#endif // UPDATER_H
