/****************************************************************************
Copyright (c) 2008-2022 by Envitia Group PLC.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free 
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more 
details.

You should have received a copy of the GNU Lesser General Public License 
along with this program. If not, see <https://www.gnu.org/licenses/>.

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
