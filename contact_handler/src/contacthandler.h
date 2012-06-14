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

#ifndef CONTACTHANDLER_H
#define CONTACTHANDLER_H

#include <QObject>
#include <QDBusConnection>

#include "../common/common.h"
#include "qtcontacts.h"

using namespace QtMobility;

class ContactHandler : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.callrecorder.contacthandler")
public:
    explicit ContactHandler(QObject *parent = 0);
    ~ContactHandler();
    
signals:
    void ready();
    
public slots:
    QString searchContactName(const QString &);
    QStringList searchContactNameIncludeAvatar(const QString &);

    QList<item_contact_cache *> getCache();

    bool ping();

private slots:
    void cacheContacts();
    void stateChanged(QContactAbstractRequest::State);

private:
    QContactManager *contactManager;
    QContactFetchRequest *contactFetchRequest;

    void fetchAllContacts();

    QList<item_contact_cache *> contactCache;
    
};

#endif // CONTACTHANDLER_H
