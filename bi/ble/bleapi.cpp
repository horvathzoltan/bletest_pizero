#include "bleapi.h"
#include <QThread>

QUuid BleApi::_serviceUuid("00001234-0000-1000-8000-00805F9B34FB");
QUuid BleApi::_char_response("00001235-0000-1000-8000-00805F9B34FB");
QUuid BleApi::_char_request("00001237-0000-1000-8000-00805F9B34FB");

QAtomicInt BleApi::_majom;

BleApi::BleApi(const QString& name)
{        
    _bleServer = new BleServer(_serviceUuid, name);

    _bleServer->AddCharacteristic(_char_response, QByteArray(2, 0), QLowEnergyCharacteristic::Notify);
    _bleServer->AddCharacteristic(_char_request, QByteArray(2, 0), QLowEnergyCharacteristic::WriteNoResponse);

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
    if(func==nullptr) return false;
    if(_functions.contains(name)) return false;
    if(name.length()<2) return false;
    _functions.insert(name, func);
    return true;
}

bool BleApi::AddRequest(quint8 cmd, BleApiFn func)
{
    if(func==nullptr) return false;
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
            //qDebug()<<"nincs byte a commandhoz:"+fk;
        }
    }

    return b;
}


/// 012345 L=6
/// abv|de
/// ix = 3
/// L = 6
///
/// 0,138415*|hwinfo
/// 0,138422*|data
void BleApi::Changed(QBluetoothUuid uuid, const QString& keyValue)
{
    const QChar SEP('|');
    if(uuid != _char_request) return;
    if(keyValue.isEmpty()) return;

    //if(value.isEmpty()) return;
    int ix = keyValue.lastIndexOf(SEP);
    QString key, value, valueData;
    if(ix>0){
        key = keyValue.left(ix); // szeparátortól balra a kulcs, jobbra az érték ami amúgy a command is
        value = keyValue.right(keyValue.length()-ix-1);
    } else{
        key = QString(); // egyébként a kulcs üres és csak command van
        value = keyValue;
    }
    // a key a kérés kulcsát tartalmazza, ezt a válaszban változtatás nélkül visszaadjuk
    // a value most a parancsot és az adatot tartalmazza.

    ix = value.indexOf('*');
    if(ix>0){
        valueData = value.right(value.length()-ix-1);
        value = value.left(ix);
    }

    if(value == "data10")
    {
        auto tokens = key.split(':');
        int respCount = 1;
        int respTime = 50;
        if(tokens.length()>1){
            bool ok;
            int r1 = tokens[1].toInt(&ok);
            if(ok) respCount = r1;
        }
        if(tokens.length()>2){
            bool ok;
            int r1 = tokens[2].toInt(&ok);
            if(ok) respTime = r1;
        }
        bool ok = _majom.testAndSetOrdered(0, respCount);
        _majom.storeRelease(respCount);
        if(!ok) return;

        // command = "id:7|data10" // 7szer ismételünk

        forever
        {
            int i = _majom--;
            if(i <= 0)
                break;

            auto r = Execute(value, valueData);

            if(!key.isEmpty()) r.append((SEP+tokens[0]+':'+QString::number(i)).toUtf8());
            //bool ok;
            //uint reqkey = tokens[0].toUInt(&ok);
            //bool kimarad = ok && (reqkey%3==1 || reqkey%3==2);

            //if(!kimarad)
            _bleServer->WriteCharacteristic(_char_response, r);

            //while(!_bleServer->IsWritten())
            {
                QThread::msleep(respTime);
            }
        }

        _majom.storeRelease(0);
    }
    else if(value == "aaa"){
        int l = keyValue.length();

        QByteArray r1 = Execute(value, valueData);

        QByteArray r = QString::number(l).toUtf8()+'/'+r1;

        if(!key.isEmpty()) r.append((SEP+key).toUtf8());

        _bleServer->WriteCharacteristic(_char_response, r);

    }
    else
    {
        QByteArray r = Execute(value, valueData);

        if(!key.isEmpty()) r.append((SEP+key).toUtf8());

        _bleServer->WriteCharacteristic(_char_response, r);
    }
}


QByteArray BleApi::Execute(const QString& value, const QString& valueData)
{
    if(value.isEmpty()) return{};
    if(value.length()==1)
        return Execute(value.at(0), valueData);

    auto function = _functions.find(value);

    if(function ==_functions.end()) return{};
    QByteArray response = function.value()(valueData);
    return response;
}

QByteArray BleApi::Execute(quint8 value, const QString& valueData)
{
    auto function =_bfunctions.find(value);

    if(function ==_bfunctions.end()) return{};
    QByteArray response = function.value()(valueData);
    return response;
}
