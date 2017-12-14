import sys

first = open("generate_map/first.html")
second = open("generate_map/second.html")


coord = open("data_for_tsp/coordinates.txt")

id_lat = dict()
id_lng = dict()

for line in coord:
	val = line.split()
	id_lat[val[0]] = val[1]
	id_lng[val[0]] = val[2]

path = open("path.txt")

for line in first:
	print line

cnt = 1
for line in path:
	val = line.split()

	for i in range(len(val)):
		sys.stdout.write("	{\n")
		sys.stdout.write("	\"title\": \'Ginasio " + str(cnt) + "\',\n")
		sys.stdout.write("	\"lat\": '" + str(id_lat[val[i]]) + "',\n")
		sys.stdout.write("	\"lng\": '" + str(id_lng[val[i]]) + "',\n")
		sys.stdout.write("	\"description\": \'\',\n")
		sys.stdout.write("	},\n")

		cnt += 1

for line in second:
	print line
