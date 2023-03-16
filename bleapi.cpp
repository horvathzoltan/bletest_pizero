#include "bleapi.h"

QUuid BleApi::_serviceUuid("00001234-0000-1000-8000-00805F9B34FB");
QUuid BleApi::_char_response("00001235-0000-1000-8000-00805F9B34FB");
//QUuid BleApi::_char2("00001236-0000-1000-8000-00805F9B34FB");
QUuid BleApi::_char_request("00001237-0000-1000-8000-00805F9B34FB");

BleApi::BleApi(const QString& name)
{        
    _bleServer = new BleServer(_serviceUuid, name);

    _bleServer->AddCharacteristic(_char_response, QByteArray(2, 0), QLowEnergyCharacteristic::Notify);
//    _bleServer->AddCharacteristic(_char2, QByteArray(2, 0), QLowEnergyCharacteristic::Read);
    _bleServer->AddCharacteristic(_char_request, QByteArray(2, 0), QLowEnergyCharacteristic::Write);

    QObject::connect(_bleServer,
                     &BleServer::CharacteristicChanged,
                     this,
                     &BleApi::Changed);
}

BleApi::~BleApi()
{
    delete _bleServer;
}

void BleApi::Start()
{
    _bleServer->StartAdvertising();
}

void BleApi::AddRequest(const QString& name, BleApiFn func)
{
    _functions.insert(name, func);
}

void BleApi::Changed(QBluetoothUuid uuid, const QString& value)
{
    if(uuid != _char_request) return;
    QMap<QString, QByteArray (*)()>::iterator function = _functions.find(value);
    if(function ==_functions.end()) return;
    QByteArray response = function.value()();
    _bleServer->WriteCharacteristic(_char_response, response);
}
