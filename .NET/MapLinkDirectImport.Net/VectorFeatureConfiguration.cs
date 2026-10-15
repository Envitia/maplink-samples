using Envitia.MapLink;
using Envitia.MapLink.DirectImport;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CSharpDirectImportSample
{
  public partial class VectorFeatureConfiguration : Form
  {
    MapViewerForm m_parent = null;
    TSLNDirectImportDataSet m_dataset;
    TSLNClientFeatureClassifierCustom m_classifier;
    public VectorFeatureConfiguration(MapViewerForm parent, TSLNDirectImportDataSet dataSet, TSLNClientFeatureClassifierCustom classifier)
    {
      m_parent = parent;
      m_dataset = dataSet;
      InitializeComponent();
      if (classifier != null)
      {
        m_classifier = classifier;
        CustomButton.Enabled = true;
      }
    }

    private void GenerateButton_Click(object sender, EventArgs e)
    {
      // Loading vector datasets requires that a TSLNFeatureClassConfig be defined for the features within the data
      // This configuration should usually be created as an offline process for the required data product(s), 
      // however may be created at runtime through the MapLink API.

      // Analysis callbacks may be set for the duration of the program, or set/cleared as needed
      DirectImportAnalysisCallbacks analysisCallbacks = new DirectImportAnalysisCallbacks();
      m_parent.DirectImportDataLayer.setAnalysisCallbacks(analysisCallbacks);

      analysisCallbacks.condVarReset();
      if (m_parent.DirectImportDataLayer.analyseData(m_dataset) == false)
      {
        string error = TSLNErrorStack.errorString("\nErrors:\n");
        MessageBox.Show(this, "Failed to perform data analysis: " + m_dataset.name() + error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        return;
      }

      analysisCallbacks.condVarWait();

      // Now set random colours for the features
      TSLNFeatureClassConfig fcc = analysisCallbacks.m_fcc;
      Random rnd = new Random();
      for( uint i = 0; i < fcc.featureList().size(); ++i )
      {
        TSLNRenderingAttributes attribs = fcc.featureList().queryFeature(i).getRendering();
        attribs.edgeColour = Color.FromArgb(rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff)).ToArgb();
        attribs.fillColour = Color.FromArgb(rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff)).ToArgb();
        attribs.symbolColour = Color.FromArgb(rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff)).ToArgb();
        attribs.textColour = Color.FromArgb(rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff), rnd.Next(0x00, 0xff)).ToArgb();
        // Translucent fill style
        attribs.fillStyle = 503;
        // With black edge
        attribs.exteriorEdgeColour = Color.FromArgb(0x00, 0x00, 0x00).ToArgb();
        fcc.featureList().queryFeature(i).setRendering(attribs);
      }


      // Clear the callbacks
      m_parent.DirectImportDataLayer.setAnalysisCallbacks(null);
      m_parent.FeatureConfig = analysisCallbacks.m_fcc;

      this.Close();
    }

    private void CustomButton_Click(object sender, EventArgs e)
    {
      // Analysis callbacks may be set for the duration of the program, or set/cleared as needed
      DirectImportAnalysisCallbacks analysisCallbacks = new DirectImportAnalysisCallbacks();
      m_parent.DirectImportDataLayer.setAnalysisCallbacks(analysisCallbacks);

      analysisCallbacks.condVarReset();
      if (m_parent.DirectImportDataLayer.analyseData(m_dataset) == false)
      {
        string error = TSLNErrorStack.errorString("\nErrors:\n");
        MessageBox.Show(this, "Failed to perform data analysis: " + m_dataset.name() + error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        return;
      }

      analysisCallbacks.condVarWait();

      // Clear the callbacks
      m_parent.DirectImportDataLayer.setAnalysisCallbacks(null);

      TSLNFeatureClassConfig config = analysisCallbacks.m_fcc;
      config.addClassifier("lines", TSLNFeatureClassifierCustom.create(m_classifier));
      m_parent.FeatureConfig = config;

      this.Close();
    }

    private void LoadButton_Click(object sender, EventArgs e)
    {
      OpenFileDialog dia = new OpenFileDialog();
      dia.Filter = "FCC files | *.fcc";
      dia.Title = "Select a fcc file";

      if (dia.ShowDialog() == System.Windows.Forms.DialogResult.OK)
      {
        TSLNFeatureClassConfig config = new TSLNFeatureClassConfig();
        config.load(dia.FileName);
        m_parent.FeatureConfig = config;
      }
      this.Close();
    }
  }
}
