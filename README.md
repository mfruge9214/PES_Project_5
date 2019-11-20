# PES_Project_5

Authors: Mike Fruge and Mark Hinkle

Repo Contents: This repository contains all code and other dosumentation for Project 5 including:
      - UART source code
      - Systick source code
      - Circular buffer source code
      - Test Cases written to test program
      
This repository also contains a FEMA chart for the program, as well as a screen grab for the UART transmission
      
Design Decisions:

Because of how we constructed our code, we decided to put our systick incrementing and UART sending and receiving inside our ISR's. This is because both operations are quite short, and we have both used similar implementations in our other classes.
      
We decided to use buffer lengths and buffer capacity's to check if the buffer was full, which works in all cases as we have implemented it.
      
      
Observations and Issues:
      There were multiple problems encountered while developing this Project. The first is that when the project was created, a setting was changed from the Semihost to the UART. We believe that this decision caused us to not be able to see any print statements using printf. Due to this, it was impossible to see output from our Unit test cases (due to the use of printf), as well as debug using printing. This slowed the development process significantly, and is an issue we still have not solved.
      
Another issue is that we could not really get the interrupt based application to work as desired, although all other modes are operational.
      
We also had significant problems with configuring the UART clocks that we solved in the final minutes of development. We were having issues seeing or sending any characters to the comm port.
      
Lastly, we had problems trying to acquire the uart communication on the oscilloscope because we were not sure how to get access to the UART0 pin.
      
      
Execution Instructions:
      Before running this application, ensure to select what combination of modes the application should run in by changing the #define PROGRAM_MODE and #define BLOCKING_MODE to see the desired output
      
