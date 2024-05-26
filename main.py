# use python script to relay information from esp to computer/mobile inorder to run hardness calculation script
import circleDetect
import serial
import time

# Define the serial port and baud rate.
serial_port = '/dev/ttyUSB0'  # Change this to your Arduino serial port
baud_rate = 9600

# Create a serial object.
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Wait for the serial connection to be established.
time.sleep(2)

# Send data to Arduino.
ser.write(b'1')  # Send '1' to turn on an LED, for example
ser.write("On")

# Wait for a response from Arduino.
arduino_response = ser.readline().decode('utf-8').rstrip()

# Print the response from Arduino.
print("Response from Arduino:", arduino_response)
if (arduino_response == "activate camera"):
    circleDetect()

# Close the serial connection.
ser.close()
