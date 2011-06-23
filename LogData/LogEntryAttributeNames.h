/*
 * LogEntryAttributeNames.h
 *
 *  Created on: Jun 23, 2011
 *      Author: sven
 */

#ifndef LOGENTRYATTRIBUTENAMES_H_
#define LOGENTRYATTRIBUTENAMES_H_
#include <QtCore/QtCore>

class LogEntryAttributeNames
{
    Q_DECLARE_TR_FUNCTIONS(LogEntryAttributeNames)
public:
    LogEntryAttributeNames();

    typedef std::pair<QString,QString> TQStringPair;

    const TQStringPair attDescNumber;
    const TQStringPair attDescTimestamp;
    const TQStringPair attDescMessage;
    const TQStringPair attDescLoglevel;
    const TQStringPair attDescNDC;
    const TQStringPair attDescThread;
    const TQStringPair attDescLogger;
    const TQStringPair attDescFileSource;
};



#endif /* LOGENTRYATTRIBUTENAMES_H_ */
