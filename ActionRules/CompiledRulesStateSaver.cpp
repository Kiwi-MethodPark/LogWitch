/*
 * CompiledRulesStateSaver.cpp
 *
 *  Created on: Jun 12, 2011
 *      Author: sven
 */

#include "CompiledRulesStateSaver.h"
#include "ActionRules/TableModelRulesCompiled.h"
#include "ActionRules/FilterRuleSelectionWindow.h"

CompiledRulesStateSaver::CompiledRulesStateSaver(  TSharedConstLogEntryParserModelConfiguration cfg, TSharedRuleTable ruleTable )
    : m_compiledRuleView(NULL)
    , m_connected( false)
{
   m_displayWidget = new QWidget(); //This is the pane

   QVBoxLayout* vbox = new QVBoxLayout(m_displayWidget);
   m_displayWidget->setLayout(vbox);

   QToolBar* toolBar = new QToolBar( m_compiledRuleView);
   m_addSelectedRules = toolBar->addAction("add");
   m_addSelectedRules->setIcon(QIcon(":/icons/plus"));
   m_removeSelectedRules = toolBar->addAction("remove");
   m_removeSelectedRules->setIcon(QIcon(":/icons/minus"));
   toolBar->setIconSize(QSize(16,16));
   vbox->addWidget(toolBar);

   // Construct view and set viewing stuff
   m_compiledRuleView = new QTableView(m_displayWidget );
   m_compiledRuleView->verticalHeader()->setDefaultSectionSize( 20 );
   m_compiledRuleView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
   m_compiledRuleView->horizontalHeader()->setDefaultSectionSize( 190 );
   m_compiledRuleView->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_compiledRuleView->setSelectionMode( QAbstractItemView::ExtendedSelection );

   // Set Model
   m_rulesCompiledModel = new TableModelRulesCompiled( m_compiledRuleView, cfg, ruleTable );
   m_compiledRuleView->setModel( m_rulesCompiledModel );

   vbox->addWidget(m_compiledRuleView);
}

void CompiledRulesStateSaver::connectActions( FilterRuleSelectionWindow *wnd )
{
    if( m_connected )
        return;

    QObject::connect(m_addSelectedRules, SIGNAL(triggered()),
            wnd, SLOT(addSelectionToCompiled()));
    QObject::connect(m_removeSelectedRules, SIGNAL(triggered()),
            wnd, SLOT(removeSelectionFromCompiled()));

    m_connected = true;
}

CompiledRulesStateSaver::~CompiledRulesStateSaver()
{
    delete m_displayWidget;
}
