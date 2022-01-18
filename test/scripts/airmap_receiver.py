#!/usr/bin/env python3

import sys
import requests
from geopoint import GeoPoint

def main(points):
    url = f'https://api.airmap.com/elevation/v1/ele?points={point}'
    req = requests.get(url)
    data = req.json()
    if data['data']:
        print(*data['data'])

if __name__ == '__main__':
    point = sys.argv[1]
    main(point)
