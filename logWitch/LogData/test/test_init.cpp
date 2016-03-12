/*
 * test_init.cpp
 *
 *  Created on: 12 Mar 2016
 *      Author: sven
 */
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE LogData
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>

#include "LogData/LogDataTypeRegistration.h"

int main( int argc, char* argv[] )
{
  registerLogDataTypes();
  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

BOOST_AUTO_TEST_CASE( exampleTest )
{
     int i = 0;

     BOOST_CHECK_EQUAL( i, 0 );
     BOOST_CHECK( i == 0 );
     i++;

     BOOST_CHECK_EQUAL( i, 1 );
     BOOST_CHECK( i == 1 );
}
