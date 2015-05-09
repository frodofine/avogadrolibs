/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2010 Geoffrey R. Hutchison
  Copyright 2013 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "gaussiancube.h"

#include <avogadro/core/molecule.h>
#include <avogadro/core/utilities.h>
#include <avogadro/core/cube.h>

#include <iostream>

namespace Avogadro {
namespace QuantumIO {

GaussianCube::GaussianCube()
{
}

GaussianCube::~GaussianCube()
{
}

std::vector<std::string> GaussianCube::fileExtensions() const
{
  std::vector<std::string> extensions;
  extensions.push_back("cube");
  return extensions;
}

std::vector<std::string> GaussianCube::mimeTypes() const
{
  return std::vector<std::string>();
}

bool GaussianCube::read(std::istream &in, Core::Molecule &molecule)
{
  // Using the existing cube class
  Core::Cube cube;

  // Variables we will need
  std::string line;
  std::vector<std::string> list;

  int nAtoms;
  Avogadro::Vector3 min, spacing;
  Avogadro::Vector3i dim;

  // Gaussian Cube format is very specific

  // Read and set name
  getline(in, line);
  cube.setName(line);

  // Read and skip field title (we may be able to use this to setCubeType in the future)
  getline(in, line);   

  // Next line contains nAtoms and m_min
  in >> nAtoms;
  for (auto i = 0; i < 3; ++i) in >> min(i);
  getline(in, line);

  // Next 3 lines contains spacing and dim
  for (auto i = 0; i < 3; ++i) {
    getline(in, line);
    line = Core::trimmed(line);
    list = Core::split(line, ' ');
    dim(i) = Core::lexicalCast<int>(list[0]);  
    spacing(i) = Core::lexicalCast<double>(list[i + 1]);  
  }

  // Geometry block
  Avogadro::Vector3 pos;
  for (auto i = 0; i < nAtoms; ++i) {
    getline(in, line);
    line = Core::trimmed(line);
    list = Core::split(line, ' ');
    short int atomNum = Core::lexicalCast<short int>(list[0]);
    Core::Atom a = molecule.addAtom(static_cast<unsigned char>(atomNum));
    for (auto j = 2; j < 5; ++j) pos(j - 2) = Core::lexicalCast<double>(list[j]);
    pos = pos * Avogadro::BOHR_TO_ANGSTROM;
    a.setPosition3d(pos);
  }

  // Render molecule  
  molecule.perceiveBondsSimple();

  // Cube block, set limits and populate data
  cube.setLimits(min, dim, spacing);
  std::vector<double> values;
  // push_back is slow for this, resize vector first
  values.resize(dim(0) * dim(1) * dim(2));
  for (auto i = 0; i < values.size(); ++i) in >> values[i];
  cube.setData(values);

  return true;
}

bool GaussianCube::write(std::ostream &out, const Core::Molecule &molecule)
{
    // Not implemented yet
    return false;
}

} // End QuantumIO namespace
} // End Avogadro namespace
