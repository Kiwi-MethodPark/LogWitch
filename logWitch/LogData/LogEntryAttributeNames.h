/*
 * LogEntryAttributeNames.h
 *
 *  Created on: Jun 23, 2011
 *      Author: sven
 */

#ifndef LOGENTRYATTRIBUTENAMES_H_
#define LOGENTRYATTRIBUTENAMES_H_

#include <boost/function.hpp>
#include <QtCore/QtCore>

class LogEntryAttributeNames
{
    Q_DECLARE_TR_FUNCTIONS(LogEntryAttributeNames)
public:
    typedef std::pair<QString,QString> TQStringPair;
    typedef boost::function<QVariant(const QString&)> EntryFactoryFunction;

    class EntryConfiguration
    {
    public:
        EntryConfiguration( bool caching, int defaultCellWidth, TQStringPair names, EntryFactoryFunction factory );

        bool caching;
        int defaultCellWidth;
        TQStringPair names;

        EntryFactoryFunction factory;
    };

    LogEntryAttributeNames();

    /**
     * Returns the default configuration for the colum named with name.
     */
    const EntryConfiguration &getDefautlForColumn( const QString &name ) const;

    const TQStringPair attDescNumber;
    const TQStringPair attDescTimestamp;
    const TQStringPair attDescMessage;
    const TQStringPair attDescLoglevel;
    const TQStringPair attDescNDC;
    const TQStringPair attDescThread;
    const TQStringPair attDescLogger;
    const TQStringPair attDescFileSource;

private:

    typedef std::map<QString, EntryConfiguration> StringIntMap;

    StringIntMap m_defaultCellIfos;

    EntryConfiguration m_defaultCellIfo;


};



#endif /* LOGENTRYATTRIBUTENAMES_H_ */
