# TinyGPSMinus

A smaller and simpler TinyGPS fork with fewer features. The idea is to only get the raw data needed and avoid unneeded parsing, especially if GPS data is being fed into APRS.

APRS requires latitude and longitude as ddmm.hhN and dddmm.hhW, which is originally how NMEA strings broadcast positions, so, this fork removes the built in decimal conversion to allow you to get positions in the format APRS desires without additional conversions.

Additionally, other functions that may not be necessary for APRS usage, especially on HABs where memory is particularly constrained have been removed, including library_version, distance_between, and course_to. Positions are also no longer available as longs or floats but rather char arrays as that is all that is needed for rudimentary comparisons and for broadcasting positions.

## Improved Constellation and GNSS Listening

The original TinyGPS library only listened to GPRMC and GPGGA, which means we are only getting data from the United State's GPS constellation and not getting every relevant update transmitted either. There are, however, many other Global Navigation Satellite Systems (GNSS) and NMEA messages that we want to listen to.

This fork was built with the [U-blox M10](https://content.u-blox.com/sites/default/files/MAX-M10_ProductSummary_UBX-20017987.pdf) in mind, however it should work for other GPS chips as well. Specifically, the [U-blox MAX-M10M outputs NMEA 4.11 messages](https://content.u-blox.com/sites/default/files/documents/MAX-M10M_DataSheet_UBX-22028884.pdf#page=15):

- [GGA](https://gpsd.gitlab.io/gpsd/NMEA.html#_gga_global_positioning_system_fix_data) - Global positioning system fix data (time, position, satellites in view, altitude, and fix)
- [GLL](https://gpsd.gitlab.io/gpsd/NMEA.html#_gll_geographic_position_latitudelongitude) - Geographic position (latitude/longitude, time)
- [GSA](https://gpsd.gitlab.io/gpsd/NMEA.html#_gsa_gps_dop_and_active_satellites) - GPS DOP and active satellites
- [GSV](https://gpsd.gitlab.io/gpsd/NMEA.html#_gsv_satellites_in_view) - Satellites in view (only at 5-second intervals in the default configuration)
- [RMC](https://gpsd.gitlab.io/gpsd/NMEA.html#_rmc_recommended_minimum_navigation_information) - Recommended minimum navigation information (time, position, speed, course, date)
- [VTG](https://gpsd.gitlab.io/gpsd/NMEA.html#_vtg_track_made_good_and_ground_speed) - Track made good and ground speed (course over ground, speed over ground)
- TXT

As such, these are the only messages we will bother adding parser support for, as others aren't ever going to be outputted by a U-blox that doesn't support receiving them.

Additionally, U-blox supports a variety of different GNSS constellations, including:

| System    | Talker ID  | Country | Notes                                     |
|-----------|------------|---------|-------------------------------------------|
| GPS       | GP         | U.S.    |                                           |
| QZSS      | GQ, PQ, QZ | Japan   | Augments GPS                              |
| Galileo   | GA         | EU      |                                           |
| SBAS      |            |         | Augments GPS and Galileo                  |
| GLONASS   | GL         | Russia  |                                           |
| BeiDou    | BD, GB     | China   |                                           |
| NMEA 1083 | GN         |         | Combination of multiple satellite systems |

However, the default configuration on MAX-M10M is GPS and Galileo with QZSS and SBAS enabled (and NMEA 1083, though it isn't a constellation). To enable the other constellations, TX to U-blox is required. For my personal usage of this library, I did not have enough pins to TX to the U-blox and stuck with the default talker IDs only, although modifying this library to parse for other options, such as the [other common talker IDs](https://gpsd.gitlab.io/gpsd/NMEA.html#_talker_ids), should be fairly straightforward should someone wish to fork this or open a pull request.

Based on the above information, this library will parse talker IDs of GP, GQ, PQ, QZ, GA, and GN. Additionally, we will not keep track of the varying different ways of counting the number of satellites we can see, as with multiple different constellations at play, this can get complicated. Therefore we will only parse GGA, GLL, RMC, and VTG strings.

## Resource Considerations

With all these extra NMEA strings to listen to, GNSS constellations to keep track of, and additional string parsing to output data in the format APRS requires, let's compare memory usage to the plain TinyGPS library.

To start, let's compare the memory usage of the various example sketches on an Arduino Nano:

|                              | TinyGPS | TinyGPSMinus | Storage/RAM Reduction |
|------------------------------|---------|--------------|-----------------------|
| **static_test.ino**          |         |              |                       |
| Program Storage Space        | 37%     | 30%          | 18.92%                |
| Dynamic Memory Usage         | 40%     | 17%          | 57.50%                |
| **simple_test.ino**          |         |              |                       |
| Program Storage Space        | 26%     | 21%          | 19.23%                |
| Dynamic Memory Usage         | 28%     | 20%          | 28.57%                |
| **test_with_gps_device.ino** |         |              |                       |
| Program Storage Space        | 42%     | 34%          | 19.05%                |
| Dynamic Memory Usage         | 49%     | 26%          | 46.94%                |

This gives a rather rough approximation that by simply switching to TinyGPSMinus, you can save ~19.07% more program storage space and ~44.34% more dynamic memory when compared to TinyGPS.

Additionally, a fully loaded TinyGPS library consumes 115 Bytes of RAM, while a fully loaded TinyGPSMinus consumes only 86 Bytes of RAM, a reduction of 25.23%

On a (admittedly small) dataset of real GPS data, GPS position parsing took an average of 1.01ms on TinyGPSMinus compared to an average of 1.26ms on TinyGPS. This includes replacing data with spaces as according to the APRS spec to designate ambiguity data. Date parsing did not yield a significant enough trend to be considered different.

## Future Work

This fork was built with the very specific purpose of parsing GPS data on a HAB with very tight memory constraints in place in order to transmit position information over APRS. It works for the use-case it was built for, but perhaps in the future the ability to customize what parsing occurs may be beneficial. For example we might add features to enable:

- Toggle on/off of location parsing, date/time parsing, datapoints, etc
- Whitelist/blacklist specific NMEA sentences or GNSS constellations
- ~~Talking to GPS TX to induce sleep periods and faster fixes with data seeding~~
- ~~Spit out location data in compressed base-91 format~~
- ~~Spit out altitude data in APRS format~~

Some of these proposed features, however, illustrate the blurred line between where GPS/NMEA parsing should end, and where APRS packet generation libraries/code should pick up. In this instance, avoiding formatting positions as floats/longs and leaving them as char arrays makes sense, as they need to be in that format for transmission. This makes less sense, however, if they need to be base-91 encoded, as then we may need to be able to read individual values. Things implemented here like modifying ambiguity don't really add to the efficiency and performance of the library, and could just as easily be done elsewhere, however I do believe that the ambiguity latitude modifications do make sense in the context of GPS only uses and isn't necessarily as APRS-specific as say a base-91 or altitude encoder would be.

## Conclusion

In summary, especially for usage in APRS environments or where very simple NMEA data is required, this library will save improve the speed of your GPS parsing by ~19.84%, lower program storage space by ~19.07%, lower dynamic memory usage by ~44.34%, and lower runtime RAM consumption by ~25.23%, all of which can be quite significant on RAM constrained micro-controllers.

This library also may very will lower your time to fix or your time until first data (although I have not tested enough to develop concrete metrics for this), as it allows you to listen in on more GNSS constellations and NMEA sentences than previously enabled. However, if you have a GPS module other than a U-blox that may support more GNSS constellations and NMEA sentences than outlined above, you may find it beneficial to modify the code, as this is tailored to do the absolute bare minimum for a U-blox.
