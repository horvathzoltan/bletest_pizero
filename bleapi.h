#ifndef BLEAPI_H
#define BLEAPI_H

#include "bleserver.h"

#include <QMap>
#include <QString>

typedef QByteArray (*BleApiFn)();

class BleApi : public QObject
{
    Q_OBJECT

private:
    BleServer* _bleServer;
    QMap<QString, BleApiFn> _functions;
    static QUuid _serviceUuid,_char_response,_char_request;//_char2

public:    
    BleApi(const QString& name);
    ~BleApi();

    void Start();

    void AddRequest(const QString& name, BleApiFn func);

public slots:
    void Changed(QBluetoothUuid uuid, const QString& value);
};

#endif // BLEAPI_H
