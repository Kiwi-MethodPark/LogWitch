/*
 * FilterListView.h
 *
 *  Created on: Jun 4, 2011
 *      Author: sven
 */

#ifndef FILTERLISTVIEW_H_
#define FILTERLISTVIEW_H_
#include <boost/shared_ptr.hpp>
#include <QtGui>

class LogEntryCombinedWidget;
class LogEntryParserModelConfiguration;
class StringCacheTreeModel;

class FilterListView
    : public QTreeView
{
        Q_OBJECT
public:
    FilterListView( QObject *parent, boost::shared_ptr<const LogEntryParserModelConfiguration> cfg, const int attr );
    ~FilterListView();

    void addToTabs( QTabWidget *tabs, LogEntryCombinedWidget* widget  );

private:
    boost::shared_ptr<const LogEntryParserModelConfiguration> m_config;
    StringCacheTreeModel *m_strModel;
    int m_attr;
};

#endif /* FILTERLISTVIEW_H_ */
