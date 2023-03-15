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

#include "bleserver.h"

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

    QUuid serviceUuid("00001234-0000-1000-8000-00805F9B34FB");
    QUuid char1("00001235-0000-1000-8000-00805F9B34FB");
    QUuid char2("00001236-0000-1000-8000-00805F9B34FB");
    QUuid char3("00001237-0000-1000-8000-00805F9B34FB");

    BleServer bleServer(serviceUuid, QStringLiteral("TesztService1"));

    bleServer.AddCharacteristic(char1, QByteArray(2, 0), QLowEnergyCharacteristic::Notify);
    bleServer.AddCharacteristic(char2, QByteArray(2, 0), QLowEnergyCharacteristic::Read);
    bleServer.AddCharacteristic(char3, QByteArray(2, 0), QLowEnergyCharacteristic::Write);

    bleServer.StartAdvertising();

    //! [Provide Heartbeat]
    QTimer heartbeatTimer;
    quint8 currentHeartRate = 60;
    enum ValueChange { ValueUp, ValueDown } valueChange = ValueUp;
    const auto heartbeatProvider = [&bleServer, &currentHeartRate, &valueChange, char1]() {
        QByteArray value;
        value.append(char(0)); // Flags that specify the format of the value.
        value.append(char(currentHeartRate)); // Actual value.
        bleServer.WriteCharacteriscic(char1, value);
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

    bleServer.WriteCharacteriscic(char2, value2);

    return app.exec();
}
