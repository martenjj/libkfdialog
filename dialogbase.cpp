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
 *  Copyright (C) 2016-2026 Jonathan Marten				*
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
#include <kstandardshortcut.h>

#include "dialogstatewatcher.h"
#include "libkfdialog_logging.h"


DialogBase::DialogBase(QWidget *pnt)
    : QDialog(pnt)
{
    qCDebug(LIBKFDIALOG_LOG);

    setModal(true);					// convenience, can reset if necessary

    mMainWidget = nullptr;				// caller not provided yet
    mStateWatcher = new DialogStateWatcher(this);	// use our own as default

    mButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    connect(mButtonBox, &QDialogButtonBox::accepted, this, &DialogBase::accept);
    connect(mButtonBox, &QDialogButtonBox::rejected, this, &DialogBase::reject);

    mModifyingMode = false;
    mOkShouldBeEnabled = true;
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
        qCDebug(LIBKFDIALOG_LOG) << "setting up OK button";
        QPushButton *okButton = mButtonBox->button(QDialogButtonBox::Ok);
        okButton->setDefault(true);
        okButton->setShortcut(Qt::CTRL|Qt::Key_Return);
    }

    if (buttons & QDialogButtonBox::Help)
    {
        qCDebug(LIBKFDIALOG_LOG) << "setting up Help button";
        QPushButton *helpButton = mButtonBox->button(QDialogButtonBox::Help);
        const QList<QKeySequence> shortcuts = KStandardShortcut::help();
        if (!shortcuts.isEmpty()) helpButton->setShortcut(shortcuts.first());
    }
}


void DialogBase::setButtonEnabled(QDialogButtonBox::StandardButton button, bool state)
{
    QPushButton *but = mButtonBox->button(button);
    if (but==nullptr) return;

    if (button==QDialogButtonBox::Ok)
    {
        mOkShouldBeEnabled = state;
        if (mModifyingMode)
        {
            qDebug() << "modify mode, modified =" << mModifiedState << "ok ->" << state;
            // Do not enable the "OK" button if it is currently disabled
            // because the dialogue is not modified, just remember whether
            // it should be enabled.
            if (!mModifiedState && state) return;
        }
    }

    but->setEnabled(state);
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


void DialogBase::setModified(bool mod)
{
    QPushButton *okBut = mButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelBut = mButtonBox->button(QDialogButtonBox::Cancel);
    QPushButton *closeBut = mButtonBox->button(QDialogButtonBox::Close);

    if (okBut==nullptr || cancelBut==nullptr || closeBut==nullptr)
    {
        qCWarning(LIBKFDIALOG_LOG) << "Using modification mode requires OK, Cancel and Close buttons";
        return;
    }

    qDebug() << "modification state =" << mod;
    mModifyingMode = true;				// in effect from now on
    mModifiedState = mod;				// note the modification state

    if (!mod)						// not modified yet
    {
        okBut->setEnabled(false);			// button "OK" disabled
        cancelBut->setVisible(false);			// button "Cancel" hidden
        closeBut->setVisible(true);			// button "Close" visible
    }
    else						// settings are modified
    {
        okBut->setEnabled(mOkShouldBeEnabled);		// button "OK" as set
        cancelBut->setVisible(true);			// button "Cancel" visible
        closeBut->setVisible(false);			// button "Close" hidden
    }
}
