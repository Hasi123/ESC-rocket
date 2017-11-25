# ESC-rocket
I converted an ESC to fire two rockers off an RC airplane using a piece of resistance wire.

The way it works is like so:
The rockets are ignited using a piece of resistance wire.
You can fire (as of now) 2 rockets with one ESC.
The ESC has 3 outputs: A, B and C (where usually the motors would go)
Solder one side of the first wire to port A of the ESC and the other side to port B
The second wire (optional) goes to ports B and C
If the input (servo) signal is between 1600 and 1800us (usually 20-60% on the TX) the first side gets ignited
If the input is between 1800 and 2200us (60-100%) the second side gets ignited
The current is limited using a PWM signal (you can configure this using the pwm_cycle variable, line 16 in the code)
That's basically it. The code has a failsafe integrated, so it turnes the outputs off if no or a false signal is received)

How to use the code:
1) Get an Atmel ESC from this list: https://docs.google.com/spreadsheets/d/13tMlu5ldLNpZXwbe6UhDHJhcgTVuljm8HDiDp9WO9Pk/edit?usp=sharing
2) Note FW file (eg. tgy.hex) and the FETs (P/N or N).
3) Open the .inc file, that corresponds to your FW file here: https://github.com/sim-/tgy
4) Open my code and change the pins to suit your ESC (lines 5-12) using the file from step 3
5) IMPORTANT: If your ESC has all N FETs set the variable nFet to 1
   If it is P/N set it to 0
6) Open Boards Manager in Arduino and install MiniCore
7) Select the board ATMega8 with the correct settings and now you can compile the code
8) Upload it to the ESC using an USBASP or any other ISP programmer.
9) Test it (ideally with a current limited power supply)