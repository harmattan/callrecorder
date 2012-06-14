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

#ifndef ITEMCREATOR_H
#define ITEMCREATOR_H

#include "model/recordfilecell.h"
#include "model/recordfilemodel.h"

class MListContentItemCreator : public MAbstractCellCreator<RecordFileCell>
    {
    public:
        MListContentItemCreator(MList *parent) : highlightText(""), parent(parent) {
        }

        MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
        {
            RecordFileCell *cell = dynamic_cast<RecordFileCell *>(recycler.take(RecordFileCell::staticMetaObject.className()));
            if (cell == NULL) {
                cell = new RecordFileCell(parent);
            }
            updateCell(index, cell);
            return cell;
        }

        void updateCell(const QModelIndex &index, MWidget *cell) const {
            RecordFileCell *listCell = qobject_cast<RecordFileCell*>(cell);
            if (listCell == NULL)
                return;

            QVariant data = index.data(Qt::DisplayRole);
            RecordFileItem *entry = static_cast<RecordFileItem *>(data.value<void *>());

            QString mainTitle = entry->phoneNumberOrIMid;

            if(!entry->displayName.isEmpty())
                mainTitle = entry->displayName;

            if(highlightText.isEmpty())
                listCell->setTitle(mainTitle);
            else {
                QString highlightedTitle = mainTitle;
                int matchingIndex = highlightedTitle.indexOf(highlightText, 0, Qt::CaseInsensitive);
                if(matchingIndex != -1) {
                    highlightedTitle.insert(matchingIndex+highlightText.length(), "</span>");
                    highlightedTitle.insert(matchingIndex, "<span style='color:#FF00FF;'>");
                }
                listCell->setTitle(highlightedTitle);
            }

            listCell->setSubtitle(entry->fileDetail);
            listCell->callType(entry->callType);
        }

        void highlightByText(QString text) {
            highlightText = text;
        }

    private:
        QString highlightText;
        MList *parent;
    };

#endif // ITEMCREATOR_H
