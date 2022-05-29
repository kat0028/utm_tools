# utm_tools

This is a header only package that contains classes for converting to and from the UTM coordinate system.
As this package devolps the names of functions may change slightly to reflect the format of the data being entered.

# Current Features

The utm_converter class has class member functions for converting between LLA MGRS and UTM. Each of the coordinate frames has a data structure associated with it. 

Note: The current version of the code that exists on this branch accounts for the administrative exceptions to UTM in Northern Europe and the regions above 72 degrees North.

# Current Stable Class Functions
- [x] LLA to UTM conversion
- [x] UTM to LLA conversion
- [x] LLA to MGRS conversion
- [x] MGRS to LLA conversion
- [x] MGRS to UTM conversion
- [x] UTM to MGRS conversion

# Current Data Structures
LLA
---
- latitude (double)
- longitude (double)
- alt (double)

UTM 
---
- easting (double)
- northing (double)
- grid_zone (int)
- alt (double)

MGRS
---
- easting (double)
- northing (double)
- grid_zone (int)
- alt (double)
- grid_letter (string)
- false_easting (string)
- false_northing (string)

# Work in Progress
- [ ] Ongoing testing
- [ ] ECEF to UTM 
- [ ] UTM to ECEF
