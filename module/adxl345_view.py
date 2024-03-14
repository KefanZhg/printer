import matplotlib
import matplotlib.pyplot as plt
import os

# file name
file_name = "adxl345_data_2024-3-13-18-40-13"
txt_file_name = file_name + ".txt"
png_file_name = file_name + ".png"
dir = "/home/pi/Repos/printer/data"

# Read data
with open(os.path.join(dir,txt_file_name), "r") as f:
    data = f.readlines()

# Process data
c_buff = []
x_buff = []
y_buff = []
z_buff = []

for line in data:
    c, x, y, z = line.strip().split(",")
    c_buff.append(int(c))
    x_buff.append(int(x))
    y_buff.append(int(y))
    z_buff.append(int(z))

# Plot data
# matplotlib.use('TkAgg')

plt.plot(c_buff, x_buff, label='x')
plt.plot(c_buff, y_buff, label='y')
plt.plot(c_buff, z_buff, label='z')
plt.xlabel('ID')
plt.ylabel('Values')
plt.title('Acc')
plt.legend()
plt.grid(True)

plt.savefig(os.path.join(dir,png_file_name))
