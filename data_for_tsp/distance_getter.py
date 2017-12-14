import urllib, json, sys

places_file = open("data_for_tsp/places_2.txt")
places = list()

for line in places_file:
	line = line.lstrip().rstrip()

	for i in range(len(line)):
		if line[i] == " ":
			line = line[:i] + "+" + line[i+1:]

	places.append(line)

dist = []

for i in range(len(places)):
	dist.append([0] * len(places))

for i in range(len(places)):
	for j in range(i+1, len(places)):
		origin = places[i]
		destination = places[j]

		url = "https://maps.googleapis.com/maps/api/directions/json?origin=" + origin + "&destination=" + destination + "&key=AIzaSyABfMCK_09oUfkg20j3O03Esg-XlNyLirA"

		response = urllib.urlopen(url)
		data = json.loads(response.read())

		distance_str= data["routes"][0]["legs"][0]["distance"]["text"]
		val = distance_str.split()
		distance_km = float(val[0])

		dist[i][j] = distance_km
		dist[j][i] = distance_km

for i in range(len(places)):
	for j in range(len(places)):
		sys.stdout.write(str(dist[i][j]) + " ")
	sys.stdout.write("\n")
