# DeBerg

This project implement a variation of the DeBerg line simplification algorithm.

The algortithm enables you to simplify lines without introducing self-intersections.
You can also define an additional set of points that will stay consitent with regard to the line simplification:
They will not change the 'side' of the line (e.g. when simplifying country borders cities will not suddently be in different countries).

This variation always computes the strongest simplification possible using the fewest edges.
![Example picture](example.png?raw=true "Optional Title")

# Building

All dependencies (other than `boost`) should be included in `third_party/`.

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

# Running

`./deberg MAX_NUMBER_EDGES lines.txt points.txt output_lines.txt`

Will write the result in `output_lines.txt` and print number of used edges in the console.

## Input format

[osm-admin-bounds](https://github.com/TheMarex/osm-admin-bounds) is a tool to generate the input data from OpenStreetMap data.

You can use `utils/data_to_geojson.py` to convert files to geojson for easy visualization.

### lines.txt

```
id:graphml_string
```

Example:

```
1:<gml:LineString srsName="EPSG:54004" xmlns:gml="http://www.opengis.net/gml"><gml:coordinates decimal="." cs="," ts=" ">-7984749.593824852,5368675.690126911 -7984392.035620423,5367570.791302501 -7983220.843257783,5366848.500158152</gml:coordinates></gml:LineString>
```

### points.txt

```
id:graphml_string
```

Example:

```
115:<gml:Point srsName="EPSG:54004" xmlns:gml="http://www.opengis.net/gml"><gml:coordinates decimal="." cs="," ts=" ">-7878323.0399299,5218629.93044479 </gml:coordinates></gml:Point>
```

### output.txt

Same format as `lines.txt`.

