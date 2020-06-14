# ---------------------------------------------------------------------------
# Frontend2 Author: jongough aka. Jon Gough    License:wxWidgets License
# ---------------------------------------------------------------------------
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
##
## ====FRONTEND 2  Version: 1.0.115 (corresponds with testplugin)=============
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
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions
SET(VERBOSE_NAME Squiddio)
# Search "getcommonname"in weather_routing_pi.cpp
set(COMMON_NAME "squiddio")
set(TITLE_NAME "squiddio")
set(PACKAGE_CONTACT "Mauro Calvi")
SET(PACKAGE "squiddio")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "3")
SET(VERSION_PATCH "4")
set(VERSION_TWEAK "7")
set(VERSION_DATE "1/14/2020")

set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried and found by PluginConfigure.cmake.
#set(GIT_USER "mauroc")
#set(GIT_USER "rgleason")
#set(GIT_REPOSITORY_NAME "squiddio_pi")

# Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")  #Standard Repos
set(BETA "beta")       #Standard Repos
set(ALPHA "alpha") #Standard Repos

#set(CLOUDSMITH_USER "mauro-calvi")
#set(CLOUDSMITH_USER "rick-gleason")
set(CLOUDSMITH_USER "opencpn")

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
set(CLOUDSMITH_BASE_REPOSITORY "squiddio")  # without the '_pi'


# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/squiddio.html")
set(XML_SUMMARY "Plugin to download and display Points of Interest on a chart")
set(XML_DESCRIPTION "Plugin to download and display harbours, anchorages, weather bouys, satellite maps and other interesting cruise-planning objects on the chart")

##
## ----- Modify section above to include all the details for your plugin ----- ##
##


##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")


##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

project(${PACKAGE})


#===========================================================
INCLUDE("cmake/PluginSetup.cmake")
#===========================================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

# Settings and Options
set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
#===========================================================
INCLUDE("cmake/PluginConfigure.cmake")
#===========================================================
## ----- do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements for source, headers, etc. ----- ##

##
##----------Below From Squiddio for SET( Headers and SRCS) ------------------

SET(SRCS
    src/squiddio_pi.h
    src/squiddio_pi.cpp
    src/squiddioPrefsDialogBase.cpp
    src/squiddioPrefsDialogBase.h
    src/squiddio_pi_thread.h
    src/squiddio_pi_thread.cpp
    src/squiddioKaps.cpp
    src/sJSON.cpp
    src/sJSON.h
    src/icons.h
    src/icons.cpp
    src/logs.cpp
    src/logs.h
    src/NavObjectCollection.h
    src/NavObjectCollection.cpp
    src/Poi.h
    src/Poi.cpp
    src/PoiMan.h
    src/PoiMan.cpp
    src/Layer.h
    src/pugixml.hpp
    src/pugixml.cpp
    src/ocpn_types.h
    src/pugiconfig.hpp
    src/Hyperlink.h
    src/gpxdocument.h
    src/wxJSON/jsonval.cpp
    src/wxJSON/jsonreader.cpp
    src/wxJSON/json_defs.h
    src/wxJSON/jsonreader.h
    src/wxJSON/jsonval.h
    src/wxJSON/jsonwriter.cpp
    src/wxJSON/jsonwriter.h
  )

SET(SRC_NMEA0183
    src/nmea0183/LatLong.hpp
    src/nmea0183/latlong.cpp
    src/nmea0183/long.cpp
    src/nmea0183/nmea0183.cpp
    src/nmea0183/nmea0183.hpp
    src/nmea0183/Response.hpp
    src/nmea0183/response.cpp
    src/nmea0183/RMB.hpp
    src/nmea0183/rmb.cpp
    src/nmea0183/Sentence.hpp
    src/nmea0183/sentence.cpp
    src/nmea0183/talkerid.cpp
    src/nmea0183/RMC.HPP
    src/nmea0183/rmc.cpp
    src/nmea0183/hexvalue.cpp
    src/nmea0183/lat.cpp
    src/nmea0183/expid.cpp
    src/nmea0183/wpl.hpp
    src/nmea0183/wpl.cpp
    src/nmea0183/rte.hpp
    src/nmea0183/rte.cpp
    src/nmea0183/hdt.hpp
    src/nmea0183/hdt.cpp
    src/nmea0183/hdg.hpp
    src/nmea0183/hdg.cpp
    src/nmea0183/hdm.hpp
    src/nmea0183/hdm.cpp
    src/nmea0183/gll.hpp
    src/nmea0183/gll.cpp
    src/nmea0183/vtg.hpp
    src/nmea0183/vtg.cpp
    src/nmea0183/gga.hpp
    src/nmea0183/gga.cpp
    src/nmea0183/gsv.hpp
    src/nmea0183/gsv.cpp
    src/nmea0183/dbt.cpp
    src/nmea0183/dbt.hpp
    src/nmea0183/dpt.cpp
    src/nmea0183/dpt.hpp
    src/nmea0183/mtw.cpp
    src/nmea0183/mtw.hpp
    src/nmea0183/mwv.cpp
    src/nmea0183/mwv.hpp
    src/nmea0183/vhw.hpp
    src/nmea0183/vhw.cpp
    src/nmea0183/vwr.cpp
    src/nmea0183/vwr.hpp
    src/nmea0183/zda.cpp
    src/nmea0183/zda.hpp
    src/nmea0183/rsa.hpp
    src/nmea0183/rsa.cpp
    src/nmea0183/SatInfo.h
    src/nmea0183/mwd.cpp
    src/nmea0183/mwd.hpp
    src/nmea0183/vwt.cpp
    src/nmea0183/vwt.hpp
    src/nmea0183/mta.cpp
    src/nmea0183/mta.hpp
    src/nmea0183/vlw.cpp
    src/nmea0183/vlw.hpp
    src/nmea0183/mda.cpp
    src/nmea0183/mda.hpp
  )
  
##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================
# EG  also in some of the cmake/Plugin____.cmake files
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
  
INCLUDE_DIRECTORIES(nmea0183)
INCLUDE_DIRECTORIES(src/wxJSON)
INCLUDE_DIRECTORIES(api-16)

##----------Above From Squiddio ------------------

##
## ----- Change below to match project requirements ----- ##
##
##================================================================================
## Required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  is used in the conditionals below.
##================================================================================
##    add_library(${PACKAGE_NAME} SHARED ${SRCS}  ${SRC_NMEA0183})
##================================================================================

ADD_DEFINITIONS(-DTIXML_USE_STL)

IF(UNIX AND NOT APPLE)
    INCLUDE("cmake/FindTinyXML.cmake")
    FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX AND NOT APPLE)

IF(TINYXML_FOUND)
    message (STATUS "Building with system tinyxml")
    INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
    ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${SRC_NMEA0183})
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES} )
ELSE(TINYXML_FOUND)
    message (STATUS "Building with embedded tinyxml")
    SET(SRC_LTINYXML
    src/tinyxml.h
    src/tinystr.h
      )
    ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${SRC_NMEA0183} ${SRC_LTINYXML})
ENDIF(TINYXML_FOUND)

##
## ----- Change above to match project requirements ----- ##
##

##
## ----- Do not change - needed to build app ----- ##
##

include("cmake/PluginInstall.cmake")
include("cmake/PluginLocalization.cmake")
include("cmake/PluginPackage.cmake")

##
## ----- do not change section above - needed to configure build process ----- ##
##
