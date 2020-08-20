////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2020 ArangoDB GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Heiko Kernbach
/// @author Lars Maier
/// @author Markus Pfeiffer
////////////////////////////////////////////////////////////////////////////////

#include "VertexData.h"

#include <Pregel/Algos/AIR/AbstractAccumulator.h>

#include "velocypack/velocypack-aliases.h"

using namespace arangodb::pregel::algos::accumulators;

void VertexData::reset(AccumulatorsDeclaration const& globalAccumulatorsDeclaration,
                       AccumulatorsDeclaration const& vertexAccumulatorsDeclaration,
                       CustomAccumulatorDefinitions const& customDefinitions,
                       DataAccessDefinitions const& dataAccessDefinitions,
                       std::string documentId, VPackSlice const& doc, std::size_t vertexId) {
  _documentId = documentId;
  _document.clear();
  _document.add(doc);
  _vertexId = vertexId;
  _dataAccess = dataAccessDefinitions;

  for (auto&& acc : globalAccumulatorsDeclaration) {
    _globalAccumulators.emplace(acc.first, instantiateAccumulator(*this, acc.second, customDefinitions));
  }
  for (auto&& acc : vertexAccumulatorsDeclaration) {
    _vertexAccumulators.emplace(acc.first, instantiateAccumulator(*this, acc.second, customDefinitions));
  }
}

std::unique_ptr<AccumulatorBase> const& VertexData::accumulatorByName(std::string_view name) {
  return _vertexAccumulators.at(std::string{name}); // FIXME C++20
}
