from xml.dom import minidom
import sys
import json

def path2segment(path) :
	segment = {}
	segment["name"] = path.getAttribute("inkscape:label")
	segment["width"] =  float(dict(map(lambda z:z.split(":"), path.getAttribute("style").split(";")))["stroke-width"][:-2])
	segment["coords"] =  tuple(map(lambda z: tuple(map(float, z.split(","))), path.getAttribute("d").split()[1:]))
	return segment

output = {}
xml=minidom.parse(sys.argv[1])

output["width"] = float(xml.getElementsByTagName("svg")[0].getAttribute("width"))
output["height"] = float(xml.getElementsByTagName("svg")[0].getAttribute("height"))
output["segments"] = tuple(map(path2segment, xml.getElementsByTagName("path")))

print(json.dumps(output, sort_keys=True,indent=2, separators=(',', ': ')))

