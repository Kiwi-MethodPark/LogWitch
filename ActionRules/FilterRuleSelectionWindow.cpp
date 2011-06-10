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
{
    QTableView *rulesView = new QTableView(  );
    TableModelRules *rulesModel = new TableModelRules( rulesView );
    rulesView->setModel( rulesModel );
    rulesView->setSelectionBehavior(QAbstractItemView::SelectRows);
    rulesView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    addWidget( rulesView );

    rulesView = new QTableView(  );
    TableModelRulesCompiled *rulesCompiledModel = new TableModelRulesCompiled( rulesView );
    rulesView->setModel( rulesCompiledModel );

    QToolBar* toolBar = new QToolBar( this);
    toolBar->addAction("add");
    toolBar->addAction("load Set");

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(toolBar);
    vbox->addWidget(rulesView);
    QWidget* widget = new QWidget(this); //This is the pane
    widget->setLayout(vbox);

    addWidget( widget );
}

FilterRuleSelectionWindow::~FilterRuleSelectionWindow()
{
}
