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
    double a;
    double b;
    double flat;
    double e;
    double e_ps;
    double n;
    double C1;
    double C2;
    double C3;
    double C4;
    double C5;
    double C6;
    double K0;
    double rho;
    double nu;
    double P;
    double S;
    double K1;
    double K2;
    double K3;
    double K4;
    double K5;
    double lon0;

    std::vector<int> boundaries;
    std::vector<int> central_meridians;

    // Inputs and Outputs
    double northing;
    double easting;
    double grid_zone;
    double lat;
    double lon;

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
            if(lon<bound)
            {
                lon0 = bound - 3;
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

    public:
    lla2utm_converter()
    {
        a = 6378137.0;
        b = 6356752.3142;

        flat = (a-b)/a;
        e = sqrt(1-(pow(b,2))/(pow(a,2)));
        e_ps = pow(e,2)/(1-(pow(e,2)));

        n = (a-b)/(a+b);

        C1 = 1.0 + 3.0/4.0*pow(e,2) + 45.0/64.0*pow(e,4) + 175.0/256.0*pow(e,6) + 11025.0/16384.0*pow(e,8) + 43659.0/65536.0*pow(e,10);
        C2 = 3.0/4.0*pow(e,2) + 15.0/16.0*pow(e,4) + 525.0/512.0*pow(e,6) + 2205.0/2048.0*pow(e,8) + 72765.0/65536.0*pow(e,10);
        C3 = 15.0/64.0*pow(e,4) + 105.0/256.0*pow(e,6) + 2205.0/4096.0*pow(e,8) + 10395.0/16384.0*pow(e,10);
        C4 = 35.0/512.0*pow(e,6) + 315.0/2048.0*pow(e,8) + 31185.0/131072.0*pow(e,10);
        C5 = 315.0/16384.0*pow(e,8) + 3465.0/65536.0*pow(e,10);
        C6 = 693.0/131072.0*pow(e,10);   
        
        K0 = 0.9996;
        
    }
    
    ~lla2utm_converter()
    {
        //DO NOTHING
    }

    std::vector<double> convert(double lat_in, double lon_in)
    {
        lat = lat_in;
        lon = lon_in;

        calc_gz();
        
        lon = lon_in * M_PI / 180.0;
        lat = lat_in * M_PI / 180.0;
        lon0 = lon0 * M_PI / 180.0;

        rho = (a*(1.0-pow(e,2.0)))/pow((1.0-pow(e,2.0)*pow(sin(lat),2.0)),(3.0/2.0));
        nu = a/pow((1.0-pow(e,2.0)*pow(sin(lat),2.0)),(1.0/2.0));

        P = lon-lon0;
        S = a*(1.0-pow(e,2.0))*(C1*lat - C2*sin(2.0*lat)/2.0 + C3*sin(4.0*lat)/4.0 - C4*sin(6.0*lat)/6.0 + C5*sin(8.0*lat)/8.0 - C6*sin(10.0*lat)/10.0);

        K1 = S*K0;
        K2 = K0*nu*sin(lat)*cos(lat)/2.0;
        K3 = ((K0*nu*sin(lat)*pow(cos(lat),3.0))/24.0)*(5.0-pow(tan(lat),2.0)+9.0*e_ps*pow(cos(lat),2.0)+pow((2.0*e_ps*pow(cos(lat),2.0)),2.0));
        K4 = K0*nu*cos(lat);
        K5 = ((K0*nu*pow(cos(lat),3.0))/6.0)*(1.0-pow(tan(lat),2.0)+e_ps*pow(cos(lat),2.0));
        northing = K1 + K2*pow(P,2.0) + K3*pow(P,4.0);
        easting = K4*P + K5*pow(P,3.0);

        if(lat<0)
        {
            northing+=10000000.0;
        }
        easting += 500000.0;

        outputs.clear();
        outputs.push_back(easting);
        outputs.push_back(northing);
        outputs.push_back(grid_zone);

        return outputs;    
    }
};

#endif