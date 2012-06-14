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

#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include "temppage.h"

#include <MImageWidget>
#include <MSeparator>
#include <MStylableWidget>

#include <QUrl>
#include <QDesktopServices>

#define APP_TITLE "<span style=\"font-family:Nokia Pure Text Bold; font-size:30px;\"><b>"+qtTrId("tr_callrecorder")+"</b></span><br>"
#define APP_LOGO "<img src=\"/usr/share/icons/hicolor/80x80/apps/callrecorder.png\"/>"
#define APP_VERSION qtTrId("tr_version_about")+": <b>0.0.9 beta</b><br>"
#define APP_LICENSE qtTrId("tr_license_about")+": <b>GNU GPLv3</b><br>"
#define APP_AUTHOR "<b>"+qtTrId("tr_author_about")+"</b><br><b>Cuong Le</b> (metacuong@gmail.com)<br>"

class AboutPage : public TempPage
{
    Q_OBJECT
public:
    AboutPage();

private slots:
    void feedback_clicked();

private:
    QString APP_THANKS;

};

#endif // ABOUTPAGE_H
