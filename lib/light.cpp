#include "../include/light.h"

Light::Light(Vector3 t_pos, float t_intensity, Vector3 t_color)
{
    pos       = t_pos;
    intensity = t_intensity;
    color     = t_color;
}


Light::Light(Vector3 t_pos, float t_intensity)
{
    pos       = t_pos;
    intensity = t_intensity;
    color     = Vector3(255);
}