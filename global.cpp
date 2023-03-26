#include "global.h"

Status status;

QString Status::ToString(){
    if(lastStatus==0) return QStringLiteral("ok");
    return QString::number(lastStatus)+":"+lastError;
}

void Status::set(Codes code, const QString &err){
    lastStatus=code;
    if(code==OK)
        lastError.clear();
    else
        lastError=err;
}
