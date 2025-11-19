#include <iostream>
#include <fstream>
#include "vec3.h"
#include "colour.h"
#include "ray.h"

const double ASPECT_RATIO = 16.0 / 9.0;

bool hit_sphere(const point3 centre, const double radius, const ray& r) {
    // find if ray hits using sphere func x^2 + y^2 + z^2 = r^2 in terms of vectors
    // rearrange for quadratic eq
    vec3 r_to_c = centre - r.origin();
    auto a = r.direction().length_squared();
    auto b = -2 * dot(r_to_c, r.direction());
    auto c = r_to_c.length_squared() - (radius*radius);

    // use discriminant to check if sphere hit
    auto discriminant = (b*b) - 4*a*c;
    return (discriminant >= 0.0);
}

colour ray_colour(const ray& r) {
    // Sphere of centre (0,0,-1), radius 0.5
    // If ray hits, return red
    point3 sph_centre(0,0,-1);
    if (hit_sphere(sph_centre, 0.5, r)) {
        return colour(1,0,0);
    }

    // Normalise
    vec3 unit_dir = unit_vector(r.direction());

    // Blend factor (scale y to [0,1])
    auto a = 0.5 * (unit_dir.getY() + 1.0);

    // Lerp between white and blue
    return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
}

int main() {
    // Image
    int image_width = 400;
    int image_height = std::max(1, static_cast<int>(image_width / ASPECT_RATIO));

    // Camera setup
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    point3 camera_point(0,0,0);

    vec3 viewport_u(viewport_width, 0, 0);
    vec3 viewport_v(0, -viewport_height, 0);
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_point - vec3(0,0,focal_length) - (viewport_u / 2) - (viewport_v / 2);
    auto pixel_centre_top_corner = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::ofstream image_file("image.ppm");

    // Render
    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_height; i++) {
        std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
        for (int j = 0; j < image_width; j++) {
            auto pixel_centre = pixel_centre_top_corner
                              + (j * pixel_delta_u)
                              + (i * pixel_delta_v); 

            auto ray_direction = pixel_centre - camera_point;
            ray r(camera_point, ray_direction);

            colour pixel = ray_colour(r);
            write_colour(image_file, pixel);
        }
    }

    image_file.close();

    std::clog << "\rDone.                 \n";
}