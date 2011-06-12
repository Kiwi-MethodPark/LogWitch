/*
 * ExpressionParser.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "ExpressionParser.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <Qt/qstring.h>

#include "ActionRules/Expression.h"
#include "ActionRules/ExpressionValueGetter.h"
#include "ActionRules/ValueGetterConstQString.h"
#include "ActionRules/ValueGetterLogEntry.h"

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

namespace expressionParser
{
    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    namespace detail
    {
        struct createVGLogEntry
        {
            template <typename S1,typename S2,typename S3>
            struct result { typedef void type; };

            void operator()(TSharedValueGetter& entry, const QString &name, TSharedConstLogEntryParserModelConfiguration configuration) const
            {
                entry = TSharedValueGetter( new ValueGetterLogEntry( name, configuration ) );
            }
        };
        struct createVGConstString
        {
            template <typename S1,typename S2>
            struct result { typedef void type; };

            void operator()(TSharedValueGetter& entry, const QString &name) const
            {
                entry = TSharedValueGetter( new ValueGetterConstQString( name ) );
            }
        };
        struct constructPartialExpVG
        {
            template <typename S1,typename S2>
            struct result { typedef void type; };

            void operator()(TSharedExpressionValueGetter& entry, TSharedValueGetter &left) const
            {
                entry = TSharedExpressionValueGetter( new ExpressionValueGetter( left ) );
            }
        };
        struct setRightToExpVG
        {
            template <typename S1,typename S2>
            struct result { typedef void type; };

            void operator()(TSharedExpressionValueGetter& entry, TSharedValueGetter &right) const
            {
                entry->setRight( right );
            }
        };
    }


   template <typename Iterator>
   struct expression_grammar : qi::grammar<Iterator, TSharedExpression(), ascii::space_type>
   {
       expression_grammar( TSharedConstLogEntryParserModelConfiguration cfg) : expression_grammar::base_type(expression)
       {
           using qi::lit;
           using qi::lexeme;
           using ascii::char_;
           using ascii::string;
           using namespace qi::labels;
           using boost::phoenix::function;
           using boost::spirit::ascii::alpha;

           function<detail::createVGLogEntry> createVGLogEntry;
           function<detail::createVGConstString> createVGConstString;
           function<detail::constructPartialExpVG> constructPartialExpVG;
           function<detail::setRightToExpVG> setRightToExpVG;

           expression = expressionVG[_val = _1];

           expressionVG = vg_LogEntry[ constructPartialExpVG(_val, _1 )] >> "==" >> vg_ConstString[setRightToExpVG(_val, _1 )]
               | vg_ConstString[ constructPartialExpVG(_val, _1 )] >> "==" >> vg_LogEntry[setRightToExpVG(_val, _1 )]
               | vg_ConstString[ constructPartialExpVG(_val, _1 )] >> "==" >> vg_ConstString[setRightToExpVG(_val, _1 )]
               | vg_LogEntry[ constructPartialExpVG(_val, _1 )] >> "==" >> vg_LogEntry[setRightToExpVG(_val, _1 )];

           quotedQString %= lexeme['"' >> +(char_ - '"') >> '"'];
           unquotedQString %=  +(alpha) ;

           vg_ConstString = quotedQString[createVGConstString(_val,_1)];
           vg_LogEntry = unquotedQString[createVGLogEntry(_val,_1,cfg)];
       }

       qi::rule<Iterator,  QString(), ascii::space_type> quotedQString;
       qi::rule<Iterator,  QString(), ascii::space_type> unquotedQString;
       qi::rule<Iterator, TSharedValueGetter(), ascii::space_type> vg_LogEntry;
       qi::rule<Iterator, TSharedValueGetter(), ascii::space_type> vg_ConstString;

       qi::rule<Iterator, TSharedExpressionValueGetter(), ascii::space_type> expressionVG;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> expression;
   };
}

ExpressionParser::ExpressionParser( TSharedConstLogEntryParserModelConfiguration cfg )
    : m_cfg( cfg )
{

}

bool ExpressionParser::parse(  const QString &expression )
{
    std::string str = expression.toStdString();

    typedef expressionParser::expression_grammar<std::string::const_iterator> exp_grammar;
    exp_grammar expGram( m_cfg ); // Our grammar

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    bool r = phrase_parse(iter, end, expGram, space, m_expression);

    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << *m_expression << std::endl;
        std::cout << "-------------------------\n";
        std::cout << "- Extended: -\n";
        m_expression->out( std::cout, true );
        std::cout << "-------------------------\n";
        m_error = "";
    }
    else
    {
        m_expression = TSharedExpression();
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

const QString &ExpressionParser::getError() const
{
    return m_error;
}

TSharedExpression ExpressionParser::get() const
{
    return m_expression;
}

bool ExpressionParser::isValid() const
{
    return m_expression;
}

ExpressionParser::~ExpressionParser()
{
}
