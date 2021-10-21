# 8051-stopwatch
Written using the reg51.h file, this program simulates a stopwatch on an 8051 microchip, the ports are programmed for the default build of the edsim51 program.  
The LCD module used in the simulation is equivalent to an HD44780U from HITACHI.  
Here's the logic diagram of the circuit:  
![EDSIM51 logic diagram](https://github.com/EduLodi/8051-stopwatch/blob/main/edsim51_LD.jpg "Logic Diagram")  
And here's how the stopwatch works:  
![stopwatch pic](https://github.com/EduLodi/8051-stopwatch/blob/main/stopwatch.png "stopwatch")  

1 - current time count (X1)  
2 - difference between the current time and the last time the button was pressed (X3 = X1 - X2)  
3 - time when the button was last pressed (X2)  
4 - biggest difference recorded (biggest X3)  
5 - smallest difference recorded (smallest X3)  

Two buttons are used as interruptions triggered by falling border, the SWITCH 2 is used to save record the current time and show the differences, and the SWITCH 3 is used to RESET the sistem.
