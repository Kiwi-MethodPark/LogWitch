/*
 * CompiledRulesStateSaver.cpp
 *
 *  Created on: Jun 12, 2011
 *      Author: sven
 */

#include "ActionRules/CompiledRulesStateSaver.h"

#include <boost/bind.hpp>

#include "ActionRules/FilterRuleSelectionWindow.h"
#include "ActionRules/TableModelRulesCompiled.h"
#include "ActionRules/ToolButtonTrashFilter.h"

#include "GUITools/EventFilterToBoostFunction.h"

CompiledRulesStateSaver::CompiledRulesStateSaver(  TSharedConstLogEntryParserModelConfiguration cfg, TSharedRuleTable ruleTable )
    : m_compiledRuleView(NULL)
    , m_connected( false)
{
   m_displayWidget = new QWidget(); //This is the pane

   QVBoxLayout* vbox = new QVBoxLayout(m_displayWidget);
   m_displayWidget->setLayout(vbox);

   QToolBar* toolBar = new QToolBar( m_compiledRuleView);
   toolBar->setIconSize(QSize(16,16));
   vbox->addWidget(toolBar);

   // Construct view and set viewing stuff
   m_compiledRuleView = new QTableView(m_displayWidget );
   m_compiledRuleView->verticalHeader()->setDefaultSectionSize( 20 );
   m_compiledRuleView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
   m_compiledRuleView->horizontalHeader()->setDefaultSectionSize( 190 );
   m_compiledRuleView->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_compiledRuleView->setSelectionMode( QAbstractItemView::ExtendedSelection );

   m_compiledRuleView->setDragEnabled( true );
   m_compiledRuleView->setDropIndicatorShown(true);
   m_compiledRuleView->setDragDropMode( QAbstractItemView::DragDrop );

   // Set Model
   m_rulesCompiledModel = new TableModelRulesCompiled( m_compiledRuleView, cfg, ruleTable );
   m_compiledRuleView->setModel( m_rulesCompiledModel );

   vbox->addWidget(m_compiledRuleView);

   m_removeSelectedRules = new QAction("Trash", this);
   m_removeSelectedRules->setIcon(QIcon(":/icons/trash"));
   toolBar->addWidget( new ToolButtonTrashFilter( m_removeSelectedRules, m_rulesCompiledModel ) );
}

void CompiledRulesStateSaver::connectActions( FilterRuleSelectionWindow *wnd )
{
    if( m_connected )
        return;

    QObject::connect(m_removeSelectedRules, SIGNAL(triggered()),
            wnd, SLOT(removeSelectionFromCompiled()));

    m_compiledRuleView->installEventFilter( new EventFilterToBoostFunction( this,
            boost::bind( &evtFunc::keyPressed, _1, _2, Qt::Key_Delete,
                    boost::function< void(void ) >( boost::bind( &FilterRuleSelectionWindow::removeSelectionFromCompiled, wnd ) ) ) ) );


    m_connected = true;
}

CompiledRulesStateSaver::~CompiledRulesStateSaver()
{
    delete m_displayWidget;
}
