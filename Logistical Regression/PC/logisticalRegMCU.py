import serial
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
import struct

SCALE_FACTOR = 1000  # Define a scale factor to convert floats to integers

def main():
    df = pd.read_csv("diabetes.csv")
    X = df.iloc[:, :-1]
    Y = df.iloc[:, -1]

    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=1 / 3, random_state=0)
    print(f"Expected Output: {Y_test}")

    mcuPredict(X_test)

def mcuPredict(X):
    rows, cols = X.shape
    X_flat = (X.to_numpy().flatten())  # Convert floats to scaled integers
    tx_buff = struct.pack(f'{rows*cols}f', *X_flat)  # Pack as integers



    try:
        ser = serial.Serial('/dev/tty.usbmodem1103', 9600)
        print("Connection Established")
        ser.write(struct.pack('ii', rows, cols))  # Send rows and cols as integers
        print(f"Cols and Rows Transmitted: {rows} and {cols}")

        while True:

            ser.write(tx_buff)
            Y_pred = struct.unpack(f'{rows}i', ser.read(rows*4))
            break

    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

    print(f"Recieved Data: {Y_pred}")  # Send the packed data

if __name__ == "__main__":
    main()
