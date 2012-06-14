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

#ifndef COMMON_H
#define COMMON_H

#include <QString>

#define ROOT_DIR_INSIDE "/home/user/MyDocs/"
#define ROOT_DIR_OUTSIDE "/home/user/"
#define STORE_FOLDER "CallRecorder"
#define NOTES_FOLDER ".notes"

#define CT_SERVICE "org.callrecorder.contacthandler"
#define CT_PATH "/callrecorder/contacthandler"
#define CT_INTERFACE "org.callrecorder.contacthandler"

#define P_OBJECT "/callrecorder/phone"
#define P_SERVICE "org.callrecorder.phone"
#define P_PATH "/callrecorder/phone"
#define P_INTERFACE "org.callrecorder.phone"

#define R_SERVICE "org.callrecorder.recorder"
#define R_PATH "/callrecorder/recorder"
#define R_INTERFACE "org.callrecorder.recorder"
#define R_OBJECT "/callrecorder/recorder"

struct item_contact_cache {
    QString displayName;
    QString phoneNumber;
    QString imageURL;
    QString syncTarget; //addressbook | telepathy | etc...
};

#endif
