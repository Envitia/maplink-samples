#pragma once

#include <stdarg.h>
#include <vector>
#include <utility>

// should be in same order as in CModelHierarchyViewerDlg::OnInitDialog
#define BLANK_IMAGE          0
#define POLYLINE_IMAGE       1
#define POLYGON_IMAGE        2
#define POLYGON_OUTER_IMAGE  3
#define POLYGON_INNER_IMAGE  4
#define TEXT_IMAGE           5
#define SYMBOL_IMAGE         6
#define GROUP_IMAGE          7
#define ARC_IMAGE            8
#define ELLIPSE_IMAGE        9
#define RECTANGLE_IMAGE      10
#define BOTTOM_LEFT_IMAGE    11
#define TOP_RIGHT_IMAGE      12
#define HEIGHT_IMAGE         13
#define ROTATION_IMAGE       14
#define TEXT_STRING_IMAGE    15
#define POINTS_IMAGE         16
#define UNKNOWN_IMAGE        17

class ModelHierarchy
{
public:
  CString m_fileName;
  TSLStandardDataLayer*  m_dataLayer;
  vector< pair< TSLEntity*, HTREEITEM> > m_entityItemPairs;
  TSLEntity*	m_foundEntity;
  CTreeCtrl&	m_tree;

  ModelHierarchy(const CString &fileName, CTreeCtrl&	_tree)
    : m_fileName(fileName)
    , m_tree(_tree)
    , m_dataLayer(NULL)
    , m_foundEntity(NULL)
  {
  }

  ~ModelHierarchy()
  {
    // Destroy the datalayer
    if(m_dataLayer)
    {
      m_dataLayer->destroy();
    }
  }

  void ViewHierarchyFile()
  {
    // Create a standard data layer into which we will load the model
    m_dataLayer = new TSLStandardDataLayer;

    // Load the model
    m_dataLayer->appendData( TSLUTF8Encoder(m_fileName) );

    // Obtain a pointer to the tile group
    TSLEntitySet*  entitySet = m_dataLayer->entitySet();

    HTREEITEM fileItem = addItem( TVI_ROOT, GROUP_IMAGE, m_fileName );

    // Iterate through the model, adding parts to the tree as we go
    recurse( entitySet, fileItem );

    m_tree.Expand( fileItem, TVE_EXPAND );
  }

private:
  HTREEITEM addItem( HTREEITEM parent, int image, const TCHAR* format, ... )
  {
    TCHAR buffer[1024];

    va_list args;
    va_start( args, format );
    _vsntprintf( buffer, _countof( buffer ), format, args );
    buffer[_countof( buffer ) - 1] = _T('\0');
    va_end( args );

    HTREEITEM newItem = m_tree.InsertItem( buffer, parent );
    if ( image != BLANK_IMAGE )
    {
      m_tree.SetItemImage( newItem, image, image );
    }
    return newItem;
  }

