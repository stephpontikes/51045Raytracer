#ifndef MESH_H
#define MESH_H

#include <type_traits>

#include "geometries.h"
#include "materials.h"
#include "variadics/my_type_traits.h"

namespace mpcs51045 {

template <typename G, typename M>
struct Mesh
    : dual_multiple_inheriter<Mesh, direct_bases_t<G, geometry_types>, direct_bases_t<M, material_types>> {
    Mesh() = default;

    template <typename... Ts>
    Mesh(Vector3<double> const& color, Ts&&... ts) {
        geometry = make_unique<G>(std::forward<Ts>(ts)...);
        material = make_unique<M>(color);
    }

    Mesh(Mesh const& m) : geometry(m->geometry->clone()), material(m->material->clone()) {}
    Mesh(G const& g, M const& m) : geometry(g.clone()), material(m.clone()) {}

    std::unique_ptr<Geometry> geometry;
    std::unique_ptr<Material> material;

    virtual ~Mesh() = default;
};

using GlossyMeshFactory = parallel_mesh_factory<Mesh, Glossy, Sphere(Vector3<double>, double), Triangle(Vector3<double>, Vector3<double>, Vector3<double>, Vector3<double>)>;
using MatteMeshFactory = parallel_mesh_factory<Mesh, Matte, Sphere(Vector3<double>, double), Triangle(Vector3<double>, Vector3<double>, Vector3<double>, Vector3<double>)>;
using LightMeshFactory = parallel_mesh_factory<Mesh, Light, Sphere(Vector3<double>, double), Triangle(Vector3<double>, Vector3<double>, Vector3<double>, Vector3<double>)>;

}  // namespace mpcs51045

#endif