#include "utm_tools/utm_converter.hpp"

// Public Functions
// CONSTRUCTOR
UTMConverter::UTMConverter()
{
    a = 6378137.0;
    f = 298.257223563;
    b = a * (1.0 - 1.0 / f);
    K0 = 0.9996;
    X0 = 500000;

    e = sqrt((a * a - b * b) / (a * a));

    e2 = pow(e, 2);
    e4 = pow(e, 4);
    e6 = pow(e, 6);
    e8 = pow(e, 8);

    n = a * K0;

    error_threshold = 1e-16;
    rad2deg = 180.0 / M_PI;
    deg2rad = M_PI / 180.0;
}

// DESTRUCTOR
UTMConverter::~UTMConverter()
{
    // DO NOTHING
}

// ------------------------------------------------------------------------------------------
// UTM to MGRS Conversion
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_utm2mgrs(utm &utm_in, mgrs &mgrs_coordinates)
{
    int false_easting;
    int false_northing;

    // Get LLA Coordinates for the grid zone letter
    lla lla_coords;
    convert_utm2lla(utm_in, lla_coords);

    false_easting = trunc(utm_in.easting / 100000);
    false_northing = trunc(utm_in.northing / 100000);

    mgrs_coordinates.easting = utm_in.easting - false_easting * 100000;
    mgrs_coordinates.northing = utm_in.northing - false_northing * 100000;
    mgrs_coordinates.false_easting = lookup_easting(utm_in.easting, utm_in.grid_zone);
    mgrs_coordinates.false_northing = lookup_northing(utm_in.northing, utm_in.grid_zone);
    mgrs_coordinates.grid_zone = utm_in.grid_zone;
    mgrs_coordinates.grid_letter = lookup_gridzone(lla_coords.latitude);
    mgrs_coordinates.alt = utm_in.alt;
}

// ------------------------------------------------------------------------------------------
// MGRS to UTM Conversion
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_mgrs2utm(mgrs &mgrs_in, utm &utm_coords)
{
    utm_coords.grid_zone = mgrs_in.grid_zone;
    utm_coords.easting = mgrs_in.easting + decode_false_easting(mgrs_in.false_easting) * 100000.0;
    utm_coords.northing = mgrs_in.northing + decode_false_northing(mgrs_in.false_northing, mgrs_in.grid_zone) * 100000.0;
    utm_coords.alt = mgrs_in.alt;
}

// ------------------------------------------------------------------------------------------
// LLA TO UTM CONVERSION
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_lla2utm(lla &lla_in, utm &utm_coordinates)
{
    lat_deg = lla_in.latitude;
    lon_deg = lla_in.longitude;

    set_ceofs1();

    if (lat_deg < 0)
    {
        Y0 = 10000000.0;
    }
    else
    {
        Y0 = 0;
    }

    calc_gz();

    lon_rad = lon_deg * deg2rad;
    lat_rad = lat_deg * deg2rad;
    L0 = L0 * deg2rad;

    L = log(tan(M_PI / 4 + lat_rad / 2) * (pow(((1 - e * sin(lat_rad)) / (1 + e * sin(lat_rad))), (e / 2))));
    z_e = log(tan(M_PI / 4 + asin(sin(lon_rad - L0) / cosh(L)) / 2));
    z_n = atan(sinh(L) / cos(lon_rad - L0));

    Z_e = n * c1 * z_e + n * (c2 * cos(2 * z_n) * sinh(2 * z_e) + c3 * cos(4 * z_n) * sinh(4 * z_e) + c4 * cos(6 * z_n) * sinh(6 * z_e) + c5 * cos(8 * z_n) * sinh(8 * z_e));
    Z_n = n * c1 * z_n + n * (c2 * sin(2 * z_n) * cosh(2 * z_e) + c3 * sin(4 * z_n) * cosh(4 * z_e) + c4 * sin(6 * z_n) * cosh(6 * z_e) + c5 * sin(8 * z_n) * cosh(8 * z_e));

    easting = Z_e + X0;
    northing = Z_n + Y0;

    utm_coordinates.easting = easting;
    utm_coordinates.northing = northing;
    if (lat_deg > 0)
    {
        utm_coordinates.grid_zone = grid_zone;
    }
    else
    {
        utm_coordinates.grid_zone = -1 * grid_zone;
    }
    utm_coordinates.alt = lla_in.alt;
}

