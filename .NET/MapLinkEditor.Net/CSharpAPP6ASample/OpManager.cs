using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample
{
  public class OpManager
  {
    public enum Operation
    {
      OperationUndetermined,
      OperationSelect,
      OperationPolygon,
      OperationPolyline,
      OperationRectangle,
      OperationSymbol,
      OperationCircle,
      OperationText,
      OperationSelectByPolygon,
      OperationSelectByRectangle,
      OperationSetText,
      OperationClear,
      OperationCopy,
      OperationMove,
      OperationScale,
      OperationRotate,
      OperationAddPoint,
      OperationDeletePoint,
      OperationMovePoint,
      OperationDelete,
      OperationAPP6AProperties,
      OperationAPP6APalette,
      OperationAPP6ASelector
    };

    public OpManager()
    {
      m_cursorReadoutType = TSLNCursorMovementType.TSLNCursorMovementTMCAbsolute;
      m_cursorReadoutFormat = "Pos: (%g, %g)";
      m_activeOperation = Operation.OperationUndetermined;
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintNone;
      m_activeConstraintValue = 0.0 ;
      m_UUperTMC = 1000.0;
    }

    public void setEditor(TSLNEditor editor)
    {
      m_editor = editor ;
      if( m_editor != null)
      {
        APP6A_Operations.APP6APropertiesOperation propsOperation = new APP6A_Operations.APP6APropertiesOperation();
        m_editor.addUserOperation ( new APP6A_Operations.APP6APaletteOperation(propsOperation), 
                                    "app6a_palette", 
                                    TSLNOperationType.TSLNOperationPrimitive ) ;

        m_editor.addUserOperation ( new APP6A_Operations.APP6ASelectorOperation(), 
                                    "app6a_selector",
                                    TSLNOperationType.TSLNOperationPrimitive);

        m_editor.addUserOperation ( propsOperation, 
                                    "app6a_properties",
                                    TSLNOperationType.TSLNOperationManipulation);

        m_editor.cursorMovementConfig( m_cursorReadoutType, true, m_cursorReadoutFormat ) ;
        if (m_pendingAttributes != null)
        {
          renderingAttributes(ref m_pendingAttributes);
          m_pendingAttributes = null;
        }
      }
    }

    public void cursorReadoutType(TSLNCursorMovementType type, string format)
    {
      m_cursorReadoutType = type;
      m_cursorReadoutFormat = format;

      if (m_editor != null)
      {
        m_editor.cursorMovementConfig(type, true, format);
      }
    }

    public void angular(double value)
    {
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintAngle;
      m_activeConstraintValue = value ;
      if (m_editor != null)
      {
        m_editor.constraint(m_activeConstraint, value);
      }
    }

    public void distance(double value )
    {
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintDistance;
      m_activeConstraintValue = value ;
      if (m_editor != null)
      {
        m_editor.constraint(m_activeConstraint, value * m_UUperTMC);
      }
    }

    public bool dataChanged()
    {
      if (m_editor != null)
      {
        return m_editor.dataChanged();
      }
      return false;
    }
    public Operation ActiveOperation
    {
      get
      {
        return m_activeOperation;
      }
    }

    public TSLNEditorConstraintType Constraint
    {
      get
      {
        return m_activeConstraint;
      }
    }

    public bool Changed
    {
      get
      {
        if (m_editor != null)
        {
          return m_editor.Changed;
        }
        return false;
      }
      set
      {
        if (m_editor != null)
        {
          m_editor.Changed = value;
        }
      }
    }

    public void reset( )
    {
      if ( m_editor != null )
        m_editor.reset() ;
    }

    public void select()
    {
      m_activeOperation = Operation.OperationSelect;
      if ( m_editor != null )
      {
        m_editor.activate( "select" ) ;
      }
    }

    public void polygon()
    {
      m_activeOperation = Operation.OperationPolygon;
      if (m_editor != null)
      {
        m_editor.activate("polygon");
      }
    }

    public void polyline()
    {
      m_activeOperation = Operation.OperationPolyline;
      if (m_editor != null)
      {
        m_editor.activate("polyline");
      }
    }

    public void rectangle()
    {
      m_activeOperation = Operation.OperationRectangle;
      if (m_editor != null)
      {
        m_editor.activate("rectangle");
      }
    }

    public void symbol()
    {
      m_activeOperation = Operation.OperationSymbol ;
      if (m_editor != null)
      {
        m_editor.activate("symbol");
      }
    }

    public void circle()
    {
      m_activeOperation = Operation.OperationCircle;
      if (m_editor != null)
      {
        m_editor.activate("circle");
      }
    }



    public void text()
    {
      m_activeOperation = Operation.OperationText;
      if (m_editor != null)
      {
        m_editor.activate("text");
      }
    }

    public void selectByExtent()
    {
      m_activeOperation = Operation.OperationSelectByRectangle;
      if (m_editor != null)
      {
        m_editor.activate("selectbyextent");
      }
    }

    public void selectByPolygon()
    {
      m_activeOperation = Operation.OperationSelectByPolygon;
      if (m_editor != null)
      {
        m_editor.activate("selectbypolygon");
      }
    }

    public void setText()
    {
      m_activeOperation = Operation.OperationSetText;
      if (m_editor != null)
      {
        m_editor.activate("settext");
      }
    }

    public void renderingAttributes(ref TSLNRenderingAttributes ra)
    {
      if (m_editor != null)
      {
        m_editor.activate("renderingattributes", ra);
      }
      else
      {
        m_pendingAttributes = ra;
      }
    }

    public void equal()
    {
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintEqual;
      if (m_editor != null)
      {
        m_editor.constraint(m_activeConstraint);
      }
    }


    public void unequal()
    {
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintNone;
      if (m_editor != null)
      {
        m_editor.constraint(m_activeConstraint);
      }
    }

    public void horizontal()
    {
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintHorizontal;
      if (m_editor != null)
      {
        m_editor.constraint(m_activeConstraint);
      }
    }

    public void vertical()
    {
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintVertical;
      if (m_editor != null)
      {
        m_editor.constraint(m_activeConstraint);
      }
    }

    public void clear()
    {
      m_activeOperation = Operation.OperationClear;
      if ( m_editor != null )
      {
        m_editor.activate( "clear" ) ;
      }
    }

    public void copy()
    {
      m_activeOperation = Operation.OperationCopy ;
      if (m_editor != null)
      {
        m_editor.activate("copy");
      }
    }

    public void undo()
    {
      if (m_editor != null)
      {
        m_editor.undo() ;
      }
    }

    public void backup()
    {
      if (m_editor != null)
      {
        m_editor.backup() ;
      }
    }

    public void selectAll()
    {
      if (m_editor != null)
      {
        m_editor.activate( "selectall" ) ;
      }
    }

    public void move()
    {
      m_activeOperation = Operation.OperationMove ;
      if ( m_editor != null )
      {
        m_editor.activate( "move" ) ;
      }
    }

    public void scale()
    {
      m_activeOperation = Operation.OperationScale;
      if (m_editor != null)
      {
        m_editor.activate( "scale" ) ;
      }
    }

    public void rotate()
    {
      m_activeOperation = Operation.OperationRotate;
      if (m_editor != null)
      {
        m_editor.activate( "rotate" ) ;
      }
    }

    public void rotate90()
    {
      if (m_editor != null)
      {
        m_editor.activate( "rotate90" ) ;
      }
    }

    public void flipVertical()
    {
      if (m_editor != null)
      {
        m_editor.activate( "flipVertical" ) ;
      }
    }

    public void flipHorizontal()
    {
      if (m_editor != null)
      {
        m_editor.activate("flipHorizontal");
      }
    }

    public void front()
    {
      if (m_editor != null)
      {
        m_editor.activate("front");
      }
    }

    public void back()
    {
      if (m_editor != null)
      {
        m_editor.activate( "back" ) ;
      }
    }

    public void group()
    {
      if (m_editor != null)
      {
        m_editor.activate( "group" ) ;
      }
    }

    public void ungroup()
    {
      if (m_editor != null)
      {
        m_editor.activate( "ungroup" ) ;
      }
    }

    public void addPoint()
    {
      m_activeOperation = Operation.OperationAddPoint ;
      if (m_editor != null)
      {
        m_editor.activate( "addpoint" ) ;
      }
    }

    public void deletePoint()
    {
      m_activeOperation = Operation.OperationDeletePoint;
      if (m_editor != null)
      {
        m_editor.activate( "deletepoint" ) ;
      }
    }

    public void movePoint()
    {
      m_activeOperation = Operation.OperationMovePoint;
      if (m_editor != null)
      {
        m_editor.activate( "movepoint" ) ;
      }
    }

    public void delete()
    {
      m_activeOperation = Operation.OperationDelete ;

      if (m_editor != null)
      {
        m_editor.activate("delete");
      }
    }

    public void textEntered(string text, bool cancelled)
    {
      if (m_editor != null)
      {
        m_editor.textEntered(text, cancelled);
      }
    }

    // Call in response to TSLNEditorRequest.requestDialog.
    // button is 1,2,3,4 (user clicked button1, ..., button4) or -1 (dialog cancelled).
    public void dialogEntered(int button)
    {
      if (m_editor != null)
      {
        m_editor.dialogEntered(button);
      }
    }

    public void app6aPalette(IntPtr parentWindow)
    {
      m_activeOperation = Operation.OperationAPP6APalette;

      if (m_editor != null)
      {
        m_editor.activate("app6a_palette", parentWindow);
      }
    }

    public void app6aProperties(IntPtr parentWindow)
    {
      m_activeOperation = Operation.OperationAPP6AProperties;

      if (m_editor != null)
      {
        m_editor.activate("app6a_properties", parentWindow);
      }
    }

    public void app6aSelector(IntPtr parentWindow)
    {
      m_activeOperation = Operation.OperationAPP6ASelector;

      if (m_editor != null)
      {
        m_editor.activate("app6a_selector", parentWindow);
      }
    }

    //This is type of constraint currently in effect
    private TSLNEditorConstraintType m_activeConstraint;

    //This is the value for constraints which have one, such as 
    //angular and distance
    private double m_activeConstraintValue;

    //This is the format that the status bar readout will use
    private TSLNCursorMovementType m_cursorReadoutType;
    //This is the string equivalent of the above attribute
    private string m_cursorReadoutFormat;

    //This is the editor operation currently in use
    private Operation m_activeOperation;

    //The conversion between UUs and TMCs
    private double m_UUperTMC;

    //The Editor
    private TSLNEditor m_editor = null;

    //When the editor is initialised, these attributes will be immediate passed to it.
    private TSLNRenderingAttributes m_pendingAttributes = null;

  }
}
