#!/usr/bin/env python3

import re
import json
import math
import sys
import pyproj

epgs54004 = pyproj.Proj('+proj=merc +lat_ts=0 +lon_0=0 +k=1.000000 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m')

def swap_coordinate(lat, lon):
    return (lon, lat)

def webmercator_to_wgs84(x, y):
    #lon = (x / 20037508.34) * 180;
    #lat = (y / 20037508.34) * 180;
    #lat = 180.0/math.pi * (2 * math.atan(math.exp(lat * math.pi / 180)) - math.pi / 2);
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

def read_lines(filepath):
    lines = []
    with open(filepath) as f:
        data = f.readlines()
        tuples = []
        for d in data:
            idx = d.find(':')
            tuples.append((d[:idx], d[idx+1:]))
        lines = [(int(d[0]), parse_graphml(d[1])) for d in tuples]
    return lines

def encode_line(line_id, mercator_coordinates):
    return {
        'type': 'Feature',
        'properties': {'id': line_id},
        'geometry': {
            'type': 'LineString',
            'coordinates': [swap_coordinate(*webmercator_to_wgs84(x, y)) for (x, y) in mercator_coordinates]
        }
    }

input_file = sys.argv[1]
features = [encode_line(*line) for line in read_lines(input_file)]
geojson = json.dumps({
    'type': 'FeatureCollection',
    'features': features
}, indent=2)
print(geojson)
