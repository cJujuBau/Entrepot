#include<stdio.h>
#include<stdlib.h>

#include <serial.h>

#define PORT_ARDUINO "/dev/ttyS0"

int main(int argc, char const *argv[])
{
    int port = openSerialPort(PORT_ARDUINO);
    setSerialPort(port);
    writeSerial(port, "Hello World\n");
    char buffer[100];
    readSerial(port, buffer, 100);
    closeSerialPort(port);  
    return 0;
}
