/*
 * ParserStreamGetter.cpp
 *
 *  Created on: 12 Mar 2016
 *      Author: sven
 */

#include "LogData/ParserStreamGetter.h"

ParserStreamGetter::ParserStreamGetter(  const QString name, boost::shared_ptr<QTextStream> textStream )
: m_name( name )
, m_textStream( textStream )
{
}

ParserStreamGetter::~ParserStreamGetter()
{
}

