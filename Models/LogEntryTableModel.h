/*
 * LogEntryTableModel.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYTABLEMODEL_H_
#define LOGENTRYTABLEMODEL_H_

#include <qabstractitemmodel.h>

#include "LogData/LogEntryTable.h"

class LogEntryTableModel: public QAbstractTableModel {
public:
	LogEntryTableModel( const TLogEntryTable *table );
	virtual ~LogEntryTableModel();

private:
	const TLogEntryTable *m_table;
};

#endif /* LOGENTRYTABLEMODEL_H_ */
