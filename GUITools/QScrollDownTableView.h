/*
 * QScrollDownTableView.h
 *
 *  Created on: Jun 26, 2011
 *      Author: sven
 */

#ifndef QSCROLLDOWNTABLEVIEW_H_
#define QSCROLLDOWNTABLEVIEW_H_
#include <QtGui>

class QScrollDownTableView
:public QTableView
{
    Q_OBJECT
public:
    QScrollDownTableView( QWidget *parent = NULL);
    ~QScrollDownTableView();

protected:
    void updateGeometries();
};

#endif /* QSCROLLDOWNTABLEVIEW_H_ */
