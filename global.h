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

    enum Codes:int{OK=0, Err};

    void set(Codes code, const QString& err=""){
        lastStatus=code;
        if(code==OK)
            lastError.clear();
        else
            lastError=err;
    }

};

#endif // GLOBAL_H
