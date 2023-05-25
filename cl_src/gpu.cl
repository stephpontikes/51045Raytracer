struct Sphere {
    float3 pos;
    float radius;
};

struct Triangle {
    float3 pos;
    float3 v1;
    float3 v2;
    float3 v3;
};

struct Glossy {
    float3 color;
    float reflectivity;
    float luminosity;
    float smoothness;
};

struct Matte {
    float3 color;
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
    float3 color;
    float reflectivity;
    float luminosity;
    float smoothness;
};




