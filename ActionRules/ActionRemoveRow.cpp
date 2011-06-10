/*
 * ActionRemoveRow.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ActionRemoveRow.h"

ActionRemoveRow::ActionRemoveRow()
{
}

ActionRemoveRow::~ActionRemoveRow()
{
}

QVariant ActionRemoveRow::toDisplay( int role ) const
{
    if( role == Qt::DisplayRole )
    {
        return QString( QObject::tr("'Remove Line") );
    }

    return QVariant();
}
