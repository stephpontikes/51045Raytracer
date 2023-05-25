
struct Color {
    float3 rgb;
};

struct Sphere {
    float3 position;
    float radius;
    struct Color color;
    float reflectivity;
    float luminosity;
    float smoothness;
};

struct Triangle {
    float3 position;
    float3 v1;
    float3 v2;
    float3 v3;
};

struct Glossy {
    struct Color color;
    float reflectivity;
    float luminosity;
    float smoothness;
};

struct Matte {
    struct Color color;
    float reflectivity;
    float luminosity;
    float smoothness;
};

struct Mirror {
    float3 color;
    float reflectivity;
    float luminosity;
    float smoothness;
};

struct Light {
    struct Color color;
    float reflectivity;
    float luminosity;
    float smoothness;
};

struct HitData {
    bool didHit;
    float distance;
    float3 hitPoint;
    float3 hitNormal;
    int hitIndex;
};

struct Ray {
    float3 position;
    float3 direction;
    struct Color color;
};

struct Camera {
    float3 position;
    float3 u;
    float3 v;
    float3 center;
};

int MAX_BOUNCE_COUNT = 3;
int NUM_RAYS_PER_PIXEL = 10;

struct Ray createRay(float x, float y, struct Camera camera) {
    float3 temp = camera.center + (camera.u * x);
    float3 screenCoord = temp + (camera.v * y);

    float3 dir = screenCoord - camera.position;

    float3 rgb = {255.0, 255.0, 255.0};
    struct Color color = {rgb};
    struct Ray result = {camera.position, dir, color};
    return result;
}

struct HitData intersection(struct Ray ray, struct Sphere sphere) {
    struct HitData result;

    float3 v = ray.position - sphere.position;

    float k1 = dot(ray.direction, ray.direction);
    float k2 = 2 * dot(v, ray.direction);
    float k3 = dot(v, v) - sphere.radius * sphere.radius;

    float d = k2 * k2 - 4 * k1 * k3;

    if (d > 0.0) {
        float discSqrt = sqrt(d);
        float i1 = (-k2 + discSqrt) / (2.0 * k1);
        float i2 = (-k2 - discSqrt) / (2.0 * k1);

        if ((i1 < 0.0) || (i2 < 0.0)) {
            result.didHit = false;
            return result;
        }

        result.didHit = true;

        if (i1 < i2) {
            result.distance = i1;
        } else {
            result.distance = i2;
        }
        result.hitPoint = ray.position + (normalize(ray.direction) * result.distance);
        result.hitNormal = normalize(result.hitPoint - sphere.position);
    } else {
        result.didHit = false;
    }

    return result;
}

// HitData sphereIntersect(Ray ray, Sphere sphere) {
//     struct HitData result;
//     // Sphere* sphere = geom.get();
//     // cout << "pass" << (sphere == nullptr) << endl;
//     float3 offsetRayPos = ray.position - sphere.coordinates;
//     float3 dir = nomalize(ray.direction);

//     // cout << "Coords: " << sphere.coordinates << ", Rad: " << sphere.radius << endl;
//     double a = Vector3<double>::dot(dir, dir);
//     double b = 2.0 * Vector3<double>::dot(offsetRayPos, dir);
//     double c = Vector3<double>::dot(offsetRayPos, offsetRayPos) -
//                sphere.radius * sphere.radius;

//     double discriminant = b * b - 4.0 * a * c;

//     if (discriminant > 0.0) {
//         double discSqrt = sqrt(discriminant);
//         double i1 = (-b + discSqrt) / (2.0 * a);
//         double i2 = (-b - discSqrt) / (2.0 * a);

//         if ((i1 < 0.0) || (i2 < 0.0)) {
//             result.didHit = false;
//             return result;
//         }

//         result.didHit = true;

//         if (i1 < i2) {
//             result.distance = i1;
//         } else {
//             result.distance = i2;
//         }

//         result.hitPoint = ray.position + (dir * result.distance);
//         result.hitNormal = result.hitPoint - sphere->coordinates;
//         result.hitNormal.normalize();
//     } else {
//         result.didHit = false;
//     }

//     return result;
// }
float3 reflect(float3 R, float3 N) {
    return 2.0f * N * dot(N, R) - R;
}

float interpolate(float from, float to, float factor) {
    return (1.0 - factor) * from + factor * to;
}

static float3 interpolate(float3 from, float3 to, float factor) {
    return float3(interpolate(from.x, to.x, factor),
                  interpolate(from.y, to.y, factor),
                  interpolate(from.z, to.z, factor));
}

