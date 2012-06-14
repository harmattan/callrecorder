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

#include "recorderservice.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDateTime>

//#include <QProcess>

RecorderService::RecorderService(QObject *parent) :
    QObject(parent)
{
    QDBusConnection::sessionBus().registerObject(R_OBJECT, this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    QDBusConnection::sessionBus().registerService(R_SERVICE);

    audioCapture = new QAudioCaptureSource;
    mediaRecorder = new QMediaRecorder(audioCapture);

    connect(mediaRecorder, SIGNAL(error(QMediaRecorder::Error)), SLOT(error(QMediaRecorder::Error)));

//    m_inputs = "oss:/dev/DspBridge" ;
    m_inputs = "pulseaudio:" ;

    setup_audio_codecs();

    emit finished(true);
}

RecorderService::~RecorderService() {
    QDBusConnection::sessionBus().unregisterObject(R_OBJECT);
    QDBusConnection::sessionBus().unregisterService(R_SERVICE);

    delete audioCapture;
    delete mediaRecorder;
}

bool RecorderService::ping() {
    return true;//pong
}

void RecorderService::setup_audio_codecs() {
    QList<QStringList> acodecs;

    //PCM
    acodecs << (QStringList() << "audio/PCM" << "wav" << "wav" << "8000" << "0" << "1");
    acodecs << (QStringList() << "audio/PCM" << "wav" << "wav" << "16000" << "0" << "1");
    acodecs << (QStringList() << "audio/PCM" << "wav" << "wav" << "8000" << "0" << "2");
    acodecs << (QStringList() << "audio/PCM" << "wav" << "wav" << "16000" << "0" << "2");
    //AMR
    acodecs << (QStringList() << "audio/AMR" << "3gpp" << "amr" << "8000" << "6700" << "1");
    acodecs << (QStringList() << "audio/AMR" << "3gpp" << "amr" << "8000" << "10200" << "1");
    //AAC
    acodecs << (QStringList() << "audio/AAC" << "avi" << "mp3" << "32000" << "64000" << "1");
    acodecs << (QStringList() << "audio/AAC" << "avi" << "mp3" << "32000" << "64000" << "2");
    acodecs << (QStringList() << "audio/AAC" << "avi" << "mp3" << "32000" << "128000" << "1");
    acodecs << (QStringList() << "audio/AAC" << "avi" << "mp3" << "32000" << "128000" << "2");

    foreach(QStringList acodec, acodecs) {
        audio_codec *ac = new audio_codec();
        ac->codec = acodec.at(0);
        ac->container = acodec.at(1);
        ac->fileExtension = acodec.at(2);
        ac->sample_rate = acodec.at(3).toInt();
        ac->bitrate = acodec.at(4).toInt();
        ac->channels = acodec.at(5).toInt();
        m_audioCodecs << ac;
    }

}

void RecorderService::set_audio_output_format() {
     audioSettings.setCodec(m_audioCodecs[settings.get_audio_output_format()]->codec);
     audioSettings.setSampleRate(m_audioCodecs[settings.get_audio_output_format()]->sample_rate);
     audioSettings.setBitRate(m_audioCodecs[settings.get_audio_output_format()]->bitrate);
     audioSettings.setQuality(QtMultimediaKit::HighQuality); //QtMultimediaKit::HighQuality | QtMultimediaKit::NormalQuality
     audioSettings.setEncodingMode(QtMultimediaKit::ConstantBitRateEncoding); //QtMultimediaKit::ConstantQualityEncoding | QtMultimediaKit::ConstantBitRateEncoding
     audioSettings.setChannelCount(m_audioCodecs[settings.get_audio_output_format()]->channels); //1:Mono | 2:Stereo
     m_container = m_audioCodecs[settings.get_audio_output_format()]->container;
     m_ext = m_audioCodecs[settings.get_audio_output_format()]->fileExtension;
}

void RecorderService::start_recorder(const QString &phoneName, const bool &phoneType) {
    set_audio_output_format();

    m_phoneNumber = phoneName;
    m_phoneType = phoneType;
    audioCapture->setAudioInput(m_inputs);
    mediaRecorder->setOutputLocation(generateAudioFilePath());
    mediaRecorder->setEncodingSettings(audioSettings,  QVideoEncoderSettings(), m_container);
    mediaRecorder->record();
}

void RecorderService::stop_recorder() {
    if (mediaRecorder->state() != QMediaRecorder::StoppedState ){
        mediaRecorder->stop();
        showNotification();
        showSaveConfirmation();
    }
}

void RecorderService::error(QMediaRecorder::Error error) {
    Q_UNUSED(error);
}

QUrl RecorderService::generateAudioFilePath()
{
    system(QString("mkdir -p %1").arg(settings.get_home_dir()).toAscii());
    QDir outputDir(settings.get_home_dir());
    QString phoneType = m_phoneType ? "In" :"Out";

    QDateTime datetime = QDateTime::currentDateTime();    

    recordFile = QString("%1_%2_%3.%4").arg(m_phoneNumber)
            .arg(phoneType)
            .arg(datetime.toString("yyyyMMdd-hhmmss"))
            .arg(m_ext);

    QUrl location(QDir::toNativeSeparators(outputDir.canonicalPath() + QString("/%1").arg(recordFile)));
    return location;
}

void RecorderService::showNotification() {
    if (settings.get_save_option() == 1){
        system(QString("dbus-send --print-reply --dest=com.meego.core.MNotificationManager /notificationmanager com.meego.core.MNotificationManager.addNotification uint32:0 uint32:0 string:\"completed\" string:\"%1 call recording\" string:\"Completed\" string:\"action\" string:\"/usr/share/icons/hicolor/80x80/apps/callrecorder.png\" uint32:0").arg(m_phoneNumber).toAscii());
    }
}

void RecorderService::showSaveConfirmation() {
    if (settings.get_save_option() == 2) {
        system(QString("/opt/callrecorder/bin/confirmation %1 &").arg(recordFile).toStdString().c_str());
    }
}
