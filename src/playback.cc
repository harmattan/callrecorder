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

#include "playback.h"

PlayBack::PlayBack(const QString &recordfile, const QString &avatarURL, const QString &contactName):
    m_recordfile(recordfile),
    m_player(new QMediaPlayer(this)),
    m_seekBarPressed(false),
    m_playPressed(false),
    m_stopPressed(false),
    m_avatarURL(avatarURL),
    m_contactName(contactName)
{
    setObjectName("CallRecorder_PlayBack");

    setContentsMargins(0,0,0,0);

    MLayout *layout = new MLayout(centralWidget());
    layout->setContentsMargins(0,0,0,0);

    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0,0,0,0);
    policy->setSpacing(0);

    MBasicSheetHeader *header = new MBasicSheetHeader(this);
    header->setObjectName("CallRecorder_PlayBack_Header");
    header->setNegativeAction(new QAction(qtTrId("tr_done"), header));
    connect(header->negativeAction(), SIGNAL(triggered()),SLOT(close_me()));

    setHeaderWidget(header);

    MLayout *wrapLayout = new MLayout;
    wrapLayoutPolicy = new MLinearLayoutPolicy(wrapLayout , Qt::Vertical);

    MStylableWidget *styleWidget = new MStylableWidget();
    styleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    styleWidget->setLayout(wrapLayout);

    MPannableViewport *viewport = new MPannableViewport;
    viewport->setObjectName("CallRecorder_PlayBack_pannableViewport");
    viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    viewport->setWidget(styleWidget);

    details_zone();
    play_zone();
    notes_zone();

    policy->addItem(viewport);
    policy->activate();

}

PlayBack::~PlayBack()
{
    if (m_player->state() == QMediaPlayer::PlayingState)
        m_player->stop();
}

void PlayBack::show()
{
    appear(MSceneWindow::DestroyWhenDone);
}

void PlayBack::close_me()
{
    notes_save();
    emit showToggle(false);
    dismiss();
}

void PlayBack::details_zone()
{
    m_player->setMedia(QUrl::fromLocalFile(QString("%1/%2")
                            .arg(settings.get_home_dir())
                            .arg(m_recordfile)));

    //we need to waiting 2 seconds before getting everything from metadata
    //because QMediaPlayer works asynchronously, we need to get duration before get it playing

    if (!m_player->isSeekable()){
        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        timer.setInterval(2000);
        loop.connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        loop.connect(m_player, SIGNAL(seekableChanged(bool)), &loop, SLOT(quit()));
        loop.exec();
    }

    m_duration = m_player->duration();

    QStringList file_meta = fileInfo(m_recordfile);
    QStringList file_base = fileBase(m_recordfile);

    QString details_spec =
            qtTrId("tr_phone_number_label")+" "+qtTrId("tr_im_id")+": <b>%1</b><br>" +
            qtTrId("tr_call_type_label")+": <b>%2</b><br>" +
            qtTrId("tr_file_format_label")+": <b>%3</b><br>" +
            qtTrId("tr_file_size_label")+": <b>%4</b><br>" +
            qtTrId("tr_recorded_date_label")+": <b>%6</b><br>" +
            qtTrId("tr_duration_label")+": <b>%7</b>";

    MLabel *pickup = new MLabel(details_spec.arg(file_base.at(0))
                                .arg(file_base.at(1))
                                .arg(file_base.at(2))
                                .arg(file_meta.at(1))
                                .arg(file_meta.at(0))
                                .arg(miliseconds_to_timestr(m_player->duration()))
                                );
    pickup->setWordWrap(true);
    pickup->setStyleName("CommonBodyText");
    pickup->setAlignment(Qt::AlignLeft);

    if (settings.get_auto_fecthing_contact_name() && !m_contactName.isEmpty()) {
        QGraphicsLinearLayout *playback_header =  new QGraphicsLinearLayout(Qt::Horizontal);
        playback_header->setContentsMargins(0,0,0,0);
        playback_header->setMaximumHeight(85);

        MImageWidget *contact_avatar = new MImageWidget(this);
        contact_avatar->setObjectName("mImageWidget");
        contact_avatar->setStyleName("CommonMainIcon");
        contact_avatar->setPreferredHeight(64);
        contact_avatar->setPreferredWidth(64);

        if (m_avatarURL.isEmpty())
            contact_avatar->setImage("icon-m-content-avatar-placeholder");
        else {
            QImage iAvatar;
            iAvatar.load(m_avatarURL.remove("file://"));
            contact_avatar->setImage(iAvatar);
        }

        MLabel *contact_name = new MLabel("<b>"+m_contactName+"</b>");
        contact_name->setObjectName("mLabel_contact_name");
        contact_name->setStyleName("CommonSingleTitle");
        contact_name->setWordWrap(true);

        playback_header->insertItem(0,contact_avatar);
        playback_header->insertItem(1,contact_name);

        wrapLayoutPolicy->addItem(playback_header );
        wrapLayoutPolicy->addItem(new MSeparator(centralWidget()));
    }

    wrapLayoutPolicy->addItem(pickup);
    wrapLayoutPolicy->addItem(new MSeparator(centralWidget()));

}

