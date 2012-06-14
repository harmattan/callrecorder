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

#ifndef RECORDERINTERFACE_H
#define RECORDERINTERFACE_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingReply>

#include "../common/common.h"

class RecorderInterface : public QObject
{
    Q_OBJECT
public:
    explicit RecorderInterface(QObject *parent = 0);

Q_SIGNALS:
    void finished(const bool&);

public Q_SLOTS:
    bool ping();

private:
    QList<QVariant> dbus_call(const QString&);
};

#endif // RECORDERINTERFACE_H
