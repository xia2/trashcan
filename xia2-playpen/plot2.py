import matplotlib.pyplot as plt

x = []
y = []

for record in open('cc_reference.log', 'r'):
    values = map(float, record.split())
    if not values:
        break
    x.append(values[3])
    y.append(values[5])

plt.plot(x, y, 'o')
plt.show()
