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
#include <boost/spirit/home/phoenix/core/value.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <Qt/qstring.h>

#include "ActionRules/Expression.h"
#include "ActionRules/ExpressionOperators.h"
#include "ActionRules/ExpressionRegEx.h"
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

std::ostream& operator<<(std::ostream&o, const QString& str)
{
    return o << str.toStdString();
}

std::ostream& operator<<(std::ostream&o, const TSharedExpression& str)
{
    return o << *str;
}

std::ostream& operator<<(std::ostream&o, const TSharedValueGetter& str)
{
    return o << *str;
}

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
        struct constructExpVG
        {
            template <typename S1,typename S2, typename S3>
            struct result { typedef void type; };

            void operator()(TSharedExpression& entry, TSharedValueGetter &left, TSharedValueGetter &right) const
            {
                entry = TSharedExpressionValueGetter( new ExpressionValueGetter( left, right ) );
            }
        };
        struct constructExpRegEx
        {
            template <typename S1,typename S2, typename S3>
            struct result { typedef void type; };

            void operator()(TSharedExpression& entry, TSharedValueGetter &value, QString &regex) const
            {
                entry = TSharedExpressionRegEx( new ExpressionRegEx( value, regex ) );
            }
        };
        struct constructExpOpNeg
        {
            template <typename S1,typename S2>
            struct result { typedef void type; };

            void operator()(TSharedExpression& entry, TSharedExpression &value) const
            {
                entry = TSharedExpressionOpNegate( new ExpressionOpNegate( value ) );
            }
        };
        struct constructExpOpAnd
        {
            template <typename S1,typename S2,typename S3>
            struct result { typedef void type; };

            void operator()(TSharedExpression& entry, TSharedExpression &left, TSharedExpression &right) const
            {
                entry = TSharedExpressionOpAnd( new ExpressionOpAnd( left, right ) );
            }
        };
        struct constructExpOpOr
        {
            template <typename S1,typename S2,typename S3>
            struct result { typedef void type; };

            void operator()(TSharedExpression& entry, TSharedExpression &left, TSharedExpression &right) const
            {
                entry = TSharedExpressionOpOr( new ExpressionOpOr( left, right ) );
            }
        };
        struct constructExpOpXOr
        {
            template <typename S1,typename S2,typename S3>
            struct result { typedef void type; };

            void operator()(TSharedExpression& entry, TSharedExpression &left, TSharedExpression &right) const
            {
                entry = TSharedExpressionOpXOr( new ExpressionOpXOr( left, right ) );
            }
        };
        struct constructExpConst
        {
            template <typename S1,typename S2>
            struct result { typedef void type; };

            void operator()(TSharedExpression& entry, bool value) const
            {
                entry = TSharedExpressionConst( new ExpressionConst( value ) );
            }
        };
    }


   template <typename Iterator>
   struct expression_grammar : qi::grammar<Iterator, TSharedExpression(), ascii::space_type>
   {
       expression_grammar( TSharedConstLogEntryParserModelConfiguration cfg) : expression_grammar::base_type(top)
       {
           using qi::lit;
           using qi::lexeme;
           using ascii::char_;
           using ascii::string;
           using namespace qi::labels;
           using boost::phoenix::function;
           using boost::phoenix::val;
           using boost::spirit::ascii::alpha;

           function<detail::createVGLogEntry> createVGLogEntry;
           function<detail::createVGConstString> createVGConstString;
           function<detail::constructExpVG> constructExpVG;
           function<detail::constructExpRegEx> constructExpRegEx;
           function<detail::constructExpOpNeg> constructExpOpNeg;
           function<detail::constructExpOpAnd> constructExpOpAnd;
           function<detail::constructExpOpOr> constructExpOpOr;
           function<detail::constructExpOpXOr> constructExpOpXOr;
           function<detail::constructExpConst> constructExpConst;

           top =
                   expression [_val=_1]
                   | qi::eps[constructExpConst(_val,val(false))];

           expression =
                     orExprTerm [_val=_1]
                   | basicExpr [_val=_1]
                   ;

           orExprTerm =
                     ((xorExprTerm >> "||" >> xorExprTerm)[constructExpOpOr(_val,_1,_2)])
                   | xorExprTerm [_val=_1]
                   ;

           xorExprTerm =
                     ((andExprTerm >> "^^" >> andExprTerm)[constructExpOpXOr(_val,_1,_2)])
                   | andExprTerm [_val=_1]
                   ;

           andExprTerm =
                     ((unaryExprTerm >> "&&" >> unaryExprTerm)[constructExpOpAnd(_val,_1,_2)])
                   | unaryExprTerm [_val=_1]
                   ;

           unaryExprTerm =
                     ("!(" >> expression[constructExpOpNeg(_val,_1)] >> ")" )
                   | ('(' >> expression >> ')') [_val=_1]
                   | basicExpr [_val=_1]
                   ;

           basicExpr =
                     expressionVG [_val=_1]
                   | expressionRegEx [_val=_1]
                   | expressionConst [_val=_1]
                   ;

           expressionVG = ( valueGetter >> "==" >> valueGetter)[ constructExpVG(_val, _1, _2 )]
                     | ( valueGetter >> "!=" >> valueGetter)[ constructExpVG(_val, _1, _2 )]
                         >> qi::eps[constructExpOpNeg(_val,_val)] ;

           expressionRegEx = (valueGetter >> "=~" >> quotedQStringRegEx)[ constructExpRegEx(_val, _1, _2)]
                     | (valueGetter >> "!~" >> quotedQStringRegEx)[ constructExpRegEx(_val, _1, _2)]
                         >> qi::eps[constructExpOpNeg(_val,_val)] ;

           expressionConst =
                     lit("true") [constructExpConst(_val,val(true))]
                   | lit("false") [constructExpConst(_val,val(false))]
                   ;

           valueGetter %= vg_ConstString
                   | vg_LogEntry;

           vg_ConstString = quotedQString('"')[createVGConstString(_val,_1)];
           vg_LogEntry = unquotedQString[createVGLogEntry(_val,_1,cfg)];

           quotedQString = lexeme[
                                  lit(_r1)
                               >> +(
                                       ( '\\' >> char_(_r1) ) [_val += _1]
                                       || (char_ - lit(_r1) ) [_val += _1]
                                   )
                               >> lit(_r1)];

           quotedQStringRegEx %=
                     quotedQString('"')
                   | quotedQString('/')
                   | quotedQString('|')
                   | quotedQString('#')
                   ;

           unquotedQString %=  +(alpha);

           BOOST_SPIRIT_DEBUG_NODE(top);
           BOOST_SPIRIT_DEBUG_NODE(expression);
           BOOST_SPIRIT_DEBUG_NODE(orExprTerm);
           BOOST_SPIRIT_DEBUG_NODE(xorExprTerm);
           BOOST_SPIRIT_DEBUG_NODE(andExprTerm);
           BOOST_SPIRIT_DEBUG_NODE(unaryExprTerm);
           BOOST_SPIRIT_DEBUG_NODE(basicExpr);

           BOOST_SPIRIT_DEBUG_NODE(expressionVG);
           BOOST_SPIRIT_DEBUG_NODE(expressionRegEx);
           BOOST_SPIRIT_DEBUG_NODE(expressionConst);

           BOOST_SPIRIT_DEBUG_NODE(valueGetter);
           BOOST_SPIRIT_DEBUG_NODE(vg_LogEntry);
           BOOST_SPIRIT_DEBUG_NODE(vg_ConstString);

           BOOST_SPIRIT_DEBUG_NODE(quotedQString);
           BOOST_SPIRIT_DEBUG_NODE(unquotedQString);
       }

       qi::rule<Iterator, TSharedExpression(), ascii::space_type> top;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> expression;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> orExprTerm;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> xorExprTerm;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> andExprTerm;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> unaryExprTerm;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> basicExpr;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> expressionVG;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> expressionRegEx;
       qi::rule<Iterator, TSharedExpression(), ascii::space_type> expressionConst;

       qi::rule<Iterator, TSharedValueGetter(), ascii::space_type> valueGetter;
       qi::rule<Iterator, TSharedValueGetter(), ascii::space_type> vg_LogEntry;
       qi::rule<Iterator, TSharedValueGetter(), ascii::space_type> vg_ConstString;
       qi::rule<Iterator,  QString(const char), ascii::space_type> quotedQString;
       qi::rule<Iterator,  QString(), ascii::space_type> quotedQStringRegEx;
       qi::rule<Iterator,  QString(), ascii::space_type> unquotedQString;
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
