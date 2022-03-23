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

#ifndef CONFIGURATIONSETTINGS_H
#define CONFIGURATIONSETTINGS_H
#include <QDomDocument>
#include <QFile>
#include <QVector>
#include "trackinformation.h"
#include "symbolset.h"

//!
//! Class to parse the xml configuration tracks and form the tracks information
//! which will be used by the simulator to create and move the tracks.
//!
class ConfigurationSettings
{
public:
  //! parse the configuration file and extract the tracks 
  bool parseConfigFile(const QString& filename, std::map<int, TrackInformation>& tracksInformation, QString& msgError);

  //! update the types and hostility for each track based on the symbol set
  bool updateTracks(std::map<int, TrackInformation>& tracks, const QString& symbolSetStr, QString& msgError);

  //! get available symbol sets to be used in the menu.
  const std::map<QString, SymbolSet>& symbolSets() const;

  //! default symbol set
  const QString& defaultSymbolSet() const;

private:
  //! check if the track hostility is valid for all the symbol sets
  bool isTrackHostilityValid(const QString& hostilityStr);

  //! check if the track type is valid for all the symbol sets
  bool isTrackTypeValid(const QString& typeStr);

  //! parse xml node to extract the default symbol set.
  bool parseDefaultSymbolSet(const QDomNodeList& configNode, QString& msgError);

  //! parse xml node to extract symbol sets key values.
  bool parseSymbolSets(const QDomNodeList& configNode, std::map<QString, SymbolSet>& symbolSets, QString& msgError);

  //! parse track nodes to extract tracks information.
  bool parseTrackNodes(const QDomNodeList& configNode, std::map<int, TrackInformation>& keyValues, QString& msgError);

  //! parse xml node to extract key values used for hostility and type nodes.
  static bool parseKeyValueNodes(const QDomNodeList& configNode, std::map<QString, uint32_t>& keyValues, QString& msgError);

  //! convert hex colours into maplink colours.
  static uint32_t rgbaToMapLinkColour(uint32_t rgba);

private:
  //! map of symbol sets key values
  std::map<QString, SymbolSet> m_symbolSets;
  //! default symbol set
  QString m_defaultSymbolSet;
  //! configuration file name.
  QString m_filename;

};
#endif // CONFIGURATIONSETTINGS_H
