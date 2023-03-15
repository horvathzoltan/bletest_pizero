#ifndef BLESERVER_H
#define BLESERVER_H

#include <QLowEnergyCharacteristicData>
#include <QLowEnergyController>
#include <QLowEnergyServiceData>
#include <qlowenergyadvertisingdata.h>



class BleServer
{
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
    void WriteCharacteriscic(QUuid serviceUuid, const QByteArray& value);
};

#endif // BLESERVER_H
