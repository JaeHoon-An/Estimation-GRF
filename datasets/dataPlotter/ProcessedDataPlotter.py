import numpy as np
import matplotlib.pyplot as plt

outfile = np.load('datasets/training/GRFDatasets.npz')
# outfile = np.load('datasets/dataPlotter/GRFDatasets_valid1.npz')

print(outfile.files)               # 저장된 data 표시
input_data = outfile['input_data']           # data1 꺼내기
output_data = outfile['output_data']         # data2 꺼내기


first_idx = 0 * 30000
last_idx = first_idx + 30000;
k = np.arange(first_idx, last_idx)

plt.subplot(4, 2, 1)
plt.title("position hip",fontsize=15)
plt.xlabel("input_k",fontsize=10)
plt.ylabel("position_hip",fontsize=10)
plt.plot(k,input_data[first_idx:last_idx, 0],".",color = 'blue')

plt.subplot(4, 2, 2)
plt.title("position knee",fontsize=15)
plt.xlabel("input_k",fontsize=10)
plt.ylabel("position_knee",fontsize=10)
plt.plot(k,input_data[first_idx:last_idx, 3],".",color = 'blue')

plt.subplot(4, 2, 3)
plt.title("velocity hip",fontsize=15)
plt.xlabel("input_k",fontsize=10)
plt.ylabel("velocity_hip",fontsize=10)
plt.plot(k,input_data[first_idx:last_idx, 6],".",color = 'blue')

plt.subplot(4, 2, 4)
plt.title("velocity knee",fontsize=15)
plt.xlabel("input_k",fontsize=10)
plt.ylabel("velocity_knee",fontsize=10)
plt.plot(k,input_data[first_idx:last_idx, 9],".",color = 'blue')

plt.subplot(4, 2, 5)
plt.title("torque hip",fontsize=15)
plt.xlabel("input_k",fontsize=10)
plt.ylabel("torque_hip",fontsize=10)
plt.plot(k,input_data[first_idx:last_idx, 12],".",color = 'blue')

plt.subplot(4, 2, 6)
plt.title("torque knee",fontsize=15)
plt.xlabel("input_k",fontsize=10)
plt.ylabel("torque_knee",fontsize=10)
plt.plot(k,input_data[first_idx:last_idx, 13],".",color = 'blue')

plt.subplot(4, 2, 7)
plt.title("true GRF",fontsize=15)
plt.xlabel("input_k",fontsize=10)
plt.ylabel("true_GRF",fontsize=10)
plt.plot(k,output_data[first_idx:last_idx],".",color = 'blue')
plt.show()