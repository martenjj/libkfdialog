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

#ifndef DIALOGSTATESAVER_H
#define DIALOGSTATESAVER_H

#include "libkfdialog_export.h"

class QDialog;
class QWidget;
class KConfigGroup;


/**
 * @short Save and restore the size and state of a dialog box.
 *
 * This class manages saving and restoring a dialog's size in the
 * application config file.  All that is necessary is to create a
 * DialogStateWatcher object in the dialog's constructor, passing the
 * dialog as a parameter.  If the dialog is a subclass of DialogBase
 * then a watcher and saver will be created automatically.
 *
 * The saver can be subclassed if necessary in order to save additional
 * information (e.g. the column states of a list view).  Since it does
 * not inherit QObject, the dialog itself can be the DialogStateSaver
 * to do its own saving and restoring, having access to its own internal
 * state.
 *
 * @author Jonathan Marten
 **/

class LIBKFDIALOG_EXPORT DialogStateSaver
{
public:
    /**
     * Constructor.
     *
     * @param pnt the parent dialog
     **/
    explicit DialogStateSaver(QDialog *pnt);

    /**
     * Destructor.
     **/
    virtual ~DialogStateSaver() = default;

    /**
     * Set the default option of whether the size of dialog boxes
     * is saved when accepted and restored when shown.  This is an
     * application-wide setting which takes effect immediately.
     * The default is @c true.
     *
     * @param on Whether the size is to be saved/restored
     *
     * @note The setting is saved in the application's default configuration
     * file (as used by @c KSharedConfig::openConfig()) in a section named
     * by the dialog's object name.  If no object name is set then the
     * dialog's class name is used.
     *
     * @see KSharedConfig
     * @see QObject::objectName()
     **/
    static void setSaveSettingsDefault(bool on);

    /**
     * Save the parent dialog size to the application config file.
     *
     * This is called by the dialog state watcher and should not need
     * to be called explicitly.  It simply calls the virtual method
     * of the same name, which may be reimplemented in a subclass in order
     * to save other settings (e.g. the column states of a list view).
     **/
    void saveConfig() const;

    /**
     * Restore the dialog size from the application config file.
     *
     * This is called by the dialog state watcher and should not need
     * to be called explicitly.  It simply calls the virtual method
     * of the same name, which may be reimplemented in a subclass in order
     * to restore other settings (e.g. the column states of a list view).
     **/
    void restoreConfig();

    /**
     * Save the state of a window.
     *
     * The window need not be a dialog, therefore this can be used for
     * saving the state of any window.  The state is saved to a group
     * named as appropriate for the window.
     *
     * @param window window to save the state of
     **/
    static void saveWindowState(QWidget *widget);

    /**
     * Save the state of a window.
     *
     * The window need not be a dialog, therefore this can be used for
     * saving the state of any window.  The state is saved to the
     * specified group.
     *
     * @param window window to save the state of
     * @param grp group to save the configuration to
     **/
    static void saveWindowState(QWidget *widget, KConfigGroup &grp);

    /**
     * Restore the state of a window.
     *
     * The window need not be a dialog, therefore this can be used for
     * restoring the state of any window.  The state is restore from a group
     * named as appropriate for the window.
     *
     * @param window window to restore the state of
     **/
    static void restoreWindowState(QWidget *widget);

    /**
     * Restore the state of a window.
     *
     * The window need not be a dialog, therefore this can be used for
     * restoring the state of any window.  The state is restored from
     * the specified group.
     *
     * @param window window to restore the state of
     * @param grp group to restore the configuration from
     **/
    static void restoreWindowState(QWidget *widget, const KConfigGroup &grp);

protected:
    /**
     * Save the dialog size to the application config file.
     *
     * This may be reimplemented in a subclass if necessary, in order
     * to save other settings (e.g. the column states of a list view).
     * Call the base class implementation to save the dialog size.
     *
     * @param dialog dialog to save the state of
     * @param grp group to save the configuration to
     **/
    virtual void saveConfig(QDialog *dialog, KConfigGroup &grp) const;

    /**
     * Restore the dialog size from the application config file.
     *
     * This may be reimplemented in a subclass if necessary, in order
     * to restore other settings (e.g. the column states of a list view).
     * Call the base class implementation to restore the dialog size.
     *
     * @param dialog dialog to restore the state of
     * @param grp group to restore the configuration from
     **/
    virtual void restoreConfig(QDialog *dialog, const KConfigGroup &grp);

private:
    QDialog *mParent;
};

#endif							// DIALOGSTATESAVER_H
