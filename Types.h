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
class QVariant;

typedef boost::shared_ptr<QString> TSharedQString;
typedef boost::shared_ptr<const QString> TSharedConstQString;

typedef boost::shared_ptr<QVariant> TSharedQVariant;
typedef boost::shared_ptr<const QVariant> TSharedConstQVariant;



Q_DECLARE_METATYPE ( TSharedConstQString );

#endif /* TYPES_H_ */
