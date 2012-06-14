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

#include "controllerpage.h"
#include "mainapp.h"

ControllerPage::ControllerPage(MApplicationWindow *mainapp) :
    TempPage(qtTrId("tr_controller_title")),
    m_mainapp(mainapp)
{
    manually_bt_services_start = new MButton(qtTrId("tr_services_start"));
    manually_bt_services_start->setStyleName(CLSB);

    manually_bt_services_stop = new MButton(qtTrId("tr_services_stop"));
    manually_bt_services_stop->setStyleName(CRSB);

    MLabel *prservice = new MLabel(QString("<b>%1</b>").arg(qtTrId("tr_services_label")));
    prservice->setAlignment(Qt::AlignTop);
    prservice->setStyleName("CommonBodyText");

    MLabel *aautostart = new MLabel(QString("<b>%1</b>").arg(qtTrId("tr_autostart_label")));
    aautostart->setAlignment(Qt::AlignTop);
    aautostart->setStyleName("CommonBodyText");
    aautostart->setWordWrap(true);

    autostart_bt_enable = new MButton(qtTrId("tr_autostart_enable"));
    autostart_bt_enable->setStyleName(CLSB);

    autostart_bt_disable = new MButton(qtTrId("tr_autostart_disable"));
    autostart_bt_disable->setStyleName(CRSB);

    connect(manually_bt_services_start,SIGNAL(clicked()),SLOT(manually_bt_services_start_clicked()));
    connect(manually_bt_services_stop,SIGNAL(clicked()),SLOT(manually_bt_services_stop_clicked()));


    connect(autostart_bt_enable,SIGNAL(clicked()),SLOT(autostart_bt_enable_clicked()));
    connect(autostart_bt_disable,SIGNAL(clicked()),SLOT(autostart_bt_disable_clicked()));

    setContentsMargins(0,0,0,0);

    MLayout* prsublayout = new MLayout;
    MLinearLayoutPolicy* prsublayoutPolicy = new MLinearLayoutPolicy(prsublayout,Qt::Horizontal);
    prsublayout->setPortraitPolicy(prsublayoutPolicy);
    prsublayout->setLandscapePolicy(prsublayoutPolicy);
    prsublayoutPolicy->addItem(manually_bt_services_start);
    prsublayoutPolicy->addItem(manually_bt_services_stop);

    MLayout* asublayout = new MLayout;
    MLinearLayoutPolicy* asublayoutPolicy = new MLinearLayoutPolicy(asublayout,Qt::Horizontal);
    asublayout->setPortraitPolicy(asublayoutPolicy);
    asublayout->setLandscapePolicy(asublayoutPolicy);
    asublayoutPolicy->addItem(autostart_bt_enable);
    asublayoutPolicy->addItem(autostart_bt_disable);

    rAudioOutput = new MComboBox;
    rAudioOutput->setStyleName("CommonComboBox");
    rAudioOutput->setTitle(qtTrId("tr_audio_format"));
    rAudioOutput->addItems(QStringList()
                            << "wav 08kHz Mono"
                            << "wav 16kHz Mono"
                            << "wav 08kHz Stereo"
                            << "wav 16kHz Stereo"

                           << "amr 8kHz 6Kbps Mono"
                           << "amr 8kHz 9Kbps Mono"

                           << "aac 32kHz 64Kbps Mono"
                           << "aac 32kHz 64Kbps Stereo"

                           << "aac 32kHz 128Kbps Mono"
                           << "aac 32kHz 128Kbps Stereo"
                           );
    rAudioOutput->setCurrentIndex(m_settings.get_audio_output_format());
    connect(rAudioOutput,SIGNAL(currentIndexChanged(int)),SLOT(rAudioOutput_changed(int)));

     rNotification = new MComboBox;
     rNotification->setStyleName("CommonComboBox");
     rNotification->setTitle(qtTrId("tr_save_option"));
     rNotification->addItems(QStringList()
                             << qtTrId("tr_just_save")
                             << qtTrId("tr_notify_save")
                             << qtTrId("tr_ask_save")
                             );
     rNotification->setCurrentIndex(m_settings.get_save_option());
     connect(rNotification,SIGNAL(currentIndexChanged(int)),SLOT(rNotification_changed(int)));


     auto_fetching_contact_name = new MButton;
     auto_fetching_contact_name->setObjectName("switch_type_of_store_folder");
     auto_fetching_contact_name->setStyleName("CommonRightSwitch");
     auto_fetching_contact_name->setViewType(MButton::switchType);
     auto_fetching_contact_name->setCheckable(true);
     auto_fetching_contact_name->setChecked(m_settings.get_auto_fecthing_contact_name());

     connect(auto_fetching_contact_name, SIGNAL(toggled(bool)), SLOT(auto_fetching_contact_name_toggled(bool)));

     MLabel *lb_auto_fetching_contact_name = new MLabel;
     lb_auto_fetching_contact_name->setTextElide(true);
     lb_auto_fetching_contact_name->setText("<b>"+qtTrId("tr_auto_fetching_contact_name")+"</b>");
     lb_auto_fetching_contact_name->setObjectName("label_auto_fetching_contact_name");
     lb_auto_fetching_contact_name->setStyleName("CommonBodyText");
     lb_auto_fetching_contact_name->setAlignment(Qt::AlignLeft);
     lb_auto_fetching_contact_name->setWordWrap(true);

     QGraphicsLinearLayout *layout_auto_fetching_contact_name = new QGraphicsLinearLayout(Qt::Horizontal);
     layout_auto_fetching_contact_name->setContentsMargins(0,10,0,0);
     layout_auto_fetching_contact_name->addItem(lb_auto_fetching_contact_name);
     layout_auto_fetching_contact_name->addItem(auto_fetching_contact_name);

     QStringList folderList;
     folderList << QString("%1%2").arg(ROOT_DIR_INSIDE).arg(".CallRecorder")
                << QString("%1%2").arg(ROOT_DIR_OUTSIDE).arg("CallRecorder");
     rfolderList = new MComboBox;
     rfolderList->setObjectName("comboBox_folderList");
     rfolderList->setTitle(qtTrId("tr_change_the_store_folder"));
     rfolderList->setIconVisible(false);
     rfolderList->addItems(folderList);
     rfolderList->setCurrentIndex(m_settings.get_store_folder());
     connect(rfolderList, SIGNAL(currentIndexChanged(int)), SLOT(folderList_currentIndexChanged(int))) ;

    MLayout *wrapLayout = new MLayout;
    MLinearLayoutPolicy *wrapLayoutPolicy = new MLinearLayoutPolicy(wrapLayout , Qt::Vertical);

    MStylableWidget *styleWidget = new MStylableWidget();
    styleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    styleWidget->setLayout(wrapLayout);

    MPannableViewport *viewport = new MPannableViewport;
    viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    viewport->setWidget(styleWidget);

    wrapLayoutPolicy->addItem(prservice);
    wrapLayoutPolicy->addItem(prsublayout);

    wrapLayoutPolicy->addItem(aautostart);
    wrapLayoutPolicy->addItem(asublayout);

    wrapLayoutPolicy->addItem(new MSeparator(centralWidget()));

    wrapLayoutPolicy->addItem(rAudioOutput);
    wrapLayoutPolicy->addItem(rNotification);

    wrapLayoutPolicy->addItem(new MSeparator(centralWidget()));

    wrapLayoutPolicy->addItem(rfolderList);

    wrapLayoutPolicy->addItem(new MSeparator(centralWidget()));
    wrapLayoutPolicy->addItem(layout_auto_fetching_contact_name);

    policy->addItem(new MSeparator(centralWidget()));
    policy->addItem(viewport);
    policy->activate();

    get_services_status();
    get_autostart_status();

    connect(&pInterface, SIGNAL(finished(bool)), SLOT(start_phone_service_finished(bool)));
    connect(&rInterface, SIGNAL(finished(bool)), SLOT(start_recorder_service_finished(bool)));
}

