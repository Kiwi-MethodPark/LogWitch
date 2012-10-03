CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/GenerateIfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/BuildIfos.h @ONLY)


SET(LFA_FORMS
  logfileanalyser.ui
)

SET(LFA_MOC_CLASSES
	ContextMenuManipulateHeader.h
	FilterListView.h
	LogEntryTableWindow.h
	logfileanalyser.h
)

SET(LFA_SOURCES
	ContextMenuManipulateHeader.cpp
	EntryToTextFormaterDefault.cpp
	EntryToTextFormaterLog4cplus.cpp
	FilterListView.cpp
	LogEntryTableWindow.cpp
	logfileanalyser.cpp
	main.cpp	
)

SET(LFA_RESOURCES 
	LogfileAnalyserResources.qrc
)

INCLUDE_DIRECTORIES(
  ${QT_INCLUDES}
  ${Boost_INCLUDE_DIR}
  ${LOG4CPLUS_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}  # Neede for moc files in out of source builds.
)

#Add source dirs
ADD_SUBDIRECTORY(ActionRules)
ADD_SUBDIRECTORY(GUITools)
ADD_SUBDIRECTORY(Help)
ADD_SUBDIRECTORY(LogData)
ADD_SUBDIRECTORY(Models)

#LINK_DIRECTORIES(
#  ${CMAKE_CURRENT_BINARY_DIR}/ActionRules
#  ${CMAKE_CURRENT_BINARY_DIR}/GUITools
#  ${CMAKE_CURRENT_BINARY_DIR}/LogData
#  ${CMAKE_CURRENT_BINARY_DIR}/Models
#)

INCLUDE(${QT_USE_FILE})
  
ADD_DEFINITIONS(${QT_DEFINITIONS})

QT4_WRAP_CPP(LFA_HEADERS_MOC ${LFA_MOC_CLASSES})
QT4_WRAP_UI(LFA_FORMS_HEADERS ${LFA_FORMS})

QT4_ADD_RESOURCES(LFA_RESOURCES_RCC ${LFA_RESOURCES})

ADD_EXECUTABLE( LogWitch 
	${LFA_SOURCES} 
    ${LFA_FORMS_HEADERS} 
    ${LFA_HEADERS_MOC}
    ${LFA_RESOURCES_RCC}
)
    
TARGET_LINK_LIBRARIES( LogWitch 
  	${LFA_RULES_LIB}
  	${LFA_GUITOOLS_LIB}
  	${LFA_HELP_LIB}
   	${LFA_LOGDATA_LIB}
  	${LFA_MODELS_LIB}
  	${QT_LIBRARIES}
  	${LOG4CPLUS_LIBRARIES}
)

###################### Installation
install (TARGETS LogWitch
         RUNTIME DESTINATION bin )
         
IF(NOT UNIX)
	INSTALL( FILES ${QT_LIBRARY_DIR}/QtCore4.dll
		${QT_LIBRARY_DIR}/QtGUI4.dll
		${QT_LIBRARY_DIR}/QtNetwork4.dll
		${QT_LIBRARY_DIR}/QtSvg4.dll
		${QT_LIBRARY_DIR}/QtXml4.dll
		${PACKAGING_ADDDIR}/qt.conf
		${LOG4CPLUS_DLL}
		DESTINATION bin )
	INSTALL( FILES ${QT_PLUGINS_DIR}/imageformats/qsvg4.dll
		DESTINATION bin/plugins/imageformats )
ENDIF()
