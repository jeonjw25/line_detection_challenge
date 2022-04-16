import pandas as pd
from pandas_ods_reader import read_ods
from numpy import genfromtxt
import matplotlib.pyplot as plt

ground_truth = read_ods("pos.ods", 1).astype(int)
estimated = genfromtxt('test.csv', delimiter=',', dtype=int)
estimated = pd.DataFrame(estimated)

result = []
for i, row in ground_truth.iterrows():
    lposl = row['new lposl']
    lposr = row['new lposr']
    rposl = row['new rposl']
    rposr = row['new rposr']
    lpos = estimated[0][i]
    rpos = estimated[1][i]

    if lposl <= lpos <= lposr and rposl <= rpos <= rposr:
        result.append(1)
    else:
        result.append(0)


print("accuracy: ", str(sum(result)/len(ground_truth)*100), "%")
plt.xlabel("fps(x30)")
plt.ylabel("correct")

plt.plot(result)
plt.show()