ControllerPage::~ControllerPage() {
    delete manually_bt_services_start;
    delete manually_bt_services_stop;
    delete autostart_bt_disable;
    delete autostart_bt_enable;
    delete rAudioOutput;
    delete rNotification;
}

/*
  GETs
  */

void ControllerPage::get_services_status() {
     bool ping = pInterface.ping() && rInterface.ping();
     manually_bt_services_stop->setEnabled(ping);
     manually_bt_services_start->setEnabled(!ping);
}

void ControllerPage::get_autostart_status(){
    bool ping = m_settings.get_autostart()==1;
    autostart_bt_disable->setEnabled(ping);
    autostart_bt_enable->setEnabled(!ping);
}

/*
  EVENTs
  */

void ControllerPage::manually_bt_services_start_clicked() {
    system("/opt/callrecorder/bin/callrecorder_phone_service inside &");
    system("/opt/callrecorder/bin/callrecorder_recorder_service inside &");
}

void ControllerPage::manually_bt_services_stop_clicked() {
    system("killall callrecorder_phone_service");
    system("killall callrecorder_recorder_service");
    get_services_status();
}

void ControllerPage::autostart_bt_enable_clicked() {
    m_settings.set_autostart(1);
    get_autostart_status();
}

void ControllerPage::autostart_bt_disable_clicked() {
    m_settings.set_autostart(0);
    get_autostart_status();
}

