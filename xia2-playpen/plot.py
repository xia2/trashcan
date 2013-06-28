import matplotlib.pyplot as plt

x = []
y = []

for record in open('cc_reference.log', 'r'):
    values = map(float, record.split())
    if not values:
        break
    x.append(values[4])
    y.append(values[6])

plt.plot(x, y, 'o')
plt.axis([-1, 1, -1, 1])
plt.show()
