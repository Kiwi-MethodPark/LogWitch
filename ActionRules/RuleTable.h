/*
 * RuleTable.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef RULETABLE_H_
#define RULETABLE_H_
#include <list>
#include <set>
#include <map>
#include <string>

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
    typedef std::set< TconstSharedRule > TRuleSet;
    typedef std::map<std::string,TRuleSet> TRuleTableMap;

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

    void addRule( const char *tableName, TSharedRule &rule );

    void clear( const char *tableName );

    // TActionList getActionsForEntry( TconstSharedLogEntry &entry ) const;
    template <class T>
    bool getMatchedActionsForType( std::list<T> &actions, TconstSharedLogEntry &entry ) const
    {
        TRuleSet::const_iterator it;
        for( it = m_rules.begin(); it != m_rules.end(); ++it )
        {
            T action = boost::dynamic_pointer_cast<typename T::element_type>( (*it)->getAction() );
            if( action && (*it)->checkRule( entry ) )
                actions.push_back( action );
        }
        return !actions.empty();
    }

    template <class T>
    bool isActionMatched( TconstSharedLogEntry &entry ) const
    {
        TRuleSet::const_iterator it;
        for( it = m_rules.begin(); it != m_rules.end(); ++it )
        {
            T action = boost::dynamic_pointer_cast<typename T::element_type>( (*it)->getAction() );
            if( action && (*it)->checkRule( entry ) )
                return true;
        }
        return false;
    }

    void beginChange();
    void endChange();
signals:
    void changed();

private:
    void dataChanged();

    bool m_onChange;

    TRuleSet m_rules;

    TRuleTableMap m_rulesFromSource;
};



#endif /* RULETABLE_H_ */
