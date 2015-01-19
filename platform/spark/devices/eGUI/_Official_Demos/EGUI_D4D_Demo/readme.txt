Freescale eGUI EGUI_D4D_Demo projects

Directories:
	- common_source: contains all common source files for graphic screens
	- mqx_demo: the system files of MQX application (tasks, etc...)
	- screen_shot: caontais sreen shots of demo graphic screen
	- TWRLCD_MCF51JM128: project for standalone TWR-LCD board and MCF51JM128 onboard MCU over SPI driver.		
	- TWR_K60D100M: project for TWR-K60 Kinetis board  and TWR-LCD 320x240 module over FlexBus driver.
	


TWR-LCD module settings:
	- touch screen through elevator: SW5 must have sets all switches to ON.
	- LCD SPI through elevator: switch are set up with in follow sequence "01111110".
	- LCD flexbus 16 bits through elevator: switch are set up with in follow sequence "10111110".
	- LCD SPI with onboard MCF51JM128: switch are set up with in follow sequence "01001110".


Tips & Tricks:
	- for TWR-LCD module: If you want to run project from TOWER (not from onboard MCF51JM128) be sure to load into onboard MCF51JM128 any empty project 
	to avoid any conflist on shared LCD signals.
