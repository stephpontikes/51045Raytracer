#ifndef MATERIALS_H
#define MATERIALS_H

#include "factory.h"
#include "opencl_struct.h"
#include "variadics/variadic_examples.h"
#include "vector3.h"

/*
Translucence
Transparence
Opacity
Reflectivity
Glossiness
*/

// TODO: implement visitor pattern for materials (visit = ray bounce)

namespace mpcs51045 {

class Material {
   public:
    Material(Vector3<double> const& c) : mat_color(c) {}
    virtual ~Material() = default;

    virtual std::unique_ptr<Material> clone() const = 0;
    virtual double reflectivity() = 0;
    virtual double luminosity() = 0;
    virtual double smoothness() = 0;
    Vector3<double> color() { return mat_color; }

   private:
    Vector3<double> mat_color;
};

class Glossy : public Material {
   public:
    Glossy(Vector3<double> c) : Material(c) {}

    virtual std::unique_ptr<Material> clone() const override {
        return std::make_unique<Glossy>(*this);
    }
    double reflectivity() override { return 0.15; }
    double luminosity() override { return 0.0; }
    double smoothness() override { return 1.0; }
};

class Mirror : public Material {
   public:
    Mirror(Vector3<double> c) : Material(c) {}

    virtual std::unique_ptr<Material> clone() const override {
        return std::make_unique<Mirror>(*this);
    }
    double reflectivity() override { return 1.0; }
    double luminosity() override { return 0.0; }
    double smoothness() override { return 1.0; }
};

class Matte : public Material {
   public:
    Matte(Vector3<double> c) : Material(c) {}

    virtual std::unique_ptr<Material> clone() const override {
        return std::make_unique<Matte>(*this);
    }
    double reflectivity() override { return 0.0; }
    double luminosity() override { return 0.0; }
    double smoothness() override { return 0.5; }
};

class Light : public Material {
   public:
    Light(Vector3<double> c) : Material(c) {}

    virtual std::unique_ptr<Material> clone() const override {
        return std::make_unique<Light>(*this);
    }
    double reflectivity() override { return 0.0; }
    double luminosity() override { return 10.0; }
    double smoothness() override { return 0.0; }
};

// using AbstractMaterialFactory = mpcs51045::abstract_factory<Material>;
// using MaterialFactory = mpcs51045::concrete_factory<AbstractMaterialFactory,
//                                                     Glossy, Matte>;

GlossyCL toCL(Glossy& mat) {
    auto matColor = mat.color();
    cl_float3 color{matColor.x, matColor.y, matColor.z};

    return GlossyCL{color, cl_float{mat.reflectivity()},
                    cl_float{mat.luminosity()},
                    cl_float{mat.smoothness()}};
}

MatteCL toCL(Matte& mat) {
    auto matColor = mat.color();
    cl_float3 color{matColor.x, matColor.y, matColor.z};

    return MatteCL{color, cl_float{mat.reflectivity()},
                   cl_float{mat.luminosity()},
                   cl_float{mat.smoothness()}};
}

MirrorCL toCL(Mirror& mat) {
    auto matColor = mat.color();
    cl_float3 color{matColor.x, matColor.y, matColor.z};

    return MirrorCL{color, cl_float{mat.reflectivity()},
                    cl_float{mat.luminosity()},
                    cl_float{mat.smoothness()}};
}

LightCL toCL(Light& mat) {
    auto matColor = mat.color();
    cl_float3 color{matColor.x, matColor.y, matColor.z};

    return LightCL{color, cl_float{mat.reflectivity()},
                   cl_float{mat.luminosity()},
                   cl_float{mat.smoothness()}};
}

using material_types = typelist<Material, Glossy, Matte>;

}  // namespace mpcs51045

#endif