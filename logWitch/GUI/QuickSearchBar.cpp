/*
 * QuickSearchBar.cpp
 *
 *  Created on: 24 Feb 2017
 *      Author: sven
 */

#include "GUI/QuickSearchBar.h"

#include <QHBoxLayout>
#include <QPushButton>

#include "ActionRules/ActionParser.h"
#include "ActionRules/ExpressionFind.h"
#include "ActionRules/ExpressionParser.h"
#include "ActionRules/ExpressionRegEx.h"
#include "ActionRules/Rule.h"
#include "ActionRules/ValueGetterLogEntry.h"

#include "Models/LogEntryTableModel.h"

QuickSearchBar::QuickSearchBar(LogEntryTableWindow* parent
    , boost::shared_ptr<LogEntryTableModel> model)
: QWidget(parent)
, m_model(model)
, m_logWindow(parent)
, m_searchMode(Text)
{
  // Create quicksearch bar
  m_quickSearch = new QLineEdit;
  QHBoxLayout* quickSearchLayout = new QHBoxLayout;
  quickSearchLayout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(quickSearchLayout);

  QIcon iconDown, iconUp, search;
  search.addFile(QString::fromUtf8(":/icons/search"), QSize(), QIcon::Normal,
      QIcon::Off);
  iconDown.addFile(QString::fromUtf8(":/icons/searchDown"), QSize(),
      QIcon::Normal, QIcon::Off);
  iconUp.addFile(QString::fromUtf8(":/icons/searchUp"), QSize(), QIcon::Normal,
      QIcon::Off);

  m_searchModeButton = new QPushButton(tr("Text"));
  QObject::connect(m_searchModeButton, SIGNAL(clicked()), this,
      SLOT(switchSearchMode()));

  m_markButton = new QPushButton(tr("Highlight"));
  m_markButton->setCheckable(true);
  m_markButton->setChecked(true);

  QObject::connect(m_markButton, SIGNAL(toggled(bool)), this,
      SLOT(updateSearch()));

  QObject::connect(m_quickSearch, SIGNAL(editingFinished()), this,
      SLOT(updateSearch()));

  QPushButton *searchDnBtn = new QPushButton(iconDown, "");
  QObject::connect(searchDnBtn, SIGNAL(clicked()), this, SLOT(searchNext()));

  QPushButton *searchUpBtn = new QPushButton(iconUp, "");
  QObject::connect(searchUpBtn, SIGNAL(clicked()), this, SLOT(searchPrev()));

  quickSearchLayout->addWidget(m_searchModeButton);
  quickSearchLayout->addWidget(m_quickSearch);
  quickSearchLayout->addWidget(m_markButton);
  quickSearchLayout->addWidget(searchDnBtn);
  quickSearchLayout->addWidget(searchUpBtn);

  // Initialize the action for highlighting log entries.
  ActionParser parser(m_model->getParserModelConfiguration());
  if (!parser.parse("rewrite(BG:#81BEF7)"))
  {
    qDebug() << "Parsing of action failed!";
  }
  else
  {
    m_quickSearchAction = parser.get();
  }
}

QuickSearchBar::~QuickSearchBar()
{
}


void QuickSearchBar::switchSearchMode()
{
  switch (m_searchMode)
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

  switch (m_searchMode)
  {
  default:
  case Text:
    m_searchModeButton->setText(tr("Text"));
    break;
  case Regex:
    m_searchModeButton->setText(tr("Regex"));
    break;
  case Expression:
    m_searchModeButton->setText(tr("Expr"));
    break;
  }

  updateSearch();
}

void QuickSearchBar::updateSearch()
{
  TSharedRuleTable ruleTableForSearching = m_logWindow->getRuleTable();
  ruleTableForSearching->clear("quicksearch");
  m_quickSearch->setStyleSheet("");
  m_quickSearch->setToolTip("");
  m_quickSearchExp.reset();

  // Do not do any filtering or searching if no text was given.
  if (m_quickSearch->text().length() == 0)
    return;

  if (m_searchMode == Expression)
  {
    ExpressionParser parser(m_model->getParserModelConfiguration());

    if (parser.parse(m_quickSearch->text()))
    {
      m_quickSearchExp = parser.get();
    }
    else
    {
      m_quickSearch->setStyleSheet("background-color:red;");
      m_quickSearch->setToolTip(
          tr("Error while parsing: ") + parser.getError());
      qDebug() << "Unable to parse expression: " << m_quickSearch->text();
    }
  }
  else if (m_searchMode == Text)
  {
    TSharedValueGetterLogEntry log(
        new ValueGetterLogEntry("message",
            m_model->getParserModelConfiguration()));
    m_quickSearchExp.reset(new ExpressionFind(log, m_quickSearch->text()));

  }
  else if (m_searchMode == Regex)
  {
    TSharedValueGetterLogEntry log(
        new ValueGetterLogEntry("message",
            m_model->getParserModelConfiguration()));
    m_quickSearchExp.reset(new ExpressionRegEx(log, m_quickSearch->text()));
  }

  if (m_markButton->isChecked() && m_quickSearchExp)
  {
    TSharedRule rule(new Rule(m_quickSearchExp, m_quickSearchAction));
    ruleTableForSearching->addRule("quicksearch", rule);
  }
}


void QuickSearchBar::searchNext()
{
  if (m_quickSearchExp)
    m_logWindow->search(*m_quickSearchExp, false);
}

void QuickSearchBar::searchPrev()
{
  if (m_quickSearchExp)
    m_logWindow->search(*m_quickSearchExp, true);
}
