using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Collections;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using Envitia.MapLink;
using Envitia.MapLink.Editor;

namespace CSharpAPP6ASample
{
  public partial class AttributesPanel : UserControl
  {
    public AttributesPanel()
    {
      InitializeComponent();
      SetStyle(ControlStyles.SupportsTransparentBackColor, true);
      m_opman = null;
      m_systemColour_highlightText = Color.FromKnownColor(KnownColor.HighlightText);
      m_systemColour_highlightBackground = Color.FromKnownColor(KnownColor.Highlight);
      m_systemColour_windowText = Color.FromKnownColor(KnownColor.WindowText);
      m_systemColour_windowBackground = Color.FromKnownColor(KnownColor.Window);
    }

    public void SetOpManager(OpManager opman)
    {
      m_attributes = new TSLNRenderingAttributes();
      m_attributes.textSizeFactorUnits = TSLNDimensionUnits.TSLNDimensionUnitsPixels;
      m_attributes.edgeThicknessUnits = TSLNDimensionUnits.TSLNDimensionUnitsPixels;
      m_attributes.exteriorEdgeThicknessUnits = TSLNDimensionUnits.TSLNDimensionUnitsPixels;

      TSLNDrawingSurface surface = new TSLNDrawingSurface(this.Handle, false);

      int num = 0;
      int[] indices;

      int width = Math.Max(EdgeStyleCombo.DropDownWidth, EdgeStyleCombo.DisplayRectangle.Width);
      int height = EdgeStyleCombo.DisplayRectangle.Height;

      // Create the edge styles list
      if (surface.getLineStyleIndices(out indices))
      {
        num = indices.Length;

        // Add in <NONE> style
        Bitmap wc = CreateAndFillBitmap(width, height, m_systemColour_windowBackground);
        Bitmap hc = CreateAndFillBitmap(width, height, m_systemColour_highlightBackground);

        ComboBoxData triplet = new ComboBoxData(wc, hc, 0);

        EdgeStyleCombo.Items.Add(triplet);

        for (int i = 0; i < num; i++)
        {
          int index = indices[i];

          Bitmap bwc = (Bitmap)wc.Clone();
          Bitmap bhc = (Bitmap)hc.Clone();

          surface.lineStyleValue(index, m_systemColour_windowText, 1, bwc);
          surface.lineStyleValue(index, m_systemColour_highlightText, 1, bhc);

          triplet = new ComboBoxData(bwc, bhc, index);

          EdgeStyleCombo.Items.Add(triplet);
          m_edgeLookup.Add(index, i + 1);
        }
      }

      // Create the fill styles list
      if (surface.getFillStyleIndices(out indices))
      {
        num = indices.Length;

        // Add in <NONE> style
        Bitmap wc = CreateAndFillBitmap(width, height, m_systemColour_windowBackground);
        Bitmap hc = CreateAndFillBitmap(width, height, m_systemColour_highlightBackground);

        ComboBoxData triplet = new ComboBoxData(wc, hc, 0);
        FillStyleCombo.Items.Add(triplet);
        TextBGStyleCombo.Items.Add(triplet);

        for (int i = 0; i < num; i++)
        {
          int index = indices[i];

          Bitmap bwc = (Bitmap)wc.Clone();
          Bitmap bhc = (Bitmap)hc.Clone();

          surface.fillStyleValue(index, m_systemColour_windowText, bwc);
          surface.fillStyleValue(index, m_systemColour_highlightText, bhc);

          triplet = new ComboBoxData(bwc, bhc, index);

          FillStyleCombo.Items.Add(triplet);
          TextBGStyleCombo.Items.Add(triplet);
          m_fillLookup.Add(index, i + 1);
        }
      }


      // Create the font styles list
      if (surface.getFontStyleIndices(out indices))
      {
        num = indices.Length;

        // Add in <NONE> style
        Bitmap wc = CreateAndFillBitmap(width, height, m_systemColour_windowBackground);
        Bitmap hc = CreateAndFillBitmap(width, height, m_systemColour_highlightBackground);

        ComboBoxData triplet = new ComboBoxData(wc, hc, 0);;

        FillStyleCombo.Items.Add(triplet);
        TextBGStyleCombo.Items.Add(triplet);

        for (int i = 0; i < num; i++)
        {
          int index = indices[i];

          Bitmap bwc = (Bitmap)wc.Clone();
          Bitmap bhc = (Bitmap)hc.Clone();

          string fontName;
          surface.fontStyleValue(index, m_systemColour_windowText, bwc, out fontName, null);
          surface.fontStyleValue(index, m_systemColour_highlightText, bhc, out fontName, null);

          triplet = new ComboBoxData(bwc, bhc, index);

          TextStyleCombo.Items.Add(triplet);
        }
      }

      // Query the initial attributes from the config file and select the appropriate entry
      // from the combo boxes.
      int selected;

      TSLNProfileHelper.setDefaultSection("initialAttributes");
      //To guarantee there isn't an exception we'll or the values with an alpha of 255

      TSLNProfileHelper.lookupProfileUsingDefaults("edgeColour", out selected, 0);
      EdgeColourCombo.BackColor = Color.FromArgb((selected | 0xff<<24));
      TSLNProfileHelper.lookupProfileUsingDefaults("fillColour", out selected, 0);
      FillColourCombo.BackColor = Color.FromArgb((selected | 0xff << 24));
      TSLNProfileHelper.lookupProfileUsingDefaults("textColour", out selected, 0);
      TextColourCombo.BackColor = Color.FromArgb((selected | 0xff << 24));

      TSLNProfileHelper.lookupProfileUsingDefaults("textFont", out selected, 1);
      TextStyleCombo.SelectedIndex = selected - 1;

      // Set up the text size
      TSLNProfileHelper.lookupProfileUsingDefaults("textHeight", out selected, 1);
      if (selected < 1)
        selected = 1;
      else if (selected > 30)
        selected = 30;

      TextSizeVal.Value = selected;

      TSLNProfileHelper.lookupProfileUsingDefaults("edgeWidth", out selected, 1);
      EdgeWidthVal.Value = (decimal)selected;

      TSLNProfileHelper.lookupProfileUsingDefaults("edgeStyle", out selected, 1);
      EdgeStyleCombo.SelectedIndex = m_edgeLookup.ContainsKey(selected) ? m_edgeLookup[selected] : 0;

      TSLNProfileHelper.lookupProfileUsingDefaults("fillStyle", out selected, 1);
      FillStyleCombo.SelectedIndex = m_fillLookup.ContainsKey(selected) ? m_fillLookup[selected] : 0;

      int halign = 0;
      int valign = 0;
      TSLNProfileHelper.lookupProfileUsingDefaults("textHorizontalAlignment", out halign, (int)TSLNHorizontalAlignment.TSLNHorizontalAlignmentLeft);
      TSLNProfileHelper.lookupProfileUsingDefaults("textVerticalAlignment", out valign, (int)TSLNVerticalAlignment.TSLNVerticalAlignmentMiddle);

      TextHAlignmentCombo.SelectedIndex = halign;
      TextVAlignmentCombo.SelectedIndex = valign;

      TSLNProfileHelper.lookupProfileUsingDefaults("textBackgroundMode", out selected, -1);
      TextBGTypeCombo.SelectedIndex = selected;

      TSLNProfileHelper.lookupProfileUsingDefaults("textBackgroundStyle", out selected, -1);
      TextBGStyleCombo.SelectedIndex = m_fillLookup.ContainsKey(selected) ? m_fillLookup[selected] : 0;

      TSLNProfileHelper.lookupProfileUsingDefaults("textBackgroundColour", out selected, 0);
      TextBGColourCombo.BackColor = Color.FromArgb((selected | 0xff<<24));

      TSLNProfileHelper.lookupProfileUsingDefaults("textBackgroundEdgeColour", out selected, 0xffffff);
      TextBGEdgeColourCombo.BackColor = Color.FromArgb((selected | 0xff << 24));

      TSLNProfileHelper.lookupProfileUsingDefaults("symbolColour", out selected, 0);
      m_attributes.symbolColour = (selected | 0xff << 24);

      TSLNProfileHelper.lookupProfileUsingDefaults("symbolStyle", out selected, 1);
      m_attributes.symbolStyle = selected;

      TSLNProfileHelper.lookupProfileUsingDefaults("symbolSize", out selected, 2000);
      m_attributes.symbolSizeFactor = (double)selected;

      opman.renderingAttributes(ref m_attributes);

      //Set at the very end to prevent every event handler from notifying the editor
      m_opman = opman;
    }

