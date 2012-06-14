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

#include "recordfilecell.h"

RecordFileCell::RecordFileCell(QGraphicsItem *parent)
    : MListItem(parent),
    layout(NULL),
    landscapePolicy(NULL),
    portraitPolicy(NULL),
    landscapeTitleLabel(NULL),
    portraitTitleLabel(NULL),
    subtitleLabel(NULL),
    imageWidget(NULL)
{
    setStyleName("CommonPanel");
}

RecordFileCell::~RecordFileCell()
{
}

MLayout *RecordFileCell::createLayout()
{
    layout = new MLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);

    landscapePolicy->addItem(callTypeImage(), 0, 2, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    landscapePolicy->addItem(landscapeTitleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
    landscapePolicy->addItem(subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);

    portraitPolicy = new MGridLayoutPolicy(layout);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    portraitPolicy->addItem(callTypeImage(), 0, 2, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    portraitPolicy->addItem(portraitTitleLabelWidget(),0,1, Qt::AlignLeft | Qt::AlignVCenter);
    portraitPolicy->addItem(subtitleLabelWidget(),1,1, Qt::AlignLeft | Qt::AlignVCenter);

    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);

    return layout;
}

MImageWidget *RecordFileCell::callTypeImage() {
    if (!imageWidget) {
        imageWidget = new MImageWidget(this);
        imageWidget->setObjectName("imageWidget_callTypeImage");
        imageWidget->setStyleName("CommonSubIcon");
    }
    return imageWidget;
}

MLabel *RecordFileCell::landscapeTitleLabelWidget()
{
    if (!landscapeTitleLabel) {
        landscapeTitleLabel = new MLabel(this);
        landscapeTitleLabel->setTextElide(true);
        landscapeTitleLabel->setStyleName("CommonTitle");
    }
    return landscapeTitleLabel;
}

MLabel *RecordFileCell::portraitTitleLabelWidget()
{
    if (!portraitTitleLabel) {
        portraitTitleLabel = new MLabel(this);
        portraitTitleLabel->setTextElide(true);
        portraitTitleLabel->setStyleName("CommonSingleTitle");
    }
    return portraitTitleLabel;
}

MLabel *RecordFileCell::subtitleLabelWidget()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(this);
        subtitleLabel->setTextElide(true);
        subtitleLabel->setStyleName("CommonSubTitle");
    }
    return subtitleLabel;
}


void RecordFileCell::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MListItem::resizeEvent(event);

    if (!layout)
        setLayout(createLayout());
}

QString RecordFileCell::title()
{
    return landscapeTitleLabelWidget()->text();
}

void RecordFileCell::setTitle(const QString &title)
{
    landscapeTitleLabelWidget()->setText(title);
    portraitTitleLabelWidget()->setText(title);
}

QString RecordFileCell::subtitle()
{
    return subtitleLabelWidget()->text();
}

void RecordFileCell::setSubtitle(const QString &subtitle)
{
    subtitleLabelWidget()->setText(subtitle);
}

RecordFileCell::CALLTYPE RecordFileCell::calltype() {
    return m_calltype;
}

void RecordFileCell::callType(const RecordFileCell::CALLTYPE &calltype) {
    callTypeImage()->setImage(calltype==INCOMING?"icon-m-telephony-callhistory-received":
                                               "icon-m-telephony-callhistory-initiated");
    m_calltype = calltype;
}

