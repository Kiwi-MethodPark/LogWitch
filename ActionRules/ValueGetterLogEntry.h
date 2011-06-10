/*
 * FieldDescriptor.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef VALUEGETTERLOGENTRY_H_
#define VALUEGETTERLOGENTRY_H_

#include <boost/shared_ptr.hpp>
#include <QtCore/QtCore>

#include "LogData/LogEntry.h"
#include "Types.h"
#include "ValueGetter.h"

class LogEntryParserModelConfiguration;

class ValueGetterLogEntry
    : public ValueGetter
{
public:
    ValueGetterLogEntry( const QString &name, boost::shared_ptr<const LogEntryParserModelConfiguration> configuration  );
    virtual ~ValueGetterLogEntry();

    bool isValid( ) const;

    int getID( ) const;

    TSharedConstQString getValue( TconstSharedLogEntry &entry ) const;

private:
    QString m_name;

    boost::shared_ptr<const LogEntryParserModelConfiguration> m_configuration;

    int m_fieldId;
};

typedef boost::shared_ptr<ValueGetterLogEntry> TSharedValueGetterLogEntry;
typedef boost::shared_ptr<const ValueGetterLogEntry> TconstSharedValueGetterLogEntry;

#endif /* VALUEGETTERLOGENTRY_H_ */
