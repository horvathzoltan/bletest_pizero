#include "helpers/networkhelper.h"
#include "helpers/textfilehelper.h"
#include "helpers/filenamehelper.h"
#include "hwinfo.h"
#include <QStringList>

const QString HwInfo::_value = GetValue();
HwInfo::Model HwInfo::_model;
bool HwInfo::_inited= false;
QString HwInfo::_lastError;

QString HwInfo::GetValue(){
    auto a = NetworkHelper::hwid_ethernet();
    if(!a.isEmpty()) return a;
    a = NetworkHelper::hwid_wlan();
    if(!a.isEmpty()) return a;
    return {};
}

bool HwInfo::Init(){
    _inited=false;
    auto hwinfoFn = FileNameHelper::HwInfFileName();
    QStringList lines; // fájltartalom
    bool ok = TextFileHelper::LoadLines(hwinfoFn, &lines);

    if(!ok){
        _lastError=TextFileHelper::LastError();
        return false;
    }
    if(lines.isEmpty()) {
        _lastError = "no hwinfo record";
        return false;
    }

    _model = Model::Parse(lines.at(0));

    if(_model.hwid.isEmpty()){
        _lastError = "invalid hwinfo record";
        return false;
    }
    if(0!=_model.hwid.compare(HwInfo::_value, Qt::CaseInsensitive)){
        _lastError = "wrong hwid in record";
        return false;
    }
    _inited = true;
    return true;
}

HwInfo::Model HwInfo::Model::Parse(const QString &str)
{
    auto tokens = str.split(';');
    if(tokens.length()<3) return {};
    HwInfo::Model a = {
        .hwid=tokens.at(0),
        .serial=tokens.at(1),
        .type=tokens.at(2)};
    return a;
}

QString HwInfo::Model::toString()
{
    auto a =  hwid+';'+serial+';'+type;
    return a;
}
