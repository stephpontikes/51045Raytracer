#include "vector3.h"
// bounce count, color, direction

class Ray {
   public:
    Ray(Vector3<double> d, double bc, Vector3<double> c) : direction(d), bounce_count(bc), color(c) {}
    ~Ray() = default;

    Vector3<double> direction;
    double bounce_count;
    Vector3<double> color;

    void bounce(Vector3<double>& new_direction, Vector3<double> new_color) {
        bounce_count++;
        direction = new_direction;

        color.x = (new_color.x + (bounce_count * color.x)) / (1 + bounce_count);
        color.y = (new_color.y + (bounce_count * color.y)) / (1 + bounce_count);
        color.z = (new_color.z + (bounce_count * color.z)) / (1 + bounce_count);
    }
};