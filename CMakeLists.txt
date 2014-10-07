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
 

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(squiddio_pi)

SET(PACKAGE_NAME squiddio_pi)
SET(CPACK_PACKAGE_CONTACT "Mauro Calvi")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "3")

#SET(CMAKE_BUILD_TYPE Debug)

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_SQUIDDIO
    src/squiddio_pi.h
 	src/squiddio_pi.cpp
 	src/icons.h
 	src/icons.cpp
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
 	src/tinyxml.h
 	src/tinystr.h
 	)
 	

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_SQUIDDIO} )

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
