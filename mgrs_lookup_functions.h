#ifndef MGRS_LOOKUP_FUNCTIONS
#define MGRS_LOOKUP_FUNCTIONS
#include <string>
// ------------------------------------------------------------------------------------------
// LOOKUP TABLES FOR MGRS
// ------------------------------------------------------------------------------------------

// UTM to MGRS
std::string lookup_easting(double easting_in, int grid_zone_in)
{
    int remain;
    remain = grid_zone_in % 3;

    int easting;
    easting = trunc(easting_in/100000);

    if (remain == 1)
    {
        if (easting == 1)
        {
            return "A";
        }
        else if (easting == 2)
        {
            return "B";
        }
        else if (easting == 3)
        {
            return "C";
        }
        else if (easting == 4)
        {
            return "D";
        }
        else if (easting == 5)
        {
            return "E";
        }
        else if (easting == 6)
        {
            return "F";
        }
        else if (easting == 7)
        {
            return "G";
        }
        else if (easting == 8)
        {
            return "H";
        }
    }
    else if (remain == 2)
    {
        if (easting == 1)
        {
            return "J";
        }
        else if (easting == 2)
        {
            return "K";
        }
        else if (easting == 3)
        {
            return "L";
        }
        else if (easting == 4)
        {
            return "M";
        }
        else if (easting == 5)
        {
            return "N";
        }
        else if (easting == 6)
        {
            return "P";
        }
        else if (easting == 7)
        {
            return "Q";
        }
        else if (easting == 8)
        {
            return "R";
        }
    }
    else if (remain == 0)
    {
        if (easting == 1)
        {
            return "S";
        }
        else if (easting == 2)
        {
            return "T";
        }
        else if (easting == 3)
        {
            return "U";
        }
        else if (easting == 4)
        {
            return "V";
        }
        else if (easting == 5)
        {
            return "W";
        }
        else if (easting == 6)
        {
            return "X";
        }
        else if (easting == 7)
        {
            return "Y";
        }
        else if (easting == 8)
        {
            return "Z";
        }
    }
    return "easting not found";
}

std::string lookup_northing(double northing_in, int grid_zone_in)
{
    int remain;
    remain = grid_zone_in % 2;
    
    int northing;
    northing = trunc(northing_in);
    northing = trunc((northing % 2000000)/100000);

    if (remain == 1)
    {
        if (northing == 0)
        {
            return "A";
        }
        else if (northing == 1)
        {
            return "B";
        }
        else if (northing == 2)
        {
            return "C";
        }
        else if (northing == 3)
        {
            return "D";
        }
        else if (northing == 4)
        {
            return "E";
        }
        else if (northing == 5)
        {
            return "F";
        }
        else if (northing == 6)
        {
            return "G";
        }
        else if (northing == 7)
        {
            return "H";
        }
        else if (northing == 8)
        {
            return "J";
        }
        else if (northing == 9)
        {
            return "K";
        }
        else if (northing == 10)
        {
            return "L";
        }
        else if (northing == 11)
        {
            return "M";
        }
        else if (northing == 12)
        {
            return "N";
        }
        else if (northing == 13)
        {
            return "P";
        }
        else if (northing == 14)
        {
            return "Q";
        }
        else if (northing == 15)
        {
            return "R";
        }
        else if (northing == 16)
        {
            return "S";
        }
        else if (northing == 17)
        {
            return "T";
        }
        else if (northing == 18)
        {
            return "U";
        }
        else if (northing == 19)
        {
            return "V";
        }
    }
    else if (remain == 0)
    {
        if (northing == 0)
        {
            return "F";
        }
        else if (northing == 1)
        {
            return "G";
        }
        else if (northing == 2)
        {
            return "H";
        }
        else if (northing == 3)
        {
            return "J";
        }
        else if (northing == 4)
        {
            return "K";
        }
        else if (northing == 5)
        {
            return "L";
        }
        else if (northing == 6)
        {
            return "M";
        }
        else if (northing == 7)
        {
            return "N";
        }
        else if (northing == 8)
        {
            return "P";
        }
        else if (northing == 9)
        {
            return "Q";
        }
        else if (northing == 10)
        {
            return "R";
        }
        else if (northing == 11)
        {
            return "S";
        }
        else if (northing == 12)
        {
            return "T";
        }
        else if (northing == 13)
        {
            return "U";
        }
        else if (northing == 14)
        {
            return "V";
        }
        else if (northing == 15)
        {
            return "A";
        }
        else if (northing == 16)
        {
            return "B";
        }
        else if (northing == 17)
        {
            return "C";
        }
        else if (northing == 18)
        {
            return "D";
        }
        else if (northing == 19)
        {
            return "E";
        }
    }
    return "northing not found";
}

