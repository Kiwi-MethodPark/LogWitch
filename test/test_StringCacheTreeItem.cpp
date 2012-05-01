/*
 * test_StringCacheTreeItem.cpp
 *
 *  Created on: May 1, 2012
 *      Author: sven
 */

#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE StringCacheTreeItem
#include <boost/test/unit_test.hpp>

#include <boost/make_shared.hpp>

#include "Models/StringCacheTreeItem.h"

using namespace boost;

/**
 * This test is testing if adding items works well.
 */
BOOST_AUTO_TEST_CASE( addItems )
{
    StringCacheTreeItem root(make_shared<const QString>("Root")
            , make_shared<const QString>("Root"));

    BOOST_CHECK_EQUAL( root.childCount(), 0);

    root.appendChild( new StringCacheTreeItem(
              make_shared<const QString>("Test")
            , make_shared<const QString>("Test") ) );

    BOOST_CHECK_EQUAL( root.childCount(), 1);

    root.appendChild( new StringCacheTreeItem(
              make_shared<const QString>("Test2")
            , make_shared<const QString>("Test2") ) );

    BOOST_CHECK_EQUAL( root.childCount(), 2);
}