    private void OnLoad(object sender, EventArgs e)
    {
      this.BackColor = Color.Transparent;
    }

    private static Bitmap CreateAndFillBitmap(int width, int height, Color fillcolor)
    {
      Bitmap b = new Bitmap(width, height);
      Graphics g = Graphics.FromImage(b);
      g.FillRectangle(new SolidBrush(fillcolor), 0, 0, width + 1, height + 1);
      g.Dispose();
      return b;
    }

#region Draw Events
   
    private void OnDrawEdgeStyle(object sender, DrawItemEventArgs e)
    {
      int itemIndex = e.Index;
      if (itemIndex == -1)
        return;

      ComboBoxData triplet = (ComboBoxData)((ComboBox)sender).Items[itemIndex];
      if (triplet == null)
        return;

      bool selected = (e.State & DrawItemState.Selected) == DrawItemState.Selected;
      e.Graphics.DrawImage(selected ? triplet.imageHighlighted : triplet.imageNormal, e.Bounds);
    }

    private void OnDrawFillStyle(object sender, DrawItemEventArgs e)
    {
      int itemIndex = e.Index;
      if (itemIndex == -1)
        return;

      ComboBoxData triplet = (ComboBoxData)((ComboBox)sender).Items[itemIndex];
      if (triplet == null)
        return;

      bool selected = (e.State & DrawItemState.Selected) == DrawItemState.Selected;
      e.Graphics.DrawImage(selected ? triplet.imageHighlighted : triplet.imageNormal, e.Bounds);
    }

