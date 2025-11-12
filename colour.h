#include "vec3.h"

void write_colour(std::ostream &out, const colour& pixel_colour) {
    out << static_cast<int>(255.999 * pixel_colour.getX()) << ' '
        << static_cast<int>(255.999 * pixel_colour.getY()) << ' '
        << static_cast<int>(255.999 * pixel_colour.getZ()) << '\n';
}