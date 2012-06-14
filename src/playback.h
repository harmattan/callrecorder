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

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "../common/common.h"
#include "../common/settings.h"

#include <QAction>

#include <MSheet>
#include <MBasicSheetHeader>
#include <MLinearLayoutPolicy>
#include <MSlider>

#include <QTime>
#include <QMediaPlayer>
#include <QGraphicsLinearLayout>

#include <MButton>
#include <MRichTextEdit>
#include <MLabel>
#include <MLayout>
#include <MPannableViewport>
#include <MBanner>
#include <MMessageBox>
#include <MStylableWidget>
#include <MSeparator>
#include <MImageWidget>

#include <QTimer>
#include <QFile>
#include <QDir>

#include "model/recordfilemodel.h"

#include "qtcontacts.h"

QT_USE_NAMESPACE
QTM_USE_NAMESPACE

class PlayBack : public MSheet
{
    Q_OBJECT
public:
    PlayBack(const QString&, const QString&, const QString&);
    ~PlayBack();

    void show();
signals:
    void showToggle(const bool &is_show);
    void removerecordFile();
    void share();


private:
    QString m_recordfile;
    MLinearLayoutPolicy *wrapLayoutPolicy;
    MSlider *sliderBar;
    MButton *m_play;
    MButton *m_del;
    MButton *m_share;
    MRichTextEdit *rt_notes;

    Settings settings;

    qint64 m_duration;

    QMediaPlayer *m_player;

    bool m_seekBarPressed;
    bool m_playPressed;
    bool m_stopPressed;

    void details_zone();
    void play_zone();
    void notes_zone();

    void notes_save();
    void notes_load();

    QString contact_name(const QString&);

    QString miliseconds_to_timestr(const qint64&);

    QStringList fileInfo(const QString&);
    QStringList fileBase(const QString&);

    QString m_avatarURL;
    QString m_contactName;

private slots:
    void close_me();
    void sliderBarHandler(const int&);
    void durationChanged(const qint64&);
    void positionChanged(const qint64&);
    void stateChanged(QMediaPlayer::State);

    void seekBarPressed();
    void seekBarReleased();

    void play();
    void del();

};

#endif // PLAYBACK_H
