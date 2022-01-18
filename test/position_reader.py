#!/usr/bin/env python3

import os
import sys
import math
from gmalthgtparser import HgtParser

RES_PATH = "/home/user/projects/elevation_map/resources"
hgt_directory = f'{RES_PATH}/hgt/'
hgt_files = os.listdir(hgt_directory)

class GeoPoint:
    def __init__(self, lat, lon):
        self._latitude = lat
        self._longitude = lon

    def latitude(self):
        return self._latitude

    def longitude(self):
        return self._longitude

    def get(self):
        return self._latitude, self._longitude

def main(point):
    filename = calculate_filename(point)
    if not os.path.isfile(filename):
        return
    elev = elevation_from_latlon(filename, point)
    if elev:
        print(elev)

def calculate_filename(point):
    lat = point.latitude()
    lon = point.longitude()
    vertical = 'N'
    horizontal = 'E'
    if (lat < 0):
        vertical = 'S'
    if (lon < 0):
        horizontal = 'W'

    return hgt_directory + '%s%02.0f%s%03.0f.hgt' % (vertical,
            math.floor(abs(lat)), horizontal, math.floor(abs(lon)))

def elevation_from_latlon(hgt_file, point):
    try:
        with HgtParser(hgt_file) as parser:
            alt = parser.get_elevation(point.get())
            return alt[2]
    except Exception:
        pass

if __name__ == '__main__':
    args = sys.argv[1].split(',')
    lat = float(args[0])
    lon = float(args[1])
    main(GeoPoint(lat, lon))