// ------------------------------------------------------------------------------------------
// UTM TO LLA CONVERSION
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_utm2lla(utm &utm_in, lla &lla_coords)
{
    easting = utm_in.easting;
    northing = utm_in.northing;
    grid_zone = utm_in.grid_zone;

    // Hemisphere Based Northing Offset
    if (grid_zone > 0.0)
    {
        Y0 = 0;
    }
    else
    {
        Y0 = 10000000.0;
    }

    // Central Meridian
    L0 = (6.0 * abs(grid_zone) - 183.0) * deg2rad;

    // Parameters for converging equation
    int max_nubmer_iterations;
    max_nubmer_iterations = 10000;
    double convergence_threshold;
    convergence_threshold = pow(10.0, -100.0);

    // Set Coefficients
    set_coefs2();

    // Calculate Footpring LLA
    z_n = (northing - Y0) / n / c1;
    z_e = (easting - X0) / n / c1;

    Z_n = z_n - c2 * sin(2.0 * z_n) * cosh(2.0 * z_e) - c3 * sin(4.0 * z_n) * cosh(4.0 * z_e) - c4 * sin(6.0 * z_n) * cosh(6.0 * z_e) - c5 * sin(8.0 * z_n) * cosh(8.0 * z_e);
    Z_e = z_e - c2 * cos(2.0 * z_n) * sinh(2.0 * z_e) - c3 * cos(4.0 * z_n) * sinh(4.0 * z_e) - c4 * cos(6.0 * z_n) * sinh(6.0 * z_e) - c5 * cos(8.0 * z_n) * sinh(8.0 * z_e);

    double p;

    p = asin(sin(Z_n) / cosh(Z_e));
    L = log(tan(M_PI / 4.0 + p / 2.0));

    lat_rad = 2.0 * atan(exp(L)) - M_PI / 2.0;
    lon_rad = L0 + atan(sinh(Z_e) / cos(Z_n));

    // Converge Latitude
    double lat0;
    lat0 = 0;

    int i = 0;
    double es;

    while (sqrt(pow(lat_rad - lat0, 2)) > convergence_threshold && i < max_nubmer_iterations)
    {
        lat0 = lat_rad;
        es = e * sin(lat0);
        lat_rad = 2 * atan(pow(((1.0 + es) / (1.0 - es)), (e / 2.0)) * exp(L)) - M_PI / 2.0;
        i++;
    }

    lat_deg = lat_rad * rad2deg;
    lon_deg = lon_rad * rad2deg;

    lla_coords.latitude = lat_deg;
    lla_coords.longitude = lon_deg;
    lla_coords.alt = utm_in.alt;
}

// ------------------------------------------------------------------------------------------
// LLADMS to LLA
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_lladms2lla(lladms &lladms_in, lla &lla_coords)
{
    lla_coords.latitude = lladms_in.latitude_degrees + lladms_in.latitude_minutes / 60.0 + lladms_in.latitude_seconds / (60.0 * 60.0);
    lla_coords.longitude = lladms_in.longitude_degrees + lladms_in.longitude_minutes / 60.0 + lladms_in.longitude_seconds / (60.0 * 60.0);
    lla_coords.alt = lladms_in.alt;
}

// ------------------------------------------------------------------------------------------
// LLA to LLADMS
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_lla2lladms(lla& lla_in, lladms& lladms_coords)
{
    lladms_coords.latitude_degrees = trunc(lla_in.latitude);
    double remain = lla_in.latitude - lladms_coords.latitude_degrees;
    lladms_coords.latitude_minutes = trunc(remain * 60.0);
    remain = remain - lladms_coords.latitude_minutes / 60.0;
    lladms_coords.latitude_seconds = remain * (60.0 * 60.0);

    lladms_coords.longitude_degrees = trunc(lla_in.longitude);
    remain = lla_in.longitude - lladms_coords.longitude_degrees;
    lladms_coords.longitude_minutes = trunc(remain * 60.0);
    remain = remain - lladms_coords.longitude_minutes / 60.0;
    lladms_coords.longitude_seconds = remain * (60.0 * 60.0);

    lladms_coords.alt = lla_in.alt;
}

