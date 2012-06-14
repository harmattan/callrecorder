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

#ifndef PHONESERVICE_H
#define PHONESERVICE_H

#include "../../../common/common.h"
#include "../../../common/settings.h"

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusMessage>

#define CALLUI_CONTEXT_NAME "com.nokia.CallUi.Context"
#define CALLUI_ACTIVECALL_PATH "/com/nokia/CallUi/ActiveCall"
#define CALLUI_ACTIVECALL_INTERFACE "org.maemo.contextkit.Property"

#define TELEPATHY_CUI_NAME "com.nokia.call-ui"
#define TELEPATHY_CUI_PATH "/Call"
#define TELEPATHY_CUI_INTERFACE "Com.Nokia.Telephony.CallUi"

class PhoneService : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.callrecorder.phone")
public:
    explicit PhoneService(QObject *parent = 0);
    ~PhoneService();

Q_SIGNALS:
    void finished(const bool&);

public Q_SLOTS:
    bool ping();

    void newcall(const QString&, const int&, const QString&);
    void endcall(const QString&, const int&, const int&, const QString&);
    void valuechanged(const QDBusMessage&);
    void callanswered(const QString&);

private:
    QString m_phoneNumber;
    bool m_phoneType; // true -> incoming | false -> outgoing (IN/OUT)

    void Init();
};

#endif // PHONESERVICE_H
