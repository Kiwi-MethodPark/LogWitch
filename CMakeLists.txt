PROJECT(LogWitch)

SET (VERSION_MAJOR "0")
SET (VERSION_MINOR "1")
SET (VERSION_BUILD "4")

SET (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD})
MESSAGE(STATUS "LogWitch ${VERSION} built with prefix: ${CMAKE_INSTALL_PREFIX}")

# Set default value for the build type to release, if nothing else was specified
IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No build type selected, default to Release.\n"
                       "Use cmake -DCMAKE_BUILD_TYPE=debug to build a debug build")
	SET(CMAKE_BUILD_TYPE "Release")
endif()

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

SET(QT_USE_QTNETWORK TRUE)

### Definitions of Directories.
SET(MAIN_DIR_NAME "LogWitch")

SET(HELPDIR "share/${MAIN_DIR_NAME}")

### Search dependencies
FIND_PACKAGE(Qt4 4.7 REQUIRED)
FIND_PACKAGE(log4cplus REQUIRED)

SET(LFA_RULES_LIB "logfileanalyzer_rules_lib")
SET(LFA_GUITOOLS_LIB "logfileanalyzer_guitools_lib")
SET(LFA_HELP_LIB "logfileanalyzer_help_lib")
SET(LFA_LOGDATA_LIB "logfileanalyzer_logdata_lib")
SET(LFA_MODELS_LIB "logfileanalyzer_models_lib")

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_SUBDIRECTORY(logWitch)

### Testing stuff
ENABLE_TESTING()

ADD_SUBDIRECTORY(test)


######################## CPack informations if CPack is available.
IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  	SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LogWitch is an analyzer for log files.")
  	SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
  	SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
  	SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_BUILD}")
  	SET(CPACK_PACKAGE_VENDOR "Sven Steckmann")
	SET(CPACK_PACKAGE_CONTACT "logwitch@steckmann.de")
  	SET(CPACK_PACKAGE_INSTALL_DIRECTORY "LogWitch ${VERSION}")
  	SET(CPACK_PACKAGE_EXECUTABLES "LogWitch")
  	SET(CPACK_GENERATOR "TGZ")
  
	IF(TARGET_SYSTEM STREQUAL "Ubuntu_12.04")
		MESSAGE(STATUS "Using library versions for Ubuntu 12.04 LTS")
		SET(CPACK_DEBIAN_PACKAGE_DEPENDS "liblog4cplus-1.0-4 (>= 1.0.4), libqtcore4 (>=4:4.7), libqtgui4 (>=4:4.7), libqt4-network (>=4:4.7), libqt4-help (>=4:4.7)")
	ENDIF()

  	INCLUDE(CPack)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
