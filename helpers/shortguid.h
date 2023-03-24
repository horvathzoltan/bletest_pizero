#ifndef COM_ZSHORTGUID_H
#define COM_ZSHORTGUID_H

#include <QUuid>

class zShortGuid {
public:
    zShortGuid();
    static const zShortGuid Empty;

    explicit zShortGuid(QUuid guid);

    QUuid uuid;
    QString value;

    static zShortGuid createNew();

    static QString Encode(QUuid);
    static QUuid Decode(QString);
};

#endif // ZSHORTGUID_H