void PlayBack::play_zone()
{
    MLayout* prsublayout = new MLayout;
    MLinearLayoutPolicy* prsublayoutPolicy = new MLinearLayoutPolicy(prsublayout,Qt::Horizontal);
    prsublayout->setPortraitPolicy(prsublayoutPolicy);
    prsublayout->setLandscapePolicy(prsublayoutPolicy);
    prsublayout->setContentsMargins(0,0,0,0);

    m_play = new MButton(qtTrId("tr_play"));
    m_del = new MButton(qtTrId("tr_delete"));
    m_share = new MButton(qtTrId("tr_share"));

    m_play->setStyleName("CommonLeftSplitButton");
    m_share->setStyleName("CommonHorizontalCenterSplitButton");
    m_del->setStyleName("CommonRightSplitButton");

    connect(m_play, SIGNAL(clicked()), SLOT(play()));
    connect(m_del, SIGNAL(clicked()), SLOT(del()));
    connect(m_share, SIGNAL(clicked()), SIGNAL(share()));

    prsublayoutPolicy->addItem(m_play);
    prsublayoutPolicy->addItem(m_share);
    prsublayoutPolicy->addItem(m_del);

    sliderBar = new MSlider;
    sliderBar->setStyleName("CommonSlider");

    sliderBar->setRange(0, m_player->duration()/1000);
    sliderBar->setMinLabelVisible(false);
    sliderBar->setMaxLabelVisible(false);
    sliderBar->setHandleLabelVisible(true);
    sliderBar->setPreferredWidth(840);

    sliderBar->setMinLabel("00:00:00");
    sliderBar->setMaxLabel(miliseconds_to_timestr(m_player->duration()));

    sliderBar->setEnabled(false);

    connect(sliderBar, SIGNAL(valueChanged(int)),this,SLOT(sliderBarHandler(int)));
    connect(sliderBar, SIGNAL(sliderPressed()),this,SLOT(seekBarPressed()));
    connect(sliderBar, SIGNAL(sliderReleased()),this,SLOT(seekBarReleased()));

    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(stateChanged(QMediaPlayer::State)));

    MLayout *silderLayout = new MLayout;
    MLinearLayoutPolicy *silderLayoutPolicy = new MLinearLayoutPolicy(silderLayout, Qt::Vertical);
    silderLayout->setPortraitPolicy(silderLayoutPolicy);
    silderLayout->setLandscapePolicy(silderLayoutPolicy);

    silderLayoutPolicy->setSpacing(0);
    silderLayoutPolicy->setContentsMargins(0, 0, 0, 0);
    silderLayoutPolicy->addItem(sliderBar);
    silderLayoutPolicy->setStretchFactor(sliderBar,1);

    wrapLayoutPolicy->addItem(silderLayout);
    wrapLayoutPolicy->addItem(prsublayout);

    wrapLayoutPolicy->addItem(new MSeparator(centralWidget()));

}

void PlayBack::notes_zone()
{

    MLabel *notes = new MLabel(qtTrId("tr_note_label"));
    notes->setStyleName("CommonBodyText");
    notes->setAlignment(Qt::AlignLeft);

    rt_notes = new MRichTextEdit(MTextEditModel::MultiLine, "");
    rt_notes->setStyleName("CommonSingleInputFieldLabeled");
    rt_notes->setPrompt(qtTrId("tr_write_a_note"));

    wrapLayoutPolicy->addItem(notes);
    wrapLayoutPolicy->addItem(rt_notes);

    notes_load();
}

