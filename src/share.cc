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

#include "share.h"

BluetoothUI::BluetoothUI():
    discoveryAgent(new QBluetoothServiceDiscoveryAgent),
    localDevice(new  QBluetoothLocalDevice),
    deviceList(new MList),
    deviceListModel(new QStringListModel(deviceList))
{
    setObjectName("BluetoothUI_MSheet");

    header = new MBasicSheetHeader(this);
    header->setObjectName("BluetoothUI_MSheet_header");
    header->setNegativeAction(new QAction(qtTrId("tr_cancel"), header));
    header->setPositiveAction(new QAction(qtTrId("tr_scan"), header));
    setHeaderWidget(header);

    MLayout *layout = new MLayout(centralWidget());
    layout->setContentsMargins(0,0,0,0);

    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0,0,0,0);
    policy->setSpacing(0);

    MPannableViewport *viewport = new MPannableViewport;
    viewport->setWidget(deviceList);

    deviceList->setObjectName("device_list");
    deviceList->setStyleName("CommonList");
    deviceList->setCellCreator(new DeviceCellCreator());
    deviceList->setItemModel(deviceListModel);

    policy->addItem(viewport, Qt::AlignCenter);
    policy->activate();

    connect(header->negativeAction(), SIGNAL(triggered()),SLOT(close_me()));
    connect(header->positiveAction(), SIGNAL(triggered()),SLOT(scan_devices()));

    connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)), SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), SLOT(discoveryFinished()));

    connect(deviceList, SIGNAL(itemClicked(QModelIndex)), SLOT(deviceClicked(QModelIndex)));
}

BluetoothUI::~BluetoothUI() {
    delete localDevice;
    delete discoveryAgent;
}

void BluetoothUI::close_me() {
    disappear();
}

void BluetoothUI::scan_devices() {
    deviceListModel->setStringList(QStringList());
    discoveredServices.clear();
    discovery();
}

void BluetoothUI::show(const QString &filePath) {
    m_filePath = filePath;
    if (!deviceListModel->stringList().count())
        discovery();
    appear(MSceneWindow::DestroyWhenDone);
}

void BluetoothUI::deviceClicked(const QModelIndex &index) {

    header->setProgressIndicatorVisible(true);
    header->positiveAction()->setEnabled(false);
    deviceList->setEnabled(false);

    QBluetoothTransferManager fTransfer;
    QBluetoothServiceInfo sInfo(discoveredServices.value(index.row()));
    QBluetoothTransferRequest fTransferRequest(sInfo.device().address());

    QBluetoothTransferReply *rTransfer = fTransfer.put(fTransferRequest, new QFile(m_filePath));

    if (rTransfer->error()) {
        MMessageBox *messageBox = new MMessageBox(qtTrId("tr_failed_to_send_the_file"), M::OkButton);
        messageBox->appear(MSceneWindow::DestroyWhenDone);
        return;
    }

    waitBox = new MMessageBox(qtTrId("tr_transfer_init"), M::CancelButton);
    waitBox->appear(MSceneWindow::DestroyWhenDone);

    connect(waitBox, SIGNAL(rejected()), rTransfer, SLOT(abort()));

    connect(rTransfer, SIGNAL(uploadProgress(qint64,qint64)), SLOT(transferProgress(qint64,qint64)));
    connect(rTransfer, SIGNAL(finished(QBluetoothTransferReply*)), SLOT(transferFinished(QBluetoothTransferReply*)));
}

void BluetoothUI::transferProgress(const qint64 &bSent, const qint64 &bTotal) {
    if(bSent == 0)
        m_eta.start();

    int percent = (bSent * 100) / bTotal;

    waitBox->setText(QString(qtTrId("tr_transferring")+" %1/%2 ...<br>"+qtTrId("tr_completed")+": %3%")
                     .arg(sizeTF(bSent))
                     .arg(sizeTF(bTotal))
                     .arg(percent));
}

QString BluetoothUI::sizeTF(const qint64 &bytes) {
    float KB = bytes / 1024.00;
    float MB = KB / 1024.00;
    float GB = MB / 1024.00;

    if (bytes <1024)
        return QString("%1 bytes").arg(bytes);
    else if (KB <1024)
        return QString("%1").number(KB,'f',2) + " KB";
    else if (MB <1024)
        return QString("%1").number(MB,'f',2) + " MB";
    else
        return QString("%1").number(GB,'f',2) + " GB";
}

