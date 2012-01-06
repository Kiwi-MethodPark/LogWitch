/*
 * LogEntryParserModelConfiguration.h
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#ifndef LOGENTRYPARSERMODELCONFIGURATION_H_
#define LOGENTRYPARSERMODELCONFIGURATION_H_
#include <QtCore/QtCore>
#include <boost/shared_ptr.hpp>


class EntryToTextFormater;
class LogEntryFactory;
/**
 * This class is responsible for modeling different kinds of behavior for the
 * GUI which is defined by the logfile and its structure.
 */
class LogEntryParserModelConfiguration
{
public:
    /**
     * Constructs configuration.
     * @param configurationString This string is an identifier to save and load default settings.
     */
	LogEntryParserModelConfiguration( const QString &configurationString );
	virtual ~LogEntryParserModelConfiguration();

	/**
	 * Returns if an entry field is a hierarchy and returns the string for a regex to split it.
	 */
	QString getHierarchySplitString( int idx ) const;

	void setHierarchySplitString( int idx, const QString & fmt);

	/**
	 * Returns an entry to text formater for this types of entries.
	 */
	boost::shared_ptr<EntryToTextFormater> getEntryToTextFormater() const { return m_formater;};

	void setEntryToTextFormater( boost::shared_ptr<EntryToTextFormater> fmt ){ m_formater = fmt;}

	/**
	 * Returns the corresponding factory for creating log entries.
	 */
	boost::shared_ptr<const LogEntryFactory> getLogEntryFactory() const { return m_attr;}

	void setLogEntryFactory( boost::shared_ptr<LogEntryFactory> factory) { m_attr = factory;}

	int getFieldWidthHint( int idx ) const;
	void setFieldWidthHint( int idx, int width );

	bool getFieldShowHint( int idx ) const;
	void setFieldShowHint( int idx, bool show );

	const std::vector<int> &getFieldOrderHint() const;
	void setFieldOrderHint( const std::vector<int> & );

private:
	QVector<QString> m_hierarchySplitstrings;

	boost::shared_ptr<EntryToTextFormater> m_formater;

	boost::shared_ptr<LogEntryFactory> m_attr;

	std::vector<int> m_fieldWidthHints;

	std::vector<bool> m_fieldShowHint;

	std::vector<int> m_fieldOrderHint;

	const QString m_configurationString;
};

typedef boost::shared_ptr<LogEntryParserModelConfiguration> TSharedLogEntryParserModelConfiguration;
typedef boost::shared_ptr<const LogEntryParserModelConfiguration> TSharedConstLogEntryParserModelConfiguration;

#endif /* LOGENTRYPARSERMODELCONFIGURATION_H_ */
