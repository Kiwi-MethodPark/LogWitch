SET(LFA_GUITOOLS_MOC_CLASSES
	LogEntryParser_dummy.h
	LogEntryParser_log4cplusSocket.h
    LogEntryParser_Logfile.h
    NewLogEntryMessage.h
    ObjectCache.hxx
)

SET(LFA_GUITOOLS_LIB_SOURCES
    LogEntry.cpp
    LogEntryAttributeNames.cpp
    LogEntryFactory.cpp
    LogEntryParser_dummy.cpp
    LogEntryParser_log4cplusSocket.cpp
    LogEntryParser_Logfile.cpp
    LogEntryParserModelConfiguration.cpp
    NewLogEntryMessage.cpp
)

QT4_WRAP_CPP(LFA_GUITOOLS_MOC_SOURCES ${LFA_GUITOOLS_MOC_CLASSES})

ADD_LIBRARY(${LFA_GUITOOLS_LIB} STATIC ${LFA_GUITOOLS_LIB_SOURCES} ${LFA_GUITOOLS_MOC_SOURCES} )
