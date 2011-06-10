/*
 * RuleTable.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef RULETABLE_H_
#define RULETABLE_H_
#include <list>

#include <boost/shared_ptr.hpp>
#include <QtCore/qobject.h>

#include "Rule.h"

class RuleTable;

typedef boost::shared_ptr<RuleTable> TSharedRuleTable;
typedef boost::shared_ptr<const RuleTable> TconstSharedRuleTable;

class RuleTable
    :public QObject
{
    Q_OBJECT
public:
    typedef std::list< TconstSharedRule > TRuleList;

    RuleTable();
    virtual ~RuleTable();

//    template <class T>
//    TSharedRuleTable filterRulesByType( ) const
//    {
//        TSharedRuleTable table( new RuleTable );
//
//        TRuleList::iterator it;
//        for( it = m_rules.begin(); it != m_rules.end(); ++it )
//        {
//            boost::shared_ptr<T> rule = boost::dynamic_pointer_cast<T>( *it );
//            if( rule )
//                table->m_rules.push_back( rule );
//        }
//        return table;
//    }

    void addRule( TSharedRule &rule );

    // TActionList getActionsForEntry( TconstSharedLogEntry &entry ) const;
    template <class T>
    bool getMatchedActionsForType( std::list<T> &actions, TconstSharedLogEntry &entry ) const
    {
        TRuleList::const_iterator it;
        for( it = m_rules.begin(); it != m_rules.end(); ++it )
        {
            T action = boost::dynamic_pointer_cast<typename T::element_type>( (*it)->getAction() );
            if( action && (*it)->checkRule( entry ) )
                actions.push_back( action );
        }
        return !actions.empty();
    }

signals:
    void beginChange();
    void endChange();

private:

    TRuleList m_rules;
};



#endif /* RULETABLE_H_ */
