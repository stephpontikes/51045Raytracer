#ifndef OPENCL_STRUCT_H
#define OPENCL_STRUCT_H

#include "CL/opencl.hpp"

typedef struct ColorCL {
    cl_float3 rgb;
} ColorCL;

typedef struct SphereCL {
    cl_float3 pos;
    cl_float radius;
    cl_float3 color;
    cl_float reflectivity;
    cl_float luminosity;
    cl_float smoothness;
} SphereCL;

typedef struct TriangleCL {
    cl_float3 pos;
    cl_float3 v1;
    cl_float3 v2;
    cl_float3 v3;
} TriangleCL;

typedef struct GlossyCL {
    cl_float3 color;
    cl_float reflectivity;
    cl_float luminosity;
    cl_float smoothness;
} GlossyCL;

typedef struct MatteCL {
    cl_float3 color;
    cl_float reflectivity;
    cl_float luminosity;
    cl_float smoothness;
} MatteCL;

typedef struct MirrorCL {
    cl_float3 color;
    cl_float reflectivity;
    cl_float luminosity;
    cl_float smoothness;
} MirrorCL;

typedef struct LightCL {
    cl_float3 color;
    cl_float reflectivity;
    cl_float luminosity;
    cl_float smoothness;
} LightCL;

typedef struct HitDataCL {
    bool didHit;
    float distance;
    cl_float3 hitPoint;
    cl_float3 hitNormal;
    int hitIndex;
} HitDataCL;

typedef struct CameraCL {
    cl_float3 position;
    cl_float3 u;
    cl_float3 v;
    cl_float3 center;
} CameraCL;

#endif