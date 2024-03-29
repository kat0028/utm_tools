//
// Created by kyle on 11/26/22.
//

#ifndef POLARCOORDINATES_UTM_CONVERTER_H
#define POLARCOORDINATES_UTM_CONVERTER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>

#include "mgrs_lookup_functions.h"

class utm_converter
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
    void calc_gz()
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
        if ((grid_zone==31 && lon_deg>=3.0)&&(lat_deg<64.0 && lat_deg>=56.0))
        {
            grid_zone = 32;
            L0 = 9;
        }
        else if (grid_zone==32 && lat_deg>=72.0)
        {
            if (lon_deg<9.0)
            {
                grid_zone = 31;
            }
            else
            {
                grid_zone = 33;
            }
        }
        else if (grid_zone==34 && lat_deg>=72.0)
        {
            if (lon_deg<21.0)
            {
                grid_zone = 33;
            }
            else
            {
                grid_zone = 35;
            }
        }
        else if (grid_zone==36 && lat_deg>=72.0)
        {
            if (lon_deg<33.0)
            {
                grid_zone = 35;
            }
            else
            {
                grid_zone = 37;
            }
        }

        // Calculate the Central Meridian for the Grid Zone
        L0 = (grid_zone*6.0)-183.0;
    }

    void set_ceofs1()
    {
        c1 = (-175.0 / 16384.0) * e8 + (-5.0 / 256.0) * e6 + (-3.0 / 64.0) * e4 + (-1.0 / 4.0) * e2 + 1;
        c2 = (-901.0 / 184320.0) * e8 + (-9.0 / 1024.0) * e6 + (-1.0 / 96.0) * e4 + (1.0 / 8.0) * e2;
        c3 = (-311.0 / 737280.0) * e8 + (17.0 / 5120.0) * e6 + (13.0 / 768.0) * e4;
        c4 = (899.0 / 430080.0) * e8 + (61.0 / 15360.0) * e6;
        c5 = (49561.0 / 41287680.0) * e8;
    }

    void set_coefs2()
    {
        c1 = (-175.0 / 16384.0) * e8 + (-5.0 / 256.0) * e6 + (-3.0 / 64.0) * e4 + (-1.0 / 4.0) * e2 + 1;
        c2 = (1.0 / 61440.0) * e8 + (7.0 / 2048.0) * e6 + (1.0 / 48.0) * e4 + (1.0 / 8.0) * e2;
        c3 = (559.0 / 368640.0) * e8 + (3.0 / 1280.0) * e6 + (1.0 / 768.0) * e4;
        c4 = (283.0 / 430080.0) * e8 + (17.0 / 30720.0) * e6;
        c5 = (4397.0 / 41287680.0) * e8;
    }

    int sign(double x)
    {
        if(x>=0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

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
    utm_converter()
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
        rad2deg = 180.0/M_PI;
        deg2rad = M_PI/180.0;
    }

    // DESTRUCTOR
    ~utm_converter()
    {
        // DO NOTHING
    }

    //*******************************************************************************************
    //*******************************************************************************************
    //*******************************************************************************************
    //*******************************************************************************************
    //                          CONVERSION FUNCTION DEFINITIONS
    //*******************************************************************************************
    //*******************************************************************************************
    //*******************************************************************************************
    //*******************************************************************************************


    // ------------------------------------------------------------------------------------------
    // UTM to MGRS Conversion
    // ------------------------------------------------------------------------------------------
    mgrs convert_utm2mgrs(utm utm_in)
    {
        int false_easting;
        int false_northing;

        // Get LLA Coordinates for the grid zone letter
        lla lla_coords = convert_utm2lla(utm_in);

        false_easting = trunc(utm_in.easting / 100000);
        false_northing = trunc(utm_in.northing / 100000);

        mgrs mgrs_coordinates;
        mgrs_coordinates.easting = utm_in.easting - false_easting * 100000;
        mgrs_coordinates.northing = utm_in.northing - false_northing * 100000;
        mgrs_coordinates.false_easting = lookup_easting(utm_in.easting, utm_in.grid_zone);
        mgrs_coordinates.false_northing = lookup_northing(utm_in.northing, utm_in.grid_zone);
        mgrs_coordinates.grid_zone = utm_in.grid_zone;
        mgrs_coordinates.grid_letter = lookup_gridzone(lla_coords.latitude);
        mgrs_coordinates.alt = utm_in.alt;

        return mgrs_coordinates;
    }

    // ------------------------------------------------------------------------------------------
    // MGRS to UTM Conversion
    // ------------------------------------------------------------------------------------------
    utm convert_mgrs2utm(mgrs mgrs_in)
    {
        utm utm_coords;
        utm_coords.grid_zone = mgrs_in.grid_zone;
        utm_coords.easting = mgrs_in.easting + decode_false_easting(mgrs_in.false_easting)*100000.0;
        utm_coords.northing = mgrs_in.northing + decode_false_northing(mgrs_in.false_northing, mgrs_in.grid_zone)*100000.0;
        utm_coords.alt = mgrs_in.alt;

        while (!check_grid_designator(utm_coords, mgrs_in) && utm_coords.northing < 10000000)
        {
            utm_coords.northing += 2000000;
        }

        return utm_coords;
    }

    bool check_grid_designator(utm utm_in, mgrs mgrs_in)
    {
        mgrs check_coords = convert_utm2mgrs(utm_in);

        if (check_coords.grid_letter != mgrs_in.grid_letter)
        {
            return false;
        }

        if (check_coords.false_northing != mgrs_in.false_northing)
        {
            return false;
        }

        if (check_coords.false_easting != mgrs_in.false_easting)
        {
            return false;
        }

        return true;
    }

    // ------------------------------------------------------------------------------------------
    // LLA TO UTM CONVERSION
    // ------------------------------------------------------------------------------------------
    utm convert_lla2utm(lla lla_in)
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

        utm utm_coordinates;
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

        return utm_coordinates;
    }

    // ------------------------------------------------------------------------------------------
    // UTM TO LLA CONVERSION
    // ------------------------------------------------------------------------------------------
    lla convert_utm2lla(utm utm_in)
    {
        easting = utm_in.easting;
        northing = utm_in.northing;
        grid_zone = utm_in.grid_zone;

        // Hemisphere Based Northing Offset
        if(grid_zone>0.0)
        {
            Y0 = 0;
        }
        else
        {
            Y0 = 10000000.0;
        }

        // Central Meridian
        L0 = (6.0*abs(grid_zone)-183.0) * deg2rad;

        // Parameters for converging equation
        int max_nubmer_iterations;
        max_nubmer_iterations = 10000;
        double convergence_threshold;
        convergence_threshold = pow(10.0,-100.0);

        // Set Coefficients
        set_coefs2();

        // Calculate Footpring LLA
        z_n = (northing-Y0)/n/c1;
        z_e = (easting-X0)/n/c1;

        Z_n = z_n - c2*sin(2.0*z_n)*cosh(2.0*z_e) - c3*sin(4.0*z_n)*cosh(4.0*z_e) - c4*sin(6.0*z_n)*cosh(6.0*z_e) - c5*sin(8.0*z_n)*cosh(8.0*z_e);
        Z_e = z_e - c2*cos(2.0*z_n)*sinh(2.0*z_e) - c3*cos(4.0*z_n)*sinh(4.0*z_e) - c4*cos(6.0*z_n)*sinh(6.0*z_e) - c5*cos(8.0*z_n)*sinh(8.0*z_e);

        double p;

        p = asin(sin(Z_n)/cosh(Z_e));
        L = log(tan(M_PI/4.0+p/2.0));

        lat_rad = 2.0*atan(exp(L)) - M_PI/2.0;
        lon_rad = L0+atan(sinh(Z_e)/cos(Z_n));

        // Converge Latitude
        double lat0;
        lat0 = 0;

        int i=0;
        double es;

        while(sqrt(pow(lat_rad-lat0, 2))>convergence_threshold && i<max_nubmer_iterations)
        {
            lat0 = lat_rad;
            es = e*sin(lat0);
            lat_rad = 2*atan(pow(((1.0+es)/(1.0-es)),(e/2.0))*exp(L)) - M_PI/2.0;
            i++;
        }

        lat_deg = lat_rad*rad2deg;
        lon_deg = lon_rad*rad2deg;

        lla lla_coords;
        lla_coords.latitude=lat_deg;
        lla_coords.longitude=lon_deg;
        lla_coords.alt=utm_in.alt;

        return lla_coords;
    }

    // ------------------------------------------------------------------------------------------
    // LLADMS to LLA
    // ------------------------------------------------------------------------------------------
    lla convert_lladms2lla(lladms lladms_in)
    {
        lla lla_coords;
        lla_coords.latitude = lladms_in.latitude_degrees + lladms_in.latitude_minutes/60.0 + lladms_in.latitude_seconds/(60.0*60.0);
        lla_coords.longitude = lladms_in.longitude_degrees + lladms_in.longitude_minutes/60.0 + lladms_in.longitude_seconds/(60.0*60.0);
        lla_coords.alt = lladms_in.alt;
        return lla_coords;
    }

    // ------------------------------------------------------------------------------------------
    // LLA to LLADMS
    // ------------------------------------------------------------------------------------------
    lladms convert_lla2lladms(lla lla_in)
    {
        lladms lladms_coords;

        lladms_coords.latitude_degrees = trunc(lla_in.latitude);
        double remain = lla_in.latitude - lladms_coords.latitude_degrees;
        lladms_coords.latitude_minutes = trunc(remain*60.0);
        remain = remain-lladms_coords.latitude_minutes/60.0;
        lladms_coords.latitude_seconds = remain*(60.0*60.0);

        lladms_coords.longitude_degrees = trunc(lla_in.longitude);
        remain = lla_in.longitude - lladms_coords.longitude_degrees;
        lladms_coords.longitude_minutes = trunc(remain*60.0);
        remain = remain-lladms_coords.longitude_minutes/60.0;
        lladms_coords.longitude_seconds = remain*(60.0*60.0);

        lladms_coords.alt = lla_in.alt;

        return lladms_coords;
    }

    // ------------------------------------------------------------------------------------------
    // LLA to MGRS
    // ------------------------------------------------------------------------------------------
    mgrs convert_lla2mgrs(lla lla_in)
    {
        return convert_utm2mgrs(convert_lla2utm(lla_in));
    }

    // ------------------------------------------------------------------------------------------
    // MGRS to LLA
    // ------------------------------------------------------------------------------------------
    lla convert_mgrs2lla(mgrs mgrs_in)
    {
        return convert_utm2lla(convert_mgrs2utm(mgrs_in));
    }

    // ------------------------------------------------------------------------------------------
    // ECEF2LLA
    // ------------------------------------------------------------------------------------------
    lla convert_ecef2lla(ecef ecef_in)
    {
        lla lla_coords;
        lla_coords.longitude = atan2(ecef_in.y, ecef_in.x) * rad2deg;

        rho = sqrt(ecef_in.x*ecef_in.x + ecef_in.y*ecef_in.y);

        u = a * rho;
        v = b * ecef_in.z * (1.0 + (b*e2/(1.0-e2))/sqrt(ecef_in.z*ecef_in.z + rho*rho));

        //std::cout<<b<<"\t"<<ecef_in.z<<"\t"<<e2<<"\t"<<rho<<std::endl;

        cosbeta = sign(u)/sqrt(1.0 + pow(v/u, 2));
        sinbeta = sign(v)/sqrt(1.0 + pow(u/v, 2));

        //std::cout<<rho<<"\t"<<u<<"\t"<<v<<"\t"<<cosbeta<<"\t"<<sinbeta<<std::endl;

        count = 0;
        converge = false;
        while (!converge)
        {
            cosprev = cosbeta;
            sinprev = sinbeta;
            u = rho - a*e2 * pow(cosbeta, 3);
            v = ecef_in.z + (b*e2/(1-e2)) * pow(sinbeta, 3);

            cosbeta = sign(a*u) / sqrt(1.0 + pow(b*v/(a*u), 2));
            sinbeta = sign(b*v) / sqrt(1.0 + pow(a*u/(b*v), 2));

            error = sqrt(pow(cosbeta-cosprev, 2) + pow(sinbeta-sinprev, 2));
            if (error<error_threshold)
            {
                converge = true;
            }
            count++;
            if (count>100)
            {
                converge = true;
                std::cout<<"Point Under Tolerance"<<std::endl;
            }
            //std::cout<<count<<": "<<error<<std::endl;
        }

        lla_coords.latitude = atan2(v, u);
        cosbeta = cos(lla_coords.latitude);
        sinbeta = sin(lla_coords.latitude);

        lla_coords.latitude = lla_coords.latitude * rad2deg;

        N = a/sqrt(1-e2*sinbeta*sinbeta);
        lla_coords.alt = rho*cosbeta + (ecef_in.z + e2*N*sinbeta) * sinbeta - N;

        return lla_coords;
    }

    // ------------------------------------------------------------------------------------------
    // ECEF2UTM
    // ------------------------------------------------------------------------------------------
    utm convert_ecef2utm(ecef ecef_in)
    {
        lla lla_coords;
        lla_coords = convert_ecef2lla(ecef_in);

        return convert_lla2utm(lla_coords);
    }

    // ------------------------------------------------------------------------------------------
    // PRINT FUNCTIONS
    // ------------------------------------------------------------------------------------------
    void print_utm(utm utm_in)
    {
        std::cout<<std::fixed<<"UTM:  "<<utm_in.easting<<" "<<utm_in.northing<<" "<<utm_in.grid_zone<<std::endl;
    }

    void print_mgrs(mgrs mgrs_in)
    {
        std::cout<<std::fixed<<"MGRS: "<<mgrs_in.grid_zone<<mgrs_in.grid_letter<<" "<<mgrs_in.false_easting<<mgrs_in.false_northing<<" "<<mgrs_in.easting<<" "<<mgrs_in.northing<<std::endl;
    }

    void print_lla(lla lla_in)
    {
        std::cout<<std::fixed<<"LLA:  "<<lla_in.latitude<<" "<<lla_in.longitude<<" "<<lla_in.alt<<std::endl;
    }

    void print_lladms(lladms lladms_in)
    {
        std::cout<<std::fixed<<"DMS:  "<<lladms_in.latitude_degrees<<" "<<lladms_in.latitude_minutes<<" "<<lladms_in.latitude_seconds<<" "<<lladms_in.longitude_degrees<<" "<<lladms_in.longitude_minutes<<" "<<lladms_in.longitude_seconds<<std::endl;
    }

};

#endif //POLARCOORDINATES_UTM_CONVERTER_H
