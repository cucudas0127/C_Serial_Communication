# C_Serial_Communication


Serial Communication using C language ( RS-232, RS-485, USB,...,)

## Basic code
it is basic code for serial communication, when only one device is connected.   
The compiler should use **gcc**.
```
gcc -o BasicCode.out main.c
```
Excute file is
```
./BasicCode.out
```

You may have to enter command if program doesn't work even though it is connected to the device.   
```
chmod 777 {Enter your serial dev location like '/dev/ttyS1'}
```
## withQueue
In this code, the receive buffer is made up of a queue.   

   
In this code, ReadFunction reads the rx_buffer very briefly and exits immediately.
This way is prevents system delay, but it also makes it difficult to find when recived signal begin or end.   
Therefore, the receive buffer must consist of a queue.   
    

you can complie using cmake    

```
cmake CMakeLists.txt
make
```

if you not want using cmake
you can use this scripts
```
gcc -c -o Queue.o Queue.c
gcc -c -o main.o main.c
gcc -o withQueue.out main.o Queue.o
```

Excute command is
```
./withQueue.out
```

## withSocket

The time will surely come have to send data to the server, when you play with the Sensors.   

So I leave a basic code here.   

## withPack


## MultiSenosorSystem
