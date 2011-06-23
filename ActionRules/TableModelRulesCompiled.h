/*
 * TableModelRulesCompiled.h
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#ifndef TABLEMODELRULESCOMPILED_H_
#define TABLEMODELRULESCOMPILED_H_
#include <vector>

#include <boost/shared_ptr.hpp>

#include <QAbstractTableModel>
#include <QtCore>

#include "ActionRules/Action.h"
#include "ActionRules/FilterRuleCompiled.h"
#include "ActionRules/RuleTable.h"

using boost::shared_ptr;

class TableModelRulesCompiled
     :public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModelRulesCompiled( QObject *parent, TSharedConstLogEntryParserModelConfiguration cfg, TSharedRuleTable ruleTable  );
    virtual ~TableModelRulesCompiled();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData( const QModelIndex &index, const QVariant& value, int role );

    Qt::ItemFlags flags(const QModelIndex & index ) const;

    void appendRule( TSharedFilterRuleRaw rule );
private:
    typedef std::vector<TSharedFilterRuleCompiled> TCompiledRulesTable;
    TCompiledRulesTable m_table;

    static const int m_columnCount = 2;

    /**
     * The configuration this table is for.
     */
    TSharedConstLogEntryParserModelConfiguration m_configuration;

    /**
     * This is the executive element which is the table for filtering.
     */
    TSharedRuleTable m_ruleTable;

};
#endif /* TABLEMODELRULESCOMPILED_H_ */
