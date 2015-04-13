


## 0.2.8

### Features

- [Brewpi Spark] New Startup screen! with tap anywhere to recalibrate touch.
- [Brewpi Spark] New Controller screen! view temperatures, mode and state. Watch these change as you make changes in the web UI. 
- [BrewPi Spark] Controller now automatically switches between the controller screen and the test screen based on mode. If you set it to test mode via the web UI (under advanced settings), the test screen will be shown. Otherwise, the main controller screen.

### Enhancements

- Heater is now active together with the light when Light as Heater is selected.
- Arduino supported with this release.
- Removed 'T' command (calibrate) that was causing issues.
- Default beer and fridge setting is now --.- (disabled). This aligns with the default mode OFF.


### Under the Hood

- Better temperature sensor initialization and disconnect detection (faster and more reliable)
- Increased OneWire reliability
- [Arduino] OneWire class refactored
- [Arduino] Updated Arduino dependencies. This should increase Serial reliability on the Leonardo and fix dropped characters
- Fixed bug that timer was shown in OFF mode
- Use git describe to generate build name

