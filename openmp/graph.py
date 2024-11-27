import os
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.tri import Triangulation

# Создание папки outputs, если она не существует
if not os.path.exists('outputs'):
    os.makedirs('outputs')

data = {
    "MINI": [0.00103, 0.00061, 0.00051, 0.00042, 0.00046, 0.00048, 0.00050, 0.00048, 0.00048, 0.00048, 0.00077, 0.00177, 0.00265, 0.06680, 0.11075, 0.11379, 0.11923, 0.01819],
    "SMALL": [0.01937, 0.00984, 0.00675, 0.00572, 0.00469, 0.00377, 0.00381, 0.00384, 0.00299, 0.00384, 0.00379, 0.00491, 0.00717, 0.05481, 0.06698, 0.16217, 0.23436, 0.04731],
    "MEDIUM": [0.52731, 0.69972, 0.75551, 0.83692, 0.96676, 0.64783, 0.70522, 0.84081, 0.64917, 0.86053, 1.40099, 2.27680, 3.13395, 4.28480, 4.53489, 3.77729, 4.13804, 4.88725],
    "LARGE": [90.12245, 42.02478, 33.51516, 29.57332, 29.54563, 18.48189, 14.67112, 18.04242, 14.70458, 14.18641, 9.46661, 10.42230, 10.34710, 12.25734, 14.32087, 19.33786, 15.16698, 12.83658],
    "EXTRALARGE": [825.13360, 384.12329, 270.55194, 195.80835, 155.52329, 155.47841, 119.41776, 129.60223, 118.51438, 83.98040, 64.73900, 90.43020, 81.33944, 72.68150, 62.42730, 66.16616, 72.74211, 77.57058]
}

# Число нитей
threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 40, 60, 80, 100, 120, 140, 160]

# Создание фигуры
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

# Построение поверхности
triangles = Triangulation(X, Y)
ax.plot_trisurf(triangles, Z, cmap='viridis')


# Поворот графика
# ax.view_init(elev=20, azim=50)  # elev - угол наклона, azim - азимут


# Метки осей
ax.set_xlabel('Threads')
ax.set_ylabel('Datasets')
ax.set_zlabel('Time')
ax.set_yticks(np.arange(len(datasets)))
ax.set_yticklabels(datasets)

# Сохранение графика в файл
plt.savefig('outputs/polus.pdf', bbox_inches='tight')
