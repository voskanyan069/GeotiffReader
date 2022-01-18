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
