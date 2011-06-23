/*
 * RuleTable.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "RuleTable.h"

RuleTable::RuleTable()
:m_onChange ( false )
{
}

RuleTable::~RuleTable()
{
}

void RuleTable::addRule( TSharedRule &rule )
{
    qDebug() << "RuleTable::addRule";
    m_rules.push_back( rule );
    dataChanged();
}

void RuleTable::clear()
{
    m_rules.clear();
    dataChanged();
}

void RuleTable::beginChange()
{
    m_onChange = true;
}

void RuleTable::endChange()
{
    m_onChange = false;
    emit changed();
}

void RuleTable::dataChanged()
{
    if( !m_onChange )
        emit changed();
}
