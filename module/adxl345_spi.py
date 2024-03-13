import spidev
import threading
import time
# Take arguments
import argparse

# ADXL345 SPI commands
WRITE_COMMAND =     0x00
REG_DATA_RATE =     0x2C
REG_POWER_CTL =     0x2D
REG_DATA_FORMAT =   0x31
REG_DATAX0 =        0x32
REG_FIFO_CTL =      0x38
REG_FIFO_STATUS =   0x39
MULTIBYTE_COMMAND = 0x40
READ_COMMAND =      0x80
STREAM_MODE =       0b10000000

# ADXL345 Data Rate
DATA_RATE_400Hz  =  0x0C
DATA_RATE_1600Hz =  0x0E

ACC_LSB_TO_MM_PER_SEC_SQR = 2.*2./1024.*9806.65 # +/- 2 g range

class Adxl345:
    '''Provides necessary accelerometer functions for autocalibration
    using the ADXL345 accelerometer device.'''

    def __init__(self, bus=0, device=0, baud=5000000, mode=3):
        self.spi = spidev.SpiDev()
        self.spi.open(bus, device)
        self.spi.max_speed_hz = baud
        self.spi.mode = mode
        self.spi.bits_per_word = 8
        self.spi.no_cs = False

        self.x_buff = []; self.y_buff = []; self.z_buff = []
        self.c_buff = []
        self.external_stop = False

        # Data rate 16000Hz
        self.write_register(REG_DATA_RATE, DATA_RATE_400Hz)
        # Full resolution, +/-2
        self.write_register(REG_DATA_FORMAT, 0x00)
        # Stream mode with FIFO
        self.write_register(REG_FIFO_CTL, STREAM_MODE)
        # Power on
        self.write_register(REG_POWER_CTL, 0x08)

    def __del__(self):
        self.spi.close()

    def write_register(self, reg_address, value):
        """Write a value to a register."""
        self.spi.xfer2([reg_address, value])

    def read_fifo(self):
        """Read and process data from the FIFO."""
        rst = self.spi.xfer2([REG_FIFO_STATUS | READ_COMMAND, 0x00])
        # print("FIFO Status: ", rst)
        entries = rst[1] & 0x3F
        # print("Entries: ", entries)
        # if entries > 0:
        #     print("Read data....")
        #     num_bytes = entries * 6
        #     read_command = [READ_COMMAND | MULTIBYTE_COMMAND | REG_DATAX0] + [0x00] * num_bytes
        #     data = self.spi.xfer2(read_command)[1:]
        #     for i in range(0, num_bytes, 6):
        #         x, y, z = self.convert_data(data[i:i+6])
        #         self.count += 1
        #         print(f"C={self.count},X={x}, Y={y}, Z={z}")

        rst = []

        for _ in range(entries):
            data = self.spi.xfer2([READ_COMMAND | MULTIBYTE_COMMAND | REG_DATAX0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])[1:]
            x, y, z = self.convert_data(data)
            rst.append((x, y, z))

        return rst

    def convert_data(self, data):
        """Convert raw data to g."""
        x = (data[1] << 8) | data[0]
        y = (data[3] << 8) | data[2]
        z = (data[5] << 8) | data[4]
        if x & (1 << 15): x -= (1 << 16)
        if y & (1 << 15): y -= (1 << 16)
        if z & (1 << 15): z -= (1 << 16)
        return x, y, z

    def start_reading(self, interval=0.1):
        """Start reading data from the FIFO."""
        def read_loop():
            self.Period = False
            while not self.stop_thread:
                rst = self.read_fifo()
                if rst is not None:
                    for i, (x, y, z) in enumerate(rst):
                        self.count += 1
                        # print(f"C={self.count}, X={x}, Y={y}, Z={z}")
                        self.x_buff.append(x)
                        self.y_buff.append(y)
                        self.z_buff.append(z)
                        self.c_buff.append(self.count)

                if self.Period: # Record data
                    # Get time in yyyy-mm-dd-hh-mm-ss format
                    t = time.localtime()
                    t_str = f"{t.tm_year}-{t.tm_mon}-{t.tm_mday}-{t.tm_hour}-{t.tm_min}-{t.tm_sec}"
                    with open(f"results/adxl345_data_{t_str}.txt", "w") as f:
                        for i,c in enumerate(self.c_buff):
                            f.write(f"{c},{self.x_buff[i]},{self.y_buff[i]},{self.z_buff[i]}\n")
                    self.Period = False

                    # Log
                    print(f"Data recorded at {t_str}.")

                    # Clean data
                    self.x_buff = []; self.y_buff = []; self.z_buff = []
                    self.c_buff = []
                        
                time.sleep(interval)

        # Reset counter
        self.count = 0
        # Start thread
        self.stop_thread = False
        self.read_thread = threading.Thread(target=read_loop)
        self.read_thread.start()

    def stop_reading(self):
        """Stop periodic FIFO reading."""
        self.stop_thread = True
        self.read_thread.join()

    def close(self):
        """Close SPI connection."""
        self.spi.close()

if __name__ == '__main__':

    print("Program started.")
    
    adxl345 = Adxl345()
    try:
        adxl345.start_reading(interval=.001)  # Read FIFO every 1 second
        # print("Reading started.")
        # print("Wait for 5 seconds.")
        # time.sleep(5)  # Keep reading for 5 seconds
        while True:
            for i in range(60):
                time.sleep(1)
            adxl345.Period = True
    except:
        pass
    finally:
        adxl345.stop_reading()
        # for i,c in enumerate(adxl345.c_buff):
        #     print(f"C={c}, X={adxl345.x_buff[i]}, Y={adxl345.y_buff[i]}, Z={adxl345.z_buff[i]}")
        adxl345.close()
