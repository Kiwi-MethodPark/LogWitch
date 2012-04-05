#define BOOST_TEST_DYN_LINK


#define BOOST_TEST_MODULE Example test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( exampleTest )
{
     int i = 0;

     BOOST_CHECK_EQUAL( i, 0 );
     BOOST_CHECK( i == 0 );
     i++;

     BOOST_CHECK_EQUAL( i, 1 );
     BOOST_CHECK( i == 1 );
}
