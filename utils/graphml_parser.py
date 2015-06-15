import re
import sys
import pyproj

epgs54004 = pyproj.Proj('+proj=merc +lat_ts=0 +lon_0=0 +k=1.000000 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m')

def swap_coordinate(lat, lon):
    return (lon, lat)

def webmercator_to_wgs84(x, y):
    lon, lat = epgs54004(x,y, inverse=True)
    return [lat, lon]

def parse_graphml(xml):
    reg_exp = re.compile('<gml:coordinates[^>]*>([^<]+)</gml:coordinates>')
    matching = reg_exp.search(xml)
    if not matching:
        print("Error: " + xml)
    string_coords = matching.group(1).split(' ')
    coords = [pair.split(',') for pair in string_coords if pair is not '']
    coords = [(float(x), float(y)) for (x, y) in coords]
    return coords
