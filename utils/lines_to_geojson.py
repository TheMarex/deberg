#!/usr/bin/env python3

import json
from graphml_parser import *

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

def convert(input_file):
    features = [encode_line(*line) for line in read_lines(input_file)]
    geojson = {
        'type': 'FeatureCollection',
        'features': features
    }
    return geojson

if __name__ == '__main__':
    print(json.dumps(convert(sys.argv[1]), indent=2))
