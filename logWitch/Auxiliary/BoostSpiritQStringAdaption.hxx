/*
 * BoostSpiritQStringAdaption.hxx
 *
 *  Created on: Jan 5, 2012
 *      Author: sven
 */

#ifndef BOOSTSPIRITQSTRINGADAPTION_HXX_
#define BOOSTSPIRITQSTRINGADAPTION_HXX_

#include <Qt/qstring.h>

#include <boost/spirit/include/qi.hpp>

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


#endif /* BOOSTSPIRITQSTRINGADAPTION_HXX_ */
