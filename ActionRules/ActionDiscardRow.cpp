/*
 * ActionRemoveRow.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ActionDiscardRow.h"

ActionDiscardRow::ActionDiscardRow()
{
}

ActionDiscardRow::~ActionDiscardRow()
{
}

QVariant ActionDiscardRow::toDisplay( int role ) const
{
    if( role == Qt::DisplayRole )
    {
        return QString( QObject::tr("Discard Logmessage") );
    }

    return QVariant();
}
