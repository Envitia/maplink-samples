namespace DrawingSurfacePanel
{
  public interface IPanel
  {
    void SafeInvalidate();
    Envitia.MapLink.TSLN2DDrawingSurface GetDrawingSurface();
  }
}
