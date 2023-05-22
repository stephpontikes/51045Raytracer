#ifndef MESH_H
#define MESH_H

#include <type_traits>

#include "geometries.h"
#include "materials.h"

using std::is_base_of_v;
using std::is_same;
using std::negation_v;

namespace mpcs51045 {
template <typename G, typename M>
    requires is_base_of_v<Geometry, G> &&
             is_base_of_v<Material, M>
struct Mesh {
    Mesh(G const& g, Light const& m) : geometry(g), material(m) {}

    G geometry;
    Light material;
};

using GlossyMeshFactory = parallel_mesh_factory<Mesh, tuple<Sphere, Triangle>, Glossy>;
using MatteMeshFactory = parallel_mesh_factory<Mesh, tuple<Sphere, Triangle>, Glossy>;

}  // namespace mpcs51045

#endif