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

FilterRuleSelectionWindow::FilterRuleSelectionWindow( QWidget* parent )
: QSplitter( Qt::Vertical, parent )
, m_compiledRules( new CompiledRulesStateSaver )
{
    m_ruleView = new QTableView(  );
    m_ruleView->verticalHeader()->setDefaultSectionSize( 20 );
    m_ruleView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    m_rulesModel = new TableModelRules( m_ruleView );
    m_ruleView->setModel( m_rulesModel );
    m_ruleView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ruleView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    addWidget( m_ruleView );

    QToolBar* toolBar = new QToolBar( this);
    QAction *addSelectedRules = toolBar->addAction("add");

    QObject::connect(addSelectedRules, SIGNAL(triggered()),
                     this, SLOT(addSelectionToCompiled()));

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(toolBar);
    vbox->addWidget(m_compiledRules->m_compiledRuleView);
    QWidget* widget = new QWidget(this); //This is the pane
    widget->setLayout(vbox);

    addWidget( widget );
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
