##########################################################################
##									##
##  This CMake file is part of libkfdialog, a helper library for	##
##  implementing QtWidgets-based dialogues under KDE Frameworks or	##
##  standalone.  Originally developed as part of Kooka, a KDE		##
##  scanning/OCR application.						##
##									##
##  The library is free software; you can redistribute and/or		##
##  modify it under the terms of the GNU General Public License		##
##  version 2 or (at your option) any later version, as published	##
##  by the Free Software Foundation and appearing in the file		##
##  COPYING included in the packaging of this library, or at		##
##  http://www.gnu.org/licenses/gpl.html				##
##									##
##  Copyright (C) 2016-2024 Jonathan Marten				##
##                          <jjm AT keelhaul DOT me DOT uk>		##
##			    and Kooka authors/contributors		##
##									##
##  Home page:  https://github.com/martenjj/libkfdialog			##
##									##
##########################################################################

project(libkfdialog)
cmake_minimum_required(VERSION 3.6)

if (NOT DEFINED QT_MAJOR_VERSION)
  set(QT_MAJOR_VERSION 6)
endif ()

if (QT_MAJOR_VERSION EQUAL 6)
  set(QT_MIN_VERSION "6.3.0")
  set(KF_MIN_VERSION "6.1.0")
else ()
  if (QT_MAJOR_VERSION EQUAL 5)
    set(QT_MIN_VERSION "5.14.0")
    set(KF_MIN_VERSION "5.68.0")
  else ()
    message(FATAL_ERROR "Expecting to build for either Qt5 or Qt6")
  endif ()
endif ()

set(QV ${QT_MAJOR_VERSION})

set(VERSION "${QV}.0.0")
set(SOVERSION ${QV})
message(STATUS "Configuring for project '${CMAKE_PROJECT_NAME}' version ${VERSION}")

##########################################################################
##  Dependencies and definitions					##
##########################################################################

find_package(ECM ${KF_MIN_VERSION} REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(FeatureSummary)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPackageConfigHelpers)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(GenerateExportHeader)
include(ECMQtDeclareLoggingCategory)

# Options
option(INSTALL_BINARIES "Install the binaries and libraries, turn off for development in place" ON)

# Required Qt components to build this package
find_package(Qt${QV} ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Widgets)
# Required KF5 components to build this package
find_package(KF${QV} ${KF_MIN_VERSION} REQUIRED COMPONENTS I18n Config WidgetsAddons KIO)

# Rigourousness
add_definitions("-DQT_USE_FAST_CONCATENATION")
add_definitions("-DQT_USE_FAST_OPERATOR_PLUS")
add_definitions("-DQT_NO_CAST_FROM_BYTEARRAY")
add_definitions("-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT")
add_definitions("-DQT_NO_CAST_TO_ASCII")
add_definitions("-DQT_NO_URL_CAST_FROM_STRING")
add_definitions("-DQT_NO_CONTEXTLESS_CONNECT")

# Permissiveness
remove_definitions("-DQT_NO_CAST_FROM_ASCII")
remove_definitions("-DQT_NO_SIGNALS_SLOTS_KEYWORDS")
remove_definitions("-DQT_NO_KEYWORDS")

# I18N
add_definitions(-DTRANSLATION_DOMAIN="libkfdialog")

##########################################################################
##  libkfdialog library							##
##########################################################################

set(dialogutil_SRCS
  dialogbase.cpp
  dialogstatesaver.cpp
  dialogstatewatcher.cpp
  recentsaver.cpp
  imagefilter.cpp
)

set(dialogutil_HDRS
  dialogbase.h
  dialogstatesaver.h
  dialogstatewatcher.h
  recentsaver.h
  imagefilter.h
  ${CMAKE_CURRENT_BINARY_DIR}/libkfdialog_export.h
)

ecm_qt_declare_logging_category(dialogutil_SRCS
  HEADER "libkfdialog_logging.h"
  IDENTIFIER "LIBKFDIALOG_LOG"
  CATEGORY_NAME "libkfdialog"
  EXPORT libkfdialoglogging
  DESCRIPTION "libkfdialog")

add_library(kfdialog SHARED ${dialogutil_SRCS})
generate_export_header(kfdialog BASE_NAME libkfdialog)
target_link_libraries(kfdialog
  Qt${QV}::Core Qt${QV}::Widgets
  KF${QV}::I18n
  KF${QV}::ConfigCore
  KF${QV}::WidgetsAddons
  KF${QV}::KIOCore KF${QV}::KIOFileWidgets
)

set_target_properties(kfdialog PROPERTIES VERSION "${VERSION}" SOVERSION ${SOVERSION})

##########################################################################
##  Package configuration						##
##########################################################################

set(DU "LibKFDialog")
set(CONFIGDIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/${DU}")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/${DU}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${DU}Config.cmake"
  INSTALL_DESTINATION ${CONFIGDIR})

##########################################################################
##  Installation							##
##########################################################################

install(TARGETS kfdialog ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${dialogutil_HDRS} DESTINATION ${KDE_INSTALL_INCLUDEDIR}/kf${QV}/kfdialog)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${DU}Config.cmake" DESTINATION ${CONFIGDIR} COMPONENT Devel)

ecm_qt_install_logging_categories(EXPORT libkfdialoglogging
  FILE libkfdialog.categories
  DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}")

##########################################################################
##  Configuration information 						##
##########################################################################

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
