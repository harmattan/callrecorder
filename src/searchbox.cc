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

#include "searchbox.h"

#include <MTextEdit>

SearchBox::SearchBox(MTextEdit *textSearch, QGraphicsItem *parent) :
    MWidgetController(parent),
    m_layout(0)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setStyleName("MBannerStyle");

    MTextEdit* m_textSearch = textSearch;
    m_textSearch->setVisible(true);
    m_textSearch->setPrompt(qtTrId("tr_search"));

    m_layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    m_layout->addItem(m_textSearch);
}
