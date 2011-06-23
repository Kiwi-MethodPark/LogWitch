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

/**
 * This is the Widget which is responsible for displaying the filter rules in
 * raw format and also the compiled ones. This window also handles the interaction
 * between them, for e.g. adding from the raw rules to the compiled rules.
 */
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
