#!/usr/bin/env python3

import json

from graphml_parser import *

def read_points(filepath):
    points = []
    with open(filepath) as f:
        data = f.readlines()
        tuples = []
        for d in data:
            idx = d.find(':')
            tuples.append((d[:idx], d[idx+1:]))
        points = [(int(d[0]), parse_graphml(d[1])[0]) for d in tuples]
    return points

def encode_point(point_id, coordinates):
    return {
        'type': 'Feature',
        'properties': {'id': point_id},
        'geometry': {
            'type': 'Point',
            'coordinates': swap_coordinate(*webmercator_to_wgs84(*coordinates))
        }
    }

def convert(input_file):
    features = [encode_point(*point) for point in read_points(input_file)]
    geojson = {
        'type': 'FeatureCollection',
        'features': features
    }
    return geojson

if __name__ == '__main__':
    print(json.dumps(convert(sys.argv[1]), indent=2))
