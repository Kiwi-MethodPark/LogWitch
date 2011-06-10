/*
 * FilterRuleSelectionWindow.h
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#ifndef FILTERRULESELECTIONWINDOW_H_
#define FILTERRULESELECTIONWINDOW_H_
#include <QtGui>

class FilterRuleSelectionWindow
: public QSplitter
{
    Q_OBJECT
public:
    FilterRuleSelectionWindow( QWidget* parent = NULL );
    virtual ~FilterRuleSelectionWindow();

};

#endif /* FILTERRULESELECTIONWINDOW_H_ */
