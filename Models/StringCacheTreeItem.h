/*
 * StringCacheTreeItem.h
 *
 *  Created on: May 15, 2011
 *      Author: sven
 */

#ifndef STRINGCACHETREEITEM_H_
#define STRINGCACHETREEITEM_H_

#include <boost/shared_ptr.hpp>
#include <list>
#include "Types.h"
#include <QtCore/QtCore>

class StringCacheTreeItem {
public:
	enum Check
	{
	    Inherit, // Inherit from parent or checked if top
		Checked,
		Unchecked
	};

//	enum CheckState
//	{
//	    ForcedChecked,
//	    ForcedUnchecked,
//	    Checked,
//	    Unchecked,
//	    PartialCheckedChilds_SelfChecked,
//	    PartialCheckedChilds_SelfUnchecked
//	};

	class CheckState
	{
	public:
	    CheckState() : forced( false ), checked( true ), partial(false ) { }

	    bool forced;
	    bool checked;
	    bool partial;
	};

public:
	StringCacheTreeItem( TSharedConstQString originalString, TSharedConstQString str, StringCacheTreeItem *parent = NULL );
	virtual ~StringCacheTreeItem();

    void appendChild(StringCacheTreeItem *child);

    StringCacheTreeItem *child(int row);
    int childCount() const;


    int row() const;
    StringCacheTreeItem *parent();

    /**
     * This is the readable name which need not be the same as the string from the StringCache
     */
    TSharedConstQString getString() const;

    /**
     * This function returns the original string from the StringCache which can be used for a direct pointer
     */
    TSharedConstQString getOriginalString() const ;

    void setOriginalString( TSharedConstQString str);

    Qt::CheckState getChecked() const
    {
        CheckState c = getCheckState();

    	if( c.partial )
    		return Qt::PartiallyChecked;
    	else if( c.checked )
    		return Qt::Checked;
    	else
    		return Qt::Unchecked;
    }

    Check getCheckedSelf( ) { return m_checkedSelf; }
    void setCheckedSelf( Check c ) { m_checkedSelf = c; }
    Check getCheckedChild( ) { return m_checkedChilds; }

    CheckState getCheckState( ) const;

    CheckState &getCheckState( CheckState &state, const StringCacheTreeItem *item ) const;
    void nextChecked();

    void recursiveSetTree( Check self, Check child );
    void recursiveSetTree( Check self );

    StringCacheTreeItem *getRootElement();

    void checkOnlyThisElement( bool includeTreepath = true, Check child = Checked, Check self=Checked);

private:
    typedef std::vector< StringCacheTreeItem *> TItemVector;
    TItemVector m_childItems;
    StringCacheTreeItem *m_parentItem;

    Check m_checkedSelf;
    Check m_checkedChilds;

    TSharedConstQString m_string;

    TSharedConstQString m_stringOriginal;
};

#endif /* STRINGCACHETREEITEM_H_ */
