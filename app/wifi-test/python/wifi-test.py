import time;
import serial;
import colorama
colorama.init()

# colorama is used to get some color on the terminal
# tcp trafic from python to photon is printed in RED
# tcp trafic from photon to python is printed in BLUE
# prints directly relaying stuff received over serial are green

tcp = None
ser = None
timer = time.time()

serial_buffer = ""

while True:

    if ser is None:
        try:
            ser = serial.serial_for_url('/dev/ttyACM0',baudrate=115200, timeout=0.1, write_timeout=1)
            print("Serial connected\n")
        except (IOError, OSError, serial.SerialException) as e:
            print('Serial Error: {0})'.format(str(e)))
            if ser:
                ser = None
                ser.close()
            time.sleep(1)
        

    # send something to the photon via TCP every 0.2 sec
    try:
        if tcp is None:
            time.sleep(1)
            tcp = serial.serial_for_url("socket://192.168.1.61:6666", baudrate=57600, timeout=0.1, write_timeout=1)
        
        if time.time() - timer > 1.0:
            tcp.write('t'.encode('utf-8'))
            print(colorama.Fore.RED + "py->hw: tic" + colorama.Style.RESET_ALL)
            timer = time.time()

        # read something back via TCP
        new_tcp_data = ""
        while tcp.in_waiting > 0:
            # for sockets, in_waiting returns 1 instead of the actual number of bytes
            # this is a workaround for that
            new_tcp_data = new_tcp_data + tcp.read(tcp.in_waiting).decode('utf-8')
        
        if len(new_tcp_data) > 0:
           print(colorama.Fore.BLUE + "py<-hw: " + new_tcp_data + colorama.Style.RESET_ALL)
    except (IOError, OSError, serial.SerialException) as e:
        print('TCP Error: {0})'.format(str(e)))
        if tcp:
            tcp.close()
        tcp = None
        time.sleep(1)



    # read serial and print complete lines (ending in \n)
    while ser and ser.in_waiting > 0:
        serial_buffer = serial_buffer + ser.read(ser.in_waiting).decode('utf-8')

        lines = serial_buffer.partition('\n') # returns 3-tuple with line, separator, rest
        if lines[1]:
            # complete line received, [0] is complete line [1] is separator [2] is the rest
            serial_buffer = lines[2]
            print(colorama.Fore.GREEN + lines[0] + colorama.Style.RESET_ALL)
