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

#include "aboutpage.h"

AboutPage::AboutPage() :
    TempPage(qtTrId("tr_about_title"))
{
    setContentsMargins(0,0,0,0);

    APP_THANKS = \
            "<br><b>Andrey Kozhevnikov</b> (icoderus)<br>" \
            "<b>Oytun Sengul</b> (MeeGo Turkey)<br>" \
            "<b>Dmitry Zaitsev</b> (hhrhhr@gmail.com)<br>";

    MLabel *lversion = new MLabel(QString("%1<br>%2<br><br>%3%4<br>%5<br><b>"+qtTrId("tr_thanks_about")+"</b>%6")
                                  .arg(APP_TITLE)
                                  .arg(APP_LOGO)
                                  .arg(APP_VERSION)
                                  .arg(APP_LICENSE)
                                  .arg(APP_AUTHOR)
                                  .arg(APP_THANKS)
                                  );
    lversion->setAlignment(Qt::AlignHCenter);
    lversion->setStyleName("CommonBodyText");

    MButton *feedback = new MButton;
    feedback->setText(qtTrId("tr_support_and_feedback"));
    feedback->setStyleName("CommonBottomButton");
    connect(feedback, SIGNAL(clicked()), SLOT(feedback_clicked()));

    MLayout *wrapLayout = new MLayout;
    MLinearLayoutPolicy *wrapLayoutPolicy = new MLinearLayoutPolicy(wrapLayout , Qt::Vertical);

    MStylableWidget *styleWidget = new MStylableWidget();
    styleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    styleWidget->setLayout(wrapLayout);

    MPannableViewport *viewport = new MPannableViewport;
    viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    viewport->setWidget(styleWidget);

    wrapLayoutPolicy->addItem(lversion);
    wrapLayoutPolicy->addItem(feedback, Qt::AlignCenter);

    policy->addItem(new MSeparator(centralWidget()));
    policy->addItem(viewport);

    policy->activate();
}

void AboutPage::feedback_clicked() {
    QDesktopServices::openUrl(QUrl("mailto:metacuong@gmail.com?subject=(CallRecorder) - Support and Feedback"));
}
