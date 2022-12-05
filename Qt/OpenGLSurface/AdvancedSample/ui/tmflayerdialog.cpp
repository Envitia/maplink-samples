/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "tmflayerdialog.h"

TMFLayerDialog::TMFLayerDialog( QWidget *parent )
  : QDialog( parent )
{
  setupUi( this );
}

TMFLayerDialog::~TMFLayerDialog()
{

}

void TMFLayerDialog::setDataLabel( QString labelText )
{
  labelDataType->setText( labelText );
}

void TMFLayerDialog::setLayerNameBox( QString layerName )
{
  layerNameText->setText( layerName );
}

void TMFLayerDialog::setCoordinateSystemBox( QString coordinateSys )
{
  coordinateSystemBox->setCurrentText( coordinateSys );
}

void TMFLayerDialog::setStylingText( QString styling )
{
  stylingText->setText( styling );
}

QString TMFLayerDialog::getCoordinateSystemBox()
{
  return coordinateSystemBox->currentText();
}

QString TMFLayerDialog::getLayerNameBox()
{
  return layerNameText->text();
}

QString TMFLayerDialog::getStylingText()
{
  return stylingText->text();
}

QDialogButtonBox* TMFLayerDialog::getButtonBox()
{
  return buttonBox;
}

QPushButton* TMFLayerDialog::getBrowseButton()
{
  return browseButton;
}
