/*
 * CompiledRulesStateSaver.cpp
 *
 *  Created on: Jun 12, 2011
 *      Author: sven
 */

#include "CompiledRulesStateSaver.h"
#include "ActionRules/TableModelRulesCompiled.h"

CompiledRulesStateSaver::CompiledRulesStateSaver()
{
   m_compiledRuleView = new QTableView( );
   m_compiledRuleView->verticalHeader()->setDefaultSectionSize( 20 );
   m_compiledRuleView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
   m_rulesCompiledModel = new TableModelRulesCompiled( m_compiledRuleView );
   m_compiledRuleView->setModel( m_rulesCompiledModel );
}

CompiledRulesStateSaver::~CompiledRulesStateSaver()
{
    delete m_compiledRuleView;
}
