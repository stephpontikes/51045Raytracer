#ifndef RAY_H
#define RAY_H

#include "ray.h"
#include "hit.h"

namespace Raytracer51045 {
    class Visitor {
    public:
        virtual void visit() = 0;
    };

    class GeometryVisitor : public Visitor {
        void visit() {
            // call intersect method
        }
    };

    class RayVisitor : public Visitor {}

}  // namespace Raytracer51045

#endif
