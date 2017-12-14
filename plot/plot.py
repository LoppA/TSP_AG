import matplotlib.pyplot as plt

points_file = open("fitnesses.txt")
points = list()

for line in points_file:
	val = line.split()

	for v in val:
		points.append(-1*float(v))

plt.plot(points)
plt.ylabel("Best of gen.")
plt.show()