void ControllerPage::start_phone_service_finished(const bool&) {
    show_banner(qtTrId("tr_phone_banner"), "icon-s-status-cellular");
    get_services_status();
}

void ControllerPage::start_recorder_service_finished(const bool&) {
    show_banner(qtTrId("tr_recorder_banner"), "icon-s-status-voicemail");
    get_services_status();
}

void ControllerPage::rAudioOutput_changed(const int &index) {
    m_settings.set_autdio_output_format(index);
}

void ControllerPage::rNotification_changed(const int &index) {
    m_settings.set_save_option(index);
}

void ControllerPage::show_banner(const QString &subtitle, const QString &icon) {
    MBanner *banner = new MBanner();
    banner->setObjectName("statusBanner");
    banner->setStyleName(MBannerType::ShortEventBanner);
    banner->setIconID(icon);
    banner->setTitle("Call Recorder");
    banner->setSubtitle(subtitle);
    banner->appear(MSceneWindow::DestroyWhenDone);
}

void ControllerPage::auto_fetching_contact_name_toggled(const bool &val) {
    if (val) {
        MMessageBox *messageBox = new MMessageBox(QString("%1<br><br>%2")
                                                  .arg(qtTrId("tr_auto_fetching_contact_warning"))
                                                  .arg(qtTrId("tr_are_you_sure")), M::YesButton | M::NoButton);
        messageBox->appear(MSceneWindow::DestroyWhenDone);
        connect(messageBox, SIGNAL(accepted()), SLOT(accepted_auto_fetching_contact_name()));
        connect(messageBox, SIGNAL(rejected()), auto_fetching_contact_name, SLOT(click()));
    }else{
        m_settings.set_auto_fecthing_contact_name(false);
        enable_auto_fetching_contact_name(false);
    }
}

void ControllerPage::accepted_auto_fetching_contact_name() {
    m_settings.set_auto_fecthing_contact_name(true);
    enable_auto_fetching_contact_name(true);

    MainApp *backtoRecordedFilesPage = dynamic_cast<MainApp *>(m_mainapp);
    backtoRecordedFilesPage->switchToRecordedFilesPage();
    backtoRecordedFilesPage->showFetchingContactSpinner();
}

void ControllerPage::enable_auto_fetching_contact_name(const bool &isEnable) {
    system(isEnable?"/opt/callrecorder/bin/callrecorder_contact_handler &":
                  "killall callrecorder_contact_handler");
}

bool ControllerPage::ping_contact_handler() {
    QDBusMessage message = QDBusMessage::createMethodCall(CT_SERVICE,CT_PATH,CT_SERVICE,"ping");
    QDBusMessage reply = QDBusConnection::sessionBus().call(message);
    if (reply.type() == QDBusMessage::ErrorMessage)
        return false;

    bool pong = reply.arguments().at(0).value<bool>();

    return pong;
}

void ControllerPage::folderList_currentIndexChanged(const int &index) {
    if (index != m_settings.get_store_folder()) {
        MMessageBox *confirmtoMoveFolder = new MMessageBox(qtTrId("tr_are_you_sure"), M::YesButton|M::NoButton);
        confirmtoMoveFolder->setObjectName("confirmtoDelete_Mbox");
        confirmtoMoveFolder->setTitle(qtTrId("tr_change_the_store_folder"));
        confirmtoMoveFolder->appear(MSceneWindow::DestroyWhenDone);
        connect(confirmtoMoveFolder,SIGNAL(accepted()), SLOT(_move_folder_accepted()));
        connect(confirmtoMoveFolder,SIGNAL(rejected()), SLOT(_move_folder_rejected()));
        __folderList_index = index;
    }
}

void ControllerPage::_move_folder_rejected() {
    rfolderList->setCurrentIndex(m_settings.get_store_folder());
}

void ControllerPage::_move_folder_accepted() {
    QString cmd = "mkdir -p ";
    if (__folderList_index)
        cmd.append(QString("%1/%2")
                           .arg(ROOT_DIR_OUTSIDE)
                           .arg(STORE_FOLDER)
                           ); //"/home/user/CallRecorder"
    else
        cmd.append(QString("%1/.%2")
                           .arg(ROOT_DIR_INSIDE)
                           .arg(STORE_FOLDER)
                           ); //"/home/user/MyDocs/.CallRecorder"
    system(cmd.toStdString().c_str());

    m_settings.set_store_folder(__folderList_index);

    MainApp *backtoRecordedFilesPage = dynamic_cast<MainApp *>(m_mainapp);
    backtoRecordedFilesPage->switchToRecordedFilesPage();
}
