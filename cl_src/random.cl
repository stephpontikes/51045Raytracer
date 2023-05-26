__constant int RAND_MAX = 0x7fffffff;

int randomInteger(uint2 randoms, uint gid)  // 1 <= *seed < m
{
    ulong seed = randoms.x + gid;
    seed = (seed * 0x6DEECE66DL + 0xBL) & ((1L << 48) - 1);
    uint result = seed >> 16;

    return result;
}

float randomFloat(uint2 seed, uint gid, float min, float max) {
    int rand = randomInteger(seed, gid);
    if (rand < 0) {
        rand += RAND_MAX;
    }
    float random = (max - min) * ((float)rand / (float)RAND_MAX) + min;
    return random;
}

float3 randomDirection(uint2 seed, uint gid) {
    float3 result;
    float low = -10.0;
    float high = 10.0;

    result.x = randomFloat(seed, gid, low, high);
    result.y = randomFloat(seed, gid, low, high);
    result.z = randomFloat(seed, gid, low, high);

    return result;
}

float3 handleInvert(float3 normal, float3 random) {
    if (dot(normal, random) < 0) {
        return random * -1.0;
    } else {
        return random;
    }
}

float3 randomReboundDirection(uint2 seed, uint gid, float3 normal) {
    return handleInvert(normal, randomDirection(seed, gid));
}
