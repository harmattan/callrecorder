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

#include "mark.h"

#include <MLayout>
#include <MLinearLayoutPolicy>

Mark::Mark(QGraphicsItem *parent) :
    MWidgetController(parent),
    m_layout(0)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setStyleName("CommonSheetHeaderPanel");
    setContentsMargins(0,0,0,0);

    markbt = new MButton(parent);
    markbt->setStyleName("CommonSingleButton");
    markbt->setMaximumHeight(55);

    m_layout = new QGraphicsLinearLayout(Qt::Horizontal, this);

    m_layout->setContentsMargins(0,0,0,0);
    m_layout->insertStretch(0);
    m_layout->insertStretch(1);
    m_layout->insertStretch(2);
    m_layout->insertItem(3,markbt);
    m_layout->insertStretch(4);
    m_layout->insertStretch(5);
    m_layout->setAlignment(markbt, Qt::AlignHCenter);
    m_layout->addStretch();
}
