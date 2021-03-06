# Soda Machine
This final project for CS120B at UCR was programmed in C with Atmel Studio 7.0.  
YouTube Link: [DIY Soda Machine](https://www.youtube.com/watch?v=1zI3h4PBV9M)

### Major Components
  * **ATmega1284:** *Microcontroller used to control the entire project.*
  * **Nokia5110 LCD:** *84x48 matrix LCD display used as a visual interface.*
  * **DIMINUS Mini Air Pump:** *6V air pump that is used to displace the beverage via air pressure.*
  * **Parallax 2-Axis Joystick:** *Joystick used to make selections.*
  * **2N3904 Transistor:** *NPN BJT used as an electronic switch.*
  
### Project Description
   This soda machine offers the user 3 beverages to select from to dispense. The dispensing mechanism functions by utilizing the DIMINUS air pumps and positive pressure to displace the beverage from the container. In order to dispense the proper beverage, the user is prompted via the Nokia5110 to make a selection with the joystick and then hold down the dispense button for as long as they wish. The ATmega1284 takes in these conditions and with the logic provided by the code, sends a signal to one of the three 2N3909 NPN transistors. Each transistor is in series with its respective air pump and the signal from the ATmega activates the transistor, thus closing the switch and completing the circuit. The user can switch between the beverages until the supply runs out.

###### Original Work Statement
*I acknowlegde all content contained herin, excluding template or example code, is my own original work.*
