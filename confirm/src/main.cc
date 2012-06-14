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

#include <MApplication>
#include <MApplicationWindow>
#include <MComponentCache>
#include <QLocale>
#include <QTranslator>

#include "mainapp.h"

M_EXPORT int main(int argc, char ** argv)
{
    MApplication *app = MComponentCache::mApplication(argc, argv);

    QTranslator translator;
    QString localePrefix = QLocale::system().name();
    if (localePrefix.length() > 2)
        localePrefix = localePrefix.left(2);
    if (translator.load("callrecorder_" + localePrefix, "/usr/share/l10n/meegotouch/"))
        app->installTranslator(&translator);
    else if (translator.load("callrecorder_en", "/usr/share/l10n/meegotouch/"))
        app->installTranslator(&translator);

    app->setOrganizationName("cuonglb");
    app->setApplicationName("CallRecorder");


    mainapp m_mainapp(0, app);
    m_mainapp.show();

    return app->exec();
}
