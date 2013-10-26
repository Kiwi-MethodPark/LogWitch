/*
 * LogEntryTableWindow.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableWindow.h"

#include <limits>

#include "ActionRules/ActionDataRewriter.h"
#include "ActionRules/ActionParser.h"
#include "ActionRules/ExpressionFind.h"
#include "ActionRules/ExpressionMatch.h"
#include "ActionRules/ExpressionParser.h"
#include "ActionRules/ExpressionRegEx.h"
#include "ActionRules/RuleTable.h"
#include "ActionRules/ValueGetterConstQString.h"
#include "ActionRules/ValueGetterLogEntry.h"

#include "GUITools/QScrollDownTableView.h"

#include "LogData/LogEntryParserModelConfiguration.h"
#include "LogData/LogEntryFactory.h"
#include "LogData/ObjectCache.hxx"

#include "Models/LogEntryTableModel.h"
#include "Models/LogEntryTableModelFileExporter.h"
#include "Models/LogEntryTableFilter.h"
#include "Models/EntryFormatingModel.h"

#include "EntryToTextFormater.h"
#include "ContextMenuManipulateHeader.h"
#include "FilterListView.h"

void LogEntryTableWindow::updateHeaderSizeToModel( int column, int, int newSize )
{
    if( newSize == 0 )
    {
        // This happens if hide is called.
        m_model->setHeaderData( column, Qt::Horizontal, false, 513 );
    }
    else
    {
        m_model->setHeaderData( column, Qt::Horizontal, true, 513 );
        m_model->setHeaderData( column, Qt::Horizontal, newSize, 512 );
    }
}

void LogEntryTableWindow::updateHeaderPositionToModel( int, int, int  )
{
    QHeaderView *view = m_tableView->horizontalHeader();
    for( int col = 0; col < view->count(); col++ )
        m_model->setHeaderData( view->logicalIndex(col), Qt::Horizontal, col, 514 );
}

LogEntryTableWindow::LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent )
	:QMdiSubWindow(parent)
	, m_model( model )
    , m_splitter( new QSplitter(Qt::Vertical ) )
    , m_myFilterTabs( NULL )
    , m_dockFilterShouldDockedTo( NULL )
    , m_tableView( new QScrollDownTableView(  ) )
    , m_searchMode( Text )
{
    // Parametrize the filtering model, connect it to the table view.
    m_proxyModel = new LogEntryTableFilter(m_tableView);
    m_proxyModel->setSourceModel(m_model.get());

    // Parametrize the table view for the log entries.
    m_timeFormatModel = new EntryFormatingModel( m_tableView );
    m_timeFormatModel->setSourceModel( m_proxyModel );

    m_tableView->setModel( m_timeFormatModel );
    m_tableView->verticalHeader()->setDefaultSectionSize( 20 );
    m_tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->hide();
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode( QAbstractItemView::SingleSelection );

    m_tableView->setContextMenuPolicy( Qt::CustomContextMenu );
    QObject::connect(m_tableView, SIGNAL(customContextMenuRequested ( const QPoint &) ),
                     this, SLOT(contextMenu(const QPoint & )));

    QObject::connect(m_tableView, SIGNAL(doubleClicked ( const QModelIndex &) ),
                     this, SLOT(onDoubleClick(const QModelIndex & )));

    // Context menu for the HorizontalHeaderView
    m_tableView->horizontalHeader()->setMovable( true );
    m_tableView->horizontalHeader()->setContextMenuPolicy( Qt::CustomContextMenu );
    new ContextMenuManipulateHeader( m_tableView->horizontalHeader() );

	// Condition the view from the model.
    {
        int count = m_model->columnCount( QModelIndex() );
        for( int col = 0; col < count; col ++)
        {
            int width = m_timeFormatModel->headerData( col, Qt::Horizontal, 512 ).value<int>();
            bool show = m_timeFormatModel->headerData( col, Qt::Horizontal, 513 ).value<bool>();

            m_tableView->horizontalHeader()->resizeSection( col, width );
            m_tableView->horizontalHeader()->setSectionHidden( col, !show );
        }

        QHeaderView *view = m_tableView->horizontalHeader();

        // This moves the header sections around to fit the needs to the order.
        int visIndex = 0;
        std::set<int> doneColumns;
        for( visIndex = 0; visIndex < view->count(); ++visIndex )
        {
            // First find the column we want to display. This is the column with the lowest number.
            int modelColumn = -1;
            int lowestPrio = std::numeric_limits<int>::max();
            // Find the first element, which has the lowest priority and is not
            // within our doneColumns set. This is then the next column to display.
            for( int i = view->count() - 1; i >= 0; i-- )
            {
                if( doneColumns.end() == doneColumns.find( i ) )
                {
                    int prio = m_timeFormatModel->headerData( i, Qt::Horizontal, 514 ).value<int>();
                    if( prio <= lowestPrio )
                    {
                        lowestPrio = prio;
                        modelColumn = i;
                    }
                }
            }

            doneColumns.insert( modelColumn );
            int visIndexToFind;

            // Find the column in the current header here.
            for( visIndexToFind = 0;
                 visIndexToFind < view->count() && modelColumn != view->logicalIndex( visIndexToFind );
                 ++visIndexToFind )
                ;

            if( visIndexToFind < view->count() )
                view->moveSection( visIndexToFind, visIndex );
        }
    }
    // Connect resize signals of the horizontal header to the model update.
    QObject::connect( m_tableView->horizontalHeader(),SIGNAL(sectionResized(int , int , int )),
            this, SLOT(updateHeaderSizeToModel(int,int,int)));
    QObject::connect( m_tableView->horizontalHeader(),SIGNAL(sectionMoved(int , int , int )),
            this, SLOT(updateHeaderPositionToModel(int,int,int)));

	// Create quicksearch bar
	QWidget *quickSearchBar = new QWidget;
	m_quickSearch = new QLineEdit;
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

    m_markButton = new QPushButton( tr("Highlight") );
    m_markButton->setCheckable( true );
    m_markButton->setChecked( true );

    QObject::connect(m_markButton, SIGNAL(toggled(bool)),
                     this, SLOT(updateSearch()));

    QObject::connect(m_quickSearch, SIGNAL(editingFinished()),
                     this, SLOT(updateSearch()));

    QPushButton *searchDnBtn = new QPushButton(iconDown, "");
    QObject::connect(searchDnBtn, SIGNAL(clicked()),
                     this, SLOT(searchNext()) );

    QPushButton *searchUpBtn = new QPushButton(iconUp, "");
    QObject::connect(searchUpBtn, SIGNAL(clicked()),
                     this, SLOT(searchPrev()) );

    quickSearchLayout->addWidget( m_searchModeButton );
	quickSearchLayout->addWidget( m_quickSearch );
    quickSearchLayout->addWidget( m_markButton );
	quickSearchLayout->addWidget( searchDnBtn );
	quickSearchLayout->addWidget( searchUpBtn );

	// Initialize the action for highlighting log entries.
	ActionParser parser( model->getParserModelConfiguration() );
	if( !parser.parse("rewrite(BG:#81BEF7)") )
	{
	    qDebug() << "Parsing of action failed!";
	}
	else
	{
	    m_quickSearchAction = parser.get();
	}

    m_text = new QTextEdit("<b>Log Message viewer</b>",this);
    QObject::connect(m_tableView->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection & , const QItemSelection & )),
                     SLOT(newSelection ( const QItemSelection &, const QItemSelection & )));
    QObject::connect(dynamic_cast<QObject*>(m_model.get()), SIGNAL(signalError( QString )),
                     this, SLOT(errorFromModel( QString )) );

    // Make the big layout, top is quickearch bar, then log table and at the bottom the log message.
    QVBoxLayout* layout = new QVBoxLayout();
    QWidget *centralWidget = new QWidget( this );

    m_splitter->addWidget( m_tableView );
    m_splitter->addWidget( m_text );
    m_splitter->setStretchFactor( 0, 20 );

    layout->addWidget( quickSearchBar );
	layout->addWidget( m_splitter );

	centralWidget->setLayout( layout );
	this->setWidget( centralWidget );

    resize( 800, 500 );

    // Just generate the tab here, because we want to catch all filter entries.
    getTabFilterWidget();
    QMdiSubWindow::setAttribute(Qt::WA_DeleteOnClose,true);
}

void LogEntryTableWindow::exportLogfile( const QString &filename )
{
    LogEntryTableModelFileExporter exporter( *m_model );
    exporter.exportTo( filename );
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


void LogEntryTableWindow::search( bool backwards )
{
    if( m_timeFormatModel->rowCount() > 0 && m_quickSearchExp )
    {
        int startRow = -1;

        if( m_tableView->selectionModel()->hasSelection() )
        {
            QItemSelection selection = m_tableView->selectionModel()->selection();
            // There seems to be a QT Bug, if the ProxyFilterModel is removing the selection
            // hasSelection retruns true while no selection is here!
            if( !selection.isEmpty() )
            {
                QModelIndex index = selection.front().topLeft();
                startRow = index.row();
            }
        }

        const int inc = backwards ? -1 : 1;
        int nextRow = startRow + inc;

        if( nextRow >= m_timeFormatModel->rowCount() )
            nextRow = 0;
        if( nextRow < 0 )
            nextRow =  m_timeFormatModel->rowCount() - 1;

        for( ; nextRow != startRow;  )
        {
            TconstSharedLogEntry entry = m_model->getEntryByIndex( mapToSource( m_timeFormatModel->index( nextRow, 0 ) ) );
            if( entry && m_quickSearchExp->match(entry ) )
                break;

            // We need to correct startRow, if
            // - we have no selection on entry
            // - the table size may change during search
            if( startRow < 0 )
              startRow = nextRow;
            if( startRow >= m_timeFormatModel->rowCount() )
              startRow = m_timeFormatModel->rowCount() - 1;

            // Incrementing part
            nextRow += inc;

            if( nextRow >= m_timeFormatModel->rowCount() )
                nextRow = 0;
            if( nextRow < 0 )
                nextRow =  m_timeFormatModel->rowCount() - 1;
        }

        if( nextRow != startRow )
        {
            m_tableView->setCurrentIndex( m_timeFormatModel->index( nextRow, 0 ) );
            m_tableView->setFocus();
        }
    }
}
void LogEntryTableWindow::searchNext()
{
    search( false );
}

void LogEntryTableWindow::searchPrev()
{
    search( true );
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
	return m_proxyModel->mapToSource ( m_timeFormatModel->mapToSource( proxyIndex ) );
}

LogEntryTableWindow::~LogEntryTableWindow()
{
    qDebug() << " Window deleted.";
}

void LogEntryTableWindow::errorFromModel( QString error )
{
    QMessageBox msgBox;
    QString errorText;
    errorText+= tr("Error received: \n") + error;
    msgBox.setText( errorText );
    msgBox.setInformativeText(tr("Close window with the logs now? This will discard all already captured messages. If you want to keep them, press ignore."));
    msgBox.setStandardButtons(QMessageBox::Ignore | QMessageBox::Close );
    msgBox.setDefaultButton(QMessageBox::Ignore);

    int ret =  msgBox.exec();
    if( ret == QMessageBox::Close )
    {
        this->deleteLater();
    }
}

void LogEntryTableWindow::setDockForFilter( QDockWidget *dock )
{
    dock->setWidget( getTabFilterWidget( ) );
}

void LogEntryTableWindow::clearTable( )
{
    m_model->clearTable();
}

void LogEntryTableWindow::capture( bool active )
{
    m_model->capture( active );
}

QTabWidget *LogEntryTableWindow::getTabFilterWidget()
{
    if ( m_myFilterTabs == NULL )
    {
        QTabWidget *tabs = new QTabWidget( );

        int attributes = m_model->getParserModelConfiguration()->getLogEntryFactory()->getNumberOfFields();
        for(int attr = 0; attr < attributes; attr++ )
        {
            // Only show tabs with an active StringCahche.
            if( m_model->getParserModelConfiguration()->getLogEntryFactory()->getCache(attr).isCaching() )
            {
                FilterListView *view = new FilterListView( this, m_model->getParserModelConfiguration(), attr );
                view->addToTabs( tabs, this );
            }
        }
        m_myFilterTabs = tabs;
    }

    return m_myFilterTabs;
}

TSharedCompiledRulesStateSaver LogEntryTableWindow::getCompiledRules()
{
    return TSharedCompiledRulesStateSaver( new CompiledRulesStateSaver( m_model->getParserModelConfiguration(), getRuleTable() ) );
}

void LogEntryTableWindow::newSelection ( const QItemSelection & selected, const QItemSelection & )
{
    TconstSharedLogEntry entry = m_model->getEntryByIndex( mapToSource( selected.front().topLeft() ) );
    m_text->setHtml( m_model->getParserModelConfiguration()->getEntryToTextFormater()->formatEntry( entry ) );
}

void LogEntryTableWindow::contextMenu( const QPoint & pt )
{
    // Query the reference timestamp.
    QDateTime dt;
    bool found = false;

    QModelIndex idxClick = mapToSource( m_tableView->indexAt( pt ) );
    QModelIndex idxClickProxyModel = m_timeFormatModel->mapToSource( m_tableView->indexAt( pt ) );

    for( int i = 0; i < m_model->columnCount(); ++i )
    {
        QModelIndex idx = m_model->index( idxClick.row(), i );
        QVariant var = m_model->data( idx, Qt::DisplayRole );
        qDebug() << " i " << i << " var " << var;
        if( var.type() == QVariant::DateTime )
        {
            dt = var.toDateTime();
            found = true;
            break;
        }
    }

    if( !found )
        return;

    qDebug() << "Using reference time: " << dt;

    QMenu *menu = new QMenu();

    QAction *timeStampsRelative = menu->addAction( tr("Set row as reference") );
    QAction *showTimestamps = menu->addAction( tr("Show absolute times") );
    QAction *showDiffTimes = menu->addAction( tr("Show difference times") );
    QAction *peekFilteredEntries = menu->addAction( tr("Peek filtered") );

    m_model->beginBlockItems();
    QAction *pressed = menu->exec( m_tableView->mapToGlobal(pt) );

    if( pressed == timeStampsRelative )
    {
        m_timeFormatModel->setTimeDiffReference( dt );
        m_timeFormatModel->setTimeDiffModeEnabled( true, false );
    }
    else if( pressed == showTimestamps )
    {
        m_timeFormatModel->setTimeDiffModeEnabled( false, false );
    }
    else if( pressed == showDiffTimes )
    {
        m_timeFormatModel->setTimeDiffModeEnabled( true, true );
    }
    else if( pressed == peekFilteredEntries )
    {
        m_proxyModel->showSurroundingLogEntries( idxClickProxyModel, 10 );
    }

    m_model->endBlockItems();
    delete menu;
}

void LogEntryTableWindow::onDoubleClick ( const QModelIndex & index )
{
    QModelIndex idxClickProxyModel = m_timeFormatModel->mapToSource( index );
    m_proxyModel->showSurroundingLogEntries( idxClickProxyModel, 10 );
}
