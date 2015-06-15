#!/usr/bin/env python3

import points_to_geojson
import lines_to_geojson
import sys
import json

lines_path = sys.argv[1]
points_path = sys.argv[2]

lines_fc = lines_to_geojson.convert(lines_path)
points_fc = points_to_geojson.convert(points_path)
lines_fc['features'] += points_fc['features']

print(json.dumps(lines_fc, indent=2))
