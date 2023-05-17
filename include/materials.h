#include "factory.h"
#include "vector3.h"

/*
Translucence
Transparence
Opacity
Reflectivity
Glossiness
*/

// TODO: implement visitor pattern for materials (visit = ray bounce)

using namespace mpcs51045;

class Material {
   public:
    virtual ~Material() = default;
    virtual double reflectivity() = 0;

   protected:
    Vector3<double> color;
};

class Glossy : public Material {
   public:
    Glossy() {
        color = Vector3<double>(0, 0, 0);
    }
    Glossy(Vector3<double> c) {
        color = c;
    }
    double reflectivity() {
        return 0.9;
    }
};

class Matte : public Material {
   public:
    Matte() {
        color = Vector3<double>(0, 0, 0);
    }
    Matte(Vector3<double> c) {
        color = c;
    }
    double reflectivity() {
        return 0.1;
    }
};

using AbstractMaterialFactory = mpcs51045::abstract_factory<Material>;
using GlossyMaterialFactory = mpcs51045::concrete_factory<AbstractMaterialFactory, Glossy>;
using MatteMaterialFactory = mpcs51045::concrete_factory<AbstractMaterialFactory, Matte>;