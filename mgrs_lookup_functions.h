#ifndef MGRS_LOOKUP_FUNCTIONS
#define MGRS_LOOKUP_FUNCTIONS
#include <string>
// ------------------------------------------------------------------------------------------
// LOOKUP TABLES FOR MGRS
// ------------------------------------------------------------------------------------------

std::string lookup_easting(int remain, int easting)
{

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

std::string lookup_northing(int remain, int northing)
{
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

#endif