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

#include "recordfilemodel.h"

#include <QDebug>

RecordFileModel::RecordFileModel()
    : MAbstractItemModel(),
      recordFileItems(),
      groupManager()
{
  recordFilesModel();
}

RecordFileModel::~RecordFileModel()
{
}


int RecordFileModel::groupCount() const
{
  return groupManager.count();
}

int RecordFileModel::rowCountInGroup(int group) const
{
  if (group == -1) {
    return recordFileItems.count();
  } else {
    return groupManager.at(group)->numberofRow;
  }
}

QString RecordFileModel::groupTitle(int group) const
{
  return  groupManager.at(group)->groupName;
}

QVariant RecordFileModel::itemData(int row, int group, int role) const
{
  int flatRow = row;
  
  if (group >= 0 && row >= 0)
    flatRow = groupManager.at(group)->row[row];
  
  if (role == Qt::DisplayRole)
    return QVariant::fromValue(static_cast<void *>(recordFileItems[flatRow]));
  else if(role == RecordFileFileNameRole)
    return QVariant::fromValue(recordFileItems[flatRow]->fileName);
  else if(role == RecordFileFileNameFilter) {
      if (recordFileItems[flatRow]->displayName.isEmpty())
        return QVariant::fromValue(recordFileItems[flatRow]->phoneNumberOrIMid);
      else
        return QVariant::fromValue(recordFileItems[flatRow]->displayName);
  }
  else if(role == CONTACTNAME)
    return QVariant::fromValue(recordFileItems[flatRow]->displayName);
  else if(role == AVATAR)
    return QVariant::fromValue(recordFileItems[flatRow]->avatarURL);
  
  return QVariant();
}

bool RecordFileModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (count <= 0)
    return true;
  
  int flatRow = row;
  int group = parent.row();
  
  if (isGrouped() && group >= 0)
    flatRow = groupManager.at(group)->row[row];
  
  if (deleteFile(flatRow)) {
    beginRemoveRows(parent, row, row + count - 1, true);
        qDeleteAll(recordFileItems.begin() + flatRow, recordFileItems.begin() + flatRow + count - 1);
        groupManager.at(group)->row.removeAt(row);
        groupManager.at(group)->numberofRow--;
        if (!groupManager.at(group)->numberofRow) {
          groupManager.removeAt(group);
        }
    endRemoveRows();
    
    showInfoBanner(qtTrId("tr_file_deleted"));
    return true;
  }
  
  showInfoBanner(qtTrId("tr_cant_delete"));
  return false;
}

void RecordFileModel::updateDisplayName(const QModelIndex &index, const QModelIndex &parent, const QString &displayName) {
    int flatRow = index.row();
    int group = parent.row();

    if (isGrouped() && group >= 0)
      flatRow = groupManager.at(group)->row[index.row()];

    RecordFileItem *tRF = static_cast<RecordFileItem *>(recordFileItems[flatRow]);
    tRF->displayName = displayName;

    if (isGrouped()) {
      layoutAboutToBeChanged();
      layoutChanged();
    }
}

void RecordFileModel::updateAvatarURL(const QModelIndex &index, const QModelIndex &parent, const QString &avatarURL) {
    int flatRow = index.row();
    int group = parent.row();

    if (isGrouped() && group >= 0)
      flatRow = groupManager.at(group)->row[index.row()];

    RecordFileItem *tRF = static_cast<RecordFileItem *>(recordFileItems[flatRow]);
    tRF->avatarURL = avatarURL;

    if (isGrouped()) {
      layoutAboutToBeChanged();
      layoutChanged();
    }
}

void RecordFileModel::clear()
{
  if (!recordFileItems.isEmpty()) {
    beginResetModel();
    qDeleteAll(recordFileItems);
    recordFileItems.clear();    
    groupManager.clear();
    endResetModel();
  }
}

void RecordFileModel::updateData(const QModelIndex &first, const QModelIndex &last)
{
  emit dataChanged(first, last);
}

