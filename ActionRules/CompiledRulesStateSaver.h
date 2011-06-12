/*
 * CompiledRulesStateSaver.h
 *
 *  Created on: Jun 12, 2011
 *      Author: sven
 */

#ifndef COMPILEDRULESSTATESAVER_H_
#define COMPILEDRULESSTATESAVER_H_
#include <QtCore>
#include <QtGui>
#include <boost/shared_ptr.hpp>
#include "ActionRules/TableModelRulesCompiled.h"

class CompiledRulesStateSaver
: public QObject
{
    Q_OBJECT
public:
    CompiledRulesStateSaver();
    ~CompiledRulesStateSaver();

    QTableView *m_compiledRuleView;
    TableModelRulesCompiled *m_rulesCompiledModel;

};

typedef boost::shared_ptr<CompiledRulesStateSaver> TSharedCompiledRulesStateSaver;
typedef boost::shared_ptr<const CompiledRulesStateSaver> TconstCompiledRulesStateSaver;

#endif /* COMPILEDRULESSTATESAVER_H_ */
