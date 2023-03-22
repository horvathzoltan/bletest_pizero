#ifndef BLEAPI_H
#define BLEAPI_H

#include "bleserver.h"
#include "nameof.h"

#include <QMap>
#include <QString>

#define addrequest(x) AddRequest(nameof_fn(x), x)

typedef QByteArray (*BleApiFn)();

class BleApi : public QObject
{
    Q_OBJECT

private:
    BleServer* _bleServer;
    QMap<QString, BleApiFn> _functions;
    QMap<quint8, BleApiFn> _bfunctions;

    static QUuid _serviceUuid,_char_response,_char_request;//_char2

public:    
    BleApi(const QString& name);
    ~BleApi();

    void Start();

    bool AddRequest(const QString& name, BleApiFn func);
    bool AddRequest(quint8 cmd, BleApiFn func);
    QStringList Requests();
    QStringList BRequests();
public slots:
    void Changed(QBluetoothUuid uuid, const QString& value);
};

#endif // BLEAPI_H
