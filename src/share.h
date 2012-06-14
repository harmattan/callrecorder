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

#ifndef SHARE_H
#define SHARE_H

#include <QObject>
#include <QUrl>
#include <QStringListModel>

#include "qbluetoothuuid.h"
#include "qbluetoothserviceinfo.h"
#include "qbluetoothservicediscoveryagent.h"
#include "qbluetoothlocaldevice.h"
#include "qbluetoothdeviceinfo.h"
#include "qbluetoothtransfermanager.h"
#include "qbluetoothtransferrequest.h"
#include "qbluetoothtransferreply.h"

#include <QDesktopServices>
#include <MPopupList>

#include <MSheet>
#include <MBasicSheetHeader>
#include <QAction>

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MPannableViewport>
#include <MList>

#include <MBasicListItem>
#include <MImageWidget>

#include <QFile>
#include <QTime>

#include <MMessageBox>

QT_FORWARD_DECLARE_CLASS(QModelIndex)

QTM_USE_NAMESPACE

class DeviceCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    DeviceCellCreator() : MAbstractCellCreator<MBasicListItem>() {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MBasicListItem *cell = dynamic_cast<MBasicListItem *>(recycler.take(MBasicListItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new MBasicListItem(MBasicListItem::TitleWithSubtitle);
            cell->setStyleName("CommonBasicListItem");
            cell->initLayout();
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MBasicListItem *item = qobject_cast<MBasicListItem*>(cell);
        if(!item)
            return;

        QStringList bdata = index.data().toString().split(",");
        item->setTitle(bdata.at(0));
        item->setSubtitle(QString("%1 | %2").arg(bdata.at(1)).arg(bdata.at(2)));
    }
};


class BluetoothUI: public MSheet {
    Q_OBJECT
public:
    BluetoothUI();
    ~BluetoothUI();

    void show(const QString&);

private slots:
    void close_me();
    void scan_devices();

    void serviceDiscovered(const QBluetoothServiceInfo&);
    void discoveryFinished();

    void deviceClicked(const QModelIndex&);

    void transferProgress(const qint64&, const qint64&);
    void transferFinished(QBluetoothTransferReply*);

private:
    QBluetoothServiceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    MBasicSheetHeader *header;

    QMap<int, QBluetoothServiceInfo> discoveredServices;

    MMessageBox *waitBox;

    MList *deviceList;
    QStringListModel *deviceListModel;

    QString m_filePath;

    QTime m_eta;

    void discovery();

    QString sizeTF(const qint64&);
};

class Share : public QObject
{
    Q_OBJECT
public:
    explicit Share(QObject *parent = 0);

    void appear(const QString&);

    void show();
    
signals:
    
public slots:
    void clicked(const QModelIndex&);

private:
    BluetoothUI *m_bluetoothUI;
    QString m_filePath;
    
};

#endif // SHARE_H
