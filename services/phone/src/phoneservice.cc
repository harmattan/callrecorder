/*
 * Call Recorder - Recording phone calls for Harmattan PR 1.1
 * Copyright (C) 2011 Cuong Le <metacuong@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "phoneservice.h"

#include <QDBusConnection>
#include <QDBusArgument>
#include <QDebug>

PhoneService::PhoneService(QObject *parent) :
    QObject(parent),
    m_phoneNumber("unknown")
{
    QDBusConnection::sessionBus().registerObject(P_OBJECT, this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    QDBusConnection::sessionBus().registerService(P_SERVICE);

    Init();
}

PhoneService::~PhoneService() {
    QDBusConnection::sessionBus().disconnect(CALLUI_CONTEXT_NAME, CALLUI_ACTIVECALL_PATH, CALLUI_ACTIVECALL_INTERFACE,  "ValueChanged", this, SLOT(voip_valuechanged(const QDBusMessage&)));
    QDBusConnection::sessionBus().disconnect(TELEPATHY_CUI_NAME, TELEPATHY_CUI_PATH, TELEPATHY_CUI_INTERFACE, "NewCall", this, SLOT(voip_newcall(QString,int,QString)));
    QDBusConnection::sessionBus().disconnect(TELEPATHY_CUI_NAME, TELEPATHY_CUI_PATH, TELEPATHY_CUI_INTERFACE, "CallEnded", this, SLOT(voip_endcall(QString, int, int, QString)));
    QDBusConnection::sessionBus().disconnect(TELEPATHY_CUI_NAME, TELEPATHY_CUI_PATH, TELEPATHY_CUI_INTERFACE, "CallAnswered", this, SLOT(voip_callanswred(QString)));

    QDBusConnection::sessionBus().unregisterObject(P_OBJECT);
    QDBusConnection::sessionBus().unregisterService(P_SERVICE);
}

bool PhoneService::ping() {
    return true;
}

void PhoneService::Init() {
    bool a = QDBusConnection::sessionBus().connect(CALLUI_CONTEXT_NAME, CALLUI_ACTIVECALL_PATH, CALLUI_ACTIVECALL_INTERFACE,  "ValueChanged", this, SLOT(valuechanged(const QDBusMessage&)));
    bool b = QDBusConnection::sessionBus().connect(TELEPATHY_CUI_NAME, TELEPATHY_CUI_PATH, TELEPATHY_CUI_INTERFACE, "NewCall", this, SLOT(newcall(QString,int,QString)));
    bool c = QDBusConnection::sessionBus().connect(TELEPATHY_CUI_NAME, TELEPATHY_CUI_PATH, TELEPATHY_CUI_INTERFACE, "CallEnded", this, SLOT(endcall(QString, int, int, QString)));
    bool d = QDBusConnection::sessionBus().connect(TELEPATHY_CUI_NAME, TELEPATHY_CUI_PATH, TELEPATHY_CUI_INTERFACE, "CallAnswered", this, SLOT(callanswered(QString)));
    emit finished(a&&b&&c&&d);
}

void PhoneService::newcall(const QString &uuid, const int &itype, const QString &phone_number_or_im_id) {
    Q_UNUSED(uuid);
    QString reMark(phone_number_or_im_id);
    int reMarkIndex = reMark.indexOf(":");
    if (reMarkIndex)
        reMark = reMark.replace(reMarkIndex,1,"+");
    m_phoneNumber = reMark;
    m_phoneType = itype;
}

void PhoneService::endcall(const QString &uuid, const int&, const int&, const QString&) {
    Q_UNUSED(uuid);
    QDBusMessage message = QDBusMessage::createMethodCall(R_SERVICE,R_PATH,R_INTERFACE,"stop_recorder");
    QDBusConnection::sessionBus().call(message);
}

void PhoneService::valuechanged(const QDBusMessage &reply) {
    Q_UNUSED(reply);
}

void PhoneService::callanswered(const QString &uuid) {
    Q_UNUSED(uuid);
    QList<QVariant> args;
    args.append(m_phoneNumber);
    args.append(m_phoneType);
    QDBusMessage message = QDBusMessage::createMethodCall(R_SERVICE,R_PATH,R_INTERFACE,"start_recorder");
    message.setArguments(args);
    QDBusConnection::sessionBus().call(message);
}
