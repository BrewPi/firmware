import time;
import serial;

tcp = None
ser = serial.serial_for_url('COM229',baudrate=256000, timeout=0.1, write_timeout=0)
timer = time.time()

while True:
    # send something to the photon via TCP every 0.2 sec
    try:
        if tcp is None:
            time.sleep(1)
            tcp = serial.serial_for_url("socket://192.168.2.149:6666", baudrate=57600, timeout=0.1, write_timeout=0)

        if time.time() - timer > 0.2:
            tcp.write('t'.encode())
            print("->tic")
            timer = time.time()

        # read something back via TCP
        new_tcp_data = ""
        while tcp.in_waiting > 0:
            # for sockets, in_waiting returns 1 instead of the actual number of bytes
            # this is a workaround for that
            new_tcp_data = new_tcp_data + str(tcp.read(tcp.in_waiting))
        
        if len(new_tcp_data) > 0:
            print("<-" + new_tcp_data)
    except (IOError, OSError, serial.SerialException) as e:
        print('Serial Error: {0})'.format(str(e)))
        if tcp:
            tcp.close()
        tcp = None
        time.sleep(1)



    # read serial and print it
    new_serial_data = ""
    while ser.in_waiting > 0:
        new_serial_data = new_serial_data + str(ser.read(ser.in_waiting))
        time.sleep(0.1)

    if len(new_serial_data) > 0:
        print("**" + new_serial_data)