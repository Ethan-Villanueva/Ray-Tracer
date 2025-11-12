#include <iostream>
#include <fstream>
#include "vec3.h"
#include "colour.h"

int main() {
    // Image
    int image_width = 256;
    int image_height = 256;
    std::ofstream image_file("image.ppm");

    // Render
    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::cout << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            colour pixel(static_cast<double>(i) / (image_width-1),
                         static_cast<double>(j) / (image_height-1),
                         0.0);

            write_colour(image_file, pixel);
        }
    }

    image_file.close();

    std::cout << "\rDone.                 \n";
}