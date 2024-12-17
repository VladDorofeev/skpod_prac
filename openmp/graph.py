import os
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.tri import Triangulation

if not os.path.exists('outputs'):
    os.makedirs('outputs')


data = {
    "MINI": [0.00103, 0.00061, 0.00051, 0.00042, 0.00046, 0.00048, 0.00050, 0.00048, 0.00048, 0.00048, 0.00077, 0.00177, 0.00265, 0.06680, 0.11075, 0.11379, 0.11923, 0.01819],
    "SMALL": [0.01937, 0.00984, 0.00675, 0.00572, 0.00469, 0.00377, 0.00381, 0.00384, 0.00299, 0.00384, 0.00379, 0.00491, 0.00717, 0.05481, 0.06698, 0.16217, 0.23436, 0.04731],
    "MEDIUM": [0.52731, 0.69972, 0.75551, 0.83692, 0.96676, 0.64783, 0.70522, 0.84081, 0.64917, 0.86053, 1.40099, 2.27680, 3.13395, 4.28480, 4.53489, 3.77729, 4.13804, 4.88725],
    "LARGE": [107.33806, 41.21526, 27.70170, 20.92078, 18.63290, 14.80778, 14.04924, 10.74723, 10.86076, 11.33927, 10.83925, 6.95163, 7.35202, 6.96709, 8.12391, 9.41564, 10.71500, 11.02543],
    "EXTRALARGE": [235.75796, 107.14582, 76.66071, 61.50010, 46.40706, 46.67019, 31.92508, 31.61193, 32.48024, 31.48445, 20.75501, 20.80182, 22.78508, 24.54375, 19.99284, 20.61582, 21.52804, 21.67131]
}

threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 40, 60, 80, 100, 120, 140, 160]


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Данные для осей
datasets = list(data.keys())
X = []
Y = []
Z = []

for i, dataset in enumerate(datasets):
    for j, value in enumerate(data[dataset]):
        X.append(threads[j])
        Y.append(i)
        Z.append(value)

triangles = Triangulation(X, Y)
ax.plot_trisurf(triangles, Z, cmap='viridis')


# Поворот графика
# ax.view_init(elev=20, azim=50)  # elev - угол наклона, azim - азимут


ax.set_xlabel('Threads')
ax.set_ylabel('Datasets')
ax.set_zlabel('Time')
ax.set_yticks(np.arange(len(datasets)))
ax.set_yticklabels(datasets)

plt.savefig('outputs/polus.pdf', bbox_inches='tight')
