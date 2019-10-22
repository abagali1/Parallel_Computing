import matplotlib.pyplot as plt



axis, x_pos, y_pos, x_vel, y_vel = [], [], [], [], []

for x in open('orbit.txt').readlines():
	parts = x.strip().split(" ")
	axis.append(parts[0])
	x_pos.append(parts[1])
	y_pos.append(parts[2])
	x_vel.append(parts[3])
	y_vel.append(parts[4])



plt.plot(x_pos, y_pos)
plt.show()