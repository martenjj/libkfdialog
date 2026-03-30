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

#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <qdialog.h>
#include <qdialogbuttonbox.h>

#include "libkfdialog_export.h"

class QShowEvent;
class QSpacerItem;
class KGuiItem;
class KConfigGroup;
class DialogStateWatcher;
class DialogStateSaver;


/**
 * @short A wrapper for QDialog incorporating some convenience functions.
 *
 * This is a lightweight wrapper around QDialog, incorporating some useful
 * functions which used to be provided by KDialog in KDE4.  These are:
 *
 * - Managing the button box and providing access to its buttons
 * - Managing the top level layout
 * - Saving and restoring the dialogue box size
 *
 * @author Jonathan Marten
 **/

class LIBKFDIALOG_EXPORT DialogBase : public QDialog
{
    Q_OBJECT

public:
    /**
     * Destructor.
     *
     **/
    virtual ~DialogBase() = default;

    /**
     * Retrieve the main widget.
     *
     * @return the main widget
     **/
    QWidget *mainWidget() const				{ return (mMainWidget); }

    /**
     * Set a state saver for the dialogue.
     *
     * This may be a subclass of a DialogStateSaver, reimplemented in
     * order to save special dialogue settings (e.g. the column states
     * of a list view).  If this is not set then a plain DialogStateSaver
     * will be created and used internally.  If a NULL state saver is
     * set explicitly using this function, then no state restoring or
     * saving will be done.
     *
     * @param saver the state saver
     *
     * @note The saver should be set before the dialogue is shown for
     * the first time.
     * @see DialogStateSaver
     **/
    void setStateSaver(DialogStateSaver *saver);

    /**
     * Access the state saver used by the dialogue.
     *
     * This may be the default one, or that set by @c setStateSaver().
     *
     * @return the state saver
     **/
    DialogStateSaver *stateSaver() const;

    /**
     * Access the state watcher used by the dialogue.
     *
     * This is created and used internally.
     *
     * @return the state watcher
     **/
    DialogStateWatcher *stateWatcher() const		{ return (mStateWatcher); }

    /**
     * Get a vertical spacing suitable for use within the dialogue layout.
     *
     * @return The spacing hint
     **/
    static int verticalSpacing();

    /**
     * Get a horizontal spacing suitable for use within the dialogue layout.
     *
     * @return The spacing hint
     **/
    static int horizontalSpacing();

    /**
     * Create a spacer item suitable for use within a vertical layout.
     *
     * @return The spacer item
     **/
    static QSpacerItem *verticalSpacerItem();

    /**
     * Create a spacer item suitable for use within a horizontal layout.
     *
     * @return The spacer item
     **/
    static QSpacerItem *horizontalSpacerItem();

    /**
     * Access the dialogue's button box.
     *
     * @return the button box
     **/
    QDialogButtonBox *buttonBox() const			{ return (mButtonBox); }

    /**
     * Set the standard buttons to be displayed within the button box.
     *
     * @param buttons The buttons required
     *
     * @note This can be called at any time and the buttons will change
     * accordingly.  However, the buttons will be regenerated which means
     * that any special button text or icons, or any signal connections from
     * them, will be lost.
     **/
    void setButtons(QDialogButtonBox::StandardButtons buttons);

    /**
     * Set the enable state of a button.
     *
     * @param button The button to set
     * @param state The enable state for the button
     *
     * @see setModified()
     **/
    void setButtonEnabled(QDialogButtonBox::StandardButton button, bool state = true);

    /**
     * Set the text of a button.
     *
     * @param button The button to set
     * @param state The new text for the button
     *
     * @note This can be called at any time, and the button will change
     * accordingly.
     **/
    void setButtonText(QDialogButtonBox::StandardButton button, const QString &text);

    /**
     * Set the icon of a button.
     *
     * @param button The button to set
     * @param state The new icon for the button
     *
     * @note This can be called at any time, and the button will change
     * accordingly.
     **/
    void setButtonIcon(QDialogButtonBox::StandardButton button, const QIcon &icon);

    /**
     * Set up a button from a @c KGuiItem.
     *
     * @param button The button to set
     * @param guiItem The @c KGuiItem for the button
     *
     * @note This can be called at any time, and the button will change
     * accordingly.
     **/
    void setButtonGuiItem(QDialogButtonBox::StandardButton button, const KGuiItem &guiItem);

    /**
     * Enable modification mode and set whether the dialogue is modified.
     *
     * This mode sets the OK, Cancel and Close buttons accordingly as to
     * whether the dialogue state has been modified.  If this is called
     * with @p mod set to @c false then the the Close button will be
     * shown, the Cancel button will be hidden, and the OK button will
     * be disabled.  If called with @p mod set to @c true then Close
     * will be hidden, Cancel will be shown, and OK will be enabled.
     *
     * If intended to be enabled as above, the state of the OK button
     * also depends on whether @c setButtonEnabled() has been called
     * for that button.  If the dialogue state is not modified, in which
     * case the OK button is disabled, @c setButtonEnabled() will not
     * enable or disable it but simply retain the intended state.  When
     * the dialogue becomes modified, the OK button will be enabled
     * accordingly.
     *
     * If this function is never called then there will be no tracking
     * of the modification state and no changes will be made to any
     * buttons.
     *
     * @param mod Whether dialogue settings have been modified
     *
     * @see setButtonEnabled()
     **/
    void setModified(bool mod);

    /**
     * Check whether the dialogue is modified.
     *
     * @return @c true if modification mode is in use and the
     * dialogue is modified.
     **/
    bool isModified() const				{ return (mModifiedState); }

protected:
    /**
     * Constructor.
     *
     * @param pnt Parent widget
     **/
    explicit DialogBase(QWidget *pnt = nullptr);

    /**
     * Set the main widget to be displayed within the dialogue.
     *
     * @param w The widget
     **/
    void setMainWidget(QWidget *w)			{ mMainWidget = w; }

    /**
     * @reimp
     **/
    void showEvent(QShowEvent *ev) override;

private:
    QDialogButtonBox *mButtonBox;
    QWidget *mMainWidget;
    DialogStateWatcher *mStateWatcher;

    bool mModifyingMode;
    bool mModifiedState;
    bool mOkShouldBeEnabled;
};

#endif							// DIALOGBASE_H
