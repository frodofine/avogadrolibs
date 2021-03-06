/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2018 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#ifndef AVOGADRO_CORE_RESIDUE_H
#define AVOGADRO_CORE_RESIDUE_H

#include "avogadrocore.h"

#include <map>
#include <string>

#include "array.h"
#include "bond.h"
#include "vector.h"

namespace Avogadro {
namespace Core {

class Atom;
class Molecule;

/**
 * @class Residue residue.h <avogadro/core/residue.h>
 * @brief The Residue class represents a chemical residue, used commonly in the
 * PDB format.
 */
class AVOGADROCORE_EXPORT Residue
{
public:
  /** Type for atom name map. */
  typedef std::map<std::string, Atom> AtomNameMap;

  /** Creates a new, empty residue. */
  Residue();
  Residue(std::string& name);
  Residue(std::string& name, Index& number);
  Residue(std::string& name, Index& number, char& id);

  Residue(const Residue& other);

  Residue& operator=(Residue other);

  virtual ~Residue();

  inline std::string residueName() { return m_residueName; }

  inline void setResidueName(std::string& name) { m_residueName = name; }

  inline Index residueId() { return m_residueId; }

  inline void setResidueId(Index& number) { m_residueId = number; }

  inline char chainId() { return m_chainId; }

  inline void setChainId(char& id) { m_chainId = id; }

  /** Adds an atom to the residue class */
  void addResidueAtom(std::string& name, Atom& atom);

  /** Returns a vector containing the atoms added to the residue */
  std::vector<Atom> residueAtoms();

  /** Sets bonds to atoms in the residue based on data from residuedata header
   */
  void resolveResidueBonds(Molecule& mol);

protected:
  std::string m_residueName;
  Index m_residueId;
  char m_chainId;
  AtomNameMap m_atomNameMap;
};

} // end Core namespace
} // end Avogadro namespace

#endif // AVOGADRO_CORE_RESIDUE_H
