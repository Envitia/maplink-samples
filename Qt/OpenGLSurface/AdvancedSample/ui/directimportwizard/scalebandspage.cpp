/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "scalebandspage.h"

#include "directimportwizard.h"

#include <iostream>
#include <set>
#include <sstream>

#include <QMessageBox>

ScaleBandsPage::ScaleBandsPage( QWidget *parent )
  : QWizardPage( parent )
  , m_scaleBandsTableModel( this )
{
  setupUi( this );

  connect( scaleBandAddButton, SIGNAL( clicked() ), this, SLOT( addScaleBand() ) );
  connect( scaleBandRemoveButton, SIGNAL( clicked() ), this, SLOT( removeScaleBand() ) );
}

ScaleBandsPage::~ScaleBandsPage()
{
  // Map is deleted when 'scaleBandsTable' is deleted, from inside subclass: 'scalebandstable.cpp'
}

void ScaleBandsPage::initializePage()
{
  DirectImportWizard* wiz( qobject_cast<DirectImportWizard*>(wizard()));
  m_scaleBandsTableModel.dataLayer( wiz->directImportLayer() );
  scaleBandsTableView->setModel( &m_scaleBandsTableModel );
}

void ScaleBandsPage::addScaleBand()
{
  m_scaleBandsTableModel.insertRow( m_scaleBandsTableModel.rowCount() );
}

void ScaleBandsPage::removeScaleBand()
{
  QModelIndexList selectedIndexes( scaleBandsTableView->selectionModel()->selectedIndexes() );

  // flatten to a list of rows, as multiple cells per row may be selected
  std::set<int> rows;
  for ( unsigned int i( 0 ); i < selectedIndexes.size(); ++i )
  {
    QModelIndex index( selectedIndexes[i] );
    rows.insert( index.row() );
  }

  // Remove all selected rows.
  // Go backwards, as removeRow alters the indexes of rows below the one being removed.
  for ( std::set<int>::reverse_iterator it = rows.rbegin(); it != rows.rend(); ++it )
  {
    int index = *it;
    m_scaleBandsTableModel.removeRow( index );
  }
}

bool ScaleBandsPage::validatePage()
{
  if( m_scaleBandsTableModel.scaleBandsValid() == false )
  {
    QMessageBox::warning( this, "Invalid Scale Band Configuration",
                          "An invalid scale band configuration has been specified. All fields must be populated and bands must be ordered from lowest detail to highest detail.");
    return false;
  }
  // Commit to the layer
  // If this fails the layer will be left in an invalid state
  if( m_scaleBandsTableModel.saveToLayer() == false )
  {
    QMessageBox::critical( this, "Failed to add scale bands to direct import layer",
                           "The specified scale bands could not be added to the direct import layer.");
    return false;
  }
  return true;
}

