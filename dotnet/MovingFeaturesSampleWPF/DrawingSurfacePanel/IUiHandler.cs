namespace DrawingSurfacePanel
{
  public interface IUiHandler
  {
    public string Property { get; }
    /// <summary>
    /// Layer Name, if applicable
    /// </summary>
    public string Name { get; set; }

    public bool OnLeftButtonClick(bool shiftDown, bool ctrlDown, int x, int y);
    public bool OnRightButtonClick(bool shiftDown, bool ctrlDown, int x, int y);
    public bool OnLeftButtonDoubleClick(bool shiftDown, bool ctrlDown, int x, int y);
  }
}
