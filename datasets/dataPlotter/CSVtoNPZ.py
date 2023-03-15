import numpy as np
path_of_csv = "datasets/training/GRFDatasets_4.csv"
# path_of_csv = "datasets/validation/GRFDatasets_valid1.csv"
data = np.genfromtxt(path_of_csv, delimiter=',')
input_data = data[:, 0:14]
output_data = data[:, 14]
np.savez('datasets/training/GRFDatasets_4.npz', input_data=input_data, output_data=output_data)
