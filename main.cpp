/***************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtBluetooth/qlowenergyadvertisingdata.h>
#include <QtBluetooth/qlowenergyadvertisingparameters.h>
#include <QtBluetooth/qlowenergycharacteristic.h>
#include <QtBluetooth/qlowenergycharacteristicdata.h>
#include <QtBluetooth/qlowenergydescriptordata.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <QtBluetooth/qlowenergyservice.h>
#include <QtBluetooth/qlowenergyservicedata.h>
#include <QtCore/qbytearray.h>
#ifndef Q_OS_ANDROID
#include <QtCore/qcoreapplication.h>
#else
#include <QtGui/qguiapplication.h>
#endif
#include <QtCore/qlist.h>
#include <QtCore/qloggingcategory.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qtimer.h>

// valami123
// sudo setcap cap_net_raw+ep ./bletest_pizero
//void written1(const QLowEnergyCharacteristic &characteristic,
//                           const QByteArray &newValue)
//{
//    qDebug()<<"Write";
//};

int main(int argc, char *argv[])
{
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
#ifndef Q_OS_ANDROID
    QCoreApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif

     qDebug()<<"Start";

    QUuid srv1("00001234-0000-1000-8000-00805F9B34FB");
    QUuid char1("00001235-0000-1000-8000-00805F9B34FB");
    QUuid char2("00001236-0000-1000-8000-00805F9B34FB");
    QUuid char3("00001237-0000-1000-8000-00805F9B34FB");

    //! [Advertising Data]
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setIncludePowerLevel(true);
    advertisingData.setLocalName("TesztService1");
    advertisingData.setServices(QList<QBluetoothUuid>() << srv1);//QBluetoothUuid::HeartRate);
    //! [Advertising Data]

    //! [Service Data]

    //! [Characteristic1 + Descriptor1]
    QLowEnergyCharacteristicData charData1;
    charData1.setUuid(char1);
    charData1.setValue(QByteArray(2, 0));
    charData1.setProperties(QLowEnergyCharacteristic::Notify);
    const QLowEnergyDescriptorData clientConfig1(QBluetoothUuid::ClientCharacteristicConfiguration,
                                                QByteArray(2, 0));
    charData1.addDescriptor(clientConfig1);

    //! [Characteristic2 + Descriptor2]
    QLowEnergyCharacteristicData charData2;
    charData2.setUuid(char2);
    charData2.setValue(QByteArray(2, 0));
    charData2.setProperties(QLowEnergyCharacteristic::Read);
    const QLowEnergyDescriptorData clientConfig2(QBluetoothUuid::ClientCharacteristicConfiguration,
                                                QByteArray(2, 0));
    charData2.addDescriptor(clientConfig2);

    //! [Characteristic3 + Descriptor3]
    QLowEnergyCharacteristicData charData3;
    charData3.setUuid(char3);
    charData3.setValue(QByteArray(2, 0));
    charData3.setProperties(QLowEnergyCharacteristic::Write);
    const QLowEnergyDescriptorData clientConfig3(QBluetoothUuid::ClientCharacteristicConfiguration,
                                                QByteArray(2, 0));
    charData3.addDescriptor(clientConfig3);

    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(srv1);
    serviceData.addCharacteristic(charData1);
    serviceData.addCharacteristic(charData2);
    serviceData.addCharacteristic(charData3);
    //! [Service Data]

    auto read = [](const QLowEnergyCharacteristic &characteristic,
                               const QByteArray &newValue)
    {
         qDebug()<<"Read:"+QString(newValue);
    };

    auto changed = [](const QLowEnergyCharacteristic &characteristic,
                               const QByteArray &newValue)
    {
         qDebug()<<"Changed:"+QString(newValue);
    };

    //! [Start Advertising]
    const QScopedPointer<QLowEnergyController> leController(QLowEnergyController::createPeripheral());
    //auto s = leController->addService(serviceData);
    QScopedPointer<QLowEnergyService> service(leController->addService(serviceData));
    QObject::connect(service.get(), &QLowEnergyService::characteristicChanged, changed);
    QObject::connect(service.get(), &QLowEnergyService::characteristicRead, read);

    //QObject::connect(s, &QLowEnergyService::characteristicWritten, written);

    leController->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData,
                                   advertisingData);
    //! [Start Advertising]

    //! [Provide Heartbeat]
    QTimer heartbeatTimer;
    quint8 currentHeartRate = 60;
    enum ValueChange { ValueUp, ValueDown } valueChange = ValueUp;
    const auto heartbeatProvider = [&service, &currentHeartRate, &valueChange, char1]() {
        QByteArray value;
        value.append(char(0)); // Flags that specify the format of the value.
        value.append(char(currentHeartRate)); // Actual value.
        QLowEnergyCharacteristic characteristic
                = service->characteristic(char1);
        Q_ASSERT(characteristic.isValid());
        service->writeCharacteristic(characteristic, value); // Potentially causes notification.
        if (currentHeartRate == 60)
            valueChange = ValueUp;
        else if (currentHeartRate == 100)
            valueChange = ValueDown;
        if (valueChange == ValueUp)
            ++currentHeartRate;
        else
            --currentHeartRate;
    };
    QObject::connect(&heartbeatTimer, &QTimer::timeout, heartbeatProvider);
    heartbeatTimer.start(1000);

    //! [Provide Heartbeat]
    QByteArray value2;
    value2.append(0x12); // Flags that specify the format of the value.
    value2.append(0x34); // Actual value.
    QLowEnergyCharacteristic characteristic2 = service->characteristic(char2);
    Q_ASSERT(characteristic2.isValid());
    service->writeCharacteristic(characteristic2, value2);

    auto reconnect = [&leController, advertisingData, &service, serviceData, &changed, &read]()
    {
        service.reset(leController->addService(serviceData));
        if (!service.isNull()){
            QObject::connect(service.get(), &QLowEnergyService::characteristicChanged, changed);
            QObject::connect(service.get(), &QLowEnergyService::characteristicRead, read);

            leController->startAdvertising(QLowEnergyAdvertisingParameters(),
                                           advertisingData, advertisingData);
        }
    };
    QObject::connect(leController.data(), &QLowEnergyController::disconnected, reconnect);




    //auto s = service.get();


    return app.exec();
}
