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

#ifndef RECORDERSERVICE_H
#define RECORDERSERVICE_H

#include "../../../common/common.h"
#include "../../../common/settings.h"

#include <QObject>
#include <qaudiocapturesource.h>
#include <qmediarecorder.h>

struct audio_codec {
    audio_codec() {}
    QString codec;
    QString container;
    QString fileExtension;
    int sample_rate;
    int bitrate;
    int channels;
};

QT_USE_NAMESPACE

class RecorderService : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.callrecorder.recorder")
public:
    explicit RecorderService(QObject *parent = 0);
    ~RecorderService();

signals:
    void finished(const bool&);

public slots:
    bool ping();

    void start_recorder(const QString&, const bool&);
    void stop_recorder();

private slots:
    void error(QMediaRecorder::Error);

private:
    Settings settings;

    QAudioCaptureSource* audioCapture;
    QMediaRecorder* mediaRecorder;
    QAudioEncoderSettings audioSettings;

    QList<audio_codec*> m_audioCodecs;

    QString m_codec;
    QString m_inputs;
    QString m_container;
    QString m_ext;
    QString m_phoneNumber;
    bool m_phoneType;

    QString recordFile;

    int m_sample_rate;
    int m_bitrate;

    void setup_audio_codecs();
    void set_audio_output_format();

    void showNotification();
    void showSaveConfirmation();

    QUrl generateAudioFilePath();

};

#endif // RECORDERSERVICE_H