// ------------------------------------------------------------------------------------------
// LLA to MGRS
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_lla2mgrs(lla& lla_in, mgrs& mgrs_out)
{
    utm utm_coords;
    convert_lla2utm(lla_in, utm_coords);
    convert_utm2mgrs(utm_coords, mgrs_out);
}

// ------------------------------------------------------------------------------------------
// MGRS to LLA
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_mgrs2lla(mgrs& mgrs_in, lla& lla_out)
{
    utm utm_coords;
    convert_mgrs2utm(mgrs_in, utm_coords);
    convert_utm2lla(utm_coords, lla_out);
}

// ------------------------------------------------------------------------------------------
// ECEF2LLA
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_ecef2lla(ecef& ecef_in, lla& lla_coords)
{
    lla_coords.longitude = atan2(ecef_in.y, ecef_in.x) * rad2deg;

    rho = sqrt(ecef_in.x * ecef_in.x + ecef_in.y * ecef_in.y);

    u = a * rho;
    v = b * ecef_in.z * (1.0 + (b * e2 / (1.0 - e2)) / sqrt(ecef_in.z * ecef_in.z + rho * rho));

    cosbeta = sign(u) / sqrt(1.0 + pow(v / u, 2));
    sinbeta = sign(v) / sqrt(1.0 + pow(u / v, 2));

    count = 0;
    converge = false;
    while (!converge)
    {
        cosprev = cosbeta;
        sinprev = sinbeta;
        u = rho - a * e2 * pow(cosbeta, 3);
        v = ecef_in.z + (b * e2 / (1 - e2)) * pow(sinbeta, 3);

        cosbeta = sign(a * u) / sqrt(1.0 + pow(b * v / (a * u), 2));
        sinbeta = sign(b * v) / sqrt(1.0 + pow(a * u / (b * v), 2));

        error = sqrt(pow(cosbeta - cosprev, 2) + pow(sinbeta - sinprev, 2));
        if (error < error_threshold)
        {
            converge = true;
        }
        count++;
        if (count > 100)
        {
            converge = true;
            std::cout << "Point Under Tolerance" << std::endl;
        }
        // std::cout<<count<<": "<<error<<std::endl;
    }

    lla_coords.latitude = atan2(v, u);
    cosbeta = cos(lla_coords.latitude);
    sinbeta = sin(lla_coords.latitude);

    lla_coords.latitude = lla_coords.latitude * rad2deg;

    N = a / sqrt(1 - e2 * sinbeta * sinbeta);
    lla_coords.alt = rho * cosbeta + (ecef_in.z + e2 * N * sinbeta) * sinbeta - N;
}

// ------------------------------------------------------------------------------------------
// ECEF2UTM
// ------------------------------------------------------------------------------------------
void UTMConverter::convert_ecef2utm(ecef& ecef_in, utm& utm_out)
{
    lla lla_coords;
    convert_ecef2lla(ecef_in, lla_coords);

    return convert_lla2utm(lla_coords, utm_out);
}

// ------------------------------------------------------------------------------------------
// PRINT FUNCTIONS
// ------------------------------------------------------------------------------------------
void UTMConverter::print_utm(utm utm_in)
{
    std::cout << std::fixed << "UTM:  " << utm_in.easting << " " << utm_in.northing << " " << utm_in.grid_zone << std::endl;
}

void UTMConverter::print_mgrs(mgrs mgrs_in)
{
    std::cout << std::fixed << "MGRS: " << mgrs_in.grid_zone << mgrs_in.grid_letter << " " << mgrs_in.false_easting << mgrs_in.false_northing << " " << mgrs_in.easting << " " << mgrs_in.northing << std::endl;
}

void UTMConverter::print_lla(lla lla_in)
{
    std::cout << std::fixed << "LLA:  " << lla_in.latitude << " " << lla_in.longitude << " " << lla_in.alt << std::endl;
}

void UTMConverter::print_lladms(lladms lladms_in)
{
    std::cout << std::fixed << "DMS:  " << lladms_in.latitude_degrees << " " << lladms_in.latitude_minutes << " " << lladms_in.latitude_seconds << " " << lladms_in.longitude_degrees << " " << lladms_in.longitude_minutes << " " << lladms_in.longitude_seconds << std::endl;
}

