/*
 * ActionParser.cpp
 *
 *  Created on: Jun 19, 2011
 *      Author: sven
 */

#include "ActionParser.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <Qt/qstring.h>
#include <QtGui>

#include "ActionRules/ActionDataRewriter.h"

using boost::spirit::tag::space;

namespace boost { namespace spirit { namespace traits
{
    // Make Qi recognize QString as a container
    template <> struct is_container<QString> : mpl::true_ {};

    // Expose the container's (QString's) value_type
    template <> struct container_value<QString> : mpl::identity<QChar> {};

    // Define how to insert a new element at the end of the container (QString)
    template <>
    struct push_back_container<QString, QChar>
    {
        static bool call(QString& c, QChar const& val)
        {
            c.append(val);
            return true;
        }
    };
}}}

namespace actionParser
{
    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    typedef std::pair<int, QVariant> TRoleVariantPair;

    namespace detail
    {
        struct constructEmptyAction
        {
            template <typename S1>
            struct result { typedef void type; };

            void operator()(TSharedAction& entry ) const
            {
                entry = TSharedAction();
            }
        };
        struct constructEmptyActionDataRewriter
        {
            template <typename S1>
            struct result { typedef void type; };

            void operator()(TSharedActionDataRewriter& entry ) const
            {
                entry = TSharedActionDataRewriter( new ActionDataRewriter );
            }
        };
        struct addRewriteRule
        {
            template <typename S1, typename S2>
            struct result { typedef void type; };

            void operator()(TSharedActionDataRewriter& entry, const TRoleVariantPair & rule ) const
            {
                entry->addChangeSet( rule.second, rule.first );
            }
        };
    }

    struct qColorSymb_ : qi::symbols<char, QColor>
    {
        qColorSymb_()
        {
            add
            ("yellow"       , QColor(Qt::yellow) )
            ("red"          , QColor(Qt::red) )
            ("green"        , QColor(Qt::green) )
            ;
        }

    } qColorSymb;

    struct qColorRoles_ : qi::symbols<char, int>
    {
        qColorRoles_()
        {
            add
            ("FG"       , Qt::ForegroundRole )
            ("BG"       , Qt::BackgroundRole )
            ;
        }

    } qColorRoles;


    template <typename Iterator>
    struct action_grammar : qi::grammar<Iterator, TSharedAction(), ascii::space_type>
    {
        action_grammar( TSharedConstLogEntryParserModelConfiguration cfg ) : action_grammar::base_type(action)
        {
            using qi::lit;
            using qi::lexeme;
            using qi::unused_type;
            using ascii::char_;
            using ascii::string;
            using namespace qi::labels;
            using boost::phoenix::function;
            using boost::spirit::ascii::alpha;

            function<detail::constructEmptyAction> constructEmptyAction;

            function<detail::constructEmptyActionDataRewriter> constructEmptyActionDataRewriter;
            function<detail::addRewriteRule> addRewriteRule;

            action = actionDataRewriter [_val = _1]
                    | qi::eps [ constructEmptyAction(_val) ];

            actionDataRewriter =
                    qi::eps [constructEmptyActionDataRewriter(_val)]
                    >> "Rewrite("
                    >>  rewriteRule [ addRewriteRule(_val, _1)] % ','
                    >> ')' ;

           rewriteRule = qColorRoles >> ':' >> qColorSymb;
        }

        // qi::rule<Iterator,  QString(), ascii::space_type> quotedQString;
        // qi::rule<Iterator,  QString(), ascii::space_type> unquotedQString;

        qi::rule<Iterator, TSharedAction(), ascii::space_type> action;
        qi::rule<Iterator, TSharedActionDataRewriter(), ascii::space_type> actionDataRewriter;

        qi::rule<Iterator, TRoleVariantPair(), ascii::space_type> rewriteRule;

        // qi::rule<Iterator, QColor(), ascii::space_type> color;
    };
}


ActionParser::ActionParser( TSharedConstLogEntryParserModelConfiguration cfg )
: m_cfg( cfg )
{
}

ActionParser::~ActionParser()
{
}


bool ActionParser::parse(  const QString &expression )
{
    std::string str = expression.toStdString();
    typedef actionParser::action_grammar<std::string::const_iterator> exp_grammar;
    exp_grammar expGram( m_cfg ); // Our grammar

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    bool r = phrase_parse(iter, end, expGram, space, m_action);

    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        // std::cout << *m_action << std::endl;
        std::cout << "-------------------------\n";
        std::cout << "- Extended: -\n";
        // m_action->out( std::cout, true );
        std::cout << "-------------------------\n";
        m_error = "";
    }
    else
    {
        m_action = TSharedAction();
        std::string::const_iterator some = iter+30;
        std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        m_error = QString::fromStdString( "Parsing failed near: " + context );
    }

    return isValid();
}

const QString &ActionParser::getError() const
{
    return m_error;
}

TSharedAction ActionParser::get() const
{
    return m_action;
}

bool ActionParser::isValid() const
{
    return m_error.length() == 0;
}
