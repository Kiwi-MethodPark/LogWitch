/*
 * ActionColorRow.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ActionColorRow.h"

ActionColorRow::ActionColorRow(const QColor &col)
    : m_color( col )
{
}

ActionColorRow::~ActionColorRow()
{
}

const QColor &ActionColorRow::getColor() const
{
    return m_color;
}
