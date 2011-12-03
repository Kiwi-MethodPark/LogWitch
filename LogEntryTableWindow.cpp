/*
 * LogEntryTableWindow.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableWindow.h"

#include "Models/LogEntryTableModel.h"
#include "Models/LogEntryTableFilter.h"
#include "ActionRules/ValueGetterConstQString.h"
#include "ActionRules/RuleTable.h"
#include "ActionRules/ActionDataRewriter.h"
#include "ActionRules/ExpressionMatch.h"
#include "ActionRules/ValueGetterLogEntry.h"
#include "GUITools/QScrollDownTableView.h"
#include "ActionRules/ExpressionParser.h"
#include "ActionRules/ActionParser.h"
#include "ActionRules/ExpressionFind.h"
#include "ActionRules/ExpressionRegEx.h"

LogEntryTableWindow::LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent )
	:QWidget(parent)
	 ,m_model( model )
     ,m_tableView( new QScrollDownTableView(  ) )
    , m_searchMode( Text )
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);

    m_proxyModel = new LogEntryTableFilter(m_tableView);
    m_proxyModel->setSourceModel(m_model.get());

    m_tableView->setModel( m_proxyModel );
    m_tableView->horizontalHeader()->moveSection(1, model->columnCount( QModelIndex() )-1 );
    m_tableView->horizontalHeader()->setMovable( true );
    m_tableView->verticalHeader()->setDefaultSectionSize( 20 );
    m_tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->hide();
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode( QAbstractItemView::SingleSelection );

	// Qt::Horizontal
	int count = m_model->columnCount( QModelIndex() );
	for( int col = 0; col < count; col ++)
	{
		int width = m_proxyModel->headerData( col, Qt::Horizontal, 512 ).value<int>();
		m_tableView->horizontalHeader()->resizeSection( col, width );
	}

	m_quickSearch = new QLineEdit;
	QWidget *quickSearchBar = new QWidget( this );
	QHBoxLayout* quickSearchLayout = new QHBoxLayout;
	quickSearchLayout->setContentsMargins( 0, 0, 0, 0 );
	quickSearchBar->setLayout( quickSearchLayout );

    QIcon iconDown, iconUp, search;
    search.addFile(QString::fromUtf8(":/icons/search"), QSize(), QIcon::Normal, QIcon::Off);
    iconDown.addFile(QString::fromUtf8(":/icons/searchDown"), QSize(), QIcon::Normal, QIcon::Off);
    iconUp.addFile(QString::fromUtf8(":/icons/searchUp"), QSize(), QIcon::Normal, QIcon::Off);

    m_searchModeButton = new QPushButton( tr("Text") );
    QObject::connect(m_searchModeButton, SIGNAL(clicked()),
                     this, SLOT(switchSearchMode()));

    m_markButton = new QPushButton( tr("mark") );
    m_markButton->setCheckable( true );
    m_markButton->setChecked( true );

    QObject::connect(m_markButton, SIGNAL(toggled(bool)),
                     this, SLOT(updateSearch()));

    QObject::connect(m_quickSearch, SIGNAL(editingFinished()),
                     this, SLOT(updateSearch()));


    quickSearchLayout->addWidget( m_searchModeButton );
	quickSearchLayout->addWidget( m_quickSearch );
    quickSearchLayout->addWidget( m_markButton );
	quickSearchLayout->addWidget( new QPushButton(iconDown, "") );
	quickSearchLayout->addWidget( new QPushButton(iconUp, "") );

	//m_quickSearch->setMaximumSize(QWIDGETSIZE_MAX,20);
    layout->addWidget( quickSearchBar );
    //layout->addWidget( m_quickSearch );
	layout->addWidget(m_tableView);

	// Initialize the rule
	ActionParser parser( model->getParserModelConfiguration() );
	if( !parser.parse("rewrite(BG:#81BEF7)") )
	{
	    qDebug() << "Parsing of action failed!";
	}
	else
	{
	    m_quickSearchAction = parser.get();
	}
}

void LogEntryTableWindow::switchSearchMode()
{
    switch( m_searchMode )
    {
    case Text:
        m_searchMode = Regex;
        break;
    case Regex:
        m_searchMode = Expression;
        break;
    default:
    case Expression:
        m_searchMode = Text;
        break;
    }

    switch( m_searchMode )
    {
    default:
    case Text:
        m_searchModeButton->setText( tr("Text") );
        break;
    case Regex:
        m_searchModeButton->setText( tr("Regex") );
        break;
    case Expression:
        m_searchModeButton->setText( tr("Expr") );
        break;
    }

    updateSearch();
}

void LogEntryTableWindow::updateSearch()
{
    m_proxyModel->getRuleTable()->clear("quicksearch");
    m_quickSearch->setStyleSheet("");
    m_quickSearch->setToolTip("");
    m_quickSearchExp.reset();

    // Do not do any filtering or searching if no text was given.
    if( m_quickSearch->text().length() == 0 )
        return;

    if( m_searchMode == Expression )
    {
        ExpressionParser parser( m_model->getParserModelConfiguration() );

        if( parser.parse( m_quickSearch->text() ) )
        {
            m_quickSearchExp = parser.get();
        }
        else
        {
            m_quickSearch->setStyleSheet("background-color:red;");
            m_quickSearch->setToolTip( tr("Error while parsing: ")+parser.getError() );
            qDebug() << "Unable to parse expression: " << m_quickSearch->text();
        }
    }
    else if( m_searchMode == Text )
    {
        TSharedValueGetterLogEntry log( new ValueGetterLogEntry( "message", m_model->getParserModelConfiguration()) );
        m_quickSearchExp.reset( new ExpressionFind( log, m_quickSearch->text() ) );

    }
    else if( m_searchMode == Regex )
    {
        TSharedValueGetterLogEntry log( new ValueGetterLogEntry( "message", m_model->getParserModelConfiguration()) );
        m_quickSearchExp.reset( new ExpressionRegEx( log, m_quickSearch->text() ) );
    }

    if(    m_markButton->isChecked()
        && m_quickSearchExp )
    {
        TSharedRule rule( new Rule( m_quickSearchExp, m_quickSearchAction ) );
        m_proxyModel->getRuleTable()->addRule( "quicksearch", rule );
    }
}

TSharedRuleTable LogEntryTableWindow::getRuleTable()
{
    return m_proxyModel->getRuleTable();
}

void LogEntryTableWindow::addFilter( boost::shared_ptr<LogEntryFilter> flt )
{
	m_proxyModel->addFilter( flt );
}

QModelIndex LogEntryTableWindow::mapToSource ( const QModelIndex & proxyIndex ) const
{
	return m_proxyModel->mapToSource ( proxyIndex );
}

QTableView *LogEntryTableWindow::tableView()
{
    return m_tableView;
}

LogEntryTableWindow::~LogEntryTableWindow()
{

}


