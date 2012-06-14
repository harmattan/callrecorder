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

#include "deletefiles.h"

DeleteFiles::DeleteFiles(RecordFileModel *recordFileItemModel):
    m_recordFiles(new MList),
    m_recordFileItemModel(recordFileItemModel),
    isMarking(false)
{
    setObjectName("CallRecorder_DeleteFiles");

    MBasicSheetHeader *header = new MBasicSheetHeader(this);

    a_delete = new QAction(qtTrId("tr_delete"), header);

    header->setObjectName("CallRecorder_PlayBack_Header");
    header->setNegativeAction(new QAction(qtTrId("tr_cancel"), header));
    header->setPositiveAction(a_delete);
    connect(header->negativeAction(), SIGNAL(triggered()),SLOT(close_me()));
    connect(header->positiveAction(), SIGNAL(triggered()),SLOT(_delete()));

    setHeaderWidget(header);

    cellCreator = new MListContentItemCreator(m_recordFiles);
    m_recordFiles->setCellCreator(cellCreator);

    m_recordFiles->setItemModel(m_recordFileItemModel);
    m_recordFiles->setIndexDisplayMode(MList::Floating);
    m_recordFiles->setShowGroups(true);
    m_recordFiles->setSelectionMode(MList::MultiSelection);

    connect(m_recordFiles, SIGNAL(itemClicked(QModelIndex)), SLOT(itemClicked(QModelIndex)));

    MLayout *layout = new MLayout(centralWidget());
    layout->setContentsMargins(0,0,0,0);

    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0,0,0,0);
    policy->setSpacing(0);

    MPannableViewport *viewport = new MPannableViewport;
    viewport->setWidget(m_recordFiles);

    policy->addItem(viewport);

    markbt = new Mark(this);
    connect(markbt->markbt,SIGNAL(clicked()), this, SLOT(markall()));
    markbt_setText();
    policy->addItem(markbt);

    bool hasRows = m_recordFileItemModel->rowCount();

    markbt->markbt->setEnabled(hasRows);
    a_delete->setEnabled(false);

    policy->activate();
}

void DeleteFiles::show() {
    appear(MSceneWindow::DestroyWhenDone);
}

void DeleteFiles::close_me() {
    dismiss();
}

void DeleteFiles::markall() {
    a_delete->setEnabled(!isMarking);
    if (isMarking) {
        m_recordFiles->selectionModel()->clearSelection();
        isMarking = false;
    }else {
        for (int i=0; i< m_recordFileItemModel->rowCount(); i++ ){
            QModelIndex parent = m_recordFileItemModel->index(i,0);
            for (int j=0;j<m_recordFileItemModel->rowCountInGroup(i);j++){
                QModelIndex index = m_recordFileItemModel->index(j,0, parent);
                m_recordFiles->selectionModel()->select(index, QItemSelectionModel::Select);
            }
        }
        isMarking = true;
    }
    markbt_setText();
}

void DeleteFiles::_delete() {
   MMessageBox *confirmtoDelete = new MMessageBox(qtTrId("tr_are_you_sure"), M::YesButton|M::NoButton);
   confirmtoDelete->setObjectName("confirmtoDelete_Mbox");
   confirmtoDelete->setTitle(qtTrId("tr_delete"));
   confirmtoDelete->appear(MSceneWindow::DestroyWhenDone);
   connect(confirmtoDelete,SIGNAL(accepted()), SLOT(_delete_accepted()));
}

void DeleteFiles::_delete_accepted() {
    QModelIndexList indexes = m_recordFiles->selectionModel()->selectedRows();
    for (int i = indexes.count() - 1; i > -1; --i) {
       m_recordFiles->itemModel()->removeRow(indexes.at(i).row(), indexes.at(i).parent());
       emit finished();
    }
    dismiss();
}

void DeleteFiles::itemClicked(const QModelIndex &index) {
    Q_UNUSED(index);
    a_delete->setEnabled(m_recordFiles->selectionModel()->hasSelection());
    isMarking = m_recordFiles->selectionModel()->hasSelection();
    markbt_setText();
}

void DeleteFiles::markbt_setText() {
    markbt->markbt->setText(isMarking?qtTrId("tr_unmark_all"):qtTrId("tr_mark_all"));
}
