import urllib, json

places_file = open("data_for_tsp/places.txt")
places = list()

for line in places_file:
	line = line.lstrip().rstrip()

	for i in range(len(line)):
		if line[i] == " ":
			line = line[:i] + "+" + line[i+1:]

	places.append(line)

for i in range(len(places)):
	location = places[i]

	url = "https://maps.googleapis.com/maps/api/geocode/json?address=" + location + "&key=AIzaSyAcb_-Wy-oUFjx3ADMJCrnrCe5A-ypA9KU"

	response = urllib.urlopen(url)
	data = json.loads(response.read())
	
	latitude = data["results"][0]["geometry"]["location"]["lat"]
	longitude = data["results"][0]["geometry"]["location"]["lng"]
	print str(i) + " " + str(latitude) + " " + str(longitude)
