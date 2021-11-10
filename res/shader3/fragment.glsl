#version 400 core
precision highp float;

layout (origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

uniform dvec2 u_one_over_scale;
uniform dvec2 u_offset;

uniform int u_max_it; 

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    dvec2 z = dvec2(0.0, 0.0);
    dvec2 c = dvec2(gl_FragCoord.xy) * u_one_over_scale + u_offset;

    int n = 0;

    while (true) {
        double x_sq = z.x*z.x;
        double y_sq = z.y*z.y;

        z = dvec2(x_sq - y_sq + c.x, 2.0 * z.x * z.y + c.y); // z = z² + c

        if (x_sq + y_sq > 4.0 || n >= u_max_it) {
            break;
        }

        n++;
    }

    if (n == u_max_it) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
       float hue = float(n) / float(u_max_it);
       
       gl_FragColor = vec4(hsv2rgb(vec3(hue, 1.0, 1.0)), 1.0);
    }
}