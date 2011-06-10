/*
 * ActionColorRow.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef ACTIONDATAREWRITER_H_
#define ACTIONDATAREWRITER_H_
#include <map>

#include <QtCore/QVariant>

#include "ActionRules/Action.h"

class ActionDataRewriter
    : public Action
{
public:
    ActionDataRewriter();
    ~ActionDataRewriter();

    bool getData( QVariant &var,  int role) const;

    void addChangeSet( const QVariant &var, int role );

    QVariant toDisplay( int role ) const;

    void execute( ) const {};

private:
    typedef std::map<int,QVariant> TChangeSet;
    TChangeSet m_changes;
};

typedef boost::shared_ptr<ActionDataRewriter> TSharedActionDataRewriter;
typedef boost::shared_ptr<const ActionDataRewriter> TconstSharedActionDataRewriter;

#endif /* ACTIONCOLORROW_H_ */