// Private Functions

void UTMConverter::calc_gz()
{
    int bound = -174;
    int i = 1;
    bool key = true;

    // Calculate the "Nominal" Grid Zone
    while (key)
    {
        if (lon_deg < bound)
        {
            grid_zone = i;
            key = false;
        }
        if (bound > 180)
        {
            key = false;
            std::cout << "Bad Longitude Value" << std::endl;
        }
        i++;
        bound += 6;
    }

    // Administrative Exceptions to Grid Zone
    if ((grid_zone == 31 && lon_deg >= 3.0) && (lat_deg < 64.0 && lat_deg >= 56.0))
    {
        grid_zone = 32;
        L0 = 9;
    }
    else if (grid_zone == 32 && lat_deg >= 72.0)
    {
        if (lon_deg < 9.0)
        {
            grid_zone = 31;
        }
        else
        {
            grid_zone = 33;
        }
    }
    else if (grid_zone == 34 && lat_deg >= 72.0)
    {
        if (lon_deg < 21.0)
        {
            grid_zone = 33;
        }
        else
        {
            grid_zone = 35;
        }
    }
    else if (grid_zone == 36 && lat_deg >= 72.0)
    {
        if (lon_deg < 33.0)
        {
            grid_zone = 35;
        }
        else
        {
            grid_zone = 37;
        }
    }

    // Calculate the Central Meridian for the Grid Zone
    L0 = (grid_zone * 6.0) - 183.0;
}

void UTMConverter::set_ceofs1()
{
    c1 = (-175.0 / 16384.0) * e8 + (-5.0 / 256.0) * e6 + (-3.0 / 64.0) * e4 + (-1.0 / 4.0) * e2 + 1;
    c2 = (-901.0 / 184320.0) * e8 + (-9.0 / 1024.0) * e6 + (-1.0 / 96.0) * e4 + (1.0 / 8.0) * e2;
    c3 = (-311.0 / 737280.0) * e8 + (17.0 / 5120.0) * e6 + (13.0 / 768.0) * e4;
    c4 = (899.0 / 430080.0) * e8 + (61.0 / 15360.0) * e6;
    c5 = (49561.0 / 41287680.0) * e8;
}

void UTMConverter::set_coefs2()
{
    c1 = (-175.0 / 16384.0) * e8 + (-5.0 / 256.0) * e6 + (-3.0 / 64.0) * e4 + (-1.0 / 4.0) * e2 + 1;
    c2 = (1.0 / 61440.0) * e8 + (7.0 / 2048.0) * e6 + (1.0 / 48.0) * e4 + (1.0 / 8.0) * e2;
    c3 = (559.0 / 368640.0) * e8 + (3.0 / 1280.0) * e6 + (1.0 / 768.0) * e4;
    c4 = (283.0 / 430080.0) * e8 + (17.0 / 30720.0) * e6;
    c5 = (4397.0 / 41287680.0) * e8;
}

