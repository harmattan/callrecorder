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

#include "settings.h"

Settings::Settings()
{
}

int Settings::get_audio_output_format() {
    QSettings settings;
    settings.beginGroup("audio_output_format");
    if (settings.childKeys().indexOf("type") == -1)
        return 0;
    return settings.value("type").toInt();
}

int Settings::get_save_option() {
    QSettings settings;
    settings.beginGroup("save_option");
    if (settings.childKeys().indexOf("type") == -1)
        return 0;
    return settings.value("type").toInt();
}

int Settings::get_autostart() {
    QSettings settings;
    settings.beginGroup("services_autostart");
    if (settings.childKeys().indexOf("type") == -1)
        return 0;
    return settings.value("type").toInt();
}

int Settings::get_use_hidden_folder() {
    QSettings settings;
    settings.beginGroup("use_hidden_folder");
    if (settings.childKeys().indexOf("type") == -1)
        return 1;
    return settings.value("type").toInt();
}

int Settings::get_auto_fecthing_contact_name() {
    QSettings settings;
    settings.beginGroup("auto_fecthing_contact_name");
    if (settings.childKeys().indexOf("type") == -1)
        return 0;
    return settings.value("type").toInt();
}

int Settings::get_limit_number_of_files() {
    QSettings settings;
    settings.beginGroup("limit_number_of_files");
    if (settings.childKeys().indexOf("type") == -1)
        return 12; //Show all
    return settings.value("type").toInt();
}

int Settings::get_store_folder() {
    QSettings settings;
    settings.beginGroup("store_folder");
    if (settings.childKeys().indexOf("type") == -1)
        return 0; //hidden folder .CallRecorder
    return settings.value("type").toInt();
}

void Settings::set_autdio_output_format(int val) {
    QSettings settings;
    settings.setValue("audio_output_format/type", val);
}

void Settings::set_save_option(int val) {
    QSettings settings;
    settings.setValue("save_option/type", val);
}

void Settings::set_autostart(int val) {
    QSettings settings;
    settings.setValue("services_autostart/type", val);
}

void Settings::set_use_hidden_folder(int val) {
    QSettings settings;
    settings.setValue("use_hidden_folder/type", val);
}

void Settings::set_auto_fecthing_contact_name(int val) {
    QSettings settings;
    settings.setValue("auto_fecthing_contact_name/type", val);
}

void Settings::set_limit_number_of_files(int val) {
    QSettings settings;
    settings.setValue("limit_number_of_files/type", val);
}

void Settings::set_store_folder(int val) {
    QSettings settings;
    settings.setValue("store_folder/type", val);
}

QString Settings::get_home_dir() {
    int wFolder = get_store_folder();
    return QString("%1%2")
            .arg(wFolder?ROOT_DIR_OUTSIDE:ROOT_DIR_INSIDE)
            .arg(wFolder?STORE_FOLDER:QString(".%1").arg(STORE_FOLDER));
}

QString Settings::get_note_dir() {
    int wFolder = get_store_folder();
    QString sFolder = wFolder?STORE_FOLDER:QString(".%1").arg(STORE_FOLDER);
    return QString("%1%2")
            .arg(wFolder?ROOT_DIR_OUTSIDE:ROOT_DIR_INSIDE)
            .arg(sFolder.append("/.notes"));
}

