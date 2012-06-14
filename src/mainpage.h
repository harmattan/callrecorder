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

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "../common/common.h"

#include "temppage.h"

#include <MPannableViewport>
#include <MList>
#include <MBasicListItem>
#include <QStringListModel>
#include <MAbstractCellCreator>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <MAction>
#include <MObjectMenu>
#include <MSortFilterProxyModel>
#include <MProgressIndicator>
#include <MApplicationWindow>
#include <MSeparator>
#include <MWidgetAction>
#include <MComboBox>

#include <MListFilter>
#include <QLineEdit>
#include <QTimer>

#include <QDBusConnection>
#include <QDBusMessage>

#include "model/recordfilecell.h"
#include "model/recordfilemodel.h"
#include "itemcreator.h"
#include "playback.h"
#include "deletefiles.h"
#include "searchbox.h"
#include "controllerpage.h"

#include "../common/settings.h"
#include "share.h"

QT_USE_NAMESPACE

class MainPage : public TempPage
{
    Q_OBJECT
public:
    MainPage(MApplicationWindow *mainapp);
    ~MainPage();

    void showSpinner();
    void resetModel();

private Q_SLOTS:
    void recordFilesitemClicked(const QModelIndex&);
    void recordFilesitemLongTapped(const QModelIndex&,const QPointF&);

    void get_record_files();
    void openrecordFile();
    void removerecordFile();
    void shareFile();

    void showToggle(const bool&);
    void showDeleteFiles();

    void orientationChanged(M::Orientation);
    void listFocus();

    void searchTextChanged();
    void reloadProxyModel();

    void map_phonenumber_with_contactname();
    void limit_number_of_files_Changed(const int&);

    void _delete_accepted();

private:
    MList *recordFiles;
    MObjectMenu *objectMenu;
    QModelIndex recordFileindex;
    RecordFileModel *recordFileItemModel;
    MListContentItemCreator *cellCreator;
    MProgressIndicator *waittoOpen;

    MSortFilterProxyModel* filterproxyModel;
    MApplicationWindow *m_mainapp;

    MComboBox *limitMenuItem;
    MAction *delete_files;

    PlayBack *playback;
    DeleteFiles *deletefiles;

    SearchBox *searchBox;
    Share *m_share;
    QString m_fileShare;

    Settings settings;

    void createObjectMenu();
    void createPageMenu();
    void showSearchBox();

    MComboBox* createCoboxBox(QString, QStringList, int);
    MAction* createAppMenuItem(QString);
};

#endif // MAINPAGE_H
