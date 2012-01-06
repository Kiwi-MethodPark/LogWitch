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
 *
 * The hints are responsible for the configuration of the views. The parser needs
 * to implement this in a favorable way. If the parser does nothing, the stored
 * default values will be used.
 */
class LogEntryParserModelConfiguration
{
public:
    /**
     * Constructs configuration.
     *
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
	 * Returns an entry to text formatter for this types of entries.
	 * The formatter is used to format a logEntry to a human readable html.
	 * This html can than be displayed to the user.
	 */
	boost::shared_ptr<EntryToTextFormater> getEntryToTextFormater() const { return m_formater;};

	void setEntryToTextFormater( boost::shared_ptr<EntryToTextFormater> fmt ){ m_formater = fmt;}

	/**
	 * Returns the corresponding factory for creating log entries.
	 * This factory also hold the names for the columns referenced here.
	 */
	boost::shared_ptr<const LogEntryFactory> getLogEntryFactory() const { return m_attr;}

	void setLogEntryFactory( boost::shared_ptr<LogEntryFactory> factory) { m_attr = factory;}

	/**
	 * Returns the suggested width of the column.
	 *
	 * @see setFieldWidthHint
	 */
	int getFieldWidthHint( int idx ) const;

	/**
	 * Sets field width hint for a given column with the width.
	 * If def is set to true, this value is marked as a default
	 * value. Default values can be overwritten by the saved settings.
	 */
	void setFieldWidthHint( int idx, int width, bool def );

	/**
	 * Returns the show hints.
	 *
	 * @return true if the column should be shown, false otherwise.
	 *
	 * @see setFieldWidthHint
	 */
	bool getFieldShowHint( int idx ) const;

    /**
     * Sets field show hint for a given column..
     * If def is set to true, this value is marked as a default
     * value. Default values can be overwritten by the saved settings.
     */
	void setFieldShowHint( int idx, bool show, bool def );

	/**
	 * Gets the order hints.
	 *
	 * @see setFieldShowHint
	 */
	int getFieldOrderHint( int idx ) const;

    /**
     * Sets order hint. The numbers inside the array will be used
     * to determine the order mapping. The index of the array is
     * the column. The value is a kind of priority, lower priorities are
     * more to the left, higher to the right side. If there are columns
     * missing, the order of them will get a higher number.
     *
     * If def is set to true, this value is marked as a default
     * value. Default values can be overwritten by the saved settings.
     */
	void setFieldOrderHint( const std::vector<int> &, bool def );

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
