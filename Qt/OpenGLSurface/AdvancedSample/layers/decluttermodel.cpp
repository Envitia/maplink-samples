/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include <map>
#include <QStringList>
#include <QWidget>

#include "decluttermodel.h"
#include "MapLink.h"

using namespace std;


DeclutterModel::DeclutterNode::DeclutterNode( DeclutterNode *parent, int index, const QString &displayName, const QString &featureName, const QString &layerName )
  : m_parent( parent )
  , m_index( index )
  , m_displayName( displayName )
  , m_featureName( featureName )
  , m_layerName( layerName )
{
}

DeclutterModel::DeclutterNode::~DeclutterNode()
{
  for( size_t i = 0; i < m_children.size(); ++i )
  {
    delete m_children[i];
  }
}

DeclutterModel::DeclutterModel()
  : m_rootNode( new DeclutterNode( NULL, 0, QString(), QString(), QString() ) )
  , m_surface( NULL )
  , m_updateView( NULL )
{
}

DeclutterModel::~DeclutterModel()
{
  delete m_rootNode;
}

Qt::ItemFlags DeclutterModel::flags( const QModelIndex &index ) const
{
  if( !index.isValid() )
  {
    return 0;
  }

  DeclutterNode *node = reinterpret_cast< DeclutterNode* >( index.internalPointer() );
  if( !node->m_featureName.isEmpty() )
  {
    // Nodes with a feature name can be checked/unchecked to toggle the visibility of that feature
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
  }
  else
  {
    return Qt::ItemIsEnabled;
  }
}

QVariant DeclutterModel::data( const QModelIndex &index, int role ) const
{
  if( !m_rootNode || !index.isValid() )
  {
    return QVariant();
  }

  DeclutterNode *node = reinterpret_cast< DeclutterNode* >( index.internalPointer() );

  switch( role )
  {
    case Qt::DisplayRole:
    {
      return QVariant( node->m_displayName );
    }

    case Qt::CheckStateRole:
    {
      // Only nodes that are from a layer can be checkable
      if( !node->m_featureName.isEmpty() )
      {
        // Query whether this feature is decluttered or not and set the checked state appropriately
        TSLDeclutterStatusResultEnum declutterStatus = TSLDeclutterStatusResultOff;

        std::string featureName( node->m_featureName.toUtf8() );
        std::string layerName( node->m_layerName.toUtf8() );
        m_surface->getDeclutterStatus( featureName.c_str(), &declutterStatus, layerName.c_str() );
        switch( declutterStatus )
        {
        case TSLDeclutterStatusResultOn:
          return QVariant( Qt::Checked );

        case TSLDeclutterStatusResultOff:
          return QVariant( Qt::Unchecked );

        case TSLDeclutterStatusResultPartial:
          return QVariant( Qt::PartiallyChecked );

        default:
          break;
        }
      }
      break;
    }

    default:
      break;
  }

  return QVariant();
}

QVariant DeclutterModel::headerData( int /*section*/, Qt::Orientation /*orientation*/, int /*role*/ ) const
{
  return QVariant();
}

int DeclutterModel::rowCount( const QModelIndex &parent ) const
{
  if( !m_rootNode || parent.column() > 0 )
  {
    return 0;
  }

  DeclutterNode *node = parent.isValid() ? reinterpret_cast< DeclutterNode* >( parent.internalPointer() ) : m_rootNode;
  return (int)node->m_children.size();
}

int DeclutterModel::columnCount( const QModelIndex& /*parent*/ ) const
{
  return 1;
}

QModelIndex DeclutterModel::index( int row, int column, const QModelIndex &parent ) const
{
  if( !m_rootNode || !hasIndex( row, column, parent ) )
  {
    return QModelIndex();
  }

  DeclutterNode *node = parent.isValid() ? reinterpret_cast< DeclutterNode* >( parent.internalPointer() ) : m_rootNode;
  if( node->m_children.size() <= row )
  {
    return QModelIndex();
  }

  return createIndex( row, column, node->m_children[row] );
}

QModelIndex DeclutterModel::parent ( const QModelIndex &index ) const
{
  if( !index.isValid() || !m_rootNode )
  {
    return QModelIndex();
  }

  DeclutterNode *node = reinterpret_cast< DeclutterNode* >( index.internalPointer() );
  if( !node->m_parent || node->m_parent == m_rootNode )
  {
    // Do not return a model index for the root node, see the Qt documentation on QAbstractItemModel
    return QModelIndex();
  }

  return createIndex( node->m_parent->m_index, 0, node->m_parent );
}

