#include "aiv_math.h"

vector3_t vector3_new(float x, float y, float z)
{
    vector3_t new_vector = {.x = x, .y = y, .z = z};
    return new_vector;
}

vector3_t vector3_zero()
{
    vector3_t new_vector = {.x = 0, .y = 0, .z = 0};
    return new_vector;
}

vector2_t vector2_new(float x, float y)
{
    vector2_t new_vector = {.x = x, .y = y};
    return new_vector;
}

vector2_t vector2_zero()
{
    vector2_t new_vector = {.x = 0, .y = 0};
    return new_vector;
}

float inversed_slope(float x0, float x1, float y0, float y1)
{
    return (x1 - x0) / (y1 - y0);
}

float lerp(float start, float end, float gradient)
{
    return start + (end - start) * gradient;
}

float gradient(float i, float p0, float p1)
{
    float to_return;
    if (p0 != p1)
        return to_return = (i - p0) / (p1 - p0);

    return p0 || p1;
}

vector2_t point_to_screen(float point_x, float point_y, int screen_width, int screen_height)
{
    vector2_t out_pixel_point;
    out_pixel_point.x = ((point_x + 1) / 2) * screen_width;
    out_pixel_point.y = screen_height - (((point_y + 1) / 2) * screen_height);
    return out_pixel_point;
}

vector2_t screen_to_point(float screen_point_x, float screen_point_y, int screen_width, int screen_height)
{
    vector2_t point;

    point.x = ((screen_point_x * 2) / screen_width) - 1;
    point.y = ((screen_point_y * 2) / screen_height) - 1;

    return point;
}