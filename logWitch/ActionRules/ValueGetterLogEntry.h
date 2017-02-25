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
#include "LogData/LogEntryParserModelConfiguration.h"


class ValueGetterLogEntry
    : public ValueGetter
{
public:
    ValueGetterLogEntry();
    ValueGetterLogEntry( const QString &name, TSharedConstLogEntryParserModelConfiguration configuration  );
    ValueGetterLogEntry( TSharedConstLogEntryParserModelConfiguration configuration  );
    virtual ~ValueGetterLogEntry();

    void setName( const QString &name );
    QString getName() const;

    bool isValid( ) const;

    int getID( ) const;

    TSharedConstQString getValue( TconstSharedLogEntry &entry ) const;

    std::ostream &out( std::ostream &o, bool extended = false ) const;

private:
    QString m_name;

    TSharedConstLogEntryParserModelConfiguration m_configuration;

    int m_fieldId;
};

typedef boost::shared_ptr<ValueGetterLogEntry> TSharedValueGetterLogEntry;
typedef boost::shared_ptr<const ValueGetterLogEntry> TconstSharedValueGetterLogEntry;

#endif /* VALUEGETTERLOGENTRY_H_ */
