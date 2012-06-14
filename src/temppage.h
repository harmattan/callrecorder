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

#ifndef TEMPPAGE_H
#define TEMPPAGE_H

#include <MPannableViewport>
#include <MLinearLayoutPolicy>
#include <MApplicationPage>
#include <MLayout>
#include <MButton>

#include "headerwidget.h"

class TempPage : public MApplicationPage
{
public:
    TempPage(const QString&);

    MLayout *layout;
    MLinearLayoutPolicy *policy;
    HeaderWidget *headerWidget;
};

#endif // TEMPPAGE_H
