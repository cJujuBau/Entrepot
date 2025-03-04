#include<stdio.h>
#include<stdlib.h>

#include <serial.h>

int main(int argc, char const *argv[])
{
    int port = openSerialPort("/dev/ttyS0");
    setSerialPort(port);
    writeSerial(port, "Hello World\n");
    char buffer[100];
    readSerial(port, buffer, 100);
    closeSerialPort(port);  
    return 0;
}
