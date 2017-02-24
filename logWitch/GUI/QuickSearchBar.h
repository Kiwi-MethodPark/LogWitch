/*
 * QuickSearchBar.h
 *
 *  Created on: 24 Feb 2017
 *      Author: sven
 */

#ifndef LOGWITCH_GUI_QUICKSEARCHBAR_H_
#define LOGWITCH_GUI_QUICKSEARCHBAR_H_

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include "ActionRules/Action.h"
#include "ActionRules/Expression.h"
#include "ActionRules/RuleTable.h"

#include "LogEntryTableWindow.h"

class LogEntryTableModel;

class QuickSearchBar:
    public QWidget
{
  Q_OBJECT
public:
  QuickSearchBar(LogEntryTableWindow* parent, boost::shared_ptr<LogEntryTableModel> model, TSharedRuleTable ruleTableForSearching);
  virtual ~QuickSearchBar();

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

private:
  enum SearchModes { Regex, Text, Expression};

  boost::shared_ptr<LogEntryTableModel> m_model;

  LogEntryTableWindow* m_logWindow;

  QLineEdit *m_quickSearch;

  SearchModes m_searchMode;

  QPushButton* m_searchModeButton;

  QPushButton* m_markButton;

  TSharedAction m_quickSearchAction;

  TSharedExpression m_quickSearchExp;

  TSharedRuleTable m_ruleTableForSearching;
};

#endif /* LOGWITCH_GUI_QUICKSEARCHBAR_H_ */
