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

#include "bi/ble/bleapi.h"
#include "bi/dowork.h"

#include <QtBluetooth/qlowenergyadvertisingdata.h>
#include <QtBluetooth/qlowenergyadvertisingparameters.h>
#include <QtBluetooth/qlowenergycharacteristic.h>
#include <QtBluetooth/qlowenergycharacteristicdata.h>
#include <QtBluetooth/qlowenergydescriptordata.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <QtBluetooth/qlowenergyservice.h>
#include <QtBluetooth/qlowenergyservicedata.h>
#include <QtCore/qbytearray.h>
#include <QCommandLineParser>
#ifndef Q_OS_ANDROID
#include <QtCore/qcoreapplication.h>
#else
#include <QtGui/qguiapplication.h>
#endif
#include <QtCore/qlist.h>
#include <QtCore/qloggingcategory.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qtimer.h>

#include "helpers/nameof.h"
#include "helpers/stringify.h"
#include "helpers/logger.h"
#include "helpers/signalhelper.h"
#include "helpers/commandlineparserhelper.h"
#include "helpers/wiringpihelper.h"

#include "bi/buildnumber.h"
#include "bi/mcpreader.h"

/*
 * edit/preferences/debugger/gdb/Additional Startup Program
 * set solib-search-path /home/zoli/pizero_bullseye/qt5.15/lib
 *
 * So, the solution is very simple, apart from adding cap_net_admin,cap_net_raw+eip to gdb, you have also apply this to your shell. i.e. setcap cap_net_admin,cap_net_raw+eip /bin/bash
 * // ez a program, amit futtatunk
 * sshpass -p "qw" ssh pi@172.16.1.14 sudo setcap cap_net_admin,cap_net_raw+eip /home/pi/bletest_pizero/bin/bletest_pizero
 * // ugyanez a devicen kiadva -> projects/run/deployment/custom remote command
 * echo "qw" | sudo -S -k setcap cap_net_admin,cap_net_raw+eip /home/pi/bletest_pizero/bin/bletest_pizero
 *
 * a debuggernek és a shellnek is kell cap
 * // helyben a gdb fut
 * sshpass -p "qw" ssh pi@172.16.1.14 sudo setcap cap_net_admin,cap_net_raw+eip /usr/bin/gdb
 * // a shell futtatja az egészet
 * sshpass -p "qw" ssh pi@172.16.1.14 sudo setcap cap_net_admin,cap_net_raw+eip /usr/bin/bash
 * // remote debuggingnál a devicen a gdbserver fut
 * sshpass -p "qw" ssh pi@172.16.1.14 sudo setcap cap_net_admin,cap_net_raw+eip /usr/bin/gdbserver
*/

/*
Custom Process Step: buildnum_ -p Insole02 -t /home/zoli/bletest_pizero/buildnumber.h
Command: ~/buildnum_
Arguments: -p Insole02 -t %{sourceDir}/bi/buildnumber.h
Working directory: %{sourceDir}
*/

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.bluetooth.bluez.debug=true\n" "qt.bluetooth.debug=true");
    SignalHelper::setShutDownSignal(SignalHelper::SIGINT_); // shut down on ctrl-c
    SignalHelper::setShutDownSignal(SignalHelper::SIGTERM_); // shut down on killall

    Logger::Init(Logger::ErrLevel::INFO, Logger::DbgLevel::TRACE, true, true);
    WiringPiHelper::Init();
    McpReader::Init(QStringLiteral("logger_2v0"));

#if defined (STRING) && defined (TARGI)
    auto target = STRING(TARGI);
#else
    auto target=QStringLiteral("ApplicationNameString");
#endif

    QString user = qgetenv("USER");
    zInfo(QStringLiteral("started ")+target+" as "+user);

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(target);
    QCoreApplication::setApplicationVersion(Buildnumber::_value);
    QCoreApplication::setOrganizationName("OrganizationNameString");
    QCoreApplication::setOrganizationDomain("OrganizationDomainString");

    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("MesterCipő - MasterGaitInsole02"));
    parser.addHelpOption();
    parser.addVersionOption();

    const QString OPTION_TEST = QStringLiteral("test");

    CommandLineParserHelper::addOption(&parser, OPTION_TEST, QStringLiteral("command or command list to test"));

    parser.process(app);

    QString test = parser.value(OPTION_TEST);

    BleApi bleApi("Teszt1");

    DoWork::init({.bleApi = &bleApi, .test=test});

//  bleApi.addrequest(DoWork::miki);
//  bleApi.addrequest(DoWork::maki);
//  bleApi.AddRequest("maki", DoWork::maki);
//  bleApi.addrequest(DoWork::maki);

    bleApi.addrequest(DoWork::lasterr);
    bleApi.addrequest(DoWork::commands);
    bleApi.addrequest(DoWork::bommands);

    bleApi.addrequest(DoWork::data);
    bleApi.addrequest(DoWork::datalength);

    bleApi.addrequest(DoWork::hwinfo);
    bleApi.addrequest(DoWork::swinfo);
    bleApi.addrequest(DoWork::instance);

// rövidített commandok
    bleApi.AddRequest(0x17, DoWork::lasterr);
    bleApi.AddRequest(0x18, DoWork::commands);
    bleApi.AddRequest(0x19, DoWork::bommands);

    bleApi.AddRequest(0x37, DoWork::data);
    bleApi.AddRequest(0x40, DoWork::datalength);

    bleApi.AddRequest(0x51, DoWork::hwinfo);
    bleApi.AddRequest(0x52, DoWork::swinfo);
    bleApi.AddRequest(0x53, DoWork::instance);

    if(DoWork::isTest())
    {
        zInfo("___TEST___")
        DoWork::Test();
        zInfo("___END_TEST___")
        //QCoreApplication::quit();
        return 12;
    } else{
        bleApi.Start();
    }

    int e = app.exec();
    return e;
}
