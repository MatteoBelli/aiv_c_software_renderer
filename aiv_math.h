#include <math.h>

typedef struct vector3
{
    float x;
    float y;
    float z;
} vector3_t;

vector3_t vector3_new(float x, float y, float z);
vector3_t vector3_zero();

typedef struct vector2
{
    float x;
    float y;
} vector2_t;

vector2_t vector2_new(float x, float y);
vector2_t vector2_zero();

float inversed_slope(float x0, float x1, float y0, float y1);
float lerp(float start, float end, float gradient);
float gradient(float i, float p0, float p1);

vector2_t point_to_screen(float point_x, float point_y, int screen_width, int screen_height);
vector2_t screen_to_point(float screen_point_x, float screen_point_y, int screen_width, int screen_height);