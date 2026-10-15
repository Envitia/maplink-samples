using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample
{
  /// <summary>
  /// This part of the partial class defines the interaction with the editor
  /// </summary>
  public partial class MapViewerForm
  {

#region Edit Menu Event Handlers
    private void menu_Edit_Undo_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.undo();
      }
    }

    private void menu_Edit_Backup_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.backup();
      }
    }

    private void menu_Edit_Copy_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.copy();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Edit_Delete_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.delete();
      }
    }

    private void menu_Edit_Clear_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.clear();
      }
      UpdateCheckedEditorButtons();
    }
#endregion //Edit Menu Event Handlers

#region Select Menu Event Handlers
    private void menu_Select_All_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.selectAll();
      }
    }

    private void menu_Select_ByRectangle_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.selectByExtent();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Select_ByPolygon_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.selectByPolygon();
      }
      UpdateCheckedEditorButtons();
    }
#endregion //Select Menu Event Handlers

#region Primitive Menu Event Handlers
    private void menu_Primitives_Polyline_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.polyline();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Primitives_Polygon_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.polygon();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Primitives_Point_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.symbol();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Primitives_Circle_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.circle();
      }
      UpdateCheckedEditorButtons();
    }


    private void menu_Primitives_Text_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.text();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Primitives_ChangeText_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.setText();
      }
      UpdateCheckedEditorButtons();
    }
#endregion Primitive Menu Event Handlers

#region Transforms Menu Event Handlers
    private void menu_Transformation_Move_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.move();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Transformation_Scale_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.scale();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Transformation_Rotate_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.rotate();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Transformation_Group_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.group();
      }
    }

    private void menu_Transformation_Ungroup_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.ungroup();
      }
    }

    private void menu_Transformation_Front_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.front();
      }
    }

    private void menu_Transformation_Back_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.back();
      }
    }

    private void menu_Transformation_AddPoint_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.addPoint();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Transformation_DeletePoint_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.deletePoint();
      }
      UpdateCheckedEditorButtons();
    }

    private void menu_Transformation_MovePoint_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.movePoint();
      }
      UpdateCheckedEditorButtons();
    }
#endregion //Transforms Menu Event Handlers

#region Constraints Menu Event Handlers
    private void btn_horizontal_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.horizontal();
      }
      UpdateConstraintButtons();
    }

    private void btn_vertical_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.vertical();
      }
      UpdateConstraintButtons();
    }

    private void btn_equal_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.equal();
      }
      UpdateConstraintButtons();
    }

    private void btn_unequal_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.unequal();
      }
      UpdateConstraintButtons();
    }

    private void btn_angular_Click(object sender, EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.angular(this.PreferencesPanel.Angle);
      }
      UpdateConstraintButtons();
    }

    private void btn_distance_Click(object sender, EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.distance(this.PreferencesPanel.Distance);
      }
      UpdateConstraintButtons();
    }

    void menu_Constraints_Horizontal_Click(object sender, System.EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.horizontal();
      }
      UpdateConstraintButtons();
    }

    void menu_Constraints_Distance_Click(object sender, System.EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.distance(this.PreferencesPanel.Distance);
      }
      UpdateConstraintButtons();
    }

    void menu_Constraints_Angular_Click(object sender, System.EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.angular(this.PreferencesPanel.Angle);
      }
      UpdateConstraintButtons();
    }

    void menu_Constraints_Unequal_Click(object sender, System.EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.unequal();
      }
      UpdateConstraintButtons();
    }

    void menu_Constraints_Equal_Click(object sender, System.EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.equal();
      }
      UpdateConstraintButtons();
    }

    void menu_Constraints_Vertical_Click(object sender, System.EventArgs e)
    {
      if (m_opman != null && this.PreferencesPanel != null)
      {
        m_opman.vertical();
      }
      UpdateConstraintButtons();
    }
#endregion Constraints Menu Event Handlers

#region APP6A Menu Event Handlers
    private void menu_aPP6ATool_Palette_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.app6aPalette(this.Handle);
      }
    }

    private void menu_aPP6ATool_Properties_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.app6aProperties(this.Handle);
      }
    }

    private void menu_aPP6ATool_Selector_Click(object sender, EventArgs e)
    {
      if (m_opman != null)
      {
        m_opman.app6aSelector(this.Handle);
      }
    }
#endregion APP6A Menu Event Handlers

#region UI Update Functions
        
    struct OpButtonMapping
    {
      public OpManager.Operation op;
      public ToolStripButton button;
      public ToolStripMenuItem menuitem;

      public OpButtonMapping(OpManager.Operation op, ToolStripButton button, ToolStripMenuItem menuitem)
      {
        this.op = op;
        this.button = button;
        this.menuitem = menuitem;
      }
    }

    private OpButtonMapping[] m_operationUiMappings;

    private void InitOperationUiMappings()
    {
      if (m_operationUiMappings != null)
        return;

      // Toolbar buttons and menu items that need to have a checkmark applied and removed
      m_operationUiMappings = new[] {
        new OpButtonMapping(OpManager.Operation.OperationPolygon, btn_polygon, menu_Primitives_Polygon),
        new OpButtonMapping(OpManager.Operation.OperationPolyline, btn_polyline, menu_Primitives_Polyline),
        new OpButtonMapping(OpManager.Operation.OperationSymbol, btn_point, menu_Primitives_Point),
        new OpButtonMapping(OpManager.Operation.OperationCircle, null, menu_Primitives_Circle),
        new OpButtonMapping(OpManager.Operation.OperationText, btn_text, menu_Primitives_Text),
        new OpButtonMapping(OpManager.Operation.OperationSelectByPolygon, btn_selectByPolygon, menu_Select_ByPolygon),
        new OpButtonMapping(OpManager.Operation.OperationSelectByRectangle, btn_selectByRectangle, menu_Select_ByRectangle),
        new OpButtonMapping(OpManager.Operation.OperationSetText, btn_changeText, menu_Primitives_ChangeText),
        new OpButtonMapping(OpManager.Operation.OperationCopy, btn_copy, menu_Edit_Copy),
        new OpButtonMapping(OpManager.Operation.OperationClear, null, menu_Edit_Clear),
        new OpButtonMapping(OpManager.Operation.OperationDelete, btn_delete, menu_Edit_Delete),
        new OpButtonMapping(OpManager.Operation.OperationMove, btn_move, menu_Transformation_Move),
        new OpButtonMapping(OpManager.Operation.OperationScale, btn_scale, menu_Transformation_Scale),
        new OpButtonMapping(OpManager.Operation.OperationRotate, btn_rotate, menu_Transformation_Rotate),
        new OpButtonMapping(OpManager.Operation.OperationAddPoint, btn_addPoint, menu_Transformation_AddPoint),
        new OpButtonMapping(OpManager.Operation.OperationDeletePoint, btn_deletePoint, menu_Transformation_DeletePoint),
        new OpButtonMapping(OpManager.Operation.OperationMovePoint, btn_movePoint, menu_Transformation_MovePoint),
      };
    }

    public void UpdateCheckedEditorButtons()
    {
      if (m_opman == null)
        return;
      InitOperationUiMappings();

      OpManager.Operation op = m_opman.ActiveOperation;
      
      foreach (OpButtonMapping mapping in m_operationUiMappings)
      {
        bool shouldBeTicked = mapping.op == op;
        if (mapping.button != null)
          mapping.button.Checked = shouldBeTicked;
        mapping.menuitem.Checked = shouldBeTicked;
      }
    }

    private void UpdateConstraintButtons()
    {
      if (m_opman == null)
        return;

      TSLNEditorConstraintType constraint = m_opman.Constraint;

      btn_horizontal.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintHorizontal;
      menu_Constraints_Horizontal.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintHorizontal;
      btn_vertical.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintVertical;
      menu_Constraints_Vertical.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintVertical;
      btn_equal.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintEqual;
      menu_Constraints_Equal.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintEqual;
      btn_unequal.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintNone;
      menu_Constraints_Unequal.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintNone;
      btn_angular.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintAngle;
      menu_Constraints_Angular.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintAngle;
      btn_distance.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintDistance;
      menu_Constraints_Distance.Checked = constraint == TSLNEditorConstraintType.TSLNEditorConstraintDistance;
    }
#endregion //UI Update Functions

  }
}
