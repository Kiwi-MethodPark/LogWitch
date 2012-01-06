/*
 * LogEntryParserModelConfiguration.cpp
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#include "LogData/LogEntryParserModelConfiguration.h"
#include "EntryToTextFormaterDefault.h"

LogEntryParserModelConfiguration::LogEntryParserModelConfiguration( const QString &configurationString )
    : m_formater( new EntryToTextFormaterDefault )
    , m_configurationString( configurationString )
{

}

LogEntryParserModelConfiguration::~LogEntryParserModelConfiguration()
{

}

QString LogEntryParserModelConfiguration::getHierarchySplitString( int idx ) const
{
    if( idx >= 0 && idx < m_hierarchySplitstrings.size() )
        return m_hierarchySplitstrings[idx];
    else
        return "";
}

void LogEntryParserModelConfiguration::setHierarchySplitString( int idx, const QString & fmt)
{
    if( idx >= m_hierarchySplitstrings.size() )
        m_hierarchySplitstrings.resize(idx+1);

    m_hierarchySplitstrings[idx] = fmt;
}


int LogEntryParserModelConfiguration::getFieldWidthHint( int idx ) const
{
    if( idx < (int)m_fieldWidthHints.size() )
        return m_fieldWidthHints[idx] < 0 ? 150 : m_fieldWidthHints[idx];
    else
        return 150;
}

void LogEntryParserModelConfiguration::setFieldWidthHint( int idx, int width )
{
    while( idx >= (int)m_fieldWidthHints.size() )
        m_fieldWidthHints.push_back(-1);

    m_fieldWidthHints[idx] = width;
}

bool LogEntryParserModelConfiguration::getFieldShowHint( int idx ) const
{
    if( idx < (int)m_fieldShowHint.size() )
        m_fieldShowHint[idx];
    else
        return true;
}

void LogEntryParserModelConfiguration::setFieldShowHint( int idx, bool show )
{
    while( idx >= (int)m_fieldShowHint.size() )
        m_fieldShowHint.push_back(true);

    m_fieldShowHint[idx] = show;
}

const std::vector<int> &LogEntryParserModelConfiguration::getFieldOrderHint() const
{
    return m_fieldOrderHint;
}

void LogEntryParserModelConfiguration::setFieldOrderHint( const std::vector<int> &in )
{
    m_fieldOrderHint = in;
}
