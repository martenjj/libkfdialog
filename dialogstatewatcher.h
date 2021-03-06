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

#ifndef DIALOGSTATEWATCHER_H
#define DIALOGSTATEWATCHER_H

#include <qobject.h>
#include "libkfdialog_export.h"

class QDialog;
class QEvent;
class QAbstractButton;
class DialogStateSaver;


/**
 * @short Monitor a dialog box to save and restore the size and state.
 *
 * This class takes care of saving and restoring a dialog's size in the
 * application config file.  All that is necessary is to create a
 * DialogStateWatcher object in the dialog's constructor, passing the
 * dialog as a parameter.  If the dialog is a subclass of DialogBase
 * then a watcher will be created automatically.
 *
 * The watcher uses a DialogStateSaver to do the actual saving and
 * restoring.  If required, a custom saver can be subclassed from that
 * to save additional information (e.g. the column states of a list view).
 *
 * @author Jonathan Marten
 **/

class LIBKFDIALOG_EXPORT DialogStateWatcher : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param pnt the parent dialog
     **/
    explicit DialogStateWatcher(QDialog *pnt);

    /**
     * Destructor.
     **/
    ~DialogStateWatcher() override = default;

    /**
     * Set a state saver for the dialog being watched.
     *
     * This may be a subclass of a DialogStateSaver, reimplemented in
     * order to save special dialog settings (e.g. the column states of
     * a list view).  If this is not set then a plain DialogStateSaver
     * will be created and used internally.  If a nullptr state saver is
     * set explicitly using this function, then no state restoring or
     * saving will be done.
     *
     * The watcher does not take ownership of the state saver, therefore
     * it is the caller's responsibility to delete it when it is no
     * longer required.
     *
     * @param saver the state saver
     *
     * @note The saver should be set before the dialog is shown for
     * the first time.
     * @see DialogStateSaver
     **/
    void setStateSaver(DialogStateSaver *saver);

    /**
     * Access the state saver used by the watcher.
     *
     * This may be the default one, or that set by @c setStateSaver().
     *
     * @return the state saver
     **/
    DialogStateSaver *stateSaver() const		{ return (mStateSaver); }

    /**
     * Sets a button to save the state of the dialog when it is used.
     *
     * Normally the dialog state will be saved when the parent dialog is accepted.
     * This means when any button with the @c QDialogButtonBox::AcceptRole is
     * clicked: that is, @c QDialogButtonBox::Ok and some others.  Notably, it
     * does not include a @c QDialogButtonBox::Close button which is used where
     * there is no difference between closing and cancelling.  This means that the
     * dialog state will not normally be saved when that button is used.
     *
     * If a button is specified here, the state will be saved when that button is
     * used, in addition to any button with the @c QDialogButtonBox::AcceptRole.
     * Additional buttons may be specified multiple times, and they will all
     * save the state.
     *
     * @param but The button to activate the saving
     */
    void setSaveOnButton(QAbstractButton *but);

protected:
    /**
     * @reimp
     **/
    bool eventFilter(QObject *obj, QEvent *ev) override;

private slots:
    void restoreConfigInternal();
    void saveConfigInternal() const;

private:
    QDialog *mParent;
    DialogStateSaver *mStateSaver;
    bool mHaveOwnSaver;
};

#endif							// DIALOGSTATEWATCHER_H
