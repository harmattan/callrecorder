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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QSettings>

#include "common.h"

class Settings
{
public:
    Settings();

    int get_audio_output_format();
    void set_autdio_output_format(int);

    int get_save_option();
    void set_save_option(int);

    int get_autostart();
    void set_autostart(int);

    int get_use_hidden_folder();
    void set_use_hidden_folder(int);

    int get_auto_fecthing_contact_name();
    void set_auto_fecthing_contact_name(int);

    int get_limit_number_of_files();
    void set_limit_number_of_files(int);

    int get_store_folder();
    void set_store_folder(int);

    QString get_home_dir();
    QString get_note_dir();
};

#endif // SETTINGS_H
