#ifndef MESH_H
#define MESH_H

#include "geometries.h"
#include "materials.h"

namespace mpcs51045 {
    template<typename Geometry, typename Material>
    struct Mesh {
        Mesh(Geometry const& g, Material const& m) : geometry(g), material(m) {}

        Geometry geometry;
        Material material;
    };
}

#endif