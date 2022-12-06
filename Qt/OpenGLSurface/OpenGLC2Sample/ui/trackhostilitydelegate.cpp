/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "trackhostilitydelegate.h"
#include <QComboBox>

#include "MapLink.h"
#include "tracks/trackmanager.h"

TrackHostilityDelegate::TrackHostilityDelegate( QWidget *parent )
  : QStyledItemDelegate( parent )
{
}

TrackHostilityDelegate::~TrackHostilityDelegate()
{
}

QWidget* TrackHostilityDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
  const TrackInfoModel *model = reinterpret_cast<const TrackInfoModel*>( index.model() );
  if( !model->isHostilityCell( index ) )
  {
    // This cell does not contain hostility information
    return NULL;
  }

  QComboBox *hostility = new QComboBox( parent );
  hostility->setFrame( false );
  hostility->setEditable( false );

  // Fill the combo box with the available hostilities
  hostility->addItem( QStringLiteral("Assumed Friend"), TSLAPP6ASymbol::HostilityAssumedFriend );
  hostility->addItem( QStringLiteral("Faker"), TSLAPP6ASymbol::HostilityFaker );
  hostility->addItem( QStringLiteral("Friend"), TSLAPP6ASymbol::HostilityFriend );
  hostility->addItem( QStringLiteral("Hostile"), TSLAPP6ASymbol::HostilityHostile );
  hostility->addItem( QStringLiteral("Joker"), TSLAPP6ASymbol::HostilityJoker );
  hostility->addItem( QStringLiteral("Neutral"), TSLAPP6ASymbol::HostilityNeutral );
  hostility->addItem( QStringLiteral("Pending"), TSLAPP6ASymbol::HostilityPending );
  hostility->addItem( QStringLiteral("Suspect"), TSLAPP6ASymbol::HostilitySuspect );
  hostility->addItem( QStringLiteral("Unknown"), TSLAPP6ASymbol::HostilityUnknown );

  return hostility;
}

void TrackHostilityDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  // The user role data contains the current track hostility
  QComboBox *hostility = reinterpret_cast<QComboBox*>( editor );

  switch( index.data( Qt::UserRole ).toInt() )
  {
  case TSLAPP6ASymbol::HostilityAssumedFriend:
    hostility->setCurrentIndex( 0 );
    break;

  case TSLAPP6ASymbol::HostilityFaker:
    hostility->setCurrentIndex( 1 );
    break;

  case TSLAPP6ASymbol::HostilityFriend:
    hostility->setCurrentIndex( 2 );
    break;

  case TSLAPP6ASymbol::HostilityHostile:
    hostility->setCurrentIndex( 3 );
    break;

  case TSLAPP6ASymbol::HostilityJoker:
    hostility->setCurrentIndex( 4 );
    break;

  case TSLAPP6ASymbol::HostilityNeutral:
    hostility->setCurrentIndex( 5 );
    break;

  case TSLAPP6ASymbol::HostilityPending:
    hostility->setCurrentIndex( 6 );
    break;

  case TSLAPP6ASymbol::HostilitySuspect:
    hostility->setCurrentIndex( 7 );
    break;

  case TSLAPP6ASymbol::HostilityUnknown:
    hostility->setCurrentIndex( 8 );
    break;

  default:
    break;
  }
}

void TrackHostilityDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  // Forward the change in hostility to the model, which will update the track information
  QComboBox *hostility = reinterpret_cast<QComboBox*>( editor );
  model->setData( index, hostility->itemData( hostility->currentIndex() ), Qt::EditRole );
}

void TrackHostilityDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
  editor->setGeometry( option.rect );
}
