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
 *  Copyright (C) 2016-2024 Jonathan Marten				*
 *                          <jjm AT keelhaul DOT me DOT uk>		*
 *			    and Kooka authors/contributors		*
 *									*
 *  Home page:  https://github.com/martenjj/libkfdialog			*
 *									*
 ************************************************************************/

#include "imagefilter.h"

#include <qimagereader.h>
#include <qimagewriter.h>
#include <qmimedatabase.h>
#include <qmimetype.h>

#include <klocalizedstring.h>


struct FilterEntry
{
    explicit FilterEntry(const QStringList &pats, const QString &cmnt)	{ patterns = pats; comment = cmnt; }

    QStringList patterns;
    QString comment;
};


static bool commentLessThan(const FilterEntry &f1, const FilterEntry &f2)
{
    return (f1.comment.toLower()<f2.comment.toLower());
}


static QList<FilterEntry> filterList(ImageFilter::FilterMode mode, ImageFilter::FilterOptions options)
{
    QList<FilterEntry> list;
    QStringList allPatterns;

    QList<QByteArray> mimeTypes;
    if (mode==ImageFilter::Writing) mimeTypes = QImageWriter::supportedMimeTypes();
    else mimeTypes = QImageReader::supportedMimeTypes();

    QMimeDatabase db;

    for (const QByteArray &mimeType : std::as_const(mimeTypes))
    {
        QMimeType mime = db.mimeTypeForName(mimeType);
        if (!mime.isValid()) continue;

        const QStringList pats = mime.globPatterns();
        FilterEntry f(pats, mime.comment());
        list.append(f);
        if (options & ImageFilter::AllImages) allPatterns.append(pats);
    }

    if (!(options & ImageFilter::Unsorted))		// unless list wanted unsorted,
    {							// sort by the mime type comment
        std::sort(list.begin(), list.end(), commentLessThan);
    }

    if (!allPatterns.isEmpty())				// want an "All Images" entry
    {
        list.prepend(FilterEntry(allPatterns, i18n("All Image Files")));
    }

    if (options & ImageFilter::AllFiles)		// want an "All Files" entry
    {
        list.prepend(FilterEntry(QStringList("*"), i18n("All Files")));
    }

    return (list);
}


QString ImageFilter::qtFilterString(ImageFilter::FilterMode mode, ImageFilter::FilterOptions options)
{
    // The standard Qt-format filter string: "Applicable Files( *.foo *.bar);;..."
    return (qtFilterList(mode, options).join(";;"));
}


QStringList ImageFilter::qtFilterList(ImageFilter::FilterMode mode, ImageFilter::FilterOptions options)
{
    // The standard Qt-format filter list: "Applicable Files( *.foo *.bar)"
    const QList<FilterEntry> filters = filterList(mode, options);
    QStringList res;
    for (const FilterEntry &f : std::as_const(filters)) res.append(f.comment+" ("+f.patterns.join(" ")+")");
    return (res);
}


QString ImageFilter::kdeFilter(ImageFilter::FilterMode mode, ImageFilter::FilterOptions options)
{
    // The deprecated KDE-format filter string: "*.foo *.bar|Applicable Files\n..."
    const QList<FilterEntry> filters = filterList(mode, options);
    QStringList res;
    for (const FilterEntry &f : std::as_const(filters)) res.append(f.patterns.join(" ")+"|"+f.comment);
    return (res.join('\n'));
}
