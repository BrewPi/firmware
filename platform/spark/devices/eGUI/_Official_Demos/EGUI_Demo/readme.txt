Freescale eGUI EGUI_Demo projects

This demo is showing external resources eGUI capability in demo that simulate standard forms. The demo is also showing capability of Keyboard & mouse handling in eGUI, 
both these device can be connected over serial card UAB connector.

Directories:
	- common_source: contains all common source files for application
	- SD_Card_Content: The basic contntent of SD card to run the demo correctly.
	- screen_shot: caontais sreen shots of demo graphic screen
	- TWR_K70: project for TWR-K70 Kinetis board  and TWR-LCDRGB 480x272 module over LCDC driver.

TWR-SER card settings:
	The card has to be setup to provide UAB host capability: J10: 1-2; J16: 1-2.

TWR-K70F120 card settings: 
	Use default setting of card

TWR-LCDRGB: 
	The demo supports revision B and newer revisions of this card.

The MQX Settings:
	The only two changes in MQX configuration is enabled Time Slice and Disabled ROM vectors:
	
	#define MQX_HAS_TIME_SLICE              1
	#define MQX_ROM_VECTORS                 0