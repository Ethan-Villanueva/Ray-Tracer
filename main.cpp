#include <iostream>
#include <fstream>
#include "vec3.h"
#include "colour.h"
#include "ray.h"

const double ASPECT_RATIO = 16.0 / 9.0;

double hit_sphere(const point3 centre, const double radius, const ray& r) {
    // find if and where ray hits using sphere func x^2 + y^2 + z^2 = r^2 in terms of vectors
    // rearrange for quadratic eq
    vec3 r_to_c = centre - r.origin();
    auto a = r.direction().length_squared();
    auto b = -2 * dot(r_to_c, r.direction());
    auto c = r_to_c.length_squared() - (radius*radius);

    // use discriminant to check if sphere hit
    // calulate t how far along ray hits surface 
    auto discriminant = (b*b) - 4*a*c;
    if (discriminant >= 0.0) {
        auto t = (-b - std::sqrt(discriminant)) / (2*a);
        return t;
    }

    // does not hit sphere
    return -1;
}

colour ray_colour(const ray& r) {
    // Sphere of centre (0,0,-1), radius 0.5
    const point3 sph_centre(0,0,-1);
    const double sph_radius = 0.5;
    double t = hit_sphere(sph_centre, sph_radius, r);

    // render sphere if hit
    if (t > 0.0) {
        point3 hit_point = r.at(t);
        // calc normal of the surface ray hits
        vec3 n = hit_point - sph_centre;
        vec3 n_normalised = unit_vector(n);
        // map to colour
        return 0.5 * (colour(n_normalised.getX() + 1, n_normalised.getY() + 1, n_normalised.getZ() + 1));
    }

    // render background
    // normalise
    vec3 unit_dir = unit_vector(r.direction());

    // blend factor (scale y to [0,1])
    auto a = 0.5 * (unit_dir.getY() + 1.0);

    // lerp between white and blue
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