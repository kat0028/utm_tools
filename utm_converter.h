#ifndef UTM_CONVERTER_H 
#define UTM_CONVERTER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>

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

    // Inputs and Outputs
    double lat_deg;
    double lon_deg;
    double lat_rad;
    double lon_rad;
    double easting;
    double northing;
    int grid_zone;

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
    // Structs for coordinate data
    struct utm {
        double easting;
        double northing;
        int grid_zone;
        double alt;
    };

    struct mgrs{
        double easting;
        double northing;
        int grid_zone;
        double alt;
        std::string gz;
        std::string false_easting;
        std::string false_northing;
    };

    struct lla{
        double latitude;
        double longitude;
        double alt;
    };

    // ------------------------------------------------------------------------------------------
    // FUNCTIONS
    // ------------------------------------------------------------------------------------------

    // CONSTRUCTOR
    utm_converter()
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
    
    // DESTRUCTOR
    ~utm_converter()
    {
        //DO NOTHING
    }

    // ------------------------------------------------------------------------------------------
    // CONVERSIONS BETWEEN UTM AND MGRS
    // ------------------------------------------------------------------------------------------
    mgrs convert_utm2mgrs(utm utm_in)
    {
        int east_remain;    
        east_remain = utm_in.grid_zone % 3;

        int north_remain;
        north_remain = utm_in.grid_zone % 2;
        
        int false_easting;
        int false_northing;

        false_easting = trunc(utm_in.easting/100000);
        false_northing = trunc(utm_in.northing/1000000);

        mgrs mgrs_coordinates;
        mgrs_coordinates.easting = utm_in.easting - false_easting;
        mgrs_coordinates.northing = utm_in.northing - false_northing;
        mgrs_coordinates.false_easting = lookup_easting(east_remain, false_easting);
        mgrs_coordinates.false_northing = lookup_northing(north_remain, false_northing);

        return mgrs_coordinates;
    }

    std::string lookup_easting(int remain, int easting)
    {
        
        if(remain==1)
        {
            if(easting==1)
            {
                return "A";
            }
            else if(easting==2)
            {
                return "B";
            }
            else if(easting==3)
            {
                return "C";
            }
            else if(easting==4)
            {
                return "D";
            }
            else if(easting==5)
            {
                return "E";
            }
            else if(easting==6)
            {
                return "F";
            }
            else if(easting==7)
            {
                return "G";
            }
            else if(easting==8)
            {
                return "H";
            }
        }
        else if(remain==2)
        {
            if(easting==1)
            {
                return "J";
            }
            else if(easting==2)
            {
                return "K";
            }
            else if(easting==3)
            {
                return "L";
            }
            else if(easting==4)
            {
                return "M";
            }
            else if(easting==5)
            {
                return "N";
            }
            else if(easting==6)
            {
                return "P";
            }
            else if(easting==7)
            {
                return "Q";
            }
            else if(easting==8)
            {
                return "R";
            }
        }
        else if(remain==0)
        {
            if(easting==1)
            {
                return "S";
            }
            else if(easting==2)
            {
                return "T";
            }
            else if(easting==3)
            {
                return "U";
            }
            else if(easting==4)
            {
                return "V";
            }
            else if(easting==5)
            {
                return "W";
            }
            else if(easting==6)
            {
                return "X";
            }
            else if(easting==7)
            {
                return "Y";
            }
            else if(easting==8)
            {
                return "Z";
            }
        }
        return "easting not found";
    }

    std::string lookup_northing(int remain, int northing)
    {
        if(remain==1)
        {
            if(northing==0)
            {
                return "A";
            }
            if(northing==1)
            {
                return "B";
            }
            if(northing==2)
            {
                return "C";
            }
            if(northing==3)
            {
                return "D";
            }
            if(northing==4)
            {
                return "E";
            }
            if(northing==5)
            {
                return "F";
            }
            if(northing==6)
            {
                return "G";
            }
            if(northing==7)
            {
                return "H";
            }
            if(northing==8)
            {
                return "J";
            }
            if(northing==9)
            {
                return "K";
            }
            if(northing==10)
            {
                return "L";
            }
            if(northing==11)
            {
                return "M";
            }
            if(northing==12)
            {
                return "N";
            }
            if(northing==13)
            {
                return "P";
            }
            if(northing==14)
            {
                return "Q";
            }
            if(northing==15)
            {
                return "R";
            }
            if(northing==16)
            {
                return "S";
            }
            if(northing==17)
            {
                return "T";
            }
            if(northing==18)
            {
                return "U";
            }
            if(northing==19)
            {
                return "V";
            }
        }  
        if(remain==0)
        {
            if(northing==0)
            {
                return "F";
            }
            if(northing==1)
            {
                return "G";
            }
            if(northing==2)
            {
                return "H";
            }
            if(northing==3)
            {
                return "J";
            }
            if(northing==4)
            {
                return "K";
            }
            if(northing==5)
            {
                return "L";
            }
            if(northing==6)
            {
                return "M";
            }
            if(northing==7)
            {
                return "N";
            }
            if(northing==8)
            {
                return "P";
            }
            if(northing==9)
            {
                return "Q";
            }
            if(northing==10)
            {
                return "R";
            }
            if(northing==11)
            {
                return "S";
            }
            if(northing==12)
            {
                return "T";
            }
            if(northing==13)
            {
                return "U";
            }
            if(northing==14)
            {
                return "V";
            }
            if(northing==15)
            {
                return "A";
            }
            if(northing==16)
            {
                return "B";
            }
            if(northing==17)
            {
                return "C";
            }
            if(northing==18)
            {
                return "D";
            }
            if(northing==19)
            {
                return "E";
            }
        }  
        return "northing not found";       
    }

    // ------------------------------------------------------------------------------------------
    // LLA TO UTM/MGRS CONVERSIONS
    // ------------------------------------------------------------------------------------------
    utm convert_lla2utm(lla lla_in)
    {
        lat_deg = lla_in.latitude;
        lon_deg = lla_in.longitude;

        if(lat_deg<0)
        {
            Y0 = 10000000;
        }
        else
        {
            Y0 = 0;
        }

        calc_gz();
        
        lon_rad = lon_deg * M_PI / 180.0;
        lat_rad = lat_deg * M_PI / 180.0;
        L0 = L0 * M_PI / 180.0;

        L = log(tan(M_PI/4+lat_rad/2)*(pow(((1-e*sin(lat_rad))/(1+e*sin(lat_rad))),(e/2))));
        z_e = log(tan(M_PI/4 + asin(sin(lon_rad - L0)/cosh(L))/2));
        z_n = atan(sinh(L)/cos(lon_rad - L0));

        Z_e = n*c1*z_e + n*(c2*cos(2*z_n)*sinh(2*z_e) + c3*cos(4*z_n)*sinh(4*z_e) + c4*cos(6*z_n)*sinh(6*z_e) + c5*cos(8*z_n)*sinh(8*z_e));
        Z_n = n*c1*z_n + n*(c2*sin(2*z_n)*cosh(2*z_e) + c3*sin(4*z_n)*cosh(4*z_e) + c4*sin(6*z_n)*cosh(6*z_e) + c5*sin(8*z_n)*cosh(8*z_e));

        easting = Z_e + X0;
        northing = Z_n + Y0;

        utm utm_coordinates;
        utm_coordinates.easting = easting;
        utm_coordinates.northing = northing;
        utm_coordinates.grid_zone = grid_zone;
        return utm_coordinates;    
    }

    
};

#endif