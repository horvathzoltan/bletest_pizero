#ifndef GLOBAL_H
#define GLOBAL_H

#include "qglobal.h"
#include <QString>

struct Status{
    quint32 lastStatus;
    QString lastError;

    QString ToString(){
        if(lastStatus==0) return QStringLiteral("ok");
        return QString::number(lastStatus)+":"+lastError;
    }
};

#endif // GLOBAL_H
