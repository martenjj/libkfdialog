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
##  Copyright (C) 2016-2021 Jonathan Marten				##
##                          <jjm AT keelhaul DOT me DOT uk>		##
##			    and Kooka authors/contributors		##
##									##
##  Home page:  https://github.com/martenjj/libkfdialog			##
##									##
##########################################################################

project(libkfdialog)
set(VERSION "1.0")
set(SOVERSION 1)
message(STATUS "Configuring for ${CMAKE_PROJECT_NAME} version ${VERSION}")

##########################################################################
##  Dependencies and definitions					##
##########################################################################

cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)
set(QT_MIN_VERSION "5.14.0")
set(KF5_MIN_VERSION "5.68.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED)
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

# Required Qt5 components to build this package
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Widgets)
# Required KF5 components to build this package
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n Config WidgetsAddons KIO)

# Rigourousness
add_definitions("-DQT_USE_FAST_CONCATENATION")
add_definitions("-DQT_USE_FAST_OPERATOR_PLUS")
add_definitions("-DQT_NO_CAST_FROM_BYTEARRAY")
add_definitions("-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT")
add_definitions("-DQT_NO_CAST_TO_ASCII")
add_definitions("-DQT_NO_URL_CAST_FROM_STRING")

# Permissiveness
remove_definitions("-DQT_NO_CAST_FROM_ASCII")
remove_definitions("-DQT_NO_SIGNALS_SLOTS_KEYWORDS")

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
  Qt5::Core Qt5::Widgets
  KF5::I18n
  KF5::ConfigCore
  KF5::WidgetsAddons
  KF5::KIOCore KF5::KIOFileWidgets
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
install(FILES ${dialogutil_HDRS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/kfdialog)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${DU}Config.cmake" DESTINATION ${CONFIGDIR} COMPONENT Devel)

ecm_qt_install_logging_categories(EXPORT libkfdialoglogging
  FILE libkfdialog.categories
  DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}")

##########################################################################
##  Configuration information 						##
##########################################################################

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
