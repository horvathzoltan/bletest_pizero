#include "bleserver.h"

#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyDescriptorData>

BleServer::BleServer(QUuid serviceUuid,
                     const QString &serviceName)
{
    _serviceUuid = serviceUuid;
    _serviceName = serviceName;
    SetAdvertisingData();
    SetServiceData();
}

void BleServer::SetAdvertisingData()
{
    _advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    _advertisingData.setIncludePowerLevel(true);
    _advertisingData.setLocalName(_serviceName);
    //auto a = QList<QBluetoothUuid>() << _serviceUuid;
    _advertisingData.setServices({_serviceUuid});
}

void BleServer::SetServiceData()
{
    _serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    _serviceData.setUuid(_serviceUuid);
}

void BleServer::AddCharacteristic(
        QUuid charUuid,
        const QByteArray& b,
        QLowEnergyCharacteristic::PropertyType props)
{
    QLowEnergyCharacteristicData charData;
    charData.setUuid(charUuid);
    charData.setValue(b);//QByteArray(2, 0));
    charData.setProperties(props);
    const QLowEnergyDescriptorData
            descriptorData(QBluetoothUuid::ClientCharacteristicConfiguration,b);
                                                //QByteArray(2, 0));
    charData.addDescriptor(descriptorData);

    _serviceData.addCharacteristic(charData);
}

void BleServer::Connect()
{
    auto changed = [](const QLowEnergyCharacteristic &characteristic,
                               const QByteArray &newValue)
    {
         qDebug()<<"Changed:["+QString::number(newValue[0])+"]"+QString(newValue);
    };

    _service.reset(_controller->addService(_serviceData));
    if (!_service.isNull()){
        QObject::connect(_service.get(), &QLowEnergyService::characteristicChanged, changed);

        qDebug()<<"StartAdvertising";
        _controller->startAdvertising(QLowEnergyAdvertisingParameters(),
                                       _advertisingData, _advertisingData);
    }
}

void BleServer::StartAdvertising()
{
    _controller.reset(QLowEnergyController::createPeripheral());
    Connect();
    auto reconnect = [this](){Connect();};
    QObject::connect(_controller.data(), &QLowEnergyController::disconnected, reconnect);
}

void BleServer::WriteCharacteriscic(QUuid characteristicUuid, const QByteArray &value)
{
    if(!_service) return;
    QLowEnergyCharacteristic characteristic
            = _service->characteristic(characteristicUuid);
    Q_ASSERT(characteristic.isValid());
    _service->writeCharacteristic(characteristic, value); // Potentially causes notification.
}
