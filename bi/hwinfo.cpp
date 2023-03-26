#include "helpers/networkhelper.h"

#include "hwinfo.h"

const QString HwInfo::_value = GetValue();

QString HwInfo::GetValue(){
    auto a = NetworkHelper::hwid_ethernet();
    if(!a.isEmpty()) return a;
    a = NetworkHelper::hwid_wlan();
    if(!a.isEmpty()) return a;
    return {};
}
