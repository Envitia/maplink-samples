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

#ifndef SYMBOLSET_H
#define SYMBOLSET_H
#include <QFile>
class SymbolSet
{
public:
  //! map of hostility key values
  std::map<QString, uint32_t> m_hostilities;
  //! map of type key values
  std::map<QString, uint32_t> m_types;
  //! map of size key values
  std::map<QString, uint32_t> m_sizes;
};
#endif // SYMBOLSET_H