void PlayBack::sliderBarHandler(const int &value)
{
    if (m_seekBarPressed)
        m_player->setPosition(value*1000);
    sliderBar->setHandleLabel(miliseconds_to_timestr(value*1000));
}

void PlayBack::play()
{
    sliderBar->setEnabled(!m_playPressed);
    if (m_playPressed){
        m_stopPressed = true;
        m_player->stop();
    }
    else{
        m_stopPressed = false;
        m_player->setPosition(0);
        m_player->play();
    }
    m_play->setText(m_playPressed?qtTrId("tr_play"):qtTrId("tr_stop"));
    m_del->setEnabled(m_playPressed);
    m_share->setEnabled(m_playPressed);
    m_playPressed = !m_playPressed;
}

void PlayBack::del()
{
    emit showToggle(false);
    emit removerecordFile();
    dismiss();
}

void PlayBack::seekBarPressed()
{
    m_seekBarPressed = true;
}

void PlayBack::seekBarReleased()
{
    m_seekBarPressed = false;
}

void PlayBack::durationChanged(const qint64 &value)
{
    sliderBar->setMaximum(value/1000);
}

void PlayBack::positionChanged(const qint64 &value)
{
    sliderBar->setValue(value/1000);
}

void PlayBack::stateChanged(QMediaPlayer::State status)
{
    switch (status) {
        case QMediaPlayer::PlayingState:  break;
        case QMediaPlayer::PausedState: break;
        case QMediaPlayer::StoppedState:
            if (!m_stopPressed){
                sliderBar->setEnabled(false);
                m_player->setPosition(0);
                m_player->stop();
                m_play->setText(qtTrId("tr_play"));
                m_del->setEnabled(true);
                m_share->setEnabled(true);
                m_playPressed = false;
                m_stopPressed = false;
            }
            break;
    }
}

QString PlayBack::miliseconds_to_timestr(const qint64 &milisec)
{
    uint SECONDS_IN_AN_HOUR = 3600;
    uint SECONDS_IN_A_MINUTE = 60;
    uint PER_SECONDS = milisec/1000;

    if (PER_SECONDS > 0){
        uint hour = (unsigned int)(PER_SECONDS / SECONDS_IN_AN_HOUR);
        uint min = (unsigned int)((PER_SECONDS % SECONDS_IN_AN_HOUR) / SECONDS_IN_A_MINUTE);
        uint sec = (unsigned int)((PER_SECONDS % SECONDS_IN_AN_HOUR) % (SECONDS_IN_A_MINUTE));
        return QString().sprintf("%02i:%02i:%02i",hour,min,sec);
    }else
        return QString("00:00:00");
}

void PlayBack::notes_save()
{
    QDir qdir(settings.get_note_dir());
    if (!qdir.exists()) system(QString("mkdir -p %1").arg(settings.get_note_dir()).toAscii());
    QFile fnote(QString("%1/%2.txt").arg(settings.get_note_dir()).arg(m_recordfile));
    if (rt_notes->text().trimmed().length()) {
        if (fnote.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&fnote);
            out << rt_notes->text().trimmed();
            fnote.close();
        }
    }else{
        if (fnote.exists())
            fnote.remove();
    }
}

void PlayBack::notes_load()
{
    QFile fnote(QString("%1/%2.txt").arg(settings.get_note_dir()).arg(m_recordfile));
    if (fnote.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&fnote);
        rt_notes->setText(in.readAll());
        fnote.close();
    }
}

QStringList PlayBack::fileInfo(const QString &filePath) {
    QFileInfo f(QString("%1/%2").arg(settings.get_home_dir()).arg(filePath));
    QDateTime flM(f.created());

    QString __time = QLocale::system().toString(flM.time(), QLocale::ShortFormat);
    QString __date = flM.date().toString(QLocale::system().dateFormat(QLocale::ShortFormat).replace("yy","yyyy"));

    QString dt("%1, %2");

    return QStringList() << dt.arg(__date).arg(__time) << RecordFileModel::fileSize(f.size());
}

QStringList PlayBack::fileBase(const QString &filePath) {
    QStringList _fs = filePath.split("_");
    QStringList _fa = filePath.split(".");
    QString call_type;

    call_type = _fs.at(_fs.length()-2) == "In"?qtTrId("tr_incoming_label"):qtTrId("tr_outgoing_label");

    return QStringList() << _fs.at(0) << call_type << _fa.at(_fa.length()-1);
}