  void recurse( TSLEntitySet* entitySet, HTREEITEM parent )
  {
    for ( int i = 0; i < entitySet->size(); i++ )
    {
      TSLEntity* thisEntity = (*entitySet)[i];

      HTREEITEM newItem;
      if( TSLEntitySet* es = TSLEntitySet::isEntitySet(thisEntity) )
      {
        newItem = addItem( parent, GROUP_IMAGE, _T( "Group" ) );
        m_entityItemPairs.push_back(make_pair(thisEntity, newItem));
        recurse( es, newItem );
        m_tree.Expand( newItem, TVE_EXPAND );
      }
      else if ( TSLArc::isArc(thisEntity) )
      {
        newItem = addItem( parent, ARC_IMAGE, _T( "Arc" ) );
      }
      else if ( TSLGeodeticArc::isGeodeticArc( thisEntity ) )
      {
        newItem = addItem( parent, ARC_IMAGE, _T( "Geodetic Arc" ) );
      }
      else if ( TSLEllipse::isEllipse(thisEntity) )
      {
        newItem = addItem( parent, ELLIPSE_IMAGE, _T( "Ellipse" ) );
      }
      else if ( TSLGeodeticEllipse::isGeodeticEllipse(thisEntity) )
      {
        newItem = addItem( parent, ELLIPSE_IMAGE, _T( "Geodetic Ellipse" ) );
      }
      else if ( const TSLPolygon* polygon = TSLPolygon::isPolygon(thisEntity) )
      {
        const TSLBorderedPolygon* borderedPolygon = TSLBorderedPolygon::isBorderedPolygon( polygon );

        newItem = addItem( parent, POLYGON_IMAGE, borderedPolygon ? _T( "Bordered Polygon" ) : _T( "Polygon" ) );

        const TSLCoordSet& coordset = polygon->outer();

        HTREEITEM outerItem = addItem( newItem, POLYGON_OUTER_IMAGE, _T( "Outer" ) );
        addItem( outerItem, POINTS_IMAGE, _T( "No. points: %ld" ), coordset.size() );
        m_tree.Expand( outerItem, TVE_EXPAND );

        for ( int j = 0; j < polygon->numInners(); j++ )
        {
          HTREEITEM innerItem = addItem( newItem, POLYGON_INNER_IMAGE, _T( "Inner %d" ), j );
          const TSLCoordSet* pcoordset = polygon->inner(j);
          addItem( innerItem, POINTS_IMAGE, _T( "No. points: %ld" ), pcoordset->size() );
          m_tree.Expand( innerItem, TVE_EXPAND );
        }
      }
      else if ( const TSLGeodeticPolygon* geodeticPolygon = TSLGeodeticPolygon::isGeodeticPolygon(thisEntity) )
      {
        newItem = addItem( parent, POLYGON_IMAGE, _T( "Geodetic Polygon" ) );

        const TSLCoordSet& coordset = geodeticPolygon->outer();

        HTREEITEM outerItem = addItem( newItem, POLYGON_OUTER_IMAGE, _T( "Outer" ) );
        addItem( outerItem, POINTS_IMAGE, _T( "No. points: %ld" ), coordset.size() );
        m_tree.Expand( outerItem, TVE_EXPAND );

        for ( int j = 0; j < geodeticPolygon->numInners(); j++ )
        {
          HTREEITEM innerItem = addItem( newItem, POLYGON_INNER_IMAGE, _T( "Inner %d" ), j );
          const TSLCoordSet* pcoordset = geodeticPolygon->inner(j);
          addItem( innerItem, POINTS_IMAGE, _T( "No. points: %ld" ), pcoordset->size() );
          m_tree.Expand( innerItem, TVE_EXPAND );
        }
      }
      else if ( const TSLPolyline* polyline = TSLPolyline::isPolyline(thisEntity) )
      {
        newItem = addItem( parent, POLYLINE_IMAGE, _T( "Polyline" ) );

        const TSLCoordSet& coordset = polyline->points();
        HTREEITEM p = addItem( newItem, POINTS_IMAGE, _T( "No. points: %ld" ), coordset.size() );
        m_tree.Expand( p, TVE_EXPAND );
      }
      else if ( const TSLGeodeticPolyline* geodeticPolyline = TSLGeodeticPolyline::isGeodeticPolyline(thisEntity) )
      {
        newItem = addItem( parent, POLYLINE_IMAGE, _T( "Geodetic Polyline" ) );

        const TSLCoordSet& coordset = geodeticPolyline->points();
        HTREEITEM p = addItem( newItem, POINTS_IMAGE, _T( "No. points: %ld" ), coordset.size() );
        m_tree.Expand( p, TVE_EXPAND );
      }
      else if ( TSLRectangle::isRectangle(thisEntity) )
      {
        newItem = addItem( parent, RECTANGLE_IMAGE, _T( "Rectangle" ) );
      }
      else if ( TSLSymbol::isSymbol(thisEntity) )
      {
        newItem = addItem( parent, SYMBOL_IMAGE, _T( "Symbol" ) );
      }
      else if ( TSLGeodeticSymbol::isGeodeticSymbol(thisEntity) )
      {
        newItem = addItem( parent, SYMBOL_IMAGE, _T("Geodetic Symbol") );
      }
      else if ( const TSLText* text = TSLText::isText(thisEntity) )
      {
        newItem = addItem( parent, TEXT_IMAGE, _T("Text") );

        const char*   value    = text->value();
        double	      rotation = text->rotation();
        long          height   = text->height();

        TSLUTF8Decoder textValueDecoded( value );
        addItem( newItem, TEXT_STRING_IMAGE, _T( "String  :  %s" ), (LPCTSTR)textValueDecoded );
        addItem( newItem, ROTATION_IMAGE, _T( "Rotation:  %g" ), rotation );
        addItem( newItem, HEIGHT_IMAGE, _T( "Height  :  %ld" ), height );
      }
      else if ( const TSLGeodeticText* geodeticText = TSLGeodeticText::isGeodeticText(thisEntity) )
      {
        newItem = addItem( parent, TEXT_IMAGE, _T("Geodetic Text") );

        const char*   value    = geodeticText->value();
        double	      rotation = geodeticText->rotation();
        long          height   = geodeticText->height();

        TSLUTF8Decoder textValueDecoded( value );
        addItem( newItem, TEXT_STRING_IMAGE, _T( "String  :  %s" ), (LPCTSTR)textValueDecoded );
        addItem( newItem, ROTATION_IMAGE, _T( "Rotation:  %g" ), rotation );
        addItem( newItem, HEIGHT_IMAGE, _T( "Height  :  %ld" ), height );
      }
      else if ( const TSLMultiPolygon* multiPolygon = TSLMultiPolygon::isMultiPolygon( thisEntity ) )
      {
        newItem = addItem( parent, POLYGON_IMAGE, _T( "Multi-Polygon" ) );

        int num = multiPolygon->numberOfParts();
        HTREEITEM p = addItem( newItem, /*POINTS_IMAGE*/ BLANK_IMAGE, _T( "No. parts: %d" ), num );

        m_tree.Expand( p, TVE_EXPAND );
      }
      else if ( const TSLMultiPolyline* multiPolyline = TSLMultiPolyline::isMultiPolyline( thisEntity ) )
      {
        newItem = addItem( parent, POLYLINE_IMAGE, _T( "Multi-Polyline" ) );

        int num = multiPolyline->numberOfParts();
        HTREEITEM p = addItem( newItem, /*POINTS_IMAGE*/ BLANK_IMAGE, _T( "No. parts: %d" ), num );

        m_tree.Expand( p, TVE_EXPAND );
      }
      else if ( const TSLMultiPoint* multiPoint = TSLMultiPoint::isMultiPoint( thisEntity ) )
      {
        newItem = addItem( parent, SYMBOL_IMAGE, _T( "Multi-Point" ) );

        int num = multiPoint->numberOfParts();
        HTREEITEM p = addItem( newItem, /*POINTS_IMAGE*/ BLANK_IMAGE, _T( "No. parts: %d" ), num );

        m_tree.Expand( p, TVE_EXPAND );
      }
      else if ( const TSLUserGeometryEntity* userGeometry = TSLUserGeometryEntity::isUserGeometry( thisEntity ) )
      {
        newItem = addItem( parent, UNKNOWN_IMAGE, _T( "User Geometry" ) );
      }
      else
      {
        newItem = addItem( parent, UNKNOWN_IMAGE, _T( "Unknown (Optimised?)" ) );
      }

      m_entityItemPairs.push_back(make_pair(thisEntity, newItem));

      TSLEnvelope envelope =  thisEntity->envelope();
      addItem( newItem, BOTTOM_LEFT_IMAGE, _T( "Bottom Left : (%ld, %ld)" ), envelope.xMin(), envelope.yMin() );
      addItem( newItem, TOP_RIGHT_IMAGE, _T( "Top Right : (%ld, %ld)" ), envelope.xMax(), envelope.yMax() );

      m_tree.Expand( newItem, TVE_EXPAND );
    }
  }
 
  ModelHierarchy( const ModelHierarchy& x );               // not implemented
  ModelHierarchy& operator= ( const ModelHierarchy& x );   // not implemented
};