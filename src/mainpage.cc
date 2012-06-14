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

#include "mainpage.h"

MainPage::MainPage(MApplicationWindow *mainapp) :
    TempPage(qtTrId("tr_records_title")),
    recordFiles(new MList),
    m_mainapp(mainapp),
    m_share(new Share)
{
    system(QString("mkdir -p %1").arg(settings.get_home_dir()).toAscii());

    cellCreator = new MListContentItemCreator(recordFiles);
    recordFiles->setCellCreator(cellCreator);
    recordFileItemModel = new RecordFileModel;
    recordFileItemModel->setParent(recordFiles);    

    searchBox = new SearchBox(recordFiles->filtering()->editor());

    showSearchBox();

    MPannableViewport *viewport = new MPannableViewport(this);
    viewport->setWidget(recordFiles);

    policy->addItem(viewport);
    policy->activate();

    waittoOpen = new MProgressIndicator(viewport, MProgressIndicator::spinnerType);
    waittoOpen->setObjectName("waittoOpen");
    waittoOpen->setStyleName("CommonLargeSpinner");
    waittoOpen->setUnknownDuration(true);
    waittoOpen->hide();    

    orientationChanged(m_mainapp->orientation());
    connect(m_mainapp, SIGNAL(orientationChanged(M::Orientation)), this, SLOT(orientationChanged(M::Orientation)));

    createObjectMenu();
    createPageMenu();

    QTimer::singleShot(500, this, SLOT(get_record_files()));
}

MainPage::~MainPage()
{
    ControllerPage::enable_auto_fetching_contact_name(false);
}

void MainPage::get_record_files() {
    recordFiles->setIndexDisplayMode(MList::Floating);
    recordFiles->setShowGroups(true);
    recordFileItemModel->setGrouped(true);

    filterproxyModel = new MSortFilterProxyModel();
    filterproxyModel->setObjectName("filterproxyModel");
    filterproxyModel->setFilterRole(RecordFileModel::RecordFileFileNameFilter);
    filterproxyModel->setDynamicSortFilter(true);
    filterproxyModel->setSourceModel(recordFileItemModel);

    recordFiles->setItemModel(filterproxyModel);

    recordFiles->filtering()->setFilterRole(RecordFileModel::RecordFileFileNameFilter);
    recordFiles->filtering()->editor()->setVisible(true);
    recordFiles->filtering()->setEnabled(true);

    connect(recordFiles->filtering()->editor(), SIGNAL(textChanged()), this, SLOT(searchTextChanged()));

    connect(recordFiles,SIGNAL(itemClicked(QModelIndex)), SLOT(recordFilesitemClicked(QModelIndex)));
    connect(recordFiles,SIGNAL(itemLongTapped(QModelIndex,QPointF)), SLOT(recordFilesitemLongTapped(QModelIndex,QPointF)));
    connect(recordFiles,SIGNAL(panningStarted()),SLOT(listFocus()));

    if (settings.get_auto_fecthing_contact_name())
        showSpinner();
    ControllerPage::enable_auto_fetching_contact_name(settings.get_auto_fecthing_contact_name());
    QDBusConnection::sessionBus().connect(CT_SERVICE, CT_PATH, CT_SERVICE,"ready", this, SLOT(map_phonenumber_with_contactname()));
}

void MainPage::map_phonenumber_with_contactname() {
    for (int i=0; i< recordFileItemModel->rowCount(); i++ ){
        QModelIndex parent = recordFileItemModel->index(i,0);
        for (int j=0;j<recordFileItemModel->rowCountInGroup(i);j++){
            QModelIndex index = recordFileItemModel->index(j,0, parent);

            QString filePath = index.data(RecordFileModel::RecordFileFileNameRole).toString();
            QStringList _fs = filePath.split("_");

            QList<QVariant> args;
            QDBusMessage message = QDBusMessage::createMethodCall(CT_SERVICE,CT_PATH,CT_SERVICE,"searchContactNameIncludeAvatar");
            args.append(_fs.at(0));
            message.setArguments(args);
            QDBusMessage reply = QDBusConnection::sessionBus().call(message);

            QStringList detail = reply.arguments().at(0).value<QStringList>();

            if (detail.count() == 2) {
                recordFileItemModel->updateDisplayName(index,parent,detail.at(0));
                recordFileItemModel->updateAvatarURL(index,parent,detail.at(1));
            }
        }
    }

    recordFiles->filtering()->proxy()->invalidate();

    headerWidget->waittoOpen->hide();
}

void MainPage::searchTextChanged() {
    cellCreator->highlightByText(recordFiles->filtering()->editor()->text());

    recordFiles->filtering()->proxy()->invalidate();
}

void MainPage::listFocus() {
    recordFiles->setFocus();
}

void MainPage::showSearchBox() {
    policy->insertItem(1,new MSeparator(centralWidget()));
    policy->insertItem(2,searchBox);
    policy->insertItem(3,new MSeparator(centralWidget()));
}

void MainPage::recordFilesitemClicked(const QModelIndex &index){
    QString filePath = index.data(RecordFileModel::RecordFileFileNameRole).toString();
    QString avatarURL = index.data(RecordFileModel::AVATAR).toString();
    QString contactName = index.data(RecordFileModel::CONTACTNAME).toString();

    showToggle(true);

    playback = new PlayBack(filePath, avatarURL, contactName);

    connect(playback,SIGNAL(showToggle(bool)), SLOT(showToggle(bool)));
    connect(playback, SIGNAL(removerecordFile()), SLOT(removerecordFile()));
    connect(playback, SIGNAL(share()), SLOT(shareFile()));

    playback->show();

    recordFileindex = index;
}

void MainPage::recordFilesitemLongTapped(const QModelIndex &index, const QPointF &position) {
    objectMenu->setCursorPosition(position);
    sceneManager()->appearSceneWindow(objectMenu);
    recordFileindex = index;
}

void MainPage::createObjectMenu() {
    objectMenu = new MObjectMenu(0);

    MAction *action = new MAction(qtTrId("tr_open"), this);
    action->setObjectName("openAction");
    objectMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(openrecordFile()));

    action = new MAction(qtTrId("tr_delete"), this);
    action->setObjectName("openDelete");
    objectMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(removerecordFile()));

    action = new MAction(qtTrId("tr_share"), this);
    action->setObjectName("openShare");
    objectMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(shareFile()));

}

void MainPage::openrecordFile() {
    recordFilesitemClicked(recordFileindex);
}

void MainPage::removerecordFile() {
    MMessageBox *confirmtoDelete = new MMessageBox(qtTrId("tr_are_you_sure"), M::YesButton|M::NoButton);
    confirmtoDelete->setObjectName("confirmtoDelete_Mbox");
    confirmtoDelete->setTitle(qtTrId("tr_delete"));
    confirmtoDelete->appear(MSceneWindow::DestroyWhenDone);
    connect(confirmtoDelete,SIGNAL(accepted()), SLOT(_delete_accepted()));
}

void MainPage::_delete_accepted() {
    if (recordFileindex.isValid()) {
        recordFiles->itemModel()->removeRow(recordFileindex.row(), recordFileindex.parent());
        reloadProxyModel();
        recordFileindex = QModelIndex();
    }
}

void MainPage::reloadProxyModel() {
    filterproxyModel->setSourceModel(recordFileItemModel);
}

void MainPage::shareFile() {
    QString fileName = recordFileindex.data(RecordFileModel::RecordFileFileNameRole).toString();

    m_share->appear(QString("%1/%2").arg(settings.get_home_dir()).arg(fileName));
}

void MainPage::showToggle(const bool &is_show) {
    recordFiles->setEnabled(!is_show);
    if (is_show)
       waittoOpen->show();
    else
       waittoOpen->hide();
}

void MainPage::orientationChanged(M::Orientation ori) {
    switch (ori) {
        case M::Landscape: waittoOpen->setPos(m_mainapp->sizeHint().width()/2+50,m_mainapp->sizeHint().height()/2-70);break;
        case M::Portrait: waittoOpen->setPos(m_mainapp->sizeHint().height()/2,m_mainapp->sizeHint().width()/2-50);break;
    }
}

void MainPage::limit_number_of_files_Changed(const int &index) {
    settings.set_limit_number_of_files(index);    
    resetModel();
}

void MainPage::resetModel() {
    delete recordFileItemModel;
    recordFileItemModel = new RecordFileModel;
    recordFileItemModel->setParent(recordFiles);
    recordFileItemModel->setGrouped(true);
    reloadProxyModel();

    if (settings.get_auto_fecthing_contact_name()) {
        headerWidget->waittoOpen->show();
        map_phonenumber_with_contactname();
        ControllerPage::enable_auto_fetching_contact_name(settings.get_auto_fecthing_contact_name());
    }
}

void MainPage::createPageMenu() {
    QStringList lLimit;

    for(int i = 1 ; i <= 12 ; i++) {
        if (i==1)
            lLimit << qtTrId("tr_in_month").arg(i);
        else
            lLimit << qtTrId("tr_in_months").arg(i);
    }

    lLimit << qtTrId("tr_show_all");

    limitMenuItem = createCoboxBox(qtTrId("tr_limit_number_of_files"), lLimit, settings.get_limit_number_of_files());
    connect(limitMenuItem, SIGNAL(currentIndexChanged(int)), SLOT(limit_number_of_files_Changed(int)));

    delete_files = createAppMenuItem(qtTrId("tr_delete_files"));
    connect(delete_files, SIGNAL(triggered()), SLOT(showDeleteFiles()));
    addAction(delete_files);
}

MAction* MainPage::createAppMenuItem(QString title) {
    MAction *ac = new MAction(centralWidget());
    ac->setText(title);
    ac->setObjectName("menuItem");
    ac->setLocation(MAction::ApplicationMenuLocation);
    return ac;
}

MComboBox* MainPage::createCoboxBox(QString title, QStringList dataList, int currentIndex) {\
    MWidgetAction *widgetAction = new MWidgetAction(centralWidget());
    widgetAction->setObjectName("widgetAction");
    widgetAction->setLocation(MAction::ApplicationMenuLocation);

    MComboBox *cb = new MComboBox;
    cb->setObjectName("comboBox");
    cb->setTitle(title);
    cb->setIconVisible(false);
    cb->addItems(dataList);
    cb->setCurrentIndex(currentIndex);
    widgetAction->setWidget(cb);

    addAction(widgetAction);

    return cb;
}

void MainPage::showDeleteFiles() {
    deletefiles = new DeleteFiles(recordFileItemModel);
    deletefiles->show();
    connect(deletefiles, SIGNAL(finished()), SLOT(reloadProxyModel()));
}

void MainPage::showSpinner() {
    headerWidget->waittoOpen->show();
}
