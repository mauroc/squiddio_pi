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
## ***************************************************************************
 
SET(PARENT "opencpn")
SET(PACKAGE squiddio)
SET(VERBOSE_NAME Squiddio)
SET(CPACK_PACKAGE_CONTACT "Mauro Calvi")
SET(CLOUDSMITH_USER "mauro-calvi")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "19")
SET(VERSION_TWEAK "0")
SET(VERSION_DATE "21/10/2019")
SET(OCPN_MIN_VERSION "ov50")
SET(OCPN_API_VERSION_MAJOR "1")
SET(OCPN_API_VERSION_MINOR "16")

SET(CMAKE_CXX_STANDARD 11)

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.1.1)
IF (COMMAND cmake_policy)
  if (POLICY CMP0043)
    CMAKE_POLICY(SET CMP0043 NEW)
  endif (POLICY CMP0043)
  if (POLICY CMP0077)
    CMAKE_POLICY(SET CMP0077 NEW)
  endif (POLICY CMP0077)
ENDIF (COMMAND cmake_policy)

# Prefer libGL.so to libOpenGL.so, see CMP0072
SET(OpenGL_GL_PREFERENCE "LEGACY")

SET(PACKAGE_NAME ${PACKAGE}_pi)
SET(PACKAGE_FILE_NAME "${PARENT}-plugin-${PACKAGE}")
STRING(TOUPPER "${PACKAGE}" TITLE_NAME)

PROJECT(${PACKAGE_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK})

SET(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

MESSAGE(STATUS "${VERBOSE_NAME} Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
MESSAGE(STATUS "OCPN API Verson: ${OCPN_API_VERSION_MAJOR}.${OCPN_API_VERSION_MINOR}")
MESSAGE(STATUS "OCPN Min Verson: ${OCPN_MIN_VERSION}")

# Don't use local version of GLU library
SET(USE_LOCAL_GLU FALSE)

IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "")
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build")
ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "")
SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel" )
MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

SET(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION}-${OCPN_MIN_VERSION}-${OCPN_API_VERSION_MAJOR}.${OCPN_API_VERSION_MINOR})
SET(PKG_BASE_URL "https://dl.cloudsmith.io/public/${CLOUDSMITH_USER}/${PACKAGE}-pi/raw/files")

SET(PKG_RELEASE "1")
SET(PKG_NVR ${PACKAGE_NAME}-${PACKAGE_VERSION}.${PKG_RELEASE})
SET(PKG_BASE_URL "https://dl.cloudsmith.io/public/mauro-calvi/squiddio-pi/raw/files")

INCLUDE("cmake/PluginSetup.cmake")
set(PLUGIN_NAME squiddio-plugin-${PKG_TARGET}-${PKG_TARGET_VERSION})

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_SQUIDDIO
    src/squiddio_pi.h
    src/squiddio_pi.cpp
    src/squiddioPrefsDialogBase.cpp
    src/squiddioPrefsDialogBase.h
    src/squiddio_pi_thread.h
    src/squiddio_pi_thread.cpp
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
INCLUDE_DIRECTORIES(nmea0183)
INCLUDE_DIRECTORIES(src/wxJSON)
INCLUDE_DIRECTORIES(api-16)

ADD_DEFINITIONS(-DTIXML_USE_STL)
IF(UNIX AND NOT APPLE)
 INCLUDE("cmake/FindTinyXML.cmake")
 FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX AND NOT APPLE)

IF(TINYXML_FOUND)
 message (STATUS "Building with system tinyxml")
 INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_SQUIDDIO} ${SRC_NMEA0183})
 TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES} )
ELSE(TINYXML_FOUND)
 message (STATUS "Building with embedded tinyxml")
 SET(SRC_LTINYXML
    src/tinyxml.h
    src/tinystr.h
 )
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_SQUIDDIO} ${SRC_NMEA0183} ${SRC_LTINYXML})
ENDIF(TINYXML_FOUND)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")

