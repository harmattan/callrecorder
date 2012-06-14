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

#include "contacthandler.h"

#include <QDebug>

ContactHandler::ContactHandler(QObject *parent) :
    QObject(parent)
{
    QDBusConnection::sessionBus().registerObject(CT_PATH, this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    QDBusConnection::sessionBus().registerService(CT_SERVICE);

    contactManager = new QContactManager("tracker");
    contactFetchRequest = new QContactFetchRequest();
    contactFetchRequest->setManager(contactManager);

    fetchAllContacts();
}

ContactHandler::~ContactHandler() {
    QDBusConnection::sessionBus().unregisterObject(CT_PATH);
    QDBusConnection::sessionBus().unregisterService(CT_SERVICE);

    delete contactFetchRequest;
    delete contactManager;
}

void ContactHandler::fetchAllContacts() {
    contactFetchRequest->start();

    connect(contactFetchRequest, SIGNAL(resultsAvailable()), SLOT(cacheContacts()));
    connect(contactFetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)), SLOT(stateChanged(QContactAbstractRequest::State)));
}

void ContactHandler::cacheContacts() {
    contactCache.clear();

    QList<QContact> cFR = contactFetchRequest->contacts();
    foreach(const QContact &ct, cFR) {
        QList<QContactPhoneNumber> phoneNumbers = ct.details<QContactPhoneNumber>();
        if (phoneNumbers.count()) {
            QString phoneNumber = phoneNumbers.at(0).value("PhoneNumber");
            QContactAvatar contactAvatar = ct.detail(QContactAvatar::DefinitionName);
            QContactSyncTarget contactSyncTarget = ct.detail(QContactSyncTarget::DefinitionName);
            item_contact_cache *icc = new item_contact_cache;
            icc->displayName = ct.displayLabel();
            icc->phoneNumber = phoneNumber;
            icc->imageURL = contactAvatar.imageUrl().toString();
            icc->syncTarget = contactSyncTarget.syncTarget();
            contactCache.append(icc);
        }
    }
}

void ContactHandler::stateChanged(QContactAbstractRequest::State state) {
    if (state == QContactAbstractRequest::FinishedState || state == QContactAbstractRequest::CanceledState) {
        emit ready();
    }
}

QString ContactHandler::searchContactName(const QString &value) {
    QString contactName;
    foreach(const item_contact_cache *icc, contactCache){
        if (icc->phoneNumber == value) {
            contactName = icc->displayName;
            break;
        }
    }
    return contactName;
}

QStringList ContactHandler::searchContactNameIncludeAvatar(const QString &value) {
    QStringList contactNameIncludeAvatar;
    foreach(const item_contact_cache *icc, contactCache){
        if (icc->phoneNumber == value) {
            contactNameIncludeAvatar << icc->displayName << icc->imageURL;
            break;
        }
    }
    return contactNameIncludeAvatar;
}

QList<item_contact_cache *> ContactHandler::getCache() {
    return contactCache;
}

bool ContactHandler::ping(){
    return true;
}
