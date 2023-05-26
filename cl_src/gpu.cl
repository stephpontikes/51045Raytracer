#pragma OPENCL EXTENSION cl_amd_printf : enable

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

struct CameraCL {
    float3 position;
    float3 u;
    float3 v;
    float3 center;
};

__constant int MAX_BOUNCE_COUNT = 1;
__constant int NUM_RAYS_PER_PIXEL = 5;

struct Ray createRay(float x, float y, __global struct CameraCL* camera) {
    float3 temp = camera->center + (camera->u * x);
    float3 screenCoord = temp + (camera->v * y);

    float3 dir = screenCoord - camera->position;

    float3 rgb = {255.0, 255.0, 255.0};
    struct Color color = {rgb};
    struct Ray result = {camera->position, dir, color};
    return result;
}

struct HitData intersection(struct Ray ray, struct Sphere sphere) {
    struct HitData result;
    result.didHit = false;
    result.distance = INFINITY;

    float3 v = ray.position - sphere.position;
    // float k1 = dot(ray.direction, ray.direction);
    float k2 = 2 * dot(v, ray.direction);
    float k3 = dot(v, v) - sphere.radius * sphere.radius;
    float d = k2 * k2 - 4 * k3;

    if (d > 0.0) {
        float discSqrt = sqrt(d);
        float i1 = (-k2 + discSqrt) / 2.0;
        float i2 = (-k2 - discSqrt) / 2.0;

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

float3 reflect(float3 R, float3 N) {
    return 2.0f * N * dot(N, R) - R;
}

float interpolateFloat(float from, float to, float factor) {
    return (1.0 - factor) * from + factor * to;
}

float3 interpolate(float3 from, float3 to, float factor) {
    return (float3)(interpolateFloat(from.x, to.x, factor),
                    interpolateFloat(from.y, to.y, factor),
                    interpolateFloat(from.z, to.z, factor));
}

struct HitData getClosestHit(struct Ray cameraRay, __global struct Sphere* objects, int objectsLen) {
    struct HitData closestHit;
    closestHit.didHit = false;
    struct HitData currentHit;
    closestHit.didHit = false;
    closestHit.hitIndex = -1;
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

float3 handleHit(struct Ray* cameraRay, struct Sphere* material,
                 struct HitData hitData, uint2 seed, uint gid) {
    struct Color color = material->color;

    // Move ray
    cameraRay->position = hitData.hitPoint;
    float3 diffuseDir = normalize(randomReboundDirection(seed, gid, hitData.hitNormal) + hitData.hitNormal);
    float3 specularDir = reflect(cameraRay->direction, hitData.hitNormal);
    bool isSpecular = material->reflectivity >= randomFloat(seed, gid, 0.0, 1.0);
    cameraRay->direction = normalize(interpolate(diffuseDir, specularDir, material->smoothness * isSpecular));
    float3 emittedLight = color.rgb * material->luminosity;

    float3 incomingLight = (emittedLight * cameraRay->color.rgb) / 255.0;
    float3 reflectivityColor = interpolate(color.rgb, (float3)(255.0, 255.0, 255.0), isSpecular);
    cameraRay->color.rgb = (cameraRay->color.rgb * reflectivityColor) / 255.0;

    return incomingLight;
}

float3 bounceRay(struct Ray cameraRay,
                 __global struct Sphere* objects, int objectsLen, uint2 seed, uint gid) {
    struct Ray* ray = &cameraRay;
    float3 incomingLight = (float3)(0.0, 0.0, 0.0);
    for (int i = 0; i < MAX_BOUNCE_COUNT; i++) {
        struct HitData hitData = getClosestHit(*ray, objects, objectsLen);

        if (hitData.didHit && hitData.hitIndex >= 0) {
            struct Sphere current = objects[i];
            incomingLight += handleHit(ray, &current, hitData, seed, gid);
        } else {
            break;
        }
    }

    return incomingLight;
}

float3 trace(int x, int y, int imgWidth, int imgHeight, __global struct CameraCL* camera, __global struct Sphere* objects, int objectsLen, uint2 seed, uint gid) {
    float xFact = 1.0 / (imgWidth / 2.0);
    float yFact = 1.0 / (imgHeight / 2.0);

    struct Ray cameraRay;
    float normX = x * xFact - 1.0;
    float normY = y * yFact - 1.0;

    float3 incomingLight = (float3)(0.0, 0.0, 0.0);

    for (int j = 0; j < NUM_RAYS_PER_PIXEL; j++) {
        cameraRay = createRay(normX, normY, camera);
        incomingLight += bounceRay(cameraRay, objects, objectsLen, seed, gid);
    }

    incomingLight /= NUM_RAYS_PER_PIXEL;
    return incomingLight;
}

__kernel void renderer(__global struct Color* screen_buffer, __global struct Sphere* sphere_buffer, uint spheres_length, __global struct CameraCL* camera, __global uint2* seed_memory) {
    uint global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);

    uint x = get_global_id(0);
    uint y = get_global_id(1);

    uint width = get_global_size(0);
    uint height = get_global_size(1);

    float3 color_set = trace(x, y, width, height, camera, sphere_buffer, spheres_length, *seed_memory, global_id);

    screen_buffer[y * width + x].rgb.x = color_set.x;
    screen_buffer[y * width + x].rgb.y = color_set.y;
    screen_buffer[y * width + x].rgb.z = color_set.z;
}
