#include "saveconfirmation.h"
#include "../common/common.h"
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

#include <MDialog>
#include <MLabel>

#include <QFile>

SaveConfirmation::SaveConfirmation(MApplication *app)
    :MApplicationPage(),
      m_recordFile(""),
      m_app(app)
{
     MApplicationPage::createContent();

     if (m_app->argc() > 0)
         m_recordFile = m_app->argv()[1];

     MDialog *dlg = new MDialog(qtTrId("tr_callrecorder"), M::YesButton | M::NoButton);
     MLabel *label = new MLabel(QString(qtTrId("tr_save_confirmation")).arg(m_recordFile));

     dlg->setObjectName("confirmationDialog");
     dlg->setSystem(true);
     dlg->setModal(true);

     label->setStyleName("CommonBodyTextInverted");
     label->setWordWrap(true);
     dlg->setCentralWidget(label);

     dlg->appear(MSceneWindow::DestroyWhenDone);

     connect(dlg, SIGNAL(accepted()), SLOT(YES()));
     connect(dlg, SIGNAL(rejected()), SLOT(NO()));
     connect(dlg, SIGNAL(destroyed()), SLOT(YES()));
}

void SaveConfirmation::YES(){
    m_app->exit();
}

void SaveConfirmation::NO(){
    QFile file(QString("%1/%2").arg(settings.get_home_dir()).arg(m_recordFile));
    file.remove();
    m_app->exit();
}
