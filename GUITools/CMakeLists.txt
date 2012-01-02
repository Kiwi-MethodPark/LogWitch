SET(LFA_LOGDATA_MOC_CLASSES
	QScrollDownTableView.h
	SignalMultiplexer.h
	SlotToBoostFunction.h
)

SET(LFA_LOGDATA_LIB_SOURCES
    QScrollDownTableView.cpp
    SignalMultiplexerStateApplier.cpp
    SignalMultiplexer.cpp
    WidgetStateSaver.cpp
)

QT4_WRAP_CPP(LFA_LOGDATA_MOC_SOURCES ${LFA_LOGDATA_MOC_CLASSES})

ADD_LIBRARY(${LFA_LOGDATA_LIB} STATIC ${LFA_LOGDATA_LIB_SOURCES} ${LFA_LOGDATA_MOC_SOURCES})