void RecordFileModel::recordFilesModel() {

  QDir outputDir(settings.get_home_dir());
  outputDir.setSorting(QDir::Time);
  outputDir.setFilter(QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files);
  
  QStringList slist = outputDir.entryList(QStringList() << "*");
  QDate cDate = QDate::currentDate();
  
  /*
    datetime ranges [today, yesterday, x days ago, last week, x weeks ago, last month and older]
    does it make sense ?
    */
  QString glable;
  for (int g=0;g<12;g++){
    GroupManager *gm = new GroupManager;
    if (g == 0)
        glable = qtTrId("tr_today");
    else if (g == 1)
        glable = qtTrId("tr_yesterday");
    else if (g > 1 && g < 7)
        glable = qtTrId("tr_days_ago").arg(QString::number(g));
    else if (g == 7)
        glable = qtTrId("tr_lastweek");
    else if (g == 8 || g == 9)
        glable = qtTrId("tr_weeks_ago").arg(QString::number(g-6));
    else if (g == 10)
        glable = qtTrId("tr_lastmonth");
    else
        glable = qtTrId("tr_older");

    gm->groupName = glable;
    gm->numberofRow = 0;
    groupManager.append(gm);
  }


  beginInsertRows(QModelIndex(), 0, slist.count() - 1, true);
  for (int i=0;i<slist.count();i++){
    QDate fd = fileDate(slist.at(i));

    bool showAll = settings.get_limit_number_of_files() == 12;
    int showLimit = settings.get_limit_number_of_files();

    if (cDate.month() == fd.month() && cDate.year() == fd.year()) { //In month
        int cdate_week = (cDate.dayOfYear() + 7 - (cDate.dayOfWeek() ? (cDate.dayOfWeek() - 1) : 6)) / 7;
        int fd_week = (fd.dayOfYear() + 7 - (fd.dayOfWeek() ? (fd.dayOfWeek() - 1) : 6)) / 7;
        if (fd == cDate) { //Today
            groupManager.at(0)->row.append(i);
            groupManager.at(0)->numberofRow++;
        } else {
            if (cdate_week == fd_week) {
                for (int j=1;j<=6;j++) {
                    if (cDate.addDays(-1*j) == fd) { //Yesterday, [2..6] days ago
                        groupManager.at(j)->row.append(i);
                        groupManager.at(j)->numberofRow++;
                    }
                }
            } else {
                for (int j=1;j<=3;j++) {
                    if (cdate_week-j == fd_week) { //Last week, [2..3] weeks ago
                        groupManager.at(j+6)->row.append(i);
                        groupManager.at(j+6)->numberofRow++;
                    }
                }
            }
        }
    } else if ((cDate.month() - fd.month() == 1) || (cDate.month() - fd.month() == -11)) { //Last month
        if (showAll || (!showAll && showLimit >= 1)) {
            groupManager.at(10)->row.append(i);
            groupManager.at(10)->numberofRow++;
        }
    } else { //Older
        if (showAll || (!showAll && (
                            (cDate.year() == fd.year() && cDate.month() - fd.month() <= showLimit +1) || //In year
                            (cDate.year() > fd.year() && fd.month() > cDate.addMonths(-(showLimit+1)).month()) //Last year or x years ago
                            ))) {
            groupManager.at(11)->row.append(i);
            groupManager.at(11)->numberofRow++;
        }
    }

    RecordFileItem *entry = new RecordFileItem;
    QString fileName = slist.at(i);
    QStringList PN_IM = fileName.split("_");
    entry->fileName = fileName;
    entry->fileDetail = fileInfo(slist.at(i));
    entry->datetime = fileMeta(slist.at(i)).created();
    entry->displayName = "";
    entry->avatarURL = "";
    entry->phoneNumberOrIMid = PN_IM.at(0);
    entry->callType = PN_IM.at(1)=="In"?RecordFileCell::INCOMING:RecordFileCell::OUTGOING;
    recordFileItems.append(entry);

  }
  endInsertRows();


  int i = 0;
  while (groupManager.count() > i) {
    if (!groupManager.at(i)->numberofRow){
      groupManager.removeAt(i);
      i--;
    }
    i++;
  }
  
  if (isGrouped()) {
    layoutAboutToBeChanged();
    layoutChanged();
  }

}

QString RecordFileModel::fileInfo(const QString &filePath) {
  QFileInfo f = fileMeta(filePath);
  QDateTime flM(f.created());
  
  QString __time = QLocale::system().toString(flM.time(), QLocale::ShortFormat);
  QString __date = flM.date().toString(QLocale::system().dateFormat(QLocale::ShortFormat).replace("yy","yyyy"));
  
  QString dt("%1, %2");
  
  return QString("%1 | <b>%2</b>")
    .arg(dt.arg(__date).arg(__time))
    .arg(fileSize(f.size()));
}

QDate RecordFileModel::fileDate(const QString &filePath) {
  QDateTime flM(fileMeta(filePath).created());
  return flM.date();
}

QFileInfo RecordFileModel::fileMeta(const QString &filePath) {
  QFileInfo f(QString("%1/%2").arg(settings.get_home_dir()).arg(filePath));
  return f;
}

QString RecordFileModel::fileSize(const qint64 &fsize){
  qint64 B = fsize;
  float KB = B / 1024.00;
  float MB = KB / 1024.00;
  float GB = MB / 1024.00;
  
  if (B <1024)
    return QString("%1 Bytes").arg(B);
  else if (KB <1024)
    return QString("%1").number(KB,'f',2) + " KB";
  else if (MB <1024)
    return QString("%1").number(MB,'f',2) + " MB";
  else
    return QString("%1").number(GB,'f',2) + " GB";
}

bool RecordFileModel::deleteFile(const int &row) {
  QFile fNote(QString("%1/%2.txt").arg(settings.get_note_dir()).arg(recordFileItems[row]->fileName));
  fNote.remove();
  QString filePath("%1/%2");
  QFile fFile(filePath.arg(settings.get_home_dir()).arg(recordFileItems[row]->fileName));
  return fFile.remove();
}

void RecordFileModel::showInfoBanner(const QString &content) {
  MBanner *rmbanner = new MBanner();
  rmbanner->setObjectName("rmBanner");
  rmbanner->setStyleName(MBannerType::InformationBanner);
  rmbanner->setTitle(content);
  rmbanner->appear(MSceneWindow::DestroyWhenDone);
}