    private void OnDrawTextStyle(object sender, DrawItemEventArgs e)
    {
      int itemIndex = e.Index;
      if (itemIndex == -1)
        return;

      ComboBoxData triplet = (ComboBoxData)((ComboBox)sender).Items[itemIndex];
      if (triplet == null)
        return;

      bool selected = (e.State & DrawItemState.Selected) == DrawItemState.Selected;
      e.Graphics.DrawImage(selected ? triplet.imageHighlighted : triplet.imageNormal, e.Bounds);
    }
#endregion

#region Value Changed Events
    private void EdgeColourComboBackColourChanged(object sender, EventArgs e)
    {
      if (sender == null || m_attributes == null) return;

      m_attributes.edgeColour = ((ComboBox)sender).BackColor.ToArgb();
      m_attributes.exteriorEdgeColour = ((ComboBox)sender).BackColor.ToArgb();
      if (m_opman != null)
      {
        m_opman.renderingAttributes( ref m_attributes );
      }
    }

    private void EdgeWidthVal_ValueChanged(object sender, EventArgs e)
    {
      m_attributes.edgeThickness = (double)EdgeWidthVal.Value;
      m_attributes.exteriorEdgeThickness = (double)EdgeWidthVal.Value;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void EdgeStyleCombo_SelectedIndexChanged(object sender, EventArgs e)
    {
      ComboBoxData triplet = (ComboBoxData)EdgeStyleCombo.SelectedItem;
      m_attributes.edgeStyle = triplet == null ? 0 : triplet.index;
      m_attributes.exteriorEdgeStyle = triplet == null ? 0 : triplet.index;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void FillColourComboBackColourChanged(object sender, EventArgs e)
    {
      if (sender == null || m_attributes == null) return;

      m_attributes.fillColour = ((ComboBox)sender).BackColor.ToArgb();
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void FillStyleCombo_SelectedIndexChanged(object sender, EventArgs e)
    {
      ComboBoxData triplet = (ComboBoxData)FillStyleCombo.SelectedItem;
      m_attributes.fillStyle = triplet == null ? 0 : triplet.index;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextColourComboBackColourChanged(object sender, EventArgs e)
    {
      if (sender == null || m_attributes == null) return;

      m_attributes.textColour = ((ComboBox)sender).BackColor.ToArgb();
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextSizeVal_ValueChanged(object sender, EventArgs e)
    {
      m_attributes.textSizeFactor = (double)TextSizeVal.Value;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextStyleCombo_SelectedIndexChanged(object sender, EventArgs e)
    {
      ComboBoxData triplet = (ComboBoxData)TextStyleCombo.SelectedItem;
      m_attributes.textFont = triplet == null ? 0 : triplet.index;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextHAlignmentCombo_SelectedIndexChanged(object sender, EventArgs e)
    {
      m_attributes.textHorizontalAlignment = (TSLNHorizontalAlignment)TextHAlignmentCombo.SelectedIndex;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextVAlignmentCombo_SelectedIndexChanged(object sender, EventArgs e)
    {
      m_attributes.textVerticalAlignment = (TSLNVerticalAlignment)TextVAlignmentCombo.SelectedIndex;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextBGTypeCombo_SelectedIndexChanged(object sender, EventArgs e)
    {
      m_attributes.textBackgroundMode = (TSLNTextBackgroundMode)TextBGTypeCombo.SelectedIndex;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextBGColourComboColourChanged(object sender, EventArgs e)
    {
      if (sender == null || m_attributes == null) return;

      m_attributes.textBackgroundColour = ((ComboBox)sender).BackColor.ToArgb();
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }


    private void TextBGStyleCombo_SelectedIndexChanged(object sender, EventArgs e)
    {
      ComboBoxData triplet = (ComboBoxData)TextBGStyleCombo.SelectedItem;
      m_attributes.textBackgroundStyle = triplet == null ? 0 : triplet.index;
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void TextBGEdgeColourComboBackColourChanged(object sender, EventArgs e)
    {
      if (sender == null || m_attributes == null) return;

      m_attributes.textBackgroundEdgeColour = ((ComboBox)sender).BackColor.ToArgb();
      if (m_opman != null)
      {
        m_opman.renderingAttributes(ref m_attributes);
      }
    }

    private void OnDroppingColourItem(object sender, EventArgs e)
    {
      ComboBox sendcombo = (ComboBox)sender;
      colourPicker.Color = sendcombo.BackColor;
      colourPicker.FullOpen = true;
      if (colourPicker.ShowDialog(this) == DialogResult.OK)
      {
        sendcombo.BackColor = colourPicker.Color;
      }
    }
#endregion

    private TSLNRenderingAttributes m_attributes;
    private OpManager m_opman;

    private Color m_systemColour_windowBackground;
    private Color m_systemColour_highlightBackground;
    private Color m_systemColour_windowText;
    private Color m_systemColour_highlightText;

    //private Dictionary<int, int> m_colourLookup = new Dictionary<int, int>();
    private Dictionary<int, int> m_edgeLookup = new Dictionary<int, int>();
    private Dictionary<int, int> m_fillLookup = new Dictionary<int, int>();
  }

  [ToolStripItemDesignerAvailability(ToolStripItemDesignerAvailability.ToolStrip)]
  public class DockableAttributePanel : ToolStripControlHost
  {
    public DockableAttributePanel()
      : base(new AttributesPanel(), "DockableAttributePanel")
    {
    }

    public void SetOpManager(OpManager opman)
    {
      ((AttributesPanel)this.Control).SetOpManager(opman);
    }
  }
  class ComboBoxData
  {
    public readonly Bitmap imageNormal;
    public readonly Bitmap imageHighlighted;
    public readonly int index;

    public ComboBoxData(Bitmap imageNormal, Bitmap imageHighlighted, int index)
    {
      this.imageNormal = imageNormal;
      this.imageHighlighted = imageHighlighted;
      this.index = index;
    }
  }
}
