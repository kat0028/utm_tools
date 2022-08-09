#ifndef UTM_CONVERTER_HPP
#define UTM_CONVERTER_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>

class UTMConverter
{
private:
    // Conversion Variables
    // Earth Parameters
    double a;
    double f;
    double b;
    double e;
    double n;
    double K0;

    // Constants
    double X0;
    double Y0;
    double L0;

    // Powers of e
    double e2;
    double e4;
    double e6;
    double e8;

    // Cofficients
    double c1;
    double c2;
    double c3;
    double c4;
    double c5;

    // Other Variables
    double L;
    double z_e;
    double z_n;
    double Z_e;
    double Z_n;
    double rad2deg;
    double deg2rad;

    // Inputs and Outputs
    double lat_deg;
    double lon_deg;
    double lat_rad;
    double lon_rad;
    double easting;
    double northing;
    int grid_zone;

    // ECEF2LLA Variables
    double rho;
    double count;
    double cosprev;
    double sinprev;
    double cosbeta;
    double sinbeta;
    double u;
    double v;
    double error;
    double error_threshold;
    bool converge;
    double N;

    // Utility Functions
    void calc_gz();

    void set_ceofs1();
    void set_coefs2();

    int sign(double x);

    // UTM to MGRS
    std::string lookup_easting(double easting_in, int grid_zone_in);
    std::string lookup_northing(double northing_in, int grid_zone_in);
    std::string lookup_gridzone(int latitude);

    // MGRS to UTM
    double decode_false_easting(std::string false_easting);
    double decode_false_northing(std::string false_northing, int grid_zone);

public:
    // Structs for coordinate data
    struct utm
    {
        double easting;
        double northing;
        int grid_zone;
        double alt;
    };

    struct mgrs
    {
        double easting;
        double northing;
        int grid_zone;
        double alt;
        std::string grid_letter;
        std::string false_easting;
        std::string false_northing;
    };

    struct lla
    {
        double latitude;
        double longitude;
        double alt;
    };

    struct lladms
    {
        int latitude_degrees;
        int latitude_minutes;
        double latitude_seconds;
        int longitude_degrees;
        int longitude_minutes;
        double longitude_seconds;
        double alt;
    };

    struct ecef
    {
        double x;
        double y;
        double z;
    };

    // ------------------------------------------------------------------------------------------
    // FUNCTIONS
    // ------------------------------------------------------------------------------------------

    // CONSTRUCTOR
    UTMConverter();

    // DESTRUCTOR
    ~UTMConverter();

    // ------------------------------------------------------------------------------------------
    // UTM to MGRS Conversion
    // ------------------------------------------------------------------------------------------
    mgrs convert_utm2mgrs(utm utm_in);

    // ------------------------------------------------------------------------------------------
    // MGRS to UTM Conversion
    // ------------------------------------------------------------------------------------------
    utm convert_mgrs2utm(mgrs mgrs_in);

    // ------------------------------------------------------------------------------------------
    // LLA TO UTM CONVERSION
    // ------------------------------------------------------------------------------------------
    utm convert_lla2utm(lla lla_in);

    // ------------------------------------------------------------------------------------------
    // UTM TO LLA CONVERSION
    // ------------------------------------------------------------------------------------------
    lla convert_utm2lla(utm utm_in);

    // ------------------------------------------------------------------------------------------
    // LLADMS to LLA
    // ------------------------------------------------------------------------------------------
    lla convert_lladms2lla(lladms lladms_in);

    // ------------------------------------------------------------------------------------------
    // LLA to LLADMS
    // ------------------------------------------------------------------------------------------
    lladms convert_lla2lladms(lla lla_in);

    // ------------------------------------------------------------------------------------------
    // LLA to MGRS
    // ------------------------------------------------------------------------------------------
    mgrs convert_lla2mgrs(lla lla_in);

    // ------------------------------------------------------------------------------------------
    // MGRS to LLA
    // ------------------------------------------------------------------------------------------
    lla convert_mgrs2lla(mgrs mgrs_in);

    // ------------------------------------------------------------------------------------------
    // ECEF2LLA
    // ------------------------------------------------------------------------------------------
    lla convert_ecef2lla(ecef ecef_in);

    // ------------------------------------------------------------------------------------------
    // ECEF2UTM
    // ------------------------------------------------------------------------------------------
    utm convert_ecef2utm(ecef ecef_in);

    // ------------------------------------------------------------------------------------------
    // PRINT FUNCTIONS
    // ------------------------------------------------------------------------------------------
    void print_utm(utm utm_in);
    void print_mgrs(mgrs mgrs_in);
    void print_lla(lla lla_in);
    void print_lladms(lladms lladms_in);
};

#endif