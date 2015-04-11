


## 0.2.8

### Features

- [Brewpi Spark] New Startup screen! with tap anywhere to recalibrate
- [Brewpi Spark] New Controller screen! view temperatures, mode and state. Watch these change as you make changes in the web UI. 

### Enhancements


- Heater is now active together with the light when Light as Heater is selected.
- Arduino supported with this release
- Removed 'T' command (calibrate) that was causing issues


### Under the Hood

- Better temperature sensor initialization and disconnect detection (faster and more reliable)
- Increased OneWire reliability
- [Arduino] OneWire class refactored
- [Arduino] Updated Arduino dependencies. This should increase Serial reliability on the Leonardo and fix dropped characters

