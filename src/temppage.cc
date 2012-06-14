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

#include "temppage.h"

TempPage::TempPage(const QString &title) :
    MApplicationPage()
{
    MApplicationPage::createContent();
    pannableViewport()->setVerticalPanningPolicy(MPannableViewport::PanningAsNeeded);

    layout = new MLayout(centralWidget());
    layout->setContentsMargins(0,0,0,0);

    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0,0,0,0);
    policy->setSpacing(0);

    headerWidget = new HeaderWidget;
    headerWidget->setTitle(title);

    policy->addItem(headerWidget);
}
