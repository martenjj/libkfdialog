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

#include "dialogstatesaver.h"

#include <qdialog.h>
#include <qwindow.h>
#include <qscreen.h>

#include <kconfiggroup.h>
#include <ksharedconfig.h>

#include "libkfdialog_logging.h"


static bool sSaveSettings = true;


DialogStateSaver::DialogStateSaver(QDialog *pnt)
{
    Q_ASSERT(pnt!=nullptr);
    mParent = pnt;
}


static KConfigGroup configGroupFor(QWidget *window)
{
    QString objName = window->objectName();
    if (objName.isEmpty())
    {
        objName = window->metaObject()->className();
        qCWarning(LIBKFDIALOG_LOG) << "object name not set, using class name" << objName;
    }
    else qCDebug(LIBKFDIALOG_LOG) << "for" << objName << "which is a" << window->metaObject()->className();

    return (KSharedConfig::openConfig(QString(), KConfig::NoCascade)->group(objName));
}


void DialogStateSaver::restoreConfig()
{
    if (!sSaveSettings) return;				// settings not to be restored

    const KConfigGroup grp = configGroupFor(mParent);
    this->restoreConfig(mParent, grp);
}


void DialogStateSaver::restoreConfig(QDialog *dialog, const KConfigGroup &grp)
{
    restoreWindowState(dialog, grp);
}


void DialogStateSaver::restoreWindowState(QWidget *widget)
{
    const KConfigGroup grp = configGroupFor(widget);
    restoreWindowState(widget, grp);
}


void DialogStateSaver::restoreWindowState(QWidget *widget, const KConfigGroup &grp)
{
    // Ensure that the widget's window() - that is, either the widget itself
    // or its nearest ancestor widget that is or could be top level - is a
    // native window, so that windowHandle() below will return a valid QWindow.
    const WId wid = widget->window()->winId();
    const QRect desk = widget->window()->windowHandle()->screen()->geometry();
    const QSize sizeDefault = widget->sizeHint();

    // originally from KDE4 KDialog::restoreDialogSize()
    qCDebug(LIBKFDIALOG_LOG) << "from" << grp.name() << "in" << grp.config()->name();
    const int width = grp.readEntry(QString::fromLatin1("Width %1").arg(desk.width()), sizeDefault.width());
    const int height = grp.readEntry(QString::fromLatin1("Height %1").arg(desk.height()), sizeDefault.height());
    widget->resize(width, height);
}


void DialogStateSaver::saveConfig() const
{
    if (!sSaveSettings) return;				// settings not to be saved

    KConfigGroup grp = configGroupFor(mParent);
    this->saveConfig(mParent, grp);
    grp.sync();
}


void DialogStateSaver::saveConfig(QDialog *dialog, KConfigGroup &grp) const
{
    saveWindowState(dialog, grp);
}


void DialogStateSaver::saveWindowState(QWidget *widget)
{
    KConfigGroup grp = configGroupFor(widget);
    saveWindowState(widget, grp);
}


void DialogStateSaver::saveWindowState(QWidget *widget, KConfigGroup &grp)
{
    const WId wid = widget->window()->winId();
    const QRect desk = widget->window()->windowHandle()->screen()->geometry();
    const QSize sizeToSave = widget->size();

    // originally from KDE4 KDialog::saveDialogSize()
    qCDebug(LIBKFDIALOG_LOG) << "to" << grp.name() << "in" << grp.config()->name();
    grp.writeEntry(QString::fromLatin1("Width %1").arg(desk.width()), sizeToSave.width());
    grp.writeEntry( QString::fromLatin1("Height %1").arg(desk.height()), sizeToSave.height());
    grp.sync();
}


void DialogStateSaver::setSaveSettingsDefault(bool on)
{
    sSaveSettings = on;
}
