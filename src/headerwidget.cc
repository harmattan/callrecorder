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

#include "headerwidget.h"

HeaderWidget::HeaderWidget(QGraphicsItem *parent) :
    MWidgetController(parent),
    m_title(0), m_layout(0)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setStyleName("CommonHeaderPanel");

    m_title = new MLabel(this);
    m_title->setTextElide(true);
    m_title->setStyleName("CallRecorderLabel");

    m_layout = new QGraphicsLinearLayout(Qt::Horizontal, this);

    waittoOpen = new MProgressIndicator(this, MProgressIndicator::spinnerType);
    waittoOpen->setObjectName("header_waittoOpen");
    waittoOpen->setStyleName("CommonListItemSpinnerInverted");
    waittoOpen->setUnknownDuration(true);
    waittoOpen->hide();

    m_layout->addItem(m_title);
    m_layout->addItem(waittoOpen);

}

void HeaderWidget::setTitle(const QString &title) {
    m_title->setText(title);
}
