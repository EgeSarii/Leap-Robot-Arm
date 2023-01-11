##  Notes on Connecting Leap Motion to Raspberry Pi Pico

The first idea to connect Leap to Pico is trying to get input from computer itself and redirect it to Pico. 
So we connect Leap and Pico to the same computer. Then we redirect output of Leap to Pico as an input. This may increase latency.

There is a way to do this. The principle is using the serial port connection. We can describe the basic algorithm in steps:
1. Open a serial connection in Leap.
2. Write the output of the program to the serial in Leap
3. Read the input of the serial connection in Pico
4. Use this input for the Robot Arm in Pico
5. Repeat the steps 1-2-3-4 until the program is terminated.


Now we should inspect and explain these steps in a technical detailed perspective.
For Step 1, we can use open() function of C++. We should use C++ for the steps 1 and 2 since we have used C++ for Leap .
Opening the serial connection looks like this:

	`int serial = open("/dev/ttyACM00", O_RDWR);`

I have chosen ttyACM0 for my USB bus but for other computers it may be different. An error handling may be added :

	if ( serial< 0 )
	{
		cout << "Error on serial port!"<<endl;
	}
	
Also the libraries must be imported :

	#include <stdio.h>      // standard input / output functions
	#include <stdlib.h>
	#include <string.h>     // string function definitions
	#include <unistd.h>     // UNIX standard function definitions`

For Step 2, we can write the output to the serial by using write() function. The variable `output` represents the output of the program to be written to serial :

	write(serial, output, sizeof(output));

For Step 3 we should use Python since we have used Python for Pico. We can use the sys package and stdin.readline() function for reading:

	import sys
	input = sys.stdin.readline().strip()
	



[C++ serial ports](https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/)


[Pico external communication](https://stackoverflow.com/questions/72151781/how-can-i-get-raspberry-pi-pico-to-communicate-with-pc-external-devices)


[Python serial ports](https://pythonhosted.org/pyserial/shortintro.html#opening-serial-ports)



