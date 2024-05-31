/************************************************************************
 *									*
 *  This source file is part of libkfdialog, a helper library for	*
 *  implementing QtWidgets-based dialogues under KDE Frameworks or	*
 *  standalone.  Originally developed as part of Kooka, a KDE		*
 *  scanning/OCR application.						*
 *									*
 *  The library is free software; you can redistribute and/or		*
 *  modify it under the terms of the GNU General Public License		*
 *  version 2 or (at your option) any later version, as published	*
 *  by the Free Software Foundation and appearing in the file		*
 *  COPYING included in the packaging of this library, or at		*
 *  http://www.gnu.org/licenses/gpl.html				*
 *									*
 *  Copyright (C) 2016-2021 Jonathan Marten				*
 *                          <jjm AT keelhaul DOT me DOT uk>		*
 *			    and Kooka authors/contributors		*
 *									*
 *  Home page:  https://github.com/martenjj/libkfdialog			*
 *									*
 ************************************************************************/

#include "dialogbase.h"

#include <qlayout.h>
#include <qframe.h>
#include <qstyle.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <QSpacerItem>

#include <kguiitem.h>

#include "dialogstatewatcher.h"
#include "libkfdialog_logging.h"


DialogBase::DialogBase(QWidget *pnt)
    : QDialog(pnt)
{
    qCDebug(LIBKFDIALOG_LOG);

    setModal(true);					// convenience, can reset if necessary

    mMainWidget = nullptr;					// caller not provided yet
    mStateWatcher = new DialogStateWatcher(this);	// use our own as default

    mButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    connect(mButtonBox, &QDialogButtonBox::accepted, this, &DialogBase::accept);
    connect(mButtonBox, &QDialogButtonBox::rejected, this, &DialogBase::reject);
}


void DialogBase::showEvent(QShowEvent *ev)
{
    if (layout()==nullptr)					// layout not yet set up
    {
        qCDebug(LIBKFDIALOG_LOG) << "setup layout";
        QVBoxLayout *mainLayout = new QVBoxLayout;
        setLayout(mainLayout);

        if (mMainWidget==nullptr)
        {
            qCWarning(LIBKFDIALOG_LOG) << "No main widget set for" << objectName();
            mMainWidget = new QWidget(this);
        }

        mainLayout->addWidget(mMainWidget);
        mainLayout->setStretchFactor(mMainWidget, 1);
        mainLayout->addWidget(mButtonBox);
    }

    QDialog::showEvent(ev);				// show the dialogue
}


void DialogBase::setButtons(QDialogButtonBox::StandardButtons buttons)
{
    qCDebug(LIBKFDIALOG_LOG) << buttons;
    mButtonBox->setStandardButtons(buttons);

    if (buttons & QDialogButtonBox::Ok)
    {
        qCDebug(LIBKFDIALOG_LOG) << "setup OK button";
        QPushButton *okButton = mButtonBox->button(QDialogButtonBox::Ok);
        okButton->setDefault(true);
        okButton->setShortcut(Qt::CTRL|Qt::Key_Return);
    }

// set F1 shortcut for Help?

}


void DialogBase::setButtonEnabled(QDialogButtonBox::StandardButton button, bool state)
{
    QPushButton *but = mButtonBox->button(button);
    if (but!=nullptr) but->setEnabled(state);
}


void DialogBase::setButtonText(QDialogButtonBox::StandardButton button, const QString &text)
{
    QPushButton *but = mButtonBox->button(button);
    if (but!=nullptr) but->setText(text);
}

void DialogBase::setButtonIcon(QDialogButtonBox::StandardButton button, const QIcon &icon)
{
    QPushButton *but = mButtonBox->button(button);
    if (but!=nullptr) but->setIcon(icon);
}

void DialogBase::setButtonGuiItem(QDialogButtonBox::StandardButton button, const KGuiItem &guiItem)
{
    QPushButton *but = mButtonBox->button(button);
    if (but!=nullptr) KGuiItem::assign(but, guiItem);
}


int DialogBase::verticalSpacing()
{
    return (QApplication::style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing));
}


int DialogBase::horizontalSpacing()
{
    return (QApplication::style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing));
}


void DialogBase::setStateSaver(DialogStateSaver *saver)
{
    mStateWatcher->setStateSaver(saver);
}


DialogStateSaver *DialogBase::stateSaver() const
{
    return (mStateWatcher->stateSaver());
}


QSpacerItem *DialogBase::verticalSpacerItem()
{
    return (new QSpacerItem(1, verticalSpacing(), QSizePolicy::Minimum, QSizePolicy::Fixed));
}


QSpacerItem *DialogBase::horizontalSpacerItem()
{
    return (new QSpacerItem(horizontalSpacing(), 1, QSizePolicy::Fixed, QSizePolicy::Minimum));
}
