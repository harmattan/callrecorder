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

#ifndef RECORDFILECELL_H
#define RECORDFILECELL_H

#include <QGraphicsWidget>

#include <MLayout>
#include <MListItem>
#include <MApplication>
#include <MLabel>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MSceneManager>
#include <MImageWidget>

class MListItem;
class MGridLayoutPolicy;
class MLabel;

class RecordFileCell : public MListItem
{
    Q_OBJECT

    Q_ENUMS(CALLTYPE)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)
    Q_PROPERTY(CALLTYPE calltype READ calltype WRITE callType)

public:
    RecordFileCell(QGraphicsItem *parent = 0);
    virtual ~RecordFileCell();

    enum CALLTYPE {OUTGOING, INCOMING};

    QString title();
    void setTitle(const QString &title);

    QString subtitle();
    void setSubtitle(const QString &title);

    CALLTYPE calltype();
    void callType(const CALLTYPE&);

private:
    MLayout *createLayout();

    MLabel *landscapeTitleLabelWidget();
    MLabel *portraitTitleLabelWidget();
    MLabel *subtitleLabelWidget();
    MImageWidget *callTypeImage();

    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    QString inv(QString stylename);

    MLayout *layout;
    MGridLayoutPolicy *landscapePolicy;
    MGridLayoutPolicy *portraitPolicy;

    MLabel *landscapeTitleLabel;
    MLabel *portraitTitleLabel;
    MLabel *subtitleLabel;
    MImageWidget *imageWidget;

    CALLTYPE m_calltype;
};

#endif // RECORDFILECELL_H
