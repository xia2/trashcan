import sys

from collections import defaultdict

even = defaultdict(list)
odd = defaultdict(list)

def meansd(values):

    if not values:
        return 0.0, -1.0
    if len(values) == 1:
        return values[0], -1.0
    
    import math
    mean = sum(values) / len(values)
    var = sum([(v - mean) ** 2 for v in values]) / (len(values) - 1)
    return mean, math.sqrt(var)

for record in sys.stdin:
    tokens = record.split()
    if len(tokens) != 10:
        continue
    hkl = map(int, tokens[-3:])
    frame = int(tokens[5])
    i, sigma = float(tokens[1]), float(tokens[2])

    if sum(hkl) % 2 == 1:
        odd[frame].append(i)
    else:
        even[frame].append(i)

for j in sorted(even):
    mse = meansd(even[j])
    mso = meansd(odd[j])
    
    print j, mse[0], mse[1], mso[0], mso[1]
