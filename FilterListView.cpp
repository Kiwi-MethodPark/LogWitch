/*
 * FilterListView.cpp
 *
 *  Created on: Jun 4, 2011
 *      Author: sven
 */
#include "FilterListView.h"
#include "LogEntryCombinedWidget.h"
#include "Models/StringCacheTreeModel.h"
#include "LogEntryParserModelConfiguration.h"
#include "LogData/LogEntryAttributeFactory.h"

FilterListView::FilterListView( QObject *parent, boost::shared_ptr<const LogEntryParserModelConfiguration> config, const int attr )
: QTreeView( )
, m_config( config )
, m_attr( attr )
{
    m_strModel = new StringCacheTreeModel( parent
            , &m_config->getLogEntryAttributeFactory()->getCache(attr)
            , attr
            , m_config->getHierarchySplitString(attr) );

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(m_strModel);
    proxyModel->setSourceModel(m_strModel);
    proxyModel->sort ( 0 );
    proxyModel->setSortCaseSensitivity ( Qt::CaseInsensitive );
    proxyModel->setDynamicSortFilter ( true );

    setHeaderHidden ( true );
    setModel(proxyModel);

}

void FilterListView::addToTabs( QTabWidget *tabs, LogEntryCombinedWidget* widget  )
{
    if( m_strModel->getFilter() )
        widget->addFilter( m_strModel->getFilter() );

    tabs->addTab( this, m_config->getLogEntryAttributeFactory()->getDescription(m_attr) );
    show();
}

FilterListView::~FilterListView()
{
    // TODO Auto-generated destructor stub
}
