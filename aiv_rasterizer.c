#include "aiv_rasterizer.h"

color_t new_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    color_t color = {.r = r, .g = g, .b = b, .a = a};
    return color;
}

color_t color_red()
{
    color_t color = {.r = 255, .g = 0, .b = 0, .a = 255};
    return color;
}

color_t color_green()
{
    color_t color = {.r = 0, .g = 255, .b = 0, .a = 255};
    return color;
}

color_t color_blue()
{
    color_t color = {.r = 0, .g = 0, .b = 255, .a = 255};
    return color;
}

vertex_t vertex_new(vector3_t position)
{
    vertex_t vertex;
    memset(&vertex, 0, sizeof(vertex_t));
    vertex.position = position;
    return vertex;
}

triangle_t triangle_new(vertex_t a, vertex_t b, vertex_t c)
{
    triangle_t triangle = {.a = a, .b = b, .c = c};
    return triangle;
}

context_t context_new(int width, int height)
{
    context_t context;
    context.width = 600;
    context.height = 600;
    context.camera_position = vector3_new(0, 0, -5);
    context.camera_fov = 60;
    context.framebuffer = NULL;
    context.triangles = NULL;
    context.triangles_array_size = 0;
    return context;
}

void clear_buffer(context_t *context, size_t size)
{
    memset(context->framebuffer, 0, size);
}

int add_triangle(context_t *context, triangle_t triangle)
{
    context->triangles_array_size++;
    void *resized_area = realloc(context->triangles, sizeof(triangle_t) * context->triangles_array_size);
    if (!resized_area)
    {
        context->triangles_array_size--;
        return -1;
    }

    context->triangles = (triangle_t *)resized_area;
    context->triangles[context->triangles_array_size - 1] = triangle;
    return 0;
}

void put_pixel(vector2_t vertex_pos, context_t *context, color_t color)
{
    if (vertex_pos.x < 0 || vertex_pos.y < 0 || vertex_pos.x >= context->width || vertex_pos.y >= context->height)
        return;

    int index = ((vertex_pos.y * context->width) + vertex_pos.x) * 4;
    context->framebuffer[index++] = color.r;
    context->framebuffer[index++] = color.g;
    context->framebuffer[index++] = color.b;
    context->framebuffer[index] = color.a;
}

void draw_line(float start_y, float start_x, float end_x, float end_y, context_t *ctx)
{
    for (float i = start_y; i < end_y; i++)
    {
        float gradient_pixel_p0_p1 = gradient(i, end_y, start_y);

        float lerp_value = lerp(end_x, start_x, gradient_pixel_p0_p1);
        vector2_t pixel;
        pixel.y = i;
        pixel.x = lerp_value;

        put_pixel(pixel, ctx, new_color(255, 0, 0, 255));
    }
}

static void update_vertex_view_position(context_t *context, vertex_t *vertex)
{
    vertex->view_position.x = vertex->position.x - context->camera_position.x;
    vertex->view_position.y = vertex->position.y - context->camera_position.y;
    vertex->view_position.z = vertex->position.z - context->camera_position.z;
}

static void calculate_raster_value(context_t *context, vertex_t *vertex)
{
    float fov = (60.0 / 2) * (3.1416 / 180);
    float camera_distance = tan(fov);
    float projected_x = vertex->view_position.x / (camera_distance * vertex->view_position.z);
    float projected_y = vertex->view_position.y / (camera_distance * vertex->view_position.z);

    vector2_t raster_point = point_to_screen(projected_x, projected_y, context->width, context->height);
    vertex->raster_x = raster_point.x;
    vertex->raster_y = raster_point.y;
}

void rasterize(context_t *context)
{
    for (int i = 0; i < context->triangles_array_size; i++)
    {
        triangle_t triangle = context->triangles[i];

        update_vertex_view_position(context, &triangle.a);
        update_vertex_view_position(context, &triangle.b);
        update_vertex_view_position(context, &triangle.c);

        calculate_raster_value(context, &triangle.a);
        calculate_raster_value(context, &triangle.b);
        calculate_raster_value(context, &triangle.c);

        vertex_t P[3] = {triangle.a, triangle.b, triangle.c};

        vertex_t temp;
        if (P[1].raster_y < P[0].raster_y)
        {
            temp = P[0];
            P[0] = P[1];
            P[1] = temp;
        }
        if (P[2].raster_y < P[1].raster_y)
        {
            temp = P[2];
            P[2] = P[1];
            P[1] = temp;
        }
        if (P[1].raster_y < P[0].raster_y)
        {
            temp = P[0];
            P[0] = P[1];
            P[1] = temp;
        }

        float slope_P0_P2;
        if (P[2].raster_y == P[0].raster_y)
        {
            slope_P0_P2 = 1.0f;
        }
        else
        {
            slope_P0_P2 = inversed_slope(P[0].raster_x, P[2].raster_x, P[0].raster_y, P[2].raster_y);
        }

        float slope_P0_P1;
        if (P[1].raster_y == P[0].raster_y)
        {
            slope_P0_P1 = 1.0f;
        }
        else
        {
            slope_P0_P1 = inversed_slope(P[0].raster_x, P[1].raster_x, P[0].raster_y, P[1].raster_y);
        }

        for (int y = P[0].raster_y; y <= P[1].raster_y; y++)
        {

            float gradient_p0_p1 = gradient(y, P[0].raster_y, P[1].raster_y);
            float start_x = lerp(P[0].raster_x, P[1].raster_x, gradient_p0_p1);

            float gradient_p0_p2 = gradient(y, P[0].raster_y, P[2].raster_y);
            float end_x = lerp(P[0].raster_x, P[2].raster_x, gradient_p0_p2);

            if (slope_P0_P1 <= slope_P0_P2)
            {
                for (int x = start_x; x <= end_x; x++)
                {
                    put_pixel(vector2_new(x, y), context, color_red());
                }
            }
            else if (slope_P0_P1 > slope_P0_P2)
            {
                for (int x = end_x; x <= start_x; x++)
                {
                    put_pixel(vector2_new(x, y), context, color_red());
                }
            }
        }

        for (int y = P[1].raster_y; y <= P[2].raster_y; y++)
        {
            float gradient_p1_p2 = gradient(y, P[1].raster_y, P[2].raster_y);
            float start_x = lerp(P[1].raster_x, P[2].raster_x, gradient_p1_p2);

            float gradient_p0_p2 = gradient(y, P[0].raster_y, P[2].raster_y);
            float end_x = lerp(P[0].raster_x, P[2].raster_x, gradient_p0_p2);

            if (slope_P0_P1 <= slope_P0_P2)
            {
                for (int x = start_x; x <= end_x; x++)
                {
                    put_pixel(vector2_new(x, y), context, color_red());
                }
            }
            else if (slope_P0_P1 > slope_P0_P2)
            {
                for (int x = end_x; x <= start_x; x++)
                {
                    put_pixel(vector2_new(x, y), context, color_red());
                }
            }
        }
    }
}