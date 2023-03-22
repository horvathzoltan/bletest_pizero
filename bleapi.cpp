#include "bleapi.h"

QUuid BleApi::_serviceUuid("00001234-0000-1000-8000-00805F9B34FB");
QUuid BleApi::_char_response("00001235-0000-1000-8000-00805F9B34FB");
QUuid BleApi::_char_request("00001237-0000-1000-8000-00805F9B34FB");

BleApi::BleApi(const QString& name)
{        
    _bleServer = new BleServer(_serviceUuid, name);

    _bleServer->AddCharacteristic(_char_response, QByteArray(2, 0), QLowEnergyCharacteristic::Notify);
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

bool  BleApi::AddRequest(const QString& name, BleApiFn func)
{
    if(_functions.contains(name)) return false;
    if(name.length()<2) return false;
    _functions.insert(name, func);
    return true;
}

bool BleApi::AddRequest(quint8 cmd, BleApiFn func)
{
    if(_bfunctions.contains(cmd)) return false;
    _bfunctions.insert(cmd, func);
    return true;
}

QStringList BleApi::Requests() {
    QStringList a = _functions.keys();
    return a;
}

QStringList BleApi::BRequests() {
    QStringList functionKeys = _functions.keys();
    QStringList b;
    for (QString &fk : functionKeys) {
        auto v = _functions[fk];
        unsigned char bk = _bfunctions.key(v);
        if(bk>0){

            b<<"0x"+QString::number(bk, 16)+":"+fk;
        } else{
            qDebug()<<"nincs byte a commandhoz:"+fk;
        }

    }

    return b;
}

void BleApi::Changed(QBluetoothUuid uuid, const QString& value)
{
    if(uuid != _char_request) return;
    QMap<QString, QByteArray (*)()>::iterator function = _functions.find(value);
    if(function ==_functions.end()) return;
    QByteArray response = function.value()();
    _bleServer->WriteCharacteristic(_char_response, response);
}
