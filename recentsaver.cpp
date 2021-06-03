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

#include "recentsaver.h"

#include <qurl.h>
#include <qfileinfo.h>

#include <krecentdirs.h>

#include "libkfdialog_logging.h"


RecentSaver::RecentSaver(const QString &fileClass)
{
    Q_ASSERT(!fileClass.isEmpty());
    mRecentClass = fileClass;
    if (!mRecentClass.startsWith(':')) mRecentClass.prepend(':');
}


QUrl RecentSaver::recentUrl(const QString &suggestedName)
{
    // QUrl::fromLocalFile("") -> QUrl(), so no need for null test here
    return (QUrl::fromLocalFile(recentPath(suggestedName)));
}


QString RecentSaver::recentPath(const QString &suggestedName)
{
    mRecentDir = KRecentDirs::dir(mRecentClass);
    if (!mRecentDir.isEmpty() && !mRecentDir.endsWith('/')) mRecentDir += '/';

    QString recentDir = mRecentDir;
    if (!suggestedName.isEmpty()) recentDir += suggestedName;
    qCDebug(LIBKFDIALOG_LOG) << "for" << mRecentClass << "dir" << mRecentDir << "->" << recentDir;
    return (recentDir);
}


void RecentSaver::save(const QUrl &url)
{
    if (!url.isValid()) return;				// didn't get a valid entry
    if (!url.isLocalFile()) return;			// can only save local dirs
    save(url.path());					// save the local path
}


void RecentSaver::save(const QString &path)
{
    if (path.isEmpty()) return;				// didn't get a valid entry

    QString rd = QFileInfo(path).path();		// just take directory path
    if (!rd.endsWith('/')) rd += '/';			// ensure saved as directory
    if (rd==mRecentDir) return;				// nothing new, no need to save

    qCDebug(LIBKFDIALOG_LOG) << "for" << mRecentClass << "saving" << rd;
    KRecentDirs::add(mRecentClass, rd);
}
