/*
 * Types.h
 *
 *  Created on: May 15, 2011
 *      Author: sven
 */

#ifndef TYPES_H_
#define TYPES_H_
#include <boost/shared_ptr.hpp>

#include <QMetaType>

class QString;

typedef boost::shared_ptr<QString> TSharedQString;
typedef boost::shared_ptr<const QString> TSharedConstQString;

Q_DECLARE_METATYPE ( TSharedConstQString );

#endif /* TYPES_H_ */
