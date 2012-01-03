/*
 * TableModelRules.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef TABLEMODELRULES_H_
#define TABLEMODELRULES_H_
#include <vector>

#include <boost/shared_ptr.hpp>

#include <QAbstractTableModel>
#include <QtCore>
#include <QtGui>

#include "ActionRules/Action.h"
#include "ActionRules/FilterRuleRaw.h"

using boost::shared_ptr;

class TableModelRules
     :public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModelRules( QObject *parent );
    virtual ~TableModelRules();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData( const QModelIndex &index, const QVariant& value, int role );

    Qt::ItemFlags flags(const QModelIndex & index ) const;

    Qt::DropActions supportedDropActions() const;

    QStringList mimeTypes() const;

    QMimeData *mimeData(const QModelIndexList &indexes) const;

    bool dropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent);

    TSharedFilterRuleRaw getRaw(const QModelIndex &index)const;

    /**
     * Removes a specified set of rules.
     * After calling this, all indexes will be invalid.
     */
    void removeRules( const QModelIndexList &idxList );

public slots:
    /**
     * This method inserts a new created rule.
     */
    void insertEmptyRule();

private:
    QByteArray getIdentification() const;

    std::vector<TSharedFilterRuleRaw> m_table;

    static const int m_columnCount = 2;
};

#endif /* TABLEMODELRULES_H_ */