int UTMConverter::sign(double x)
{
    if (x >= 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// MGRS LOOKUP FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////
// UTM to MGRS
std::string UTMConverter::lookup_easting(double easting_in, int grid_zone_in)
{
    int remain;
    remain = grid_zone_in % 3;

    int easting;
    easting = trunc(easting_in / 100000);

    if (remain == 1)
    {
        switch (easting)
        {
        case 1:
            return "A";
            break;

        case 2:
            return "B";
            break;

        case 3:
            return "C";
            break;

        case 4:
            return "D";
            break;

        case 5:
            return "E";
            break;

        case 6:
            return "F";
            break;

        case 7:
            return "G";
            break;

        case 8:
            return "H";
            break;
        }
    }
    else if (remain == 2)
    {
        switch (easting)
        {
        case 1:
            return "J";
            break;

        case 2:
            return "K";
            break;

        case 3:
            return "L";
            break;

        case 4:
            return "M";
            break;

        case 5:
            return "N";
            break;

        case 6:
            return "P";
            break;

        case 7:
            return "Q";
            break;

        case 8:
            return "R";
            break;
        }
    }
    else if (remain == 0)
    {
        switch (easting)
        {
        case 1:
            return "S";
            break;

        case 2:
            return "T";
            break;

        case 3:
            return "U";
            break;

        case 4:
            return "V";
            break;

        case 5:
            return "W";
            break;

        case 6:
            return "X";
            break;

        case 7:
            return "Y";
            break;

        case 8:
            return "Z";
            break;
        }
    }
    return "easting not found";
}

std::string UTMConverter::lookup_northing(double northing_in, int grid_zone_in)
{
    int remain;
    remain = grid_zone_in % 2;

    int northing;
    northing = trunc(northing_in);
    northing = trunc((northing % 2000000) / 100000);

    if (remain == 1)
    {
        switch (northing)
        {
        case 0:
            return "A";
            break;

        case 1:
            return "B";
            break;

        case 2:
            return "C";
            break;

        case 3:
            return "D";
            break;

        case 4:
            return "E";
            break;

        case 5:
            return "F";
            break;

        case 6:
            return "G";
            break;

        case 7:
            return "H";
            break;

        case 8:
            return "J";
            break;

        case 9:
            return "K";
            break;

        case 10:
            return "L";
            break;

        case 11:
            return "M";
            break;

        case 12:
            return "N";
            break;

        case 13:
            return "P";
            break;

        case 14:
            return "Q";
            break;

        case 15:
            return "R";
            break;

        case 16:
            return "S";
            break;

        case 17:
            return "T";
            break;

        case 18:
            return "U";
            break;

        case 19:
            return "V";
            break;
        }
    }
    else if (remain == 0)
    {
        switch (northing)
        {
        case 0:
            return "F";
            break;

        case 1:
            return "G";
            break;

        case 2:
            return "H";
            break;

        case 3:
            return "J";
            break;

        case 4:
            return "K";
            break;

        case 5:
            return "L";
            break;

        case 6:
            return "M";
            break;

        case 7:
            return "N";
            break;

        case 8:
            return "P";
            break;

        case 9:
            return "Q";
            break;

        case 10:
            return "R";
            break;

        case 11:
            return "S";
            break;

        case 12:
            return "T";
            break;

        case 13:
            return "U";
            break;

        case 14:
            return "V";
            break;

        case 15:
            return "A";
            break;

        case 16:
            return "B";
            break;

        case 17:
            return "C";
            break;

        case 18:
            return "D";
            break;

        case 19:
            return "E";
            break;
        }
    }
    return "northing not found";
}

std::string UTMConverter::lookup_gridzone(int latitude)
{
    if (latitude < -80)
    {
        return "NA";
    }
    else if (latitude < -72)
    {
        return "C";
    }
    else if (latitude < -64)
    {
        return "D";
    }
    else if (latitude < -56)
    {
        return "E";
    }
    else if (latitude < -48)
    {
        return "F";
    }
    else if (latitude < -40)
    {
        return "G";
    }
    else if (latitude < -32)
    {
        return "H";
    }
    else if (latitude < -24)
    {
        return "J";
    }
    else if (latitude < -16)
    {
        return "K";
    }
    else if (latitude < -8)
    {
        return "L";
    }
    else if (latitude < 0)
    {
        return "M";
    }
    else if (latitude < 8)
    {
        return "N";
    }
    else if (latitude < 16)
    {
        return "P";
    }
    else if (latitude < 24)
    {
        return "Q";
    }
    else if (latitude < 32)
    {
        return "R";
    }
    else if (latitude < 40)
    {
        return "S";
    }
    else if (latitude < 48)
    {
        return "T";
    }
    else if (latitude < 56)
    {
        return "U";
    }
    else if (latitude < 64)
    {
        return "V";
    }
    else if (latitude < 72)
    {
        return "W";
    }
    else if (latitude < 84)
    {
        return "X";
    }
    else
    {
        return "NA";
    }
}

// MGRS to UTM
double UTMConverter::decode_false_easting(std::string false_easting)
{
    if (false_easting == "A")
    {
        return 1.0;
    }
    else if (false_easting == "B")
    {
        return 2.0;
    }
    else if (false_easting == "C")
    {
        return 3.0;
    }
    else if (false_easting == "D")
    {
        return 4.0;
    }
    else if (false_easting == "E")
    {
        return 5.0;
    }
    else if (false_easting == "F")
    {
        return 6.0;
    }
    else if (false_easting == "G")
    {
        return 7.0;
    }
    else if (false_easting == "H")
    {
        return 8.0;
    }
    else if (false_easting == "J")
    {
        return 1.0;
    }
    else if (false_easting == "K")
    {
        return 2.0;
    }
    else if (false_easting == "L")
    {
        return 3.0;
    }
    else if (false_easting == "M")
    {
        return 4.0;
    }
    else if (false_easting == "N")
    {
        return 5.0;
    }
    else if (false_easting == "P")
    {
        return 6.0;
    }
    else if (false_easting == "Q")
    {
        return 7.0;
    }
    else if (false_easting == "R")
    {
        return 8.0;
    }
    else if (false_easting == "S")
    {
        return 1.0;
    }
    else if (false_easting == "T")
    {
        return 2.0;
    }
    else if (false_easting == "U")
    {
        return 3.0;
    }
    else if (false_easting == "V")
    {
        return 4.0;
    }
    else if (false_easting == "W")
    {
        return 5.0;
    }
    else if (false_easting == "X")
    {
        return 6.0;
    }
    else if (false_easting == "Y")
    {
        return 7.0;
    }
    else if (false_easting == "Z")
    {
        return 8.0;
    }
    return 0;
}

double UTMConverter::decode_false_northing(std::string false_northing, int grid_zone)
{
    int remain = grid_zone % 2;
    if (remain == 1)
    {
        if (false_northing == "A")
        {
            return 0.0;
        }
        else if (false_northing == "B")
        {
            return 1.0;
        }
        else if (false_northing == "C")
        {
            return 2.0;
        }
        else if (false_northing == "D")
        {
            return 3.0;
        }
        else if (false_northing == "E")
        {
            return 4.0;
        }
        else if (false_northing == "F")
        {
            return 5.0;
        }
        else if (false_northing == "G")
        {
            return 6.0;
        }
        else if (false_northing == "H")
        {
            return 7.0;
        }
        else if (false_northing == "J")
        {
            return 8.0;
        }
        else if (false_northing == "K")
        {
            return 9.0;
        }
        else if (false_northing == "L")
        {
            return 10.0;
        }
        else if (false_northing == "M")
        {
            return 11.0;
        }
        else if (false_northing == "N")
        {
            return 12.0;
        }
        else if (false_northing == "P")
        {
            return 13.0;
        }
        else if (false_northing == "Q")
        {
            return 14.0;
        }
        else if (false_northing == "R")
        {
            return 15.0;
        }
        else if (false_northing == "S")
        {
            return 16.0;
        }
        else if (false_northing == "T")
        {
            return 17.0;
        }
        else if (false_northing == "U")
        {
            return 18.0;
        }
        else if (false_northing == "V")
        {
            return 19.0;
        }
    }
    else
    {
        if (false_northing == "F")
        {
            return 0.0;
        }
        else if (false_northing == "G")
        {
            return 1.0;
        }
        else if (false_northing == "H")
        {
            return 2.0;
        }
        else if (false_northing == "J")
        {
            return 3.0;
        }
        else if (false_northing == "K")
        {
            return 4.0;
        }
        else if (false_northing == "L")
        {
            return 5.0;
        }
        else if (false_northing == "M")
        {
            return 6.0;
        }
        else if (false_northing == "N")
        {
            return 7.0;
        }
        else if (false_northing == "P")
        {
            return 8.0;
        }
        else if (false_northing == "Q")
        {
            return 9.0;
        }
        else if (false_northing == "R")
        {
            return 10.0;
        }
        else if (false_northing == "S")
        {
            return 11.0;
        }
        else if (false_northing == "T")
        {
            return 12.0;
        }
        else if (false_northing == "U")
        {
            return 13.0;
        }
        else if (false_northing == "V")
        {
            return 14.0;
        }
        else if (false_northing == "A")
        {
            return 15.0;
        }
        else if (false_northing == "B")
        {
            return 16.0;
        }
        else if (false_northing == "C")
        {
            return 17.0;
        }
        else if (false_northing == "D")
        {
            return 18.0;
        }
        else if (false_northing == "E")
        {
            return 19.0;
        }
    }
    return 0;
}