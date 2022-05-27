#ifndef LLA2UTM_CONVERTER_H 
#define LLA2UTM_CONVERTER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

class lla2utm_converter
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

    // Inputs and Outputs
    double lat_deg;
    double lon_deg;
    double lat_rad;
    double lon_rad;
    double easting;
    double northing;
    int grid_zone;

    // For returning the function
    std::vector<double> outputs;

    // Utility Functions
    void calc_gz()
    {
        int bound = -174;
        int i = 1;
        bool key = true;
        //boundaries.clear();
        //central_meridians.clear();
        while (key)
        {
            if(lon_deg<bound)
            {
                L0 = bound - 3;
                grid_zone = i;
                key = false;
            }
            if(bound>180)
            {
                key = false;
                std::cout<<"Bad Longitude Value"<<std::endl;
            }
            i++;
            bound += 6;
        }
    }

    void set_ceofs()
    {
        c1 = (-175.0/16384.0)*e8 + (-5.0/256.0)*e6 + (-3.0/64.0)*e4 + (-1.0/4.0)*e2 + 1;
        c2 = (-901.0/184320.0)*e8 + (-9.0/1024.0)*e6 + (-1.0/96.0)*e4 + (1.0/8.0)*e2;
        c3 = (-311.0/737280.0)*e8 + (17.0/5120.0)*e6 + (13.0/768.0)*e4;
        c4 = (899.0/430080.0)*e8 + (61.0/15360.0)*e6;
        c5 = (49561.0/41287680.0)*e8;
    }

    public:
    lla2utm_converter()
    {
        a = 6378137.0;
        f = 298.257223563;
        b = a * (1-1/f);
        K0 = 0.9996;
        X0 = 500000;

        e = sqrt((a*a-b*b)/(a*a));
        
        e2 = pow(e,2);
        e4 = pow(e,4);
        e6 = pow(e,6);
        e8 = pow(e,8);

        n = a*K0;

        set_ceofs();       
    }
    
    ~lla2utm_converter()
    {
        //DO NOTHING
    }

    std::vector<double> convert(double lat_in, double lon_in)
    {
        lat_deg = lat_in;
        lon_deg = lon_in;

        if(lat_deg<0)
        {
            Y0 = 10000000;
        }
        else
        {
            Y0 = 0;
        }

        calc_gz();
        
        lon_rad = lon_in * M_PI / 180.0;
        lat_rad = lat_in * M_PI / 180.0;
        L0 = L0 * M_PI / 180.0;

        L = log(tan(M_PI/4+lat_rad/2)*(pow(((1-e*sin(lat_rad))/(1+e*sin(lat_rad))),(e/2))));
        z_e = log(tan(M_PI/4 + asin(sin(lon_rad - L0)/cosh(L))/2));
        z_n = atan(sinh(L)/cos(lon_rad - L0));

        Z_e = n*c1*z_e + n*(c2*cos(2*z_n)*sinh(2*z_e) + c3*cos(4*z_n)*sinh(4*z_e) + c4*cos(6*z_n)*sinh(6*z_e) + c5*cos(8*z_n)*sinh(8*z_e));
        Z_n = n*c1*z_n + n*(c2*sin(2*z_n)*cosh(2*z_e) + c3*sin(4*z_n)*cosh(4*z_e) + c4*sin(6*z_n)*cosh(6*z_e) + c5*sin(8*z_n)*cosh(8*z_e));

        easting = Z_e + X0;
        northing = Z_n + Y0;

        outputs.clear();
        outputs.push_back(easting);
        outputs.push_back(northing);
        outputs.push_back(grid_zone);

        return outputs;    
    }
};

#endif