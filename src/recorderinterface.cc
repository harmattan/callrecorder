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

#include "recorderinterface.h"

#include <QDBusReply>

RecorderInterface::RecorderInterface(QObject *parent) :
    QObject(parent)
{
    QDBusConnection::sessionBus().connect(R_SERVICE, R_PATH, R_SERVICE,"finished",this,SIGNAL(finished(bool)));
}

bool RecorderInterface::ping() {
    QDBusMessage message = QDBusMessage::createMethodCall(R_SERVICE,R_PATH,R_INTERFACE,"ping");
    QDBusMessage reply = QDBusConnection::sessionBus().call(message);
    if (reply.type() == QDBusMessage::ErrorMessage)
        return false;

    QList<QVariant> rlist = reply.arguments();

    Q_UNUSED(rlist);
    return true;
}

QList<QVariant> RecorderInterface::dbus_call(const QString &method) {
    QDBusMessage message = QDBusMessage::createMethodCall(R_SERVICE,R_PATH,R_INTERFACE, method);
    QDBusReply<QList<QVariant> > reply = QDBusConnection::sessionBus().call(message);

    if(reply.isValid())
        return reply.value();

    return QList<QVariant>();
}
