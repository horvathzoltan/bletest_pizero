#ifndef GLOBAL_H
#define GLOBAL_H

#include "qglobal.h"
#include <QString>

struct Status{
    quint32 lastStatus;
    QString lastError;

    QString ToString();

    enum Codes:int{OK=0, Err};
    void set(Codes code, const QString& err="");
};

#endif // GLOBAL_H
