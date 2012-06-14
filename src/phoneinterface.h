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

#ifndef PHONEINTERFACE_H
#define PHONEINTERFACE_H

#include <QObject>
#include "../common/common.h"

class PhoneInterface : public QObject
{
    Q_OBJECT
public:
    explicit PhoneInterface(QObject *parent = 0);

    bool ping();

Q_SIGNALS:
    void finished(const bool&);

public Q_SLOTS:

};

#endif // PHONEINTERFACE_H
