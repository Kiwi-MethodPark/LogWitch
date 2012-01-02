FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(log4cplus REQUIRED)

SET(LFA_FORMS
  logfileanalyser.ui
)

SET(LFA_MOC_CLASSES
	ContextMenuLogEntryHeader.h
	FilterListView.h
	LogEntryTableWindow.h
	logfileanalyser.h
)

SET(LFA_SOURCES
	ContextMenuLogEntryHeader.cpp
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
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QT_INCLUDES}
)

SET(LFA_RULES_LIB "logfileanalyzer_rules_lib")
SET(LFA_GUITOOLS_LIB "logfileanalyzer_guitools_lib")
SET(LFA_HELP_LIB "logfileanalyzer_help_lib")
SET(LFA_LOGDATA_LIB "logfileanalyzer_logdata_lib")
SET(LFA_MODELS_LIB "logfileanalyzer_models_lib")

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
         

