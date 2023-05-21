#ifndef MATERIALS_H
#define MATERIALS_H

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

namespace mpcs51045
{

    using std::make_unique;

    class Material
    {
    public:
        Material(Vector3<double> const &c) : mat_color(c) {}
        virtual ~Material() = default;

        virtual double reflectivity();
        virtual double luminosity();
        Vector3<double> color() { return mat_color; }

    private:
        Vector3<double> mat_color;
    };

    class Glossy : public Material
    {
    public:
        Glossy() = default;
        Glossy(Vector3<double> c) : Material(c) {}

        double reflectivity() { return 0.9; }
        double luminosity() { return 0; }
    };

    class Matte : public Material
    {
    public:
        Matte() = default;
        Matte(Vector3<double> c) : Material(c) {}

        double reflectivity() { return 0.1; }
        double luminosity() { return 0; }
    };

    using AbstractMaterialFactory = mpcs51045::abstract_factory<Material>;
    using GlossyMaterialFactory = mpcs51045::concrete_factory<AbstractMaterialFactory, Glossy>;
    using MatteMaterialFactory = mpcs51045::concrete_factory<AbstractMaterialFactory, Matte>;

}

#endif