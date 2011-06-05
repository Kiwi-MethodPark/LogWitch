/*
 * ActionColorRow.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef ACTIONCOLORROW_H_
#define ACTIONCOLORROW_H_
#include <QtGui>

#include "ActionRules/Action.h"

class ActionColorRow
    : public Action
{
public:
    ActionColorRow( const QColor &col);
    ~ActionColorRow();

    const QColor &getColor() const;

private:
    QColor m_color;
};

#endif /* ACTIONCOLORROW_H_ */
