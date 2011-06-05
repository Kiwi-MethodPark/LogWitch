/*
 * RuleTable.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "RuleTable.h"

RuleTable::RuleTable()
{
}

RuleTable::~RuleTable()
{
}

void RuleTable::addRule( TSharedRule &rule )
{
    m_rules.push_back( rule );
}
