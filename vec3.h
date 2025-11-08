#pragma once

#include <assert.h>
#include <math.h>

typedef struct {
    float x, y, z;
} vec3_t;

static inline vec3_t vec3_add(const vec3_t v, const vec3_t w) {
    return (vec3_t){
        .x = v.x + w.x,
        .y = v.y + w.y,
        .z = v.z + w.z
    };
}

static inline vec3_t vec3_sub(const vec3_t v, const vec3_t w) {
    return (vec3_t){
        .x = v.x - w.x,
        .y = v.y - w.y,
        .z = v.z - w.z
    };
}

static inline vec3_t vec3_scale(const vec3_t v, float s) {
    return (vec3_t){
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };
}

static inline float vec3_dot(const vec3_t v, const vec3_t w) {
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

static inline float vec3_length(const vec3_t v) {
    return sqrtf(vec3_dot(v, v));
}

static inline vec3_t vec3_norm(const vec3_t v) {
    float len = vec3_length(v);
    assert(len != 0.0f && "Cannot normalize zero-length vector");

    return vec3_scale(v, 1.0f / len);
}

