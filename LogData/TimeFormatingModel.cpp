/*
 * TimeFormatingModel.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: sven
 */

#include "TimeFormatingModel.h"

TimeFormatingModel::TimeFormatingModel( QObject *parent )
: QSortFilterProxyModel( parent )
, m_formatString("dd.MM.yyyy hh:mm:ss.zzz")
, m_referenceRowForTimeDiffs( 0 )
, m_timeDiffMode( false )
{
}

void TimeFormatingModel::setTimeDiffModeEnabled( bool value )
{
    m_timeDiffMode = value;
}

void TimeFormatingModel::setTimeDiffReferenceRow( int row )
{
    m_referenceRowForTimeDiffs = row;
}

QVariant TimeFormatingModel::data(const QModelIndex &index, int role) const
{
    QVariant var = QAbstractProxyModel::data( index, role );

    if( role == Qt::DisplayRole )
    {
        if( var.type() == QVariant::DateTime )
        {
            if( m_timeDiffMode )
            {
                const int row = m_referenceRowForTimeDiffs >= 0 ? m_referenceRowForTimeDiffs : index.row() - 1;

                if( row >= 0 && row <= sourceModel()->rowCount() )
                {
                    QModelIndex idxBefore = sourceModel()->index( row, index.column() );
                    QVariant before = sourceModel()->data( idxBefore, role );

                    QDateTime befTime = before.toDateTime();
                    QDateTime yetTime = var.toDateTime();

                    qint64 diffMsec = yetTime.toMSecsSinceEpoch() - befTime.toMSecsSinceEpoch();
                    const bool neg = diffMsec < 0;
                    diffMsec *= neg ? -1 : 1;

                    qint64 secs = (neg ? -1 : 1 )*( diffMsec / 1000 );
                    qint64 ms = diffMsec % 1000;

                    QString diff = QString("%1.%2").arg( secs ).arg( ms, 3, 10, QChar('0') );
                    var = QVariant( diff );
                }
                else
                {
                    if( m_referenceRowForTimeDiffs < 0 )
                        var = QString("0.000");
                    else
                        var = QString("N/A");
                }
            }
            else
            {
                var = QVariant( var.toDateTime().toString( m_formatString ) );
            }
        }
    }

    return var;
}
