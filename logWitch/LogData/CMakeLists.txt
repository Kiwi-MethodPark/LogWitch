SET(LFA_LOGDATA_MOC_CLASSES
	LogEntryParser_dummy.h
	LogEntryParser_log4cplusSocket.h
  LogEntryParser_Logfile.h
  LogEntryParser_LogfileLWI.h
  NewLogEntryMessage.h
  ObjectCache.hxx
  ParserStreamGetter.h
)

SET(LFA_LOGDATA_LIB_SOURCES
  LogEntry.cpp
  LogEntryAttributeNames.cpp
  LogEntryFactory.cpp
  LogEntryParser_dummy.cpp
  LogEntryParser_log4cplusSocket.cpp
  LogEntryParser_Logfile.cpp
  LogEntryParser_LogfileLWI.cpp
  LogEntryParserModelConfiguration.cpp
  NewLogEntryMessage.cpp
  ParserStreamGetter.cpp
  ParserStreamGetterFile.cpp
)

QT4_WRAP_CPP(LFA_LOGDATA_MOC_SOURCES ${LFA_LOGDATA_MOC_CLASSES})

ADD_LIBRARY(${LFA_LOGDATA_LIB} STATIC ${LFA_LOGDATA_LIB_SOURCES} ${LFA_LOGDATA_MOC_SOURCES} )
