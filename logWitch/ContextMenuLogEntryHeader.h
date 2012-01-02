/*
 * ContextMenuLogEntryHeader.h
 *
 *  Created on: Dec 29, 2011
 *      Author: sven
 */

#ifndef CONTEXTMENULOGENTRYHEADER_H_
#define CONTEXTMENULOGENTRYHEADER_H_
#include <QtGui>

/**
 * This class is responsible for showing the context menu which
 * manipulates the horizontal header of the log entry table window.
 *
 */
class ContextMenuLogEntryHeader
: public QMenu
{
    Q_OBJECT
public:
    ContextMenuLogEntryHeader( QHeaderView *header );

public slots:
    void contextMenuRequest( const QPoint & pos );

    void hideColumn();

    void showColumn( int col );

private:
    /// This is the logical index of the header the user has clicked on.
    int m_headerToWorkOn;

    /// this is the header we are responsible for.
    QHeaderView *m_header;

    /// Our hide / show action
    QAction *m_hideAction;

    /// Menu for showing hidden columns
    QMenu *m_showMenu;
};

#endif /* CONTEXTMENULOGENTRYHEADER_H_ */
