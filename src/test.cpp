#include "utm_tools/utm_converter.hpp"

int main(int argc, char **argv)
{
    UTMConverter::lla lla_coords;
    lla_coords.latitude = 39.286694;
    lla_coords.longitude = 35.683594;
    lla_coords.alt = 200;

    UTMConverter converter;
    UTMConverter::utm utm_coords = converter.convert_lla2utm(lla_coords);
    converter.print_utm(utm_coords);

    return 0;
}