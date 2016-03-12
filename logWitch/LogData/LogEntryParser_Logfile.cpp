/*
 * LogEntryParser_Logfile.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntryParser_Logfile.h"

#include <boost/assign/list_of.hpp>
#include <QtCore/QtCore>
#include <QRegExp>

#include "LogEntryFactory.h"
#include "LogEntryAttributeNames.h"
#include "LogData/LogEntry.h"
#include "LogData/LogEntryFactory.h"
#include "LogData/LogEntryParserModelConfiguration.h"
#include "LogData/ParserStreamGetter.h"

LogEntryParser_Logfile::LogEntryParser_Logfile(  boost::shared_ptr<ParserStreamGetter> getter  )
  : m_abort(false )
  , m_getter(getter)
  , lineMessageRegex( new QRegExp("^([\\d-]+\\s+[\\d\\,\\:]+)\\s+-\\s+((?:(?!\\s+-\\s+).)*)\\s+-\\s+((?:(?!\\s+-\\s+).)*)\\s+-\\s+(\\[(.*)\\]|((?!\\s+-\\s+).)*)\\s+-\\s+(.*)$") )
  , cellRegex( "\\s+-\\s+" )
  , timeFormat( "yyyy-MM-dd HH:mm:ss,zzz" )
  , myFactory( new LogEntryFactory )
  , m_logEntryNumber( 0 )
{
  lineMessageRegex->setMinimal(true);

  // Preparing attributes in factory
  LogEntryAttributeNames names;
  myFactory->addField(names.getConfiguration("number"));
  myFactory->addField(names.getConfiguration("timestamp"));
  myFactory->addField(names.getConfiguration("message"));

  myFactory->addField(names.getConfiguration("level"));
  myFactory->addField(names.getConfiguration("logger"));
  myFactory->addField(names.getConfiguration("fsource"));
  myFactory->disallowAddingFields();

  m_myModelConfig = boost::shared_ptr<LogEntryParserModelConfiguration>(
      new LogEntryParserModelConfiguration("Logfile", myFactory));
  m_myModelConfig->setHierarchySplitString(4, "\\.");

  for (int i = 0; i < myFactory->getNumberOfFields(); ++i)
  {
    m_myModelConfig->setFieldWidthHint(i,
        myFactory->getFieldConfiguration(i).defaultCellWidth, true);
  }

  m_myModelConfig->setFieldOrderHint(
      QVector<int>::fromStdVector(boost::assign::list_of(0)(5)(1)(2)(3)(4)),
      true);
}

LogEntryParser_Logfile::~LogEntryParser_Logfile()
{
  m_abort = true;
  wait();
}

QString LogEntryParser_Logfile::getName() const
{
  return m_getter->getName();
}

void LogEntryParser_Logfile::startEmiting()
{
  if (!isRunning() && !m_abort)
    start(LowPriority);
}

boost::shared_ptr<LogEntryParserModelConfiguration> LogEntryParser_Logfile::getParserModelConfiguration() const
{
  return m_myModelConfig;
}

bool LogEntryParser_Logfile::initParser()
{
  m_logfileStream = m_getter->getStream();

  if (!m_logfileStream)
  {
    m_initError = m_getter->getError();
    return false;
  }

  return true;
}

void LogEntryParser_Logfile::run()
{
  int i = 0;

  TSharedNewLogEntryMessage newEntryMessage(new NewLogEntryMessage);

  forever
  {
    if (m_abort)
      return;

    TSharedLogEntry entry(getNextLogEntry());

    if (entry)
      newEntryMessage->entries.push_back(entry);
    else
    {
      m_abort = true;
      emit newEntry(newEntryMessage);
      qDebug() << "We got " << i << " entries from logfile.";

      emit finished();
      return;
    }

    i++;
  }
}

TSharedLogEntry LogEntryParser_Logfile::getNextLogEntry()
{
  TSharedLogEntry entryReturn;

  if (m_logfileStream)
  {
    bool entryComplete = false;

    while (!entryComplete)
    {
      if (stashedLine.isEmpty() && !m_logfileStream->atEnd())
        stashedLine = m_logfileStream->readLine();

      if (m_logfileStream->atEnd() && stashedLine.isEmpty())
      {
        // End of logfile
        entryComplete = true;
      }
      else
      {
        //qDebug() << "StashedLine = " << stashedLine;
        int idx = -1;
        if ((idx = lineMessageRegex->indexIn(stashedLine)) != -1)
        {
          if (m_entry) // first entry
          {
            //qDebug() << "Appending Message to last entry = " << message;

            m_entry->setAttribute(QVariant(++m_logEntryNumber), 0);
            m_entry->setAttribute(QVariant(QString(message)), 2);
            entryComplete = true;
            entryReturn = m_entry;
          }

          m_entry = myFactory->getNewLogEntry();

          m_entry->setAttribute(
              QVariant(
                  QDateTime::fromString(lineMessageRegex->cap(1), timeFormat)),
              1);
          // File Source
          if (lineMessageRegex->cap(5).isEmpty())
            m_entry->setAttribute(QVariant(lineMessageRegex->cap(4)), 5);
          else
            m_entry->setAttribute(QVariant(lineMessageRegex->cap(5)), 5);

          message = lineMessageRegex->cap(7);

          // Severity
          m_entry->setAttribute(QVariant(lineMessageRegex->cap(2)), 3);
          // Component
          m_entry->setAttribute(QVariant(lineMessageRegex->cap(3)), 4);
          /*
           qDebug() << "Entry detected: timestamp = "<< timestamp
           << " 0 " << lineMessageRegex->cap(0)
           << " 1 " << lineMessageRegex->cap(1)
           << " 2 " << lineMessageRegex->cap(2)
           << " 3 " << lineMessageRegex->cap(3)
           << " 4 " << lineMessageRegex->cap(4)
           << " 5 " << lineMessageRegex->cap(5)
           << " 6 " << lineMessageRegex->cap(6)
           << " IDX: " << idx;
           */

        }
        else
        {
          //qDebug() << " appending stashed line to message";
          message += "\n" + stashedLine;
        }

        stashedLine = "";
      }
    }
  }

  return entryReturn;
}
