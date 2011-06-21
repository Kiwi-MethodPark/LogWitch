/*
 * FilterRuleSelectionWindow.h
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#ifndef FILTERRULESELECTIONWINDOW_H_
#define FILTERRULESELECTIONWINDOW_H_
#include <QtGui>
#include "ActionRules/TableModelRules.h"
#include "ActionRules/TableModelRulesCompiled.h"
#include "ActionRules/CompiledRulesStateSaver.h"

class CompiledRulesStateSaver;
class LogEntryTableFilter;

class FilterRuleSelectionWindow
: public QSplitter
{
    Q_OBJECT
public:
    FilterRuleSelectionWindow( QWidget* parent = NULL );
    virtual ~FilterRuleSelectionWindow();

    void setWindow( TSharedCompiledRulesStateSaver state );

public slots:
    void addSelectionToCompiled();

private:
    QTableView *m_ruleView;
    TableModelRules *m_rulesModel;

    TSharedCompiledRulesStateSaver m_compiledRules;
};

#endif /* FILTERRULESELECTIONWINDOW_H_ */
