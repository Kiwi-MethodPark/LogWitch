/*
 * TimeFormatingModel.h
 *
 *  Created on: Dec 30, 2011
 *      Author: sven
 */

#ifndef TIMEFORMATINGMODEL_H_
#define TIMEFORMATINGMODEL_H_
#include <QtGui>

class TimeFormatingModel
: public QSortFilterProxyModel // This shpuld be moved to the identity model in QT4.8 ....
{
    Q_OBJECT
public:
    TimeFormatingModel( QObject *parent );

    QVariant data(const QModelIndex &index, int role) const;

    /**
     * Enables or disables the TimeDiff mode.
     */
    void setTimeDiffModeEnabled( bool value );

    /**
     * Sets the reference row for the differences.
     */
    void setTimeDiffReferenceRow( int row = -1 );

private:
    QString m_formatString;

    /**
     * This is the row which should be used for the TimeDiff mode. If it is set
     * to a value < 0, the timeDiff mode will work only between two entries.
     */
    int m_referenceRowForTimeDiffs;

    /**
     * Enable the timeDiffMode.
     */
    bool m_timeDiffMode;
};

#endif /* TIMEFORMATINGMODEL_H_ */
