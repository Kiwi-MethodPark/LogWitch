#include <LogData/test/LogEntryStorerForTesting.h>
#include "LogData/LogEntryParser_Logfile.h"

#include "LogData/ParserStreamGetter.h"
/*
 * test_init.cpp
 *
 *  Created on: 12 Mar 2016
 *      Author: sven
 */
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "LogEntryStorerForTesting.h"

void fillStream(QTextStream& ts, bool withEndl=false)
{
  ts << "2016-03-08 15:23:23,904 - INFO - Mix.Dist.Command - [/Test.cpp:143:executeCommand] - Received command";
  ts << endl;
  ts << "2016-03-08 15:23:23,904 - INFO - Mix.Dist.Mode - [/Mode.cpp:313:exit] - Exiting Mode ";
  ts << endl;
  ts << "2016-03-08 15:23:23,908 - INFO - Mix.Dist.Control - [/Control.cpp:113:handleExit] - handleExit";
  ts << endl;
  ts << "2016-03-08 15:23:23,909 - INFO - Mix.Dist.Control - [/Control.cpp:65:shutdown] - Control shutdown";
  if (withEndl)
    ts << endl;
}

/**
 * Performs the test and returns the testing object for final inspection.
 */
boost::shared_ptr<LogEntryStorerForTesting> performTest(
      boost::function<void(QTextStream&)> prepare )
{
  QString s;
  boost::shared_ptr<QTextStream> textStream( new QTextStream(&s) );
  fillStream(*textStream);

  boost::shared_ptr<ParserStreamGetter> getter( new ParserStreamGetter("name", textStream) );
  boost::shared_ptr<LogEntryParser> parser( new LogEntryParser_Logfile( getter ) );

  boost::shared_ptr<LogEntryStorerForTesting> tester( new LogEntryStorerForTesting( parser.get() ) );
  tester->start();
  return tester;
}

BOOST_AUTO_TEST_CASE( bugLastLogLineWithoutEndl )
{
  boost::shared_ptr<LogEntryStorerForTesting> tester = performTest( boost::bind(&fillStream, _1, false) );
  BOOST_CHECK( tester->m_finished );

  BOOST_CHECK_EQUAL( tester->m_entries.size(), 4 );
  BOOST_CHECK_EQUAL( tester->m_errors.size(), 0 );
}

BOOST_AUTO_TEST_CASE( bugLastLogLineWithEndl )
{
  boost::shared_ptr<LogEntryStorerForTesting> tester = performTest( boost::bind(&fillStream, _1, true) );
  BOOST_CHECK( tester->m_finished );

  BOOST_CHECK_EQUAL( tester->m_entries.size(), 4 );
  BOOST_CHECK_EQUAL( tester->m_errors.size(), 0 );
}


