import matplotlib.pyplot as plt 
from matplotlib.ticker import MaxNLocator
import numpy as np
import pandas as pd
from astropy.visualization import hist

if __name__ == '__main__':
	collisions_num = pd.read_csv("collisions_num.txt")

	hash_function = collisions_num.columns[0]

	data = collisions_num.to_numpy()
	hash_table_size = data[0]
	data = data[1:]

	fig, ax = plt.subplots(figsize = (10,10))

	bins = len(np.unique(data))
	ax.hist(data, bins = 'auto', density = False)

	ax.set_title(f'Distribution: {hash_function}')
	#plt.xscale('log')
	ax.set_xlim(1, hash_table_size + 1)
	ax.set_xlabel("Collisions for hash table's cell", fontsize = 20)
	ax.set_yticks([])
	plt.savefig(f'Plots/{hash_function}.svg', format = 'svg')
