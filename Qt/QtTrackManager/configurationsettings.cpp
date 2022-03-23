/****************************************************************************
Copyright (c) 2008-2022 by Envitia Group PLC.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

****************************************************************************/

#include "configurationsettings.h"

#include <algorithm>

//! parse the configuration file and extract the tracks 
bool ConfigurationSettings::parseConfigFile(const QString &filename, std::map<int, TrackInformation> &tracksInformation, QString &msgError)
{
  QDomDocument doc;
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
  {
    msgError = "Failed to open the configuration file.";
    return false;
  }

  //! read the symbol sets
  QDomNodeList symbolSetNode = doc.elementsByTagName("SymbolSet");
  if (!parseSymbolSets(symbolSetNode, m_symbolSets, msgError))
  {
    return false;
  }

  // read the tracks
  QDomNodeList tracksNode = doc.elementsByTagName("Track");
  if (!parseTrackNodes(tracksNode, tracksInformation, msgError))
  {
    return false;
  }
  if (tracksInformation.empty())
  {
    msgError = "No track nodes found in the configuration file.";
    return false;
  }

  //! read the default symbol set and update the tracks colours and types
  QDomNodeList defSymbolSetNode = doc.elementsByTagName("DefaultSymbolSet");
  if (!parseDefaultSymbolSet(defSymbolSetNode, msgError))
  {
    return false;
  }
  if (!updateTracks(tracksInformation, m_defaultSymbolSet, msgError))
  {
    return false;
  }

  m_filename = filename;
  return true;
}

//! update the types and hostility for each track based on the symbol set
bool ConfigurationSettings::updateTracks(std::map<int, TrackInformation> &tracks, const QString &symbolSetStr, QString &msgError)
{
  if (m_symbolSets.find(symbolSetStr) == m_symbolSets.end())
  {
    msgError = "Invalid Symbol set (" + symbolSetStr + ").";
    return false;
  }

  // update types for each track
  auto& types = m_symbolSets[symbolSetStr].m_types;
  for (auto& track : tracks)
  {
    const auto& typeStr = track.second.type;
    if (types.find(typeStr) != types.end())
    {
      track.second.symbolId = types[typeStr];
    }
    else if (types.find("*") != types.end())
    {
      track.second.symbolId = types["*"];
    }
    else
    {
      msgError = "Invalid type (" + typeStr + ") for track id = " + QString::number(track.second.id) + " found in the configuration file.";
      return false;
    }
  }

  // update hostilities for each track
  auto& hostilities = m_symbolSets[symbolSetStr].m_hostilities;
  for (auto& track : tracks)
  {
    const auto& hostilityStr = track.second.hostility;
    if (hostilities.find(hostilityStr) != hostilities.end())
    {
      track.second.colour = hostilities[hostilityStr];
    }
    else if (hostilities.find("*") != hostilities.end())
    {
      track.second.colour = hostilities["*"];
    }
    else
    {
      msgError = "Invalid hostility (" + hostilityStr + ") for track id = " + QString::number(track.second.id) + " found in the configuration file.";
      return false;
    }
  }

  // update sizes for each track
  auto& sizes = m_symbolSets[symbolSetStr].m_sizes;
  for (auto& track : tracks)
  {
    const auto& typeStr = track.second.type;
    if (sizes.find(typeStr) != sizes.end())
    {
      track.second.size = sizes[typeStr];
    }
    else if (sizes.find("*") != sizes.end())
    {
      track.second.size = sizes["*"];
    }
    else
    {
      msgError = "Invalid size (" + typeStr + ") for track id = " + QString::number(track.second.id) + " found in the configuration file.";
      return false;
    }
  }

  return true;
}

//! get available symbol sets to be used in the menu.
const std::map<QString, SymbolSet>& ConfigurationSettings::symbolSets() const
{
  return m_symbolSets;
}

//! default symbol set
const QString& ConfigurationSettings::defaultSymbolSet() const
{
  return m_defaultSymbolSet;
}

//! check if the track hostility is valid for all the symbol sets
bool ConfigurationSettings::isTrackHostilityValid(const QString &hostilityStr)
{
  bool validHostility = std::all_of(m_symbolSets.cbegin(), m_symbolSets.cend(),
    [&hostilityStr](const std::pair<QString, SymbolSet>& item)
  {
    return item.second.m_hostilities.find("*") != item.second.m_hostilities.end() ||
      item.second.m_hostilities.find(hostilityStr) != item.second.m_hostilities.end();
  });

  return validHostility;
}

//! check if the track type is valid for all the symbol sets
bool ConfigurationSettings::isTrackTypeValid(const QString &typeStr)
{
  bool validType = std::all_of(m_symbolSets.cbegin(), m_symbolSets.cend(),
    [&typeStr](const std::pair<QString, SymbolSet>& item)
  {
    return item.second.m_types.find("*") != item.second.m_types.end() ||
      item.second.m_types.find(typeStr) != item.second.m_types.end();
  });

  return validType;
}

//! parse xml node to extract the default symbol set.
bool ConfigurationSettings::parseDefaultSymbolSet(const QDomNodeList &configNode, QString &msgError)
{
  for (int i = 0; i < configNode.size(); i++)
  {
    auto node = configNode.at(i);
    auto map = node.attributes();
    for (int i = 0; i < map.length(); ++i)
    {
      auto inode = map.item(i);
      auto attr = inode.toAttr();
      QString val = attr.value();
      if (!val.isEmpty())
      {
        m_defaultSymbolSet = val;
        break;
      }
    }
  }

  if (m_defaultSymbolSet.isEmpty())
  {
    msgError = "No value found for the default symbol set in the configuration file.";
    return false;
  }

  // check if valid default symbol set.
  if (m_symbolSets.find(m_defaultSymbolSet) == m_symbolSets.end())
  {
    msgError = "Invalid Symbol set (" + m_defaultSymbolSet + ").";
    return false;
  }

  return true;
}

//! parse xml node to extract symbol sets key values.
bool ConfigurationSettings::parseSymbolSets(const QDomNodeList &configNode, std::map<QString, SymbolSet> &symbolSets, QString &msgError)
{
  for (int i = 0; i < configNode.size(); i++)
  {
    auto node = configNode.at(i);
    auto map = node.attributes();
    for (int i = 0; i < map.length(); ++i)
    {
      auto inode = map.item(i);
      auto attr = inode.toAttr();
      QString key = attr.value();
      if (key.isEmpty())
      {
        msgError = "Invalid empty key for the node(" + node.nodeName() + ") in the configuration file.";
        return false;
      }

      SymbolSet temSymbolSet;

      auto element = node.toElement();
      QDomNodeList typesNode = element.elementsByTagName("Type");
      if (!parseKeyValueNodes(typesNode, temSymbolSet.m_types, msgError))
      {
        return false;
      }

      QDomNodeList hostilityNode = element.elementsByTagName("Hostility");
      if (!parseKeyValueNodes(hostilityNode, temSymbolSet.m_hostilities, msgError))
      {
        return false;
      }

      QDomNodeList sizeNode = element.elementsByTagName("Size");
      if (!parseKeyValueNodes(sizeNode, temSymbolSet.m_sizes, msgError))
      {
        return false;
      }
      // check if valid sizes ["*" or for types in the symbol set]
      for (auto& size : temSymbolSet.m_sizes)
      {
        if (size.first == "*")
        {
          continue;
        }
        else
        {
          if (temSymbolSet.m_types.find(size.first) == temSymbolSet.m_types.end())
          {
            msgError = "Invalid Size = " + size.first + " defined in the symbol set: " + key;
            return false;
          }
        }
      }

      symbolSets[key] = std::move(temSymbolSet);
    }
  }
  return true;
}

