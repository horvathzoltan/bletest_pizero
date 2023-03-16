#ifndef BLESERVER_H
#define BLESERVER_H

#include <QLowEnergyCharacteristicData>
#include <QLowEnergyController>
#include <QLowEnergyServiceData>
#include <qlowenergyadvertisingdata.h>



class BleServer: public QObject
{
    Q_OBJECT

private:
    QUuid _serviceUuid;
    QString _serviceName;
    QLowEnergyServiceData _serviceData;
    QLowEnergyAdvertisingData _advertisingData;
    QScopedPointer<QLowEnergyController> _controller;
    QScopedPointer<QLowEnergyService> _service;

    void SetAdvertisingData();
    void SetServiceData();
    void Connect();
public:
    BleServer(QUuid serviceUuid, const QString &name);
    void AddCharacteristic(QUuid charUuid,
                           const QByteArray& b,
                           QLowEnergyCharacteristic::PropertyType props);
    void StartAdvertising();
    void WriteCharacteristic(QUuid serviceUuid, const QByteArray& value);

signals:
    void CharacteristicChanged(QBluetoothUuid uuid, const QString &value);

public slots:
    void Changed(const QLowEnergyCharacteristic &characteristic,
                 const QByteArray &newValue);
};

#endif // BLESERVER_H
