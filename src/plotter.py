import matplotlib.pyplot as plt 
from matplotlib.ticker import MaxNLocator
import numpy as np
import pandas as pd

if __name__ == '__main__':
	collisions_num = pd.read_csv("collisions_num.txt")

	hash_function = collisions_num.columns[0]

	data = collisions_num.to_numpy()
	bin_num = len(np.unique(data))

	fig, ax = plt.subplots(figsize = (10,10))
	plt.xticks(range(np.min(data), np.max(data) + 1, 1000))
	ax.hist(data, bins = bin_num)
	ax.set_title(f'{hash_function}')

	plt.savefig(f'Plots/{hash_function}.svg', format = 'svg')