/*
 * StringCacheTreeModel.h
 *
 *  Created on: May 15, 2011
 *      Author: sven
 */

#ifndef STRINGCACHETREEMODEL_H_
#define STRINGCACHETREEMODEL_H_
#include "LogData/StringCache.h"
#include <QAbstractTableModel>
#include "StringCacheTreeItem.h"
#include <QRegExp>

class LogEntryFilter;
class LogEntryRemoveFilter;

class StringCacheTreeModel
	: public QAbstractItemModel
{
	Q_OBJECT
public:
	StringCacheTreeModel( QObject *parent, const StringCache * cache, int attributeId, const QString &splitString = QString() );
	virtual ~StringCacheTreeModel();

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

//    QVariant headerData(int section, Qt::Orientation orientation,
//                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool setData( const QModelIndex &index, const QVariant& value, int role );

    boost::shared_ptr<LogEntryFilter> getFilter() const;

private:
    void dataChangedToChildren(const QModelIndex &index );

public slots:
	void newStringElement( TSharedConstQString );

private:
	boost::shared_ptr<StringCacheTreeItem> m_rootNode;

	QString m_modelname;

	boost::shared_ptr<QRegExp> m_splitRegex;

	boost::shared_ptr<LogEntryRemoveFilter> m_myFilter;
};

#endif /* STRINGCACHETREEMODEL_H_ */
