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

#ifndef RECENTSAVER_H
#define RECENTSAVER_H

#include <qstring.h>
#include "libkfdialog_export.h"

class QUrl;


/**
 * @short A helper to look up and save recent locations for a file dialogue.
 *
 * Saving the recent locations is handled by @c KRecentDirs, but a bit of code
 * is needed before and after each file dialogue to look up the appropriate
 * recent location and save it afterwards.  This class automates that.
 *
 * For a simple use of the static QFileDialog functions, simply create and
 * use a RecentSaver like this:
 *
 * @code
 * RecentSaver recent("key");
 * QUrl u = QFileDialog::getSaveFileUrl(parent, i18n("caption"),
 *                                      recent.recentUrl("untitled.ext"))
 * if (u.isValid())
 * {
 *   recent.save(u);
 *   // use the URL
 * }
 * @endcode
 *
 * @see KRecentDirs
 * @see QFileDialog
 * @author Jonathan Marten
 **/

class LIBKFDIALOG_EXPORT RecentSaver
{
public:
    /**
     * Constructor.
     *
     * @param fileClass The name of the file class.  If the application-global
     * recent locations list is to be used then this may start with a single ":",
     * but need not do so; if it does not start with a ":" then one is assumed.
     * If the system-global recent locations list is to be used then this
     * must start with "::".
     **/
    explicit RecentSaver(const QString &fileClass);

    /**
     * Destructor.
     **/
    ~RecentSaver() = default;

    /**
     * Resolve the saved recent location (if there is one) and a suggested
     * file name (if required) into a URL to pass to a @c QFileDialog
     * which expects a URL.
     *
     * @param suggestedName The suggested file name, or a null string
     * if none is required.
     * @return The resolved URL, or a null URL if there is no saved
     * history.
     **/
    QUrl recentUrl(const QString &suggestedName = QString());

    /**
     * Resolve the saved recent location (if there is one) and a suggested
     * file name (if required) into a path, to pass to a @c QFileDialog
     * which expects a pathname.
     *
     * @param suggestedName The suggested file name, or a null string
     * if none is required.
     * @return The resolved file path, or a null string if there is no
     * saved history.
     **/
    QString recentPath(const QString &suggestedName = QString());

    /**
     * Save the location selected by the file dialogue as a new recent location.
     *
     * @param url The URL returned from the file dialogue.
     **/
    void save(const QUrl &url);

    /**
     * Save the location selected by the file dialogue as a new recent location.
     *
     * @param url The file path returned from the file dialogue.
     **/
    void save(const QString &path);

private:
    QString mRecentClass;
    QString mRecentDir;
};

#endif							// RECENTSAVER_H
