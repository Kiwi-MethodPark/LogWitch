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

class AttributeConfiguration
{
public:
    typedef std::pair<QString,QString> TQStringPair;
    typedef boost::function<QVariant(const QString&)> EntryFactoryFunction;

    AttributeConfiguration( bool caching, int defaultCellWidth, TQStringPair names, EntryFactoryFunction factory );

    bool caching;
    int defaultCellWidth;
    TQStringPair names;

    EntryFactoryFunction attributeFactory;
};

class LogEntryAttributeNames
{
    Q_DECLARE_TR_FUNCTIONS(LogEntryAttributeNames)
public:


    LogEntryAttributeNames();

    /**
     * Returns the default configuration for the column named with columnName.
     */
    const AttributeConfiguration &getConfiguration( const QString &columnName ) const;

private:
    const AttributeConfiguration::TQStringPair attDescNumber;
    const AttributeConfiguration::TQStringPair attDescTimestamp;
    const AttributeConfiguration::TQStringPair attDescMessage;
    const AttributeConfiguration::TQStringPair attDescLoglevel;
    const AttributeConfiguration::TQStringPair attDescNDC;
    const AttributeConfiguration::TQStringPair attDescThread;
    const AttributeConfiguration::TQStringPair attDescLogger;
    const AttributeConfiguration::TQStringPair attDescFileSource;

    typedef std::map<QString, AttributeConfiguration> StringIntMap;

    StringIntMap m_defaultCellIfos;

    AttributeConfiguration m_defaultCellIfo;


};



#endif /* LOGENTRYATTRIBUTENAMES_H_ */
