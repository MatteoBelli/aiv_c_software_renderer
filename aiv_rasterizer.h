#include "aiv_math.h"
#include <string.h>

typedef struct context
{
    int width;
    int height;
    vector3_t camera;

    unsigned char *framebuffer;
} context_t;

typedef struct color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} color_t;

typedef struct vertex
{
    struct vector3 position;
    struct vector3 normal;
    struct color color;

    int raster_x;
    int raster_y;
} vertex_t;

typedef struct triangle
{
    struct vertex a;
    struct vertex b;
    struct vertex c;
} triangle_t;

context_t context_new(int width, int height);
void clear_buffer(context_t *context, size_t size);

color_t new_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
color_t color_red();
color_t color_green();
color_t color_blue();

vertex_t vertex_new(vector3_t position);

triangle_t triangle_new(vertex_t a, vertex_t b, vertex_t c);

void put_pixel(vector2_t vertex_pos, context_t *context, color_t color);
void draw_line(float start_y, float start_x, float end_x, float end_y, context_t *ctx);
void rasterize(struct context *ctx, struct triangle *triangle);