from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import MaxNLocator
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np

def visualize_cost_function(sizes_transformed, prices_transformed, w0_values, w1_values, loss_function):
	points = []
	for i in w0_values:
		for j in w1_values:
			points.append([i, j, loss_function(sizes_transformed, prices_transformed, i,j)])
	points = np.array(points)

	Xs = points[:,0]
	Ys = points[:,1]
	Zs = points[:,2]


	fig = plt.figure()


	ax = fig.add_subplot(111, projection='3d')
	surf = ax.plot_trisurf(Xs, Ys, Zs, cmap=plt.cm.coolwarm, linewidth=0)

	# 'The left and right margins cannot be made large' tight_layout fix
	for spine in ax.spines.values():
		spine.set_visible(False)
	
	fig.colorbar(surf)
	ax.xaxis.set_major_locator(MaxNLocator(5))
	ax.yaxis.set_major_locator(MaxNLocator(6))
	ax.zaxis.set_major_locator(MaxNLocator(5))
	ax.set_title('Wartosc f.kosztu w dziedzinie wartosci w0 i w1')
	ax.set_zlabel('Wartosc f. kosztu')
	ax.set_xlabel('Wartosc wagi w0')
	ax.set_ylabel('Wartosc wagi w1')
	fig.tight_layout()

	plt.show() 
