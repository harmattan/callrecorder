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

#ifndef CONTROLLERPAGE_H
#define CONTROLLERPAGE_H

#include "temppage.h"

#include <MGridLayoutPolicy>
#include <MPannableViewport>
#include <MComboBox>
#include <MStylableWidget>
#include <MBanner>
#include <MSeparator>
#include <MMessageBox>

#include <QDBusConnection>
#include <QDBusMessage>

#include "phoneinterface.h"
#include "recorderinterface.h"

#include "../common/common.h"
#include "../common/settings.h"
#include "mainpage.h"

#define CLSB "CommonLeftSplitButton"
#define CRSB "CommonRightSplitButton"


class MainApp;

class ControllerPage : public TempPage
{
    Q_OBJECT
public:
    ControllerPage(MApplicationWindow *mainapp);
    ~ControllerPage();

    static void enable_auto_fetching_contact_name(const bool&);

private Q_SLOTS:
    void manually_bt_services_start_clicked();
    void manually_bt_services_stop_clicked();

    void autostart_bt_enable_clicked();
    void autostart_bt_disable_clicked();

    void start_phone_service_finished(const bool&);
    void start_recorder_service_finished(const bool&);

    void rAudioOutput_changed(const int&);
    void rNotification_changed(const int&);
    void auto_fetching_contact_name_toggled(const bool&);
    void accepted_auto_fetching_contact_name();

    void folderList_currentIndexChanged(const int&);

    void _move_folder_rejected();
    void _move_folder_accepted();

private:
    MButton *manually_bt_services_start;
    MButton *manually_bt_services_stop;

    MButton *autostart_bt_enable;
    MButton *autostart_bt_disable;

    Settings m_settings;

    MComboBox *rAudioOutput;
    MComboBox *rNotification;
    MComboBox *rfolderList;

    MButton *auto_fetching_contact_name;

    PhoneInterface pInterface;
    RecorderInterface rInterface;

    MApplicationWindow *m_mainapp;

    void get_services_status();
    void get_autostart_status();
    void show_banner(const QString&, const QString&);
    bool ping_contact_handler();

    int __folderList_index;
};

#endif // CONTROLLERPAGE_H
