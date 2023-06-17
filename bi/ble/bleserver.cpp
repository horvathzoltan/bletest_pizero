#include "bleserver.h"

#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyConnectionParameters>
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
    charData.setValue(b);
    charData.setProperties(props);
    const QLowEnergyDescriptorData
            descriptorData(QBluetoothUuid::ClientCharacteristicConfiguration,b);
    charData.addDescriptor(descriptorData);

    _serviceData.addCharacteristic(charData);
}


void BleServer::Changed(const QLowEnergyCharacteristic &characteristic,
             const QByteArray &newValue)
{
    //QLowEnergyCharacteristic::PropertyTypes props = characteristic.properties();
    //bool isIndicate = props.testFlag(QLowEnergyCharacteristic::Indicate);
    //_isWritten = true;
    emit CharacteristicChanged(characteristic.uuid(), QString(newValue));
}

void BleServer::Written(const QLowEnergyCharacteristic &characteristic,
                        const QByteArray &newValue)
{
    Q_UNUSED(characteristic)
    Q_UNUSED(newValue)

    //_isWritten = true;
}

void BleServer::Connect()
{
    _service.reset(_controller->addService(_serviceData));
    if (!_service.isNull()){
        QLowEnergyService *s = _service.get();

        QObject::connect(s, &QLowEnergyService::characteristicChanged, this,
                         &BleServer::Changed);
//        QObject::connect(s, &QLowEnergyService::cha, this,
//                         &BleServer::Written);

        qDebug() << "StartAdvertising";
        if (_controller->AdvertisingState)
        _controller->startAdvertising(QLowEnergyAdvertisingParameters(), _advertisingData, _advertisingData);
    }
}

void BleServer::StartAdvertising()
{
    _controller.reset(QLowEnergyController::createPeripheral());// itt kap értéket a _controller
    //_controller.reset(QLowEnergyController::createCentral());// itt kap értéket a _controller

    //QLowEnergyConnectionParameters connP;
    //connP.setIntervalRange(32.5, 32.5);
    //connP.setIntervalRange(16.25, 16.25);
    //_controller->requestConnectionUpdate(connP);

    Connect();
    auto reconnect = [this](){Connect();};
    QObject::connect(_controller.data(), &QLowEnergyController::disconnected, reconnect);
}

void BleServer::WriteCharacteristic(QUuid characteristicUuid, const QByteArray &value)
{    
    if(!_service) return;
    //_isWritten = false;
    QLowEnergyCharacteristic characteristic
            = _service->characteristic(characteristicUuid);
    Q_ASSERT(characteristic.isValid());
    _service->writeCharacteristic(characteristic, value); // Potentially causes notification.
}

//QByteArray BleServer::ReadCharacteristic(QUuid characteristicUuid)
//{
//    QLowEnergyCharacteristic characteristic
//            = _service->characteristic(characteristicUuid);
//    Q_ASSERT(characteristic.isValid());
//    QByteArray value;
//    _service->characteristicRead(characteristic, value);
//    return value;
//}