bool DeclutterModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  if( !index.isValid() || !m_surface )
  {
    return false;
  }

  DeclutterNode *node = reinterpret_cast< DeclutterNode* >( index.internalPointer() );
  if( node->m_layerName.isEmpty() )
  {
    // This node is not checkable and therefore can't be modified
    return false;
  }

  switch( role )
  {
    case Qt::CheckStateRole:
      {
        // Update the declutter status of the feature name for this node based on whether it is being checked or unchecked
        std::string featureName( node->m_featureName.toUtf8() );
        std::string layerName( node->m_layerName.toUtf8() );
        if( m_surface->setDeclutterStatus( featureName.c_str(), value == Qt::Checked ? TSLDeclutterStatusOn : TSLDeclutterStatusOff,
                                           layerName.c_str() ) )
        {
          // Tell Qt that the relevant items have changed. Altering a node changes that node, all it's parent nodes and all its child nodes.
          dataChanged( index, index );

          DeclutterNode *parentNode = node;
          while( parentNode->m_parent )
          {
            parentNode = parentNode->m_parent;
            QModelIndex parentIndex( createIndex( parentNode->m_index, 0, parentNode ) );

            dataChanged( parentIndex, index );
          }

          updateChildNodes( node );

          m_updateView->update();
          return true;
        }

        return false;
      }

    default:
      return false;
  }
}

void DeclutterModel::addLayerFeatures( const QString &layerName, TSLDataLayer *layer )
{
  // Tell the model and any attached views that the data has changed and needs reloading
  beginResetModel();

  // Delete any existing node for this layer
  vector< DeclutterNode* >::iterator nodeIt( m_rootNode->m_children.begin() );
  vector< DeclutterNode* >::iterator nodeItE( m_rootNode->m_children.end() );
  for( ; nodeIt != nodeItE; ++nodeIt )
  {
    if( (*nodeIt)->m_layerName == layerName )
    {
      delete *nodeIt;
      m_rootNode->m_children.erase( nodeIt );
      break;
    }
  }

  // Create a node for this layer
  DeclutterNode *layerNode = new DeclutterNode( m_rootNode, (int)m_rootNode->m_children.size(), layerName, QString(), layerName );
  m_rootNode->m_children.push_back( layerNode );

  // Create child nodes for each feature in the layer's feature class list. Feature groups are
  // categorised by a '.', so use this to build up the tree of features
  std::map< QString, DeclutterNode* > featureGroups;
  featureGroups.insert( std::make_pair( QString(), layerNode ) );

  const TSLFeatureClassList *featureList = layer->featureList();
  if( layerNode )
  {
    int numFeatures = featureList->size();
    for( int i = 0; i < numFeatures; ++i )
    {
      QString featureName = QString::fromUtf8( featureList->getDetails( i, NULL ) );

      // Tokenize the feature name around '.'. This defines the nodes that we need to create/use
      // in order to create a tree that matches the feature name.
      QStringList tokens = featureName.split( '.' );
      
      // Build up the feature name from it's constituent parts, ensuring we have nodes for each
      // partial feature name.
      QString parentFeatureName;
      for( int token = 0; token < tokens.size(); ++token )
      {
        QString currentFeatureName( parentFeatureName );
        if( token != 0 )
        {
          currentFeatureName.append( '.' );
        }
        currentFeatureName.append( tokens.at(token) );

        if( featureGroups.find( currentFeatureName ) == featureGroups.end() )
        {
          // We have no node for this partial feature name, create one now. We need the
          // node for the parent partial feature to do this, which will exist at this point. 
          DeclutterNode *parentNode = featureGroups[parentFeatureName];
          DeclutterNode *newNode = new DeclutterNode( parentNode, (int)parentNode->m_children.size(), tokens.at(token), currentFeatureName, layerName );
          parentNode->m_children.push_back( newNode );
          featureGroups.insert( std::make_pair( currentFeatureName, newNode ) );
        }

        parentFeatureName = currentFeatureName;
      }
    }
  }

  endResetModel();
}

void DeclutterModel::removeLayerFeatures( const QString &layerName )
{
  // Find the layer node that corresponds to this layer (if there is one)
  vector< DeclutterNode* >::iterator layerIt( m_rootNode->m_children.begin() );
  vector< DeclutterNode* >::iterator layerItE( m_rootNode->m_children.end() );
  for( ; layerIt != layerItE; ++layerIt )
  {
    if( (*layerIt)->m_layerName == layerName )
    {
      // Tell the model and any attached views that the data has changed and needs reloading as we are deleting the node for
      // the layer
      beginResetModel();
      delete *layerIt;
      m_rootNode->m_children.erase( layerIt );
      endResetModel();

      return;
    }
  }
}

void DeclutterModel::updateChildNodes( DeclutterNode *node )
{
  if( node->m_children.empty() )
  {
    return;
  }

  QModelIndex firstChild( createIndex( node->m_children.front()->m_index, 0, node->m_children.front() ) );
  QModelIndex lastChild( createIndex( node->m_children.back()->m_index, 0, node->m_children.back() ) );
  dataChanged( firstChild, lastChild );

  for( size_t i = 0; i < node->m_children.size(); ++i )
  {
    updateChildNodes( node->m_children[i] );
  }
}

void DeclutterModel::updateLayerFeatures( const QString& layerName )
{
  if( !m_surface )
  {
    return;
  }
  TSLDataLayer* layer( m_surface->getDataLayer( layerName.toUtf8() ) );
  if( !layer )
  {
    return;
  }

  // Replace all data for 'layerName' in the model
  addLayerFeatures( layerName, layer );
}
