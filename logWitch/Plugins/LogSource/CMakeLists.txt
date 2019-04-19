# This is the other stuff needed for the plugin integration library part.
SET(LFA_PLUGINS_BASIC_LIB_MOC_CLASSES
  Interface/LogSourcePlugin.h
)

SET(LFA_PLUGINS_BASIC_LIB_SOURCES
  Interface/LogSourcePlugin.cpp
)

IF(USEQT5)
  QT5_WRAP_CPP(LFA_PLUGINS_BASIC_LIB_MOC_SOURCES ${LFA_PLUGINS_BASIC_LIB_MOC_CLASSES})
ELSE()
  QT4_WRAP_CPP(LFA_PLUGINS_BASIC_LIB_MOC_SOURCES ${LFA_PLUGINS_BASIC_LIB_MOC_CLASSES})
ENDIF()

ADD_LIBRARY(${LFA_PLUGINS_BASIC_LIB} STATIC ${LFA_PLUGINS_BASIC_LIB_SOURCES} ${LFA_PLUGINS_BASIC_LIB_MOC_SOURCES})

################################################################################
# Add plugins now
ADD_SUBDIRECTORY(log4cplus)
ADD_SUBDIRECTORY(dummy)
