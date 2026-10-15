using System;
using System.Collections.Generic;
using System.Text;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpEditorSample
{
  public class OpManager : IOpManager
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
        m_editor.cursorMovementConfig( m_cursorReadoutType, true, m_cursorReadoutFormat ) ;
        if (m_attributesArePending)
        {
            m_attributesArePending = false;
            renderingAttributes(m_latestAttributes);
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

    public override void cursorReadoutType(string type)
    {
        switch (type)
        {
            case "TMC Absolute":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementTMCAbsolute, "Pos : X %.0f   Y %.0f");
                break;

            case "TMC Relative":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementTMCRelative, "Pos : X %.0f   Y %.0f");
                break;

            case "MU Absolute":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementMUAbsolute, "Pos : X %.0f   Y %.0f");
                break;

            case "MU Relative":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementMURelative, "Pos : X %.0f   Y %.0f");
                break;

            case "UU Absolute":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementUUAbsolute, "Pos : X %.0f   Y %.0f");
                break;

            case "UU Relative":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementUURelative, "Pos : X %.0f   Y %.0f");
                break;

            case "Lat/Long Decimal":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementLatLongDecimal, "Pos : Lat %.3f   Long %.3f");
                break;

            case "Lat/Long Angular":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementLatLongAngular, "Pos : Lat %s   Long %s");
                break;

            case "Angle/TMC Distance":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementAngleTMCDistance, "Pos : Angle %3.3f   Distance %.0f");
                break;

            case "Bearing/TMC Distance":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementBearingTMCDistance, "Pos : Brg %3.3f   Distance %.0f");
                break;

            case "Angle/MU Distance":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementAngleMUDistance, "Pos : Angle %3.3f   Distance %.0f");
                break;

            case "Bearing/MU Distance":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementBearingMUDistance, "Pos : Brg %3.3f   Distance %.0f");
                break;

            case "Angle/UU Distance":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementAngleUUDistance, "Pos : Angle %3.3f   Distance %.0f");
                break;

            case "Bearing/UU Distance":
                cursorReadoutType(TSLNCursorMovementType.TSLNCursorMovementBearingUUDistance, "Pos : Brg %3.3f   Distance %.0f");
                break;
        }
    }

    public override void angular(double value)
    {
      m_activeConstraint = TSLNEditorConstraintType.TSLNEditorConstraintAngle;
      m_activeConstraintValue = value ;
      if (m_editor != null)
      {
        m_editor.constraint(m_activeConstraint, value);
      }
    }

    public override void distance(double value )
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

    public override void renderingAttributes(RenderingAttr ra)
    {
        m_latestAttributes = ra;

        if (m_editor != null)
        {
            TSLNRenderingAttributes tra = new TSLNRenderingAttributes();
            tra.borderColour = ra.borderColour.ToArgb();
            tra.borderWidth = ra.borderWidth;
            tra.edgeColour = ra.edgeColour.ToArgb();
            tra.edgeStyle = ra.edgeStyle;
            tra.edgeThickness = ra.edgeThickness;
            tra.edgeThicknessUnits = (TSLNDimensionUnits)ra.edgeThicknessUnits;
            tra.exteriorEdgeColour = ra.exteriorEdgeColour.ToArgb();
            tra.exteriorEdgeStyle = ra.exteriorEdgeStyle;
            tra.exteriorEdgeThickness = ra.exteriorEdgeThickness;
            tra.exteriorEdgeThicknessUnits = (TSLNDimensionUnits)ra.exteriorEdgeThicknessUnits;
            tra.fillColour = ra.fillColour.ToArgb();
            tra.fillStyle = ra.fillStyle;
            tra.symbolColour = ra.symbolColour.ToArgb();
            tra.symbolSizeFactor = ra.symbolSizeFactor;
            tra.symbolStyle = ra.symbolStyle;
            tra.textBackgroundColour = ra.textBackgroundColour.ToArgb();
            tra.textBackgroundEdgeColour = ra.textBackgroundEdgeColour.ToArgb();
            tra.textBackgroundMode = (TSLNTextBackgroundMode)ra.textBackgroundMode;
            tra.textBackgroundStyle = ra.textBackgroundStyle;
            tra.textColour = ra.textColour.ToArgb();
            tra.textFont = ra.textFont;
            tra.textHorizontalAlignment = (TSLNHorizontalAlignment)ra.textHorizontalAlignment;
            tra.textSizeFactor = ra.textSizeFactor;
            tra.textSizeFactorUnits = (TSLNDimensionUnits)ra.textSizeFactorUnits;
            tra.textVerticalAlignment = (TSLNVerticalAlignment)ra.textVerticalAlignment;
            m_editor.activate("renderingattributes", tra);
        }
        else
        {
            m_attributesArePending = true;
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
    private RenderingAttr m_latestAttributes = null;
    private bool m_attributesArePending = false;

    }

    public abstract class IOpManager
    {
        public abstract void renderingAttributes(RenderingAttr ra);
        public abstract void angular(double value);
        public abstract void distance(double value);
        public abstract void cursorReadoutType(string type);
    }
}
