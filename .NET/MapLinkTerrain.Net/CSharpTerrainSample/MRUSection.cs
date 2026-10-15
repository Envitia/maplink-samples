using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace CSharpTerrainSample
{
  public class MRUSection : ConfigurationSection
  {
    private static int MaximumNumberOfEntries = 4;

    public MRUSection()
    {
    }

    public void addPath(string path)
    {
      int count = MRUs.Count;
      for (int k = 0; k < count; ++k)
      {
        if (MRUs[k].Path.ToLower().CompareTo(path.ToLower()) == 0)
        {
          moveToTop(k);
          return;
        }
      }

      if (count >= MaximumNumberOfEntries )
      {
        --count;
      }
      MRUConfigElement[] array = new MRUConfigElement[count];
      for (int i = 0; i < count; ++i)
      {
        array[i] = MRUs[i];
      }

      MRUs.Clear();
      MRUs.Add(new MRUConfigElement(path));

      for (int j = 0; j < count; ++j)
      {
        MRUs.Add(array[j]);
      }
    }

    private void moveToTop(int index)
    {
      if (index == 0)
      {
        return;
      }
      int count = MRUs.Count;
      MRUConfigElement[] array = new MRUConfigElement[count >= MaximumNumberOfEntries ? count - 1 : count];

      for (int i = 0, k = 0; i < count; ++i)
      {
        if (i == index)
          continue;
        array[k] = MRUs[i];
        ++k;
      }

      MRUConfigElement top = MRUs[index];
      MRUs.Clear();
      MRUs.Add(top);

      for (int j = 0; j < count - 1; ++j)
      {
        MRUs.Add(array[j]);
      }
    }

    public string this[int index]
    {
      get
      {
        if (index >= MRUs.Count)
          return null;

        return MRUs[index].Path;
      }
    }

    public void removePath(int index)
    {
      if (index == MaximumNumberOfEntries - 1)
      {
        MRUs.Remove(MRUs[MaximumNumberOfEntries - 1]);
        return;
      }

      int count = MRUs.Count;
      MRUConfigElement[] array = new MRUConfigElement[count >= MaximumNumberOfEntries ? count - 1 : count];

      for (int i = 0, k = 0; i < count; ++i)
      {
        if (i == index)
          continue;
        array[k] = MRUs[i];
        ++k;
      }

      MRUs.Clear();

      for (int j = 0; j < count - 1; ++j)
      {
        MRUs.Add(array[j]);
      }
    }

    public int Count
    {
      get { return MRUs.Count; }
    }

    [ConfigurationProperty("MRUs",
        IsDefaultCollection = false)]
    protected MRUCollection MRUs
    {
      get
      {
        return (MRUCollection)base["MRUs"];
      }
      set
      {
        base["MRUs"] = value;
      }
    }
  }

  public class MRUConfigElement :  ConfigurationElement
  {
    public MRUConfigElement()
    {
      Path = "";
    }

    public MRUConfigElement(string path)
    {
      Path = path;
    }

    [ConfigurationProperty("path",
        DefaultValue = "",
        IsRequired = true, 
        IsKey = true)]
    public string Path
    {
      get
      {
        return (string)this["path"];
      }
      set
      {
        this["path"] = value;
      }
    }
  }

  public sealed class MRUCollection : ConfigurationElementCollection
  {
    protected override ConfigurationElement CreateNewElement()
    {
      return new MRUConfigElement();
    }

    protected override object GetElementKey(ConfigurationElement element)
    {
      return ((MRUConfigElement)element).Path;
    }

    public override ConfigurationElementCollectionType CollectionType
    {
      get
      {
        return ConfigurationElementCollectionType.BasicMap;
      }
    }

    protected override string ElementName
    {
      get
      {
        return "MRUEntry";
      }
    }

    public void Add(MRUConfigElement element)
    {
      BaseAdd(element);
    }

    public void Clear()
    {
      BaseClear();
    }

    public MRUConfigElement this[int index]
    {
      get
      {
        return (MRUConfigElement)BaseGet(index);
      }
      set
      {
        if (BaseGet(index) != null)
        {
          BaseRemoveAt(index);
        }
        BaseAdd(index, value);
      }
    }

    public void Remove(MRUConfigElement item)
    {
      BaseRemove(item);
    }

    public new int Count
    {
      get { return base.Count; }
    }
  }
}
