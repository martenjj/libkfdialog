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

#include "dialogstatewatcher.h"

#include <qdialog.h>
#include <qdesktopwidget.h>
#include <qevent.h>
#include <qapplication.h>
#include <qabstractbutton.h>

#include "dialogstatesaver.h"
#include "libkfdialog_logging.h"


DialogStateWatcher::DialogStateWatcher(QDialog *pnt)
    : QObject(pnt)
{
    Q_ASSERT(pnt!=nullptr);
    mParent = pnt;
    mParent->installEventFilter(this);
    connect(mParent, &QDialog::accepted, this, &DialogStateWatcher::saveConfigInternal);

    mStateSaver = new DialogStateSaver(mParent);	// use our own as default
    mHaveOwnSaver = true;				// note that we created it
}


void DialogStateWatcher::setSaveOnButton(QAbstractButton *but)
{
    qCDebug(LIBKFDIALOG_LOG) << "button" << but->text();
    connect(but, &QAbstractButton::clicked, this, &DialogStateWatcher::saveConfigInternal);
}


bool DialogStateWatcher::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj==mParent && ev->type()==QEvent::Show)	// only interested in show event
    {
        restoreConfigInternal();			// restore size and config
    }
    return (false);					// always pass the event on
}


void DialogStateWatcher::restoreConfigInternal()
{
    if (mStateSaver==nullptr) return;			// no saver set or provided
    mStateSaver->restoreConfig();
}


void DialogStateWatcher::saveConfigInternal() const
{
    if (mStateSaver==nullptr) return;			// no saver set or provided
    mStateSaver->saveConfig();
}


void DialogStateWatcher::setStateSaver(DialogStateSaver *saver)
{
    // We only delete the existing saver if we created it.
    if (mStateSaver!=nullptr && mHaveOwnSaver) delete mStateSaver;

    mStateSaver = saver;				// set the new one
    mHaveOwnSaver = false;				// note that it's not ours
}
