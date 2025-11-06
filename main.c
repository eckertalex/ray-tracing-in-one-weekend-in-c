#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    float x, y, z;
} vec3_t;

inline vec3_t vec3_add(const vec3_t v, const vec3_t w) {
    return (vec3_t){
        .x = v.x + w.x,
        .y = v.y + w.y,
        .z = v.z + w.z
    };
}

inline vec3_t vec3_sub(const vec3_t v, const vec3_t w) {
    return (vec3_t){
        .x = v.x - w.x,
        .y = v.y - w.y,
        .z = v.z - w.z
    };
}

inline vec3_t vec3_scale(const vec3_t v, float s) {
    return (vec3_t){
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };
}

inline float vec3_dot(const vec3_t v, const vec3_t w) {
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

inline float vec3_length(const vec3_t v) {
    return sqrtf(vec3_dot(v, v));
}

inline vec3_t vec3_norm(const vec3_t v) {
    float len = vec3_length(v);
    assert(len != 0.0f && "Cannot normalize zero-length vector");

    return vec3_scale(v, 1.0f / len);
}

typedef struct {
    vec3_t origin;
    vec3_t direction;
} ray_t;

inline bool hit_sphere(const vec3_t center, float radius, const ray_t r) {
    vec3_t oc = vec3_sub(r.origin, center);
    float a = vec3_dot(r.direction, r.direction);
    float b = 2.0f * vec3_dot(oc, r.direction);
    float c = vec3_dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return discriminant >= 0;
}

inline vec3_t ray_color(const ray_t r) {
    if (hit_sphere((vec3_t){.x = 0.0f, .y = 0.0f, .z = -1.0f}, 0.5f, r)) {
        // red
        return (vec3_t){.x = 1.0f, .y = 0.0f, .z = 0.0f};
    }

    vec3_t unit_direction = vec3_norm(r.direction);
    float t = 0.5f * (unit_direction.y + 1.0f);

    return vec3_add(
        vec3_scale(
            // white
            (vec3_t){.x = 1.0f, .y = 1.0f, .z = 1.0f},
            1.0f - t
        ),
        vec3_scale(
            // sky
            (vec3_t){.x = 0.5f, .y = 0.7f, .z = 1.0f},
            t
        )
    );
}

void write_color(FILE *out, vec3_t color) {
#ifndef NDEBUG
    if (color.x < 0.0f || color.x > 1.0f ||
        color.y < 0.0f || color.y > 1.0f ||
        color.z < 0.0f || color.z > 1.0f) {
        fprintf(stderr, "Color out of range: (%f, %f, %f)\n", color.x, color.y, color.z);
        assert(0);
    }
#endif

    color = vec3_scale(color, 255.999f);

    int r = (int)color.x;
    int g = (int)color.y;
    int b = (int)color.z;

    fprintf(out, "%d %d %d\n", r, g, b);
}

int main(void) {
    float aspect_ratio = 16.0f / 9.0f;
    int image_width = 400;
    int image_height = (int)(image_width / aspect_ratio);
    if (image_height < 1) image_height = 1;

    float viewport_height = 2.0f;
    float viewport_width = viewport_height * ((float)image_width / image_height);
    float focal_length = 1.0f;

    vec3_t camera_center = {.x = 0.0f, .y = 0.0f, .z = 0.0f};

    vec3_t viewport_u = {.x = viewport_width, .y = 0.0f, .z = 0.0f};
    vec3_t viewport_v = {.x = 0.0f, .y = -viewport_height, .z = 0.0f};

    vec3_t pixel_delta_u = vec3_scale(viewport_u, 1.0f / image_width);
    vec3_t pixel_delta_v = vec3_scale(viewport_v, 1.0f / image_height);

    vec3_t viewport_upper_left =
        vec3_sub(
            vec3_sub(
                vec3_sub(camera_center, (vec3_t){.x = 0.0f, .y = 0.0f, .z = focal_length}),
                vec3_scale(viewport_u, 0.5f)
            ),
            vec3_scale(viewport_v, 0.5f)
        );

    vec3_t pixel00_loc =
        vec3_add(
            viewport_upper_left,
            vec3_scale(vec3_add(pixel_delta_u, pixel_delta_v), 0.5f)
        );

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {

            vec3_t pixel_center =
                vec3_add(
                    vec3_add(
                        pixel00_loc,
                        vec3_scale(pixel_delta_u, (float)x)
                    ),
                    vec3_scale(pixel_delta_v, (float)y)
                );

            vec3_t ray_direction = vec3_sub(pixel_center, camera_center);

            ray_t r = {.origin = camera_center, .direction = ray_direction};

            vec3_t color = ray_color(r);
            write_color(stdout, color);
        }
    }

    return 0;
}
