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

#include "mainapp.h"

#include <MToolBar>
#include <MAction>

MainApp::MainApp(QWidget *parent) :
    MApplicationWindow(parent)
{
    m_tabindex = 0;

    create_tabbar();
    create_pages();

    m_actions << create_tab("icon-m-toolbar-list", true, 0)
    << create_tab("icon-m-toolbar-settings", false, 1)
    << create_tab("icon-m-invitation-pending", false, 2);

    m_pagelist.at(0)->appear(this);

    connect(m_actionGroup,SIGNAL(triggered(QAction*)), SLOT(switch_page(QAction*)));

    /*
    setOrientationAngleLocked(false);
    setOrientationAngle(M::Angle270);
    setOrientationLocked(true);
    */
}

void MainApp::create_tabbar() {
    setToolbarViewType(MToolBar::tabType);

    m_actionGroup = new QActionGroup(this);
    m_actionGroup->setExclusive(true);
}

MAction* MainApp::create_tab(const QString &tabicon, bool checked, int pos) {
    MAction *action = new MAction(this);
    action->setLocation(MAction::ToolBarLocation);
    action->setIconID(tabicon);
    action->setCheckable(true);
    action->setChecked(checked);
    action->setToolTip(QString("%1").arg(pos));
    action->setActionGroup(m_actionGroup);

    addAction(action);
    return action;
}

void MainApp::create_pages() {
    m_mainPage = new MainPage(this);
    m_controllerPage = new ControllerPage(this);
    m_aboutPage = new AboutPage;

    m_pagelist << m_mainPage << m_controllerPage << m_aboutPage;

    for(int i=0;i<m_pagelist.count();i++)
        m_pagelist.at(i)->setEscapeMode(MApplicationPageModel::EscapeCloseWindow);
}

void MainApp::switch_page(QAction *action){
    if (m_tabindex != action){
        int __index = action->toolTip().toInt();
        action->setChecked(true);
        m_tabindex = action;
        m_pagelist.at(__index)->appear(this);
    }
}

void MainApp::switchToRecordedFilesPage() {    
    switch_page(m_actions.at(0));
    m_mainPage->resetModel();
}

void MainApp::showFetchingContactSpinner() {
    m_mainPage->showSpinner();
}
