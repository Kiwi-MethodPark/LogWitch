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

using boost::shared_ptr;

class TableModelRulesCompiled
     :public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModelRulesCompiled( QObject *parent, TSharedConstLogEntryParserModelConfiguration cfg );
    virtual ~TableModelRulesCompiled();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData( const QModelIndex &index, const QVariant& value, int role );

    Qt::ItemFlags flags(const QModelIndex & index ) const;

    void appendRule( TSharedFilterRuleRaw rule );
private:
    typedef std::vector<TSharedFilterRuleCompiled> TRuleTable;
    TRuleTable m_table;

    static const int m_columnCount = 2;

    TSharedConstLogEntryParserModelConfiguration m_configuration;

};
#endif /* TABLEMODELRULESCOMPILED_H_ */
