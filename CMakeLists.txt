##---------------------------------------------------------------------------
## Author:      Mauro Calvi
##***************************************************************************
## *   Copyright (C) 2010 by Mauro Calvi                               *
## *   License:     GPLv3+
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,                                       *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                                       *
#-----------------------------------------------------------------------------
# Frontend2 Author: Jon Gough 
# Testplugin(TP) v1.0.260 07/10/2023 DD/MM/YYYY format
# buildCI:yes useODAPI:no validateJSON:no
# ---------------------------------------------------------------------------
## -- When changing this file do NOT change the order of the sections.    -- ##
## -- Make changes only between the section blocks identified             -- ##
## -- The whole configuration process relies on this sequence for success.-- ##
##
## -- Modify section below to include all the details for your plugin.    -- ##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.5.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
# Need cmake 3.13 to implement this, but raspbian builds are at 3.7.2 and bionic at 3.10 and xenial builds at 3.5.1
#    cmake_policy(SET CMP0076 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions
SET(VERBOSE_NAME "Squiddio")
set(COMMON_NAME "squiddio")
set(TITLE_NAME "squiddio")
set(PACKAGE_CONTACT "Mauro Calvi")
SET(PACKAGE "squiddio")
set(SHORT_DESCRIPTION "Plugin to download and display Points of Interest on a chart")
set(LONG_DESCRIPTION "User Sourced database to download and display harbours, anchorages, weather bouys, satellite maps and other interesting cruise-planning objects on the chart and log sharing.")

## "== User-sourced database of sailing Points of Interest ==\n\
##To download (or update) Points of Interest (POIs) for a desired region (requires Internet connection):\n\
##* Position cursor on area where you want to view POIs and right click mouse\n\
##* Select 'sQuiddio: Download (or Update) local Points of Interest' from context-sensitive menu.\n\n\
## Destinations appear as OpenCPN waypoints (default) or Draw Text Points (if the Draw plugin is installed):  \n\
##* Hover on waypoints to view a synopsis of POI information\
##* Right-click on waypoint for link to sQuiddio's destination page. \n\
##* Follow link to rate destination and add comments online.\n\n\
##Other menu options: \n\
## * Toggle visibility for local POIs on/off \n\
##* Submit a new destination (requires Internet connection and free user account)\n\
##* Download Google Maps as OCPN charts for all POIs in viewport\n\n\
##== In-chart log-sharing for cruisers ==\n\
##* Share your GPS coordinates and other navigational information with your cruising friends\n\
##and visualize their position on your OpenCPN charts (requires a free sQuiddio account)\n\n\
##IMPORTANT: By using this plugin you are agreeing to the sQuidd.io Terms \n\
## and Conditions, available at http://squidd.io/enduser_agreement");

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "3")
SET(VERSION_PATCH "50")
set(VERSION_TWEAK "0")
set(VERSION_DATE  "10/10/2023")#DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov58")

set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "17")
set(TP_COMMENT "  * Release using CI")

set(PARENT "opencpn")

#Use to set up a local webserver with git installed.
#Default is github.com. Now set in pluginconfigure.cmake
#set(GIT_REPOSITORY_SERVER "github.com")

## Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")

set(CLOUDSMITH_BASE_REPOSITORY "squiddio")  # without the '_pi'

# Set the following to use OpenCPN cloudsmith repositories instead of own repositories
set(CLOUDSMITH_USER "opencpn")
#set(CLOUDSMITH_USER "mauro-calvi")

set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/squiddio.html")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

## ----- Modify section above to include all the details for your plugin ------- ##
## ----- Modify section below if there are special requirements for the plugin --##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Use local version of GLU library
# requires libs/glu directory
set(USE_LOCAL_GLU TRUE)
# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")   #not operational

option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

## ----- Statements below used to setup standard variables required by the CMAKE process -- Do not remove - ##

project(${PACKAGE})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

##========================================================

include("PluginSetup")

##========================================================
## ----- Above sets up standard variables required by CMAKE - Do not remove -----##
## ----- Modify section below if there are special requirements for the plugin --##

set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin --##
## ----- Do not change next section - needed to configure build process ---------##
##===========================================================

include("PluginConfigure")

##===========================================================
## ----- Do not change section above - needed to configure build process --------##
## ----- Change below to match project requirements for source, headers, etc. ---##

add_definitions(-DUSE_S57)

## ---- Section Below - Add your project SET(SRCS and SET(HDRS  etc..

SET(SRCS
    src/squiddio_pi.cpp
    src/squiddioPrefsDialogBase.cpp
    src/squiddio_pi_thread.cpp
    src/squiddioKaps.cpp
    src/sJSON.cpp
    src/icons.cpp
    src/logs.cpp
    src/NavObjectCollection.cpp
    src/Poi.cpp
    src/PoiMan.cpp
    src/pugixml.hpp
    src/pugixml.cpp
    src/pugiconfig.hpp
  )

SET(HDRS
    src/squiddio_pi.h
    src/squiddioPrefsDialogBase.h
    src/squiddio_pi_thread.h
    src/sJSON.h
    src/icons.h
    src/logs.h
    src/NavObjectCollection.h
    src/Poi.h
    src/PoiMan.h
    src/Layer.h
    src/ocpn_types.h
    src/Hyperlink.h
    src/gpxdocument.h
  )


add_definitions(-DPLUGIN_USE_SVG)

## ----- Section above - Add your project SET(SRCS and SET(HDRS  etc.-------##
## ----- If using JSON validation in plugin section below is needed --------##

# Do not need Json Schema Validation

## ----- If using JSON validation in plugin section above is needed --------##


##=============================================================
## Change as required to collect all the set( headers and SRCS
## This add_library  can be used in conditionals below.
##=============================================================

SET(SRC ${SRCS} ${HDRS} )

##=============================================================
## Modify/configure 'Include' Directories below for the Plugin
##=============================================================

#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)

## Change target_sources to match your project needs.
##==============================================================

target_sources(${PACKAGE_NAME} PUBLIC ${SRC} )

##==============================================================

if(NOT OCPN_FLATPAK_CONFIG)
    # Build environment not available when flatpak is being configured 
	# so following statements will not work
    message(STATUS "${CMLOC}Adding target link libraries to ${PACKAGE_NAME}")

#    add_subdirectory(opencpn-libs/WindowsHeaders)
#    target_link_libraries(${PACKAGE_NAME} windows::headers)

    add_subdirectory(opencpn-libs/api-17)
    target_link_libraries(${PACKAGE_NAME} ocpn::api)

    add_subdirectory(opencpn-libs/tinyxml)
    target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

    add_subdirectory(opencpn-libs/wxJSON)
    target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

    add_subdirectory(opencpn-libs/plugingl)
    target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)
	
	add_subdirectory(opencpn-libs/jsonlib)
    target_link_libraries(${PACKAGE_NAME} ocpn::jsonlib)

    add_subdirectory(opencpn-libs/nmea0183)
    target_link_libraries(${PACKAGE_NAME} ocpn::nmea0183)

endif(NOT OCPN_FLATPAK_CONFIG)

add_definitions(-DTIXML_USE_STL)

##
## ----- If using JSON validation section below is needed ---------------##
##

# Do not need Json Schema Validation


##------- Change below to match project requirements --------------------##

# Needed for android builds
if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)

## Needed for all builds Do not change - needed to build app -------------##
##===============================================================

include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")

##================================================================
## ----- Do not change section above - needed to configure build process --##

message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
