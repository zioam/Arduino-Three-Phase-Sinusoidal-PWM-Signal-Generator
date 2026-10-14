# pip install pyserial
import serial.tools.list_ports
from pynput.keyboard import Key, Listener
from time import sleep

ports = serial.tools.list_ports.comports() # Get all ports on the computer and list them
serialInst = serial.Serial() # Set serial communication
portsList = [] # Create empty table

for one in ports: # Iterate over the ports
    portsList.append(str(one)) # Make the port a string and store it in portsList
    print(str(one))

com = input("Select COM Port number for Arduino: ")

for i in range(len(portsList)):
    if portsList[i].startswith("COM" + str(com)): # Check if the user's input is a valid port
        use = "COM" + str(com)
        print(use)

serialInst.baudrate = 9600
serialInst.port = use
serialInst.open()
        
while True:
    try:
        wave_period = input("-Set wave period time (in seconds): ")
        serialInst.write(wave_period.encode())
        maxV = input("-Set max voltage value: ")
        serialInst.write(maxV.encode())
        minV = input("-Set min voltage value: ")
        serialInst.write(minV.encode())
        
        print("Successfully created wave.")
        print("If you want to create a new wave press SHIFT")
                
        # Collect all event until Enter key is pressed
        with Listener(on_press=lambda key: None if key != Key.shift else False) as listener:
            if not listener.join():
                 continue
                     
    except ValueError:
        print("Please enter a valid int/float value.")
        
#with: This keyword is used to manage resources in Python. It ensures that certain operations are properly handled, such as opening and closing files, or in this case, setting up and handling keyboard listeners.

#Listener: This is a class provided by the pynput.keyboard module. It creates a new keyboard listener object.

#on_press: This is an argument of the Listener class. It expects a function that will be called when a key is pressed.

#(lambda key: None if key != Key.enter else False:) This is a lambda function defined inline. It takes a key parameter and returns None if the key is not the Enter key (Key.enter), otherwise it returns False. Essentially, it filters out any key presses except the Enter key.

#as listener: This part assigns the created listener object to the variable listener. It's important because it allows us to refer to the listener object within the context of the with block.