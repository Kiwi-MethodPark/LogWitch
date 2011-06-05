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

#include "Rule.h"

class RuleTable;

typedef boost::shared_ptr<RuleTable> TSharedRuleTable;
typedef boost::shared_ptr<const RuleTable> TconstSharedRuleTable;

class RuleTable
{
public:
    RuleTable();
    virtual ~RuleTable();

    template <class T>
    TSharedRuleTable filterRulesByType( ) const
    {
        TSharedRuleTable table( new RuleTable );

        TRuleList::iterator it;
        for( it = m_rules.begin(); it != m_rules.end(); ++it )
        {
            boost::shared_ptr<T> rule = boost::dynamic_pointer_cast<T>( *it );
            if( rule )
                table->m_rules.push_back( rule );
        }
        return table;
    }

    void addRule( TSharedRule &rule );

private:
    typedef std::list< TconstSharedRule > TRuleList;
    TRuleList m_rules;
};



#endif /* RULETABLE_H_ */
