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

#ifndef DELETEFILES_H
#define DELETEFILES_H

#include <MSheet>
#include <MAction>
#include <MBasicSheetHeader>

#include <MList>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MPannableViewport>
#include <MMessageBox>

#include <QAction>

#include "model/recordfilecell.h"
#include "model/recordfilemodel.h"
#include "itemcreator.h"
#include "mark.h"

class DeleteFiles : public MSheet
{
    Q_OBJECT
public:
    DeleteFiles(RecordFileModel *recordFileItemModel);

    void show();

signals:
    void finished();

private:
    QAction *a_delete;
    MList *m_recordFiles;
    RecordFileModel *m_recordFileItemModel;
    MListContentItemCreator *cellCreator;

    bool isMarking;

    Mark *markbt;

private slots:
    void close_me();
    void _delete();
    void _delete_accepted();

    void markall();
    void itemClicked(const QModelIndex&);

    void markbt_setText();
};

#endif // DELETEFILES_H