struct HitData getClosestHit(struct Ray cameraRay, struct Sphere* objects, int objectsLen) {
    struct HitData closestHit;
    struct HitData currentHit;
    int closestIndex = -1;
    // Glossy g{Vector3<double>{255.0, 0.0, 255.0}};
    // Material& closestMat = g;
    // unique_ptr<Material> closestMat = make_unique<Glossy>(Glossy{Vector3<double>{0.0, 0.0, 0.0}});
    closestHit.distance = INFINITY;
    // Test for intersections with all objects (replace with visitor in future)
    for (int i = 0; i < objectsLen; i++) {
        struct Sphere current = objects[i];
        currentHit = intersection(cameraRay, current);

        if (currentHit.didHit && currentHit.distance < closestHit.distance) {
            closestHit.didHit = currentHit.didHit;
            closestHit.distance = currentHit.distance;
            closestHit.hitPoint = currentHit.hitPoint;
            closestHit.hitNormal = currentHit.hitNormal;
            closestHit.hitIndex = i;
        }
    }

    return closestHit;
}

float3 handleHit(struct Ray cameraRay, struct Sphere* material,
                 struct HitData hitData) {
    float3 color = material->color;

    // cout << "Ray Dir: " << cameraRay.direction << endl;
    // Move ray
    cameraRay.position = hitData.hitPoint;
    float3 diffuseDir = normalize(randomReboundDirection(hitData.hitNormal) + hitData.hitNormal);
    float3 specularDir = reflect(cameraRay.direction, hitData.hitNormal);
    bool isSpecular = material->reflectivity >= getRandomProbValue();
    cameraRay.direction = normalize(interpolate(diffuseDir, specularDir, material->smoothness * isSpecular));
    // cout << "New Direction: " << cameraRay.direction << endl;

    // cout << "Material Color: " << color << endl;
    // color /= 255.0;
    float3 emittedLight = color * material->luminosity;

    // cout << "Emitted Light: " << emittedLight << endl;
    // cout << "Luminosity: " << material->luminosity() << endl;
    float3 incomingLight = (emittedLight * cameraRay.color.rgb) / 255.0;
    // incomingLight *= 255.0;
    // cout << "Incoming Light: " << incomingLight << endl;
    // cout << "Old Ray Color: " << cameraRay.color << endl;
    float3 reflectivityColor = interpolate(color, float3(255.0, 255.0, 255.0), isSpecular);
    // cout << "Reflective Color: " << reflectivityColor << endl;
    cameraRay.color = (cameraRay.color.rgb * reflectivityColor) / 255.0;
    // cout << "New Ray Color: " << cameraRay.color << endl;

    return incomingLight;
}

float3 bounceRay(struct Ray cameraRay,
                 struct Sphere* objects, int objectsLen) {
    float3 incomingLight{0.0, 0.0, 0.0};
    for (int i = 0; i < MAX_BOUNCE_COUNT; i++) {
        struct HitData hitData = getClosestHit(cameraRay, objects);

        if (hitData.didHit) {
            struct Sphere current = objects[i];
            incomingLight += handleHit(cameraRay, &current, hitData);
        } else {
            break;
        }
    }

    return incomingLight;
}

float3 trace(int x, int y, int imgWidth, int imgHeight, struct Camera camera, struct Sphere* objects, int objectsLen) {
    float xFact = 1.0 / (imgWidth / 2.0);
    float yFact = 1.0 / (imgHeight / 2.0);

    int x = i % imgWidth;
    int y = i / imgWidth;

    struct Ray cameraRay;
    float normX = x * xFact - 1.0;
    float normY = y * yFact - 1.0;

    float3 incomingLight;

    for (int j = 0; j < NUM_RAYS_PER_PIXEL; j++) {
        cameraRay = createRay(normX, normY, camera);
        incomingLight += bounceRay(cameraRay, objects, objectsLen);
    }

    incomingLight /= NUM_RAYS_PER_PIXEL;
    // cout << "Light at " << x << ", " << y << ": " << incomingLight << endl;

    return incomingLight;
}

__kernel void render(__global struct Color* screen_buffer, __global struct Sphere* sphere_buffer, uint spheres_length) {
    uint x = get_global_id(0);
    uint y = get_global_id(1);

    uint width = get_global_size(0);
    uint height = get_global_size(1);

    float3 color_set = trace(x, y, width, height, camera, sphere_buffer, spheres_length);

    // set screen buffer color
    screen_buffer[y * width + x].x = color_set.x;
    screen_buffer[y * width + x].y y color_set.y;
    screen_buffer[y * width + x].z = color_set.z;
}