/*
 * FilterRuleSelectionWindow.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "FilterRuleSelectionWindow.h"
#include <QtGui>
#include "ActionRules/TableModelRules.h"
#include "ActionRules/TableModelRulesCompiled.h"
#include "ActionRules/CompiledRulesStateSaver.h"
#include "Models/LogEntryTableFilter.h"

FilterRuleSelectionWindow::FilterRuleSelectionWindow( QWidget* parent )
: QSplitter( Qt::Vertical, parent )
, m_compiledRules( )
{
    m_ruleView = new QTableView(  );
    m_ruleView->verticalHeader()->setDefaultSectionSize( 20 );
    m_ruleView->horizontalHeader()->setDefaultSectionSize( 190 );
    m_ruleView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    m_rulesModel = new TableModelRules( m_ruleView );
    m_ruleView->setModel( m_rulesModel );
    m_ruleView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ruleView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    addWidget( m_ruleView );
    addWidget( new QLabel ( tr("Context missing - open or select a Logger window to add rules."), this ) );
    //addWidget( generateNewParserWindow() );
    QList<int> he; he << 300 << 500;
    setSizes ( he );
    setStretchFactor ( 0, 10 );
    setStretchFactor ( 1, 10 );
}

void FilterRuleSelectionWindow::setWindow( TSharedCompiledRulesStateSaver state )
{
    if( !state )
    {
        m_compiledRules.reset();

        for( int i = 2; i < count(); i++ )
            widget( i )->hide();
        widget( 1 )->show();
    }
    else
    {
        state->connectActions( this );
        m_compiledRules = state;

        for( int i = 1; i < count(); i++ )
        {
            if( widget(i) != state->m_displayWidget)
                widget( i )->hide();
            else
                widget( i )->show();
        }

        addWidget( state->m_displayWidget );
    }
}

TSharedCompiledRulesStateSaver FilterRuleSelectionWindow::getWindow( )
{
    return m_compiledRules;
}

void FilterRuleSelectionWindow::addSelectionToCompiled()
{
    if( m_compiledRules )
    {
        QItemSelectionModel *selMod = m_ruleView->selectionModel();
        QModelIndexList selection = selMod->selectedRows();
        QModelIndexList::Iterator it;
        for( it = selection.begin(); it != selection.end(); ++it )
        {
            TSharedFilterRuleRaw rule = m_rulesModel->getRaw( *it );
            m_compiledRules->m_rulesCompiledModel->appendRule( rule );
        }
    }
}

FilterRuleSelectionWindow::~FilterRuleSelectionWindow()
{
}
