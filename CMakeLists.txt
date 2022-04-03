##---------------------------------------------------------------------------
## Author:      Mauro Calvi
##***************************************************************************
## *   Copyright (C) 2010 by Mauro Calvi                               *
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
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx          *
## ***
# ---------------------------------------------------------------------------
## Author: jongough, Jon Gough Frontend2 v 1.0.194.1 02/04/2022 # DD/MM/YYYY format *
# ---------------------------------------------------------------------------
##     
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##
##
## ----- Modify section below to include all the details for your plugin ----- ##
##

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
SET(VERSION_PATCH "19")
set(VERSION_TWEAK "0")
set(VERSION_DATE "02/04/2022") # DD/MM/YYYY format

set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
set(GIT_REPOSITORY_SERVER "github.com")

# Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
set(CLOUDSMITH_BASE_REPOSITORY "squiddio")  # without the '_pi'

#No longer needed, queried and found by PluginConfigure.cmake.
#set(GIT_USER "mauroc")
#set(GIT_REPOSITORY_NAME "squiddio_pi")

set(CLOUDSMITH_USER "opencpn")
#set(CLOUDSMITH_USER "mauro-calvi")

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/squiddio.html")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

## ----- Modify section above to include all the details for your plugin ----- ##
## ----- Modify below for plugin special requirements------------ ##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Use local version of GLU library
set(USE_LOCAL_GLU TRUE)
# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
#set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##

project(${PACKAGE})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#===========================================================
include("PluginSetup")
#===========================================================

## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##


set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- Do not change next section - needed to configure build process ----- ##
#===========================================================
include("PluginConfigure")
#===========================================================
## ----- Do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements for source, headers, etc. ----- ##

##----- Below From Squiddio for SET( Headers and SRCS) ------------------

add_definitions(-DUSE_S57)

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


#  Below Added ====
set(EXTINCLUDE
    extinclude/ODAPI.h
    extinclude/ODJSONSchemas.h
)

set(LIBSSRC libs/wxJSON/src/jsonreader.cpp libs/wxJSON/src/jsonval.cpp libs/wxJSON/src/jsonwriter.cpp)

set(LIBHDRS
    libs/GL/gl.h
    libs/GL/gl_private.h
    libs/GL/glext.h
    libs/GL/glu.h
    libs/wxJSON/include/json_defs.h
    libs/wxJSON/include/jsonreader.h
    libs/wxJSON/include/jsonval.h
    libs/wxJSON/include/jsonwriter.h
    )

add_definitions(-DPLUGIN_USE_SVG)

set(EXTINCLUDE_DIR ${EXTINCLUDE_DIR} extinclude libs/ocpn-api/)

# Above added ====

#
# ----- If using JSON validation in plugin section below is needed ----- ##
#
# Not using Json validator  

#
# ----- If using JSON validation in plugin section above is needed ----- ##
#


##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================

# Not doing JSON Schema Validator

#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/src/nmea0183)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/src/wxJSON)

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/glu/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/${EXTINCLUDE_DIR})
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/GL)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/ocpn-api)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wxJSON/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/nmea0183/src)

#
# ----- Change below to match project requirements for android build ----- ##
#

if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
    include_directories( ${CMAKE_SOURCE_DIR}/extinclude/GLES2)
endif(QT_ANDROID)

##
## ----- Change above to match project requirements for android build ----- ##
##




## Statement below is required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
#======================================================================================
target_sources(${PACKAGE_NAME} PUBLIC ${SRCS} ${HDRS} ${EXTSRC} ${LIBHDRS} ${LIBSSRC})
#=======================================================================================
##  OLD   add_library(${PACKAGE_NAME} SHARED ${SRCS}  ${SRC_NMEA0183})
##================================================================================

if(NOT OCPN_FLATPAK_CONFIG)
    # Build environment not available when flatpak is being configured so following statements will not work
    message(STATUS "${CMLOC}Adding target link libraries to ${PACKAGE_NAME}")

    add_subdirectory(libs/nmea0183)
    add_subdirectory(libs/tinyxml)

    target_link_libraries(${PACKAGE_NAME} ocpn::nmea0183)
    target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)
	
endif(NOT OCPN_FLATPAK_CONFIG)

add_definitions(-DTIXML_USE_STL)

##----------Above From Squiddio ------------------
#
# ----- If using JSON validation in plugin section below is needed ----- ##
#

# Not using Json validator  
#
# ----- If using JSON validation in plugin section above is needed ----- ##
#


## ----- Change above to match project requirements ----- ##
##
## ----- do not change next section - needed to configure build process ----- ##
##

#
# Needed for android builds
if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)


## Needed for all builds
##=======================================
include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")
##=======================================
##
## ----- Do not change section above - needed to configure build process ----- ##
##
message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
