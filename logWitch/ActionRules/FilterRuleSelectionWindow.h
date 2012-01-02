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
#include "ActionRules/RulesTableView.h"

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

    /**
     * This displays a previously hidden window or adds a new one.
     * If an empty pointer is set as state, the window will switch
     * to the inital case.
     */
    void setWindow( TSharedCompiledRulesStateSaver state );

    /**
     * This returns the currently active state for the window to restore
     * it via setWindow. If no window was set before, it returns a
     * empty pointer.
     */
    TSharedCompiledRulesStateSaver getWindow();

public slots:
    void addSelectionToCompiled();
    void removeSelectionFromCompiled();
    void trashSelectedRules();

private:

    RulesTableView *m_ruleView;
    TableModelRules *m_rulesModel;

    TSharedCompiledRulesStateSaver m_compiledRules;

    QAction *m_addSelectedRules;
    QAction *m_trashSelectedRules;
};

class FilterRuleSelWndStateSaverTypes
{
public:
    typedef FilterRuleSelectionWindow ObjectToSet;
    typedef TSharedCompiledRulesStateSaver value;

    static value get( ObjectToSet *obj ) { return obj->getWindow(); }
    static void set( ObjectToSet *obj, value s ) { obj->setWindow( s ); }
};

#endif /* FILTERRULESELECTIONWINDOW_H_ */