std::string lookup_gridzone(int latitude)
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
double decode_false_easting(std::string false_easting)
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

double decode_false_northing(std::string false_northing, int grid_zone)
{
    int remain = grid_zone % 2;
    if (remain==1)
    {
        if (false_northing=="A")
        {
            return 0.0;
        }
        else if(false_northing=="B")
        {
            return 1.0;
        }
        else if(false_northing=="C")
        {
            return 2.0;
        }
        else if(false_northing=="D")
        {
            return 3.0;
        }
        else if(false_northing=="E")
        {
            return 4.0;
        }
        else if(false_northing=="F")
        {
            return 5.0;
        }
        else if(false_northing=="G")
        {
            return 6.0;
        }
        else if(false_northing=="H")
        {
            return 7.0;
        }
        else if(false_northing=="J")
        {
            return 8.0;
        }
        else if(false_northing=="K")
        {
            return 9.0;
        }
        else if(false_northing=="L")
        {
            return 10.0;
        }
        else if(false_northing=="M")
        {
            return 11.0;
        }
        else if(false_northing=="N")
        {
            return 12.0;
        }
        else if(false_northing=="P")
        {
            return 13.0;
        }
        else if(false_northing=="Q")
        {
            return 14.0;
        }
        else if(false_northing=="R")
        {
            return 15.0;
        }
        else if(false_northing=="S")
        {
            return 16.0;
        }
        else if(false_northing=="T")
        {
            return 17.0;
        }
        else if(false_northing=="U")
        {
            return 18.0;
        }
        else if(false_northing=="V")
        {
            return 19.0;
        }
    }
    else
    {
        if (false_northing=="F")
        {
            return 0.0;
        }
        else if(false_northing=="G")
        {
            return 1.0;
        }
        else if(false_northing=="H")
        {
            return 2.0;
        }
        else if(false_northing=="J")
        {
            return 3.0;
        }
        else if(false_northing=="K")
        {
            return 4.0;
        }
        else if(false_northing=="L")
        {
            return 5.0;
        }
        else if(false_northing=="M")
        {
            return 6.0;
        }
        else if(false_northing=="N")
        {
            return 7.0;
        }
        else if(false_northing=="P")
        {
            return 8.0;
        }
        else if(false_northing=="Q")
        {
            return 9.0;
        }
        else if(false_northing=="R")
        {
            return 10.0;
        }
        else if(false_northing=="S")
        {
            return 11.0;
        }
        else if(false_northing=="T")
        {
            return 12.0;
        }
        else if(false_northing=="U")
        {
            return 13.0;
        }
        else if(false_northing=="V")
        {
            return 14.0;
        }
        else if(false_northing=="A")
        {
            return 15.0;
        }
        else if(false_northing=="B")
        {
            return 16.0;
        }
        else if(false_northing=="C")
        {
            return 17.0;
        }
        else if(false_northing=="D")
        {
            return 18.0;
        }
        else if(false_northing=="E")
        {
            return 19.0;
        }        
    }
    return 0;
}

#endif