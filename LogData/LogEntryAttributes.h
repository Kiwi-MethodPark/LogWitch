/*
 * LogEntryAttributes.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYATTRIBUTES_H_
#define LOGENTRYATTRIBUTES_H_
#include <boost/shared_ptr.hpp>
#include <vector>
#include "Types.h"

class LogEntryAttributeFactory;
class QString;

class LogEntryAttributes {
public:
	virtual ~LogEntryAttributes();

	LogEntryAttributes( LogEntryAttributeFactory *factory, const std::vector< TSharedConstQString > &defAttributes );

	void setAttribute( TSharedConstQString, int idx );

	TSharedConstQString getAttribute( int idx ) const;

private:
	std::vector< TSharedConstQString > attributes;

	LogEntryAttributeFactory *myFactory;
};

#endif /* LOGENTRYATTRIBUTES_H_ */
