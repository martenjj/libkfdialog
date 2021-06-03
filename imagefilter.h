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

#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <qstringlist.h>

#include "libkfdialog_export.h"


/**
 * @short Generation of a filter string for image selection or saving.
 *
 * This class generates a filter suitable to use with either Qt or
 * KDE Frameworks file dialogues.  Although @c KFileDialog is deprecated
 * in Frameworks, a KDE filter is still required for a @c KUrlRequester
 * or if KFileWidget is used directly.
 *
 * @author Jonathan Marten
 **/

namespace ImageFilter
{
    /**
     * Enumeration specifying the intended use of the filter.
     **/
    enum FilterMode
    {
        Reading = 0x01,					///< Image reading
        Writing = 0x02					///< Image writing
    };

    /**
     * Enumeration specifying options for the filter.
     **/
    enum FilterOption
    {
        NoOptions = 0x00,				///< No options specified
        AllImages = 0x10,				///< Include an "All images" entry
        AllFiles  = 0x20,				///< Include an "All files" entry
        Unsorted  = 0x40				///< Do not sort the returned list
    };
    Q_DECLARE_FLAGS(FilterOptions, FilterOption)

    /**
     * Generate a Qt-style filter list.
     *
     * This is a filter list suitable for passing
     * to @c QFileDialog::setNameFilters().
     * 
     * @param mode The intended file operation mode
     * @param options Options for the filter generation.
     * @return The filter list
     **/
    LIBKFDIALOG_EXPORT QStringList qtFilterList(ImageFilter::FilterMode mode,
                                                  ImageFilter::FilterOptions options = ImageFilter::NoOptions);

    /**
     * Generate a Qt-style filter string.
     *
     * This is a filter string suitable for passing
     * to @c QFileDialog::getOpenFileName() or the similar static functions.
     * 
     * @param mode The intended file operation mode
     * @param options Options for the filter generation.
     * @return The filter string
     **/
    LIBKFDIALOG_EXPORT QString qtFilterString(ImageFilter::FilterMode mode,
                                                ImageFilter::FilterOptions options = ImageFilter::NoOptions);

    /**
     * Generate a KDE-style filter list.
     *
     * This is a filter string suitable for passing
     * to @c KUrlRequester::setFilter().
     * @param mode The intended file operation mode
     * @param options Options for the filter generation.
     * @return The filter string
     **/
    LIBKFDIALOG_EXPORT QString kdeFilter(ImageFilter::FilterMode mode,
                                           ImageFilter::FilterOptions options = ImageFilter::NoOptions);
}

Q_DECLARE_OPERATORS_FOR_FLAGS(ImageFilter::FilterOptions)


#endif							// IMAGEFILTER_H
