/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "helpdialog.h"
#include <QUrl>
#include "MapLink.h"

HelpDialog::HelpDialog( QWidget *parent )
{
  setupUi( this );

  // Turn off the help button as there is no additional help to display
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  

  std::string helpIndex = TSLUtilityFunctions::getMapLinkHome();
  helpIndex += "/Samples/Qt/OGCViewer/doc/index.html";
  if( TSLFileHelper::fileExists( helpIndex.c_str() ))
  {
    webView->load(QUrl::fromLocalFile(helpIndex.c_str()));
  }
  else
  {
    QString errorMessage = "<html><body><p>Help not found: ";
    errorMessage += helpIndex.c_str();
    errorMessage += "</body></html>";
    webView->setHtml( errorMessage );
  }
}

HelpDialog::~HelpDialog()
{
}