/*
 * DialogExportOptions.cpp
 *
 *  Created on: 26 Oct 2013
 *      Author: sven
 */

#include "DialogExportOptions.h"

DialogExportOptions::DialogExportOptions(QWidget * parent)
: QDialog(parent)
{
    ui.setupUi(this);
}

void DialogExportOptions::setExportOption( ExportOption o )
{
    switch( o )
    {
    case Selection:
        ui.radioButtonSel->setChecked(true);
        break;
    case SelectionWithoutFilter:
        ui.radioButtonSelHidden->setChecked(true);
        break;
    case All:
        ui.radioButtonAll->setChecked(true);
        break;
    case None:
        ui.radioButtonSel->setChecked(false);
        ui.radioButtonSelHidden->setChecked(false);
        ui.radioButtonAll->setChecked(false);
        break;
    }
}

DialogExportOptions::ExportOption DialogExportOptions::getExportOption() const
{
    if (ui.radioButtonSel->isChecked())
        return Selection;
    else if (ui.radioButtonSelHidden->isChecked())
        return SelectionWithoutFilter;
    else if (ui.radioButtonAll->isChecked())
        return All;
    else
        return None;
}
