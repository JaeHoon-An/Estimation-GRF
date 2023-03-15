import numpy as np

filePath = "datasets/GRFDatasets_2.npz"
data = np.load(filePath)
for key, value in data.items():
    np.savetxt("GRFDatasets_" + key + ".csv", value)