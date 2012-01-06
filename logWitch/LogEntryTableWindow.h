/*
 * LogEntryTableWindow.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYTABLEWINDOW_H_
#define LOGENTRYTABLEWINDOW_H_
#include <QtGui>
#include <boost/shared_ptr.hpp>
#include "ActionRules/RuleTable.h"
#include "ActionRules/CompiledRulesStateSaver.h"

class LogEntryTableModel;
class QSortFilterProxyModel;
class LogEntryFilter;
class LogEntryTableFilter;
class QScrollDownTableView;
class EntryFormatingModel;

class LogEntryTableWindow
	: public QMdiSubWindow
{
	Q_OBJECT
public:
	LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent = NULL );
	virtual ~LogEntryTableWindow();

	QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;

	void addFilter( boost::shared_ptr<LogEntryFilter> flt );

	TSharedRuleTable getRuleTable();

    QTabWidget *getTabFilterWidget();

    TSharedCompiledRulesStateSaver getCompiledRules();

    void setDockForFilter( QDockWidget *dock );

public slots:
    /**
     * Switches between the different modes for the quicksearch
     * text line.
     */
    void switchSearchMode();

    /**
     * Updates the view from the quicksearch line
     */
    void updateSearch();

    /**
     * Search the next log entry which will match to out quicksearch expression.
     */
    void searchNext();

    /**
     * Search the previous log entry which will match to out quicksearch expression.
     */
    void searchPrev();

    void newSelection ( const QItemSelection & selected, const QItemSelection & deselected );

    void clearTable( );

    void capture( bool active );

    void errorFromModel( QString error );

    void exportLogfile( const QString &filename );

    /**
     * Opens the context menu at the given position. This will be connected to the table view.
     */
    void contextMenu( const QPoint & );

private slots:
    /**
     * This function is used to update the size of the header to the model.
     */
    void updateHeaderSizeToModel( int section, int, int newSize );

    void updateHeaderPositionToModel( int section, int, int  );

private:
    enum SearchModes { Regex, Text, Expression};

    /// Private helper which performs the search
    void search( bool backwards );
private:
    boost::shared_ptr<LogEntryTableModel> m_model;

    QSplitter *m_splitter;

    /// Text field containing the formated log entry.
    QTextEdit *m_text;

    QTabWidget *m_myFilterTabs;

    QDockWidget *m_dockFilterShouldDockedTo;

	QScrollDownTableView *m_tableView;

	LogEntryTableFilter *m_proxyModel;

	EntryFormatingModel *m_timeFormatModel;

	QLineEdit *m_quickSearch;

	SearchModes m_searchMode;

	QPushButton* m_searchModeButton;

	QPushButton* m_markButton;

	TSharedAction m_quickSearchAction;

	TSharedExpression m_quickSearchExp;
};

#endif /* LOGENTRYTABLEWINDOW_H_ */
