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

#ifndef RECORDFILEMODEL_H
#define RECORDFILEMODEL_H

#include "../common/common.h"
#include "../common/settings.h"
#include "recordfilecell.h"

#include <QStringList>
#include <QDateTime>
#include <QFileInfo>
#include <QLocale>
#include <MBanner>
#include <QDate>
#include <QDir>

#include <QAbstractTableModel>
#include <MAbstractItemModel>

struct RecordFileItem {
    RecordFileItem() {}
    QString fileName;
    QString fileDetail;
    QString phoneNumberOrIMid;
    RecordFileCell::CALLTYPE callType;
    QString displayName;
    QString avatarURL;
    QDateTime datetime;
};

struct GroupManager {
    GroupManager() {}
    QString groupName;
    int numberofRow;
    QList<int> row;
};

class ContactsHandler;

class RecordFileModel: public MAbstractItemModel
{
    Q_OBJECT
    
public:
    enum RecordFileModelRoles {
        RecordFileFileNameRole = Qt::UserRole + 1,
        RecordFileFileNameFilter,
        AVATAR,
        CONTACTNAME
    };

    RecordFileModel();
    virtual ~RecordFileModel();

    int groupCount() const;
    int rowCountInGroup(int group) const;

    QString groupTitle(int group) const;
    QVariant itemData(int row, int group, int role) const;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void clear();

    void updateData(const QModelIndex &first, const QModelIndex &last);

    void updateDisplayName(const QModelIndex&, const QModelIndex &,const QString&);
    void updateAvatarURL(const QModelIndex&, const QModelIndex &,const QString&);

    static QString fileSize(const qint64&);

private:
    QString fileInfo(const QString&);
    QDate fileDate(const QString&);
    QFileInfo fileMeta(const QString&);

    Settings settings;

    bool deleteFile(const int&);

    void showInfoBanner(const QString&);
    void recordFilesModel();

private:
    QVector<RecordFileItem *> recordFileItems;
    QList<GroupManager*> groupManager;
};

#endif //RECORDFILEMODEL_H