void BluetoothUI::transferFinished(QBluetoothTransferReply *reply) {
    waitBox->disappear();

    QString msg;
    if (reply->error() == QBluetoothTransferReply::NoError) {
        msg = qtTrId("tr_transfer_completed");
    }else{
        switch(reply->error()) {
        case QBluetoothTransferReply::UnknownError: msg=qtTrId("tr_bluetooth_UnknownError");break;
        case QBluetoothTransferReply::FileNotFoundError: msg=qtTrId("tr_bluetooth_FileNotFoundError");break;
        case QBluetoothTransferReply::HostNotFoundError: msg=qtTrId("tr_bluetooth_HostNotFoundError");break;
        case QBluetoothTransferReply::UserCanceledTransferError: msg=qtTrId("tr_bluetooth_UserCanceledTransferError");break;
        default: break;
        }
    }
    MMessageBox *messageBox = new MMessageBox(msg, M::OkButton);
    messageBox->appear(MSceneWindow::DestroyWhenDone);

    header->setProgressIndicatorVisible(false);
    header->positiveAction()->setEnabled(true);
    deviceList->setEnabled(true);
}

void BluetoothUI::discovery() {
    if (discoveryAgent->isActive())
        discoveryAgent->stop();

    discoveryAgent->setUuidFilter(QBluetoothUuid(QBluetoothUuid::ObexObjectPush));
    discoveryAgent->start();

    header->setProgressIndicatorVisible(true);
    header->positiveAction()->setEnabled(false);
}

void BluetoothUI::serviceDiscovered(const QBluetoothServiceInfo &sInfo) {
    QString deviceName;

    if (sInfo.device().name().isEmpty())
        deviceName = sInfo.device().address().toString();
    else
        deviceName = sInfo.device().name();

    QMutableMapIterator<int, QBluetoothServiceInfo> dS(discoveredServices);
    while(dS.hasNext()){
        dS.next();
        if(sInfo.device().address() == dS.value().device().address()){
            dS.setValue(sInfo);
            return;
        }
    }

    QBluetoothLocalDevice::Pairing pairing;
    pairing = localDevice ->pairingStatus(sInfo.device().address());

    bool sParing = (pairing&QBluetoothLocalDevice::Paired) || (pairing&QBluetoothLocalDevice::AuthorizedPaired);
    bool sAuthParing = (pairing&QBluetoothLocalDevice::AuthorizedPaired);

    QStringList oldlist = deviceListModel->stringList()
            << QString("%1,%2,%3")
               .arg(deviceName)
               .arg(sInfo.device().address().toString())
               .arg(sParing?sAuthParing?qtTrId("tr_authorized_paired"):qtTrId("tr_paired"):qtTrId("tr_no_paired"));

    oldlist.removeDuplicates();

    int row = oldlist.count()-1;

    deviceListModel->setStringList(oldlist);

    discoveredServices.insert(row, sInfo);
}

void BluetoothUI::discoveryFinished() {
    header->setProgressIndicatorVisible(false);
    header->positiveAction()->setEnabled(true);
}

Share::Share(QObject *parent) :
    QObject(parent)
{
}

void Share::appear(const QString &filePath) {
    m_filePath = filePath;

    MPopupList *popuplist = new MPopupList();
    QStringListModel *model = new QStringListModel(this);
    QStringList stringList;
    stringList << qtTrId("tr_email") << qtTrId("tr_bluetooth");
    model->setStringList(stringList);
    popuplist->setTitle(qtTrId("tr_share_via"));
    popuplist->setItemModel(model);
    popuplist->appear();

    connect(popuplist, SIGNAL(clicked(QModelIndex)), SLOT(clicked(QModelIndex)));
}

void Share::clicked(const QModelIndex &index) {
    switch(index.row()) {
        case 0: QDesktopServices::openUrl(QUrl(QString("mailto:?subject=%1&body=%2&attach=%3").arg("").arg("").arg(m_filePath))); break;
        case 1: m_bluetoothUI = new BluetoothUI;
                m_bluetoothUI->show(m_filePath);
        break;
    }
}
