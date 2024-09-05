#This code will try to read the data send from the micro-controller
import serial
import time

# Open serial port
ser = serial.Serial('/dev/tty.usbmodem1103', 9600)  # Open serial port at COM4 (or whichever COM port your device is connected to) and 19200 baud rate. Ensure the same baud rate is set in MC code
tx_buff = []
rx_buff = []


#m = data.encode()
try:
    print(f"Connection established with port.")

    size = int(input("How many numbers do you want to input? "))

    for i in range(0, size):
        tx = int(input())
        tx_buff.append(tx)

    ser.write(bytearray(tx_buff))

    while True:
        rx_buff = ser.read(size)
        int_list = list(rx_buff)
        print("Received data: ", int_list)



except KeyboardInterrupt:
    print("Stopped by user.")
    for i in rx_buff:
        print(i + "\n")

finally:
    # Close the serial port when done
    ser.close()
    print("Serial port closed.")


