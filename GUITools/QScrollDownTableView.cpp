/*
 * QScrollDownTableView.cpp
 *
 *  Created on: Jun 26, 2011
 *      Author: sven
 */

#include "QScrollDownTableView.h"

QScrollDownTableView::QScrollDownTableView(QWidget *parent )
:QTableView( parent )
{
}

QScrollDownTableView::~QScrollDownTableView()
{
}

void QScrollDownTableView::updateGeometries()
{
    bool scrollDown = verticalScrollBar()->maximum() == verticalScrollBar()->value();
    QTableView::updateGeometries();
    if( scrollDown )
        scrollToBottom();
}