//! parse track nodes to extract tracks information.
bool ConfigurationSettings::parseTrackNodes(const QDomNodeList &configNode, std::map<int, TrackInformation> &keyValues, QString &msgError)
{
  for (int i = 0; i < configNode.size(); i++)
  {
    auto node = configNode.at(i);
    auto map = node.attributes();

    QString idStr, hostilityStr, typeStr, latStr, lonStr, latOffsetStr, lonOffsetStr;
    for (int i = 0; i < map.length(); ++i)
    {
      auto inode = map.item(i);
      auto attr = inode.toAttr();
      if (attr.name() == "id")
      {
        idStr = attr.value();
      }
      else if (attr.name() == "Hostility")
      {
        hostilityStr = attr.value();
      }
      else if (attr.name() == "Type")
      {
        typeStr = attr.value();
      }
      else if (attr.name() == "lat")
      {
        latStr = attr.value();
      }
      else if (attr.name() == "lon")
      {
        lonStr = attr.value();
      }
      else if (attr.name() == "latOffset")
      {
        latOffsetStr = attr.value();
      }
      else if (attr.name() == "lonOffset")
      {
        lonOffsetStr = attr.value();
      }
    }

    //! check attributes
    if (idStr.isEmpty())
    {
      msgError = "Invalid empty id in the configuration file.";
      return false;
    }
    if (hostilityStr.isEmpty())
    {
      msgError = "Invalid empty Hostility in the configuration file.";
      return false;
    }
    if (typeStr.isEmpty())
    {
      msgError = "Invalid empty Type in the configuration file.";
      return false;
    }
    if (latStr.isEmpty())
    {
      msgError = "Invalid empty lat in the configuration file.";
      return false;
    }
    if (lonStr.isEmpty())
    {
      msgError = "Invalid empty lon in the configuration file.";
      return false;
    }
    if (latOffsetStr.isEmpty())
    {
      msgError = "Invalid empty latOffset in the configuration file.";
      return false;
    }
    if (lonOffsetStr.isEmpty())
    {
      msgError = "Invalid empty lonOffset in the configuration file.";
      return false;
    }

    //! check if valid hostility and types
    if (!isTrackHostilityValid(hostilityStr))
    {
      msgError = "Invalid Hostility in the configuration file does not have reference in all symbol sets. " + hostilityStr;
      return false;
    }
    if (!isTrackTypeValid(typeStr))
    {
      msgError = "Invalid Type in the configuration file does not have reference in all symbol sets. " + typeStr;
      return false;
    }

    //! create the track information
    uint32_t id = idStr.toUInt();
    keyValues[id] = (TrackInformation{
      id,
      latStr.toDouble(),
      latOffsetStr.toDouble(),
      lonStr.toDouble(),
      lonOffsetStr.toDouble(),
      typeStr,
      hostilityStr
    });
  }

  return true;
}

//! parse xml node to extract key values used for hostility and type nodes.
bool ConfigurationSettings::parseKeyValueNodes(const QDomNodeList &configNode, std::map<QString, uint32_t> &keyValues, QString &msgError)
{
  for (int i = 0; i < configNode.size(); i++)
  {
    auto node = configNode.at(i);
    auto map = node.attributes();
    for (int i = 0; i < map.length(); ++i)
    {
      auto inode = map.item(i);
      auto attr = inode.toAttr();
      QString key = attr.value();
      if (key.isEmpty())
      {
        msgError = "Invalid empty key for the node(" + node.nodeName() + ") in the configuration file.";
        return false;
      }

      if (node.hasAttributes())
      {
        QString val = node.toElement().text();
        if (val.isEmpty())
        {
          msgError = "No value found for the key (" + key + ") in the configuration file.";
          return false;
        }
        if (val.startsWith("0x"))
        {
          const QString str = QLatin1String(val.toLatin1().data());
          bool ok;
          const unsigned int parsedValue = str.toUInt(&ok, 16);
          if (ok)
          {
            keyValues[key] = rgbaToMapLinkColour(parsedValue);
          }
        }
        else
        {
          const unsigned int parsedValue = val.toUInt();
          keyValues[key] = parsedValue;
        }

      }
    }
  }
  return true;
}

//! convert hex colours into maplink colours.
uint32_t ConfigurationSettings::rgbaToMapLinkColour(uint32_t rgba)
{
  return (rgba >> 8) | (0x40 << 24);
}