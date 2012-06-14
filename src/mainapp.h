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

#ifndef MAINAPP_H
#define MAINAPP_H

#include <MApplicationWindow>
#include <QActionGroup>
#include <MAction>

#include "mainpage.h"
#include "controllerpage.h"
#include "aboutpage.h"

class MainApp : public MApplicationWindow
{
    Q_OBJECT
public:
    MainApp(QWidget *parent = 0);

    void switchToRecordedFilesPage();
    void showFetchingContactSpinner();

private Q_SLOTS:
    void switch_page(QAction*);

private:
    QActionGroup *m_actionGroup;
    QAction *m_tabindex;

    MainPage *m_mainPage;
    ControllerPage *m_controllerPage;
    AboutPage *m_aboutPage;

    void create_tabbar();
    MAction *create_tab(const QString&, bool, int);

    void create_pages();

    QList<MApplicationPage*> m_pagelist;
    QList<MAction *> m_actions;
};

#endif // MAINAPP_H
