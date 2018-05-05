## 0.8.0-rc.3

### ENHANCEMENTS

- [Photon/P1] A number of networking-related fixes v2 [#1500](https://github.com/particle-iot/firmware/pull/1500)
- [Photon/P1] Enables support for SHA384/SHA512 certificates for WPA Enterprise [#1501](https://github.com/particle-iot/firmware/pull/1501)

### BUGFIXES

- [Photon/P1] A number of networking-related fixes [#1492](https://github.com/particle-iot/firmware/pull/1492)
- [Photon/P1] A number of networking-related fixes v2 [#1500](https://github.com/particle-iot/firmware/pull/1500)
- [Photon/Electron] WKP pin needs to be disabled as a wakeup source on boot to allow its normal operation [#1496](https://github.com/particle-iot/firmware/pull/1496)
- [Photon/P1/Electron] Recursive logging freezes the application thread while the LogHandler is trying to acquire a lock on the resource [#1517](https://github.com/particle-iot/firmware/pull/1517)

## 0.8.0-rc.2

### FEATURES

- USB request handlers [#1444](https://github.com/particle-iot/firmware/pull/1444)
- Out of heap system event and heap fragmentation detection [#1452](https://github.com/particle-iot/firmware/pull/1452)
- Network and cloud diagnostics [#1424](https://github.com/particle-iot/firmware/pull/1424)
- [Photon/P1] TCPClient: non-blocking, blocking, blocking with timeout writes support [#1485](https://github.com/particle-iot/firmware/pull/1485)
- [Electron] adds UPSV handling to cellular_hal [#1480](https://github.com/particle-iot/firmware/pull/1480)

### ENHANCEMENTS

- Serialize access to the CRC peripheral (STM32F2xx) [#1465](https://github.com/particle-iot/firmware/pull/1465)
- System.sleep() wake up reason [#1410](https://github.com/particle-iot/firmware/pull/1410)
- System.sleep(): support for multiple wake up pins [#1405](https://github.com/particle-iot/firmware/pull/1405)
- Disable WKP pin waking device up from `SLEEP_MODE_DEEP` [#1409](https://github.com/particle-iot/firmware/pull/1409)
- [Photon/P1] Increase maximum supported number of simultaneously active TCP connections [#1350](https://github.com/particle-iot/firmware/pull/1350)
- [Photon/P1] WiFi.dnsServerIP()/WiFi.dhcpServerIP() support [#1386](https://github.com/particle-iot/firmware/pull/1386)
- Generalize FuelGauge to also use alternative I2C interfaces. [#1443](https://github.com/particle-iot/firmware/pull/1443)
- Firmware update and access to internal flash via USB requests [#1456](https://github.com/particle-iot/firmware/pull/1456)
- Added docs on local build setup [#1374](https://github.com/particle-iot/firmware/pull/1374)
- Use 'using std::**' instead of define * std::* [#1258](https://github.com/particle-iot/firmware/pull/1258)
- Only remake $(TARGET_BASE).elf el al. if necessary [#1223](https://github.com/particle-iot/firmware/pull/1223)

### BUGFIXES

- [Electron] Fix heap bounds build for system part1 [#1478](https://github.com/particle-iot/firmware/pull/1478)
- [Electron] Guard cellular_command() with a global lock [#1415](https://github.com/particle-iot/firmware/pull/1415)
- [Electron] Fix caching of the description CRCs in the backup RAM [#1413](https://github.com/particle-iot/firmware/pull/1413)
- [Electron] connect_cancel() fix [#1464](https://github.com/particle-iot/firmware/pull/1464)
- [Electron] DCD fixes [#1454](https://github.com/particle-iot/firmware/pull/1454)
- [Electron] moves some newlib functions into part1 [#1471](https://github.com/particle-iot/firmware/pull/1471)
- [Core] Fixes I2C slave mode [#1309](https://github.com/particle-iot/firmware/pull/1309)
- [Virtual] Fixes virtual device running with UDP protocol [#1462](https://github.com/particle-iot/firmware/pull/1462)
- Fix usage of an incorrect prerequisite name in program-* targets [#1463](https://github.com/particle-iot/firmware/pull/1463)
- Fixed shadowing of write(const unint_8_t`*`, sizte_t) in USBKeyboard [#1372](https://github.com/particle-iot/firmware/pull/1372)

### INTERNAL

- Fixes some 0.8.0-rc.2 tests [#1476](https://github.com/particle-iot/firmware/pull/1476)
- fixes the unit test build [#1474](https://github.com/particle-iot/firmware/pull/1474)
- IS_CLAIMED request fixes [#1472](https://github.com/particle-iot/firmware/pull/1472)
- Documents low level USB request completion notifications [#1475](https://github.com/particle-iot/firmware/pull/1475)
- [Electron] Flash size optimizations [#1469](https://github.com/particle-iot/firmware/pull/1469)
- Minor refactoring of the USB protocol implementation [#1473](https://github.com/particle-iot/firmware/pull/1473)

## 0.8.0-rc.1

## FEATURES

- Low-latency interrupt handlers [#1394](https://github.com/spark/firmware/pull/1394)
- [Electron] Reworked power management [#1412](https://github.com/spark/firmware/pull/1412)
- Battery State diagnostics [#1398](https://github.com/spark/firmware/pull/1398)
- Battery charge diagnostics [#1395](https://github.com/spark/firmware/pull/1395)
- RAM usage diagnostic sources data [#1411](https://github.com/spark/firmware/pull/1411)
- Network Signal Quality/Strength rework and diagnostics [#1423](https://github.com/spark/firmware/pull/1423)
- System uptime diagnoatics [#1393](https://github.com/spark/firmware/pull/1393)
- Diagnostics for unacked messages and rate limited events [#1391](https://github.com/spark/firmware/pull/1391)
- Network and Cloud diagnostics [#1424](https://github.com/spark/firmware/pull/1424)
- Diagnostics service [#1390](https://github.com/spark/firmware/pull/1390)

## ENHANCEMENTS

- [Photon/P1] Wi-FI firmware compression [#1421](https://github.com/spark/firmware/pull/1421)
- [Photon/P1] Moves Wi-Fi tester into application module [#1378](https://github.com/spark/firmware/pull/1378)

## BUGFIXES

- Cloud random seed not working [#1312](https://github.com/spark/firmware/issues/1312)
- [Electron] Error handling in the data usage API [#1435](https://github.com/spark/firmware/pull/1435)

## 0.7.0 (see additional changelog 0.7.0-rc.1 ~ 0.7.0-rc.7)

### BUGFIX

- [Photon/Electron] WKP pin needs to be disabled as a wakeup source on boot to allow its normal operation [#1496](https://github.com/particle-iot/firmware/pull/1496)

## 0.7.0-rc.7

### BUGFIX

- [Photon] Regression with SoftAP and URL-encoded form query [#1432](https://github.com/spark/firmware/issues/1432)
- Particle.connect() hard blocking since 0.6.1-rc.1 [#1399](https://github.com/spark/firmware/issues/1399)
- [Electron] Cellular resolve does not return 0 / false when it receives bad DNS resolution related to bad cell service [#1304](https://github.com/spark/firmware/issues/1304)
- [Core] Use the device ID as the USB serial number [#1367](https://github.com/spark/firmware/issues/1367)
- [Electron] Fix heap bounds for system part 1 [#1478](https://github.com/particle-iot/firmware/pull/1478)
- [Electron] connect_cancel() fix [#1464](https://github.com/particle-iot/firmware/pull/1464)
- Fixed shadowing of `write(const unint_8_t*, sizte_t)` in USBKeyboard [#1372](https://github.com/particle-iot/firmware/pull/1372)

## 0.7.0-rc.6

### BUGFIX

- [Electron] Add dependency in system-part-1 on 0.6.4 system-part-3 to prevent upgrades from 0.6.3 or earlier to avoid incompatibilties
with these releases.

## 0.7.0-rc.5

### BUGFIX

 - The device ID is output in lowercase in DFU mode. [#1432](https://github.com/spark/firmware/issues/1432)
 - increase the DTLS buffer from 768 to 800 bytes, so that the system describe message is sent.
 - remove rigid dependency check in bootloader that was causing DCT functions to not load in 0.8.0-rc.1 [#1436](https://github.com/spark/firmware/pull/1436)

## 0.7.0-rc.4

### ENHANCEMENTS

 - USART Half-duplex enhancements [#1308](https://github.com/spark/firmware/pull/1380)

### BUGFIX

 - KRACK WPA2 security bugfix [#1420](https://github.com/spark/firmware/pull/1420)
 - Monolithic build linker error [#1370](https://github.com/spark/firmware/pull/1370)
 - 4-digit serial numbers had additional characters [#1380](https://github.com/spark/firmware/pull/1380)


## 0.7.0-rc.3

### DEPRECATED API

[`[PR #1365]`](https://github.com/spark/firmware/pull/1365) Beginning with 0.8.0 release, `Particle.publish()` and `Particle.subscribe()` methods will require event scope to be specified explicitly. Please update your apps now to include the event scope to avoid compilation errors in >=0.8.0.

### BUGFIX

[`[PR #1362]`](https://github.com/spark/firmware/pull/1362) [`[Fixes #1360]`](https://github.com/spark/firmware/issues/1360) Fixed SoftAP HTTP usage hard faulting in 0.7.0-rc.1 and 0.7.0-rc.2


## 0.7.0-rc.2

### ENHANCEMENTS

- [`[PR #1357]`](https://github.com/spark/firmware/pull/1357) Expands the device code from 4 digits to 6 digits for Photon/P1/Electron platforms

### BUGFIX

- [`[PR #1346]`](https://github.com/spark/firmware/pull/1346) [`[Fixes #1344]`](https://github.com/spark/firmware/issues/1344) `[Photon/P1]` When using `SYSTEM_THREAD(ENABLED)` the TCPServer and WPA Enterprise connections were broken.
- [`[PR #1354]`](https://github.com/spark/firmware/pull/1354) [`[Fixes #1062]`](https://github.com/spark/firmware/issues/1062) A call to `WiFi.scan()` when Wi-Fi module is off or not ready was resulting in a hard fault.
- [`[PR #1357]`](https://github.com/spark/firmware/pull/1357) [`[Fixes #1348]`](https://github.com/spark/firmware/issues/1348) SoftAP SSID was not respecting the string's null terminator, 2 char SSID would appear as 4.
- [`[PR #1355]`](https://github.com/spark/firmware/pull/1355) When using WPA Enterprise access point and constantly reconnecting to it, heap was becoming fragmented which resulted in inability to connect to the access point anymore. Also reduced overall heap usage.

### INTERNAL

- [`[PR #1342]`](https://github.com/spark/firmware/pull/1342) Removed the `firmware-docs` subtree from the `firmware` repo.  Docs updates are made directly to `docs` repo again.
- [`[PR #1352]`](https://github.com/spark/firmware/pull/1352) Added test for `RGB.onChange()` handler leak
- [`[PR #1358]`](https://github.com/spark/firmware/pull/1358) Updates minimal ARM gcc version required to 5.3.1
- [`[PR #1359]`](https://github.com/spark/firmware/pull/1359) Fixes build with `PLATFORM=gcc` on OSX with clang's gcc wrapper


## 0.7.0-rc.1

### FEATURES

- [`[PR #1245]`](https://github.com/spark/firmware/pull/1245) `Particle.publish()` now able to use Future API
- [`[PR #1289]`](https://github.com/spark/firmware/pull/1289) [`[Implements #914]`](https://github.com/spark/firmware/issues/914) `[Photon/P1]` WPA/WPA2 Enterprise support added (PEAP/MSCHAPv2 and EAP-TLS)! [Photon/P1] Automatic cipher/security detection when configuring WiFi settings over Serial.

### ENHANCEMENTS

- [`[PR #1242]`](https://github.com/spark/firmware/pull/1242) `[Photon/P1/Electron]` DFU transfer speeds increased! v100 bootloader is now 41% faster than v7 and 60% faster than the latest v11.
- [`[PR #1236]`](https://github.com/spark/firmware/pull/1236) [`[Fixes #1201]`](https://github.com/spark/firmware/issues/1201) [`[Fixes #1194]`](https://github.com/spark/firmware/issues/1194) Added type-safe wrapper for enum-based flags for `Particle.publish()` which enables logical OR'ed flag combinations `PRIVATE | WITH_ACK`
- [`[PR #1247]`](https://github.com/spark/firmware/pull/1247) Adds error checking to `WiFi.setCredentials()`, will return `true` if credentials has been stored successfully, or `false` otherwise.
- [`[PR #1248]`](https://github.com/spark/firmware/pull/1248) Added an overload to `map()` function that takes `double` arguments.
- [`[PR #1296]`](https://github.com/spark/firmware/pull/1296) `[Photon/P1]` Added support for setting a custom DNS hostname, default is device ID.
- [`[PR #1260]`](https://github.com/spark/firmware/pull/1260) [`[Implements #1067]`](https://github.com/spark/firmware/issues/1067) Adds ability to interrupt the blinking cyan cloud connection with the SETUP/MODE button.
- [`[PR #1271]`](https://github.com/spark/firmware/pull/1271) [`[Implements #1180]`](https://github.com/spark/firmware/issues/1180) `[Photon/P1]` Constrains `WiFi.RSSI()` to -1dBm max.
- [`[PR #1270]`](https://github.com/spark/firmware/pull/1270) Removes `spark/device/ota_result` event and instead sends OTA'd module info as a payload in UpdateDone message, or as an ACK to UpdateDone.
- [`[PR #1300]`](https://github.com/spark/firmware/pull/1300) Restores public server key and server address if missing
- [`[PR #1325]`](https://github.com/spark/firmware/pull/1325) Use backup registers instead of DCT to store system flags to avoid chance of a DCT corruption.
- [`[PR #1306]`](https://github.com/spark/firmware/pull/1306) Bootloader module dependency and integrity checks have been added to system-part2.  If they fail, the device is forced into safe mode and a new bootloader will be OTA transferred to the device.
- [`[PR #1329]`](https://github.com/spark/firmware/pull/1329) Adds a verification and retry scheme to the bootloader flashing routine.
- [`[PR #1330]`](https://github.com/spark/firmware/pull/1330) `[Electron]` Added CRC checking to the Electron DCD implementation so that write errors are detected. Added a critical section around flash operations and around DCD operations to make them thread safe.
- [`[PR #1307]`](https://github.com/spark/firmware/pull/1307) `[Photon/P1]` New version of WICED adds CRC checking to the DCT implementation so that write errors are detected. Added a critical section around flash operations and around DCT operations to make them thread safe.
- [`[PR #1269]`](https://github.com/spark/firmware/pull/1269) [`[Closes #1165]`](https://github.com/spark/firmware/issues/1165) Cloud connection can be closed gracefully allowing confirmable messages to reach the cloud before the connection is terminated

### BUGFIX

- [`[PR #1246]`](https://github.com/spark/firmware/pull/1246) Fixes possible corruption of event data in multi-threaded firmware
- [`[PR #1234]`](https://github.com/spark/firmware/pull/1234) [`[Fixes #1139]`](https://github.com/spark/firmware/issues/1139) `[Electron]` `spark/hardware/max_binary` event was sent in error, adding 69 more bytes of data to handshake (full or session resume). Also fixes other preprocessor errors.
- [`[PR #1236]`](https://github.com/spark/firmware/pull/1236) [`[Fixes #1201]`](https://github.com/spark/firmware/issues/1201) [`[Fixes #1194]`](https://github.com/spark/firmware/issues/1194) Sanitized `Particle.publish()` overloads.
- [`[PR #1237]`](https://github.com/spark/firmware/pull/1237) Fixes potential memory leak and race condition issues in `RGB.onChange()` function.
- [`[PR #1247]`](https://github.com/spark/firmware/pull/1247) Previously no null pointer checks on password argument of `WiFi.setCredentials()`.
- [`[PR #1248]`](https://github.com/spark/firmware/pull/1248) [`[Fixes #1193]`](https://github.com/spark/firmware/issues/1193) Fixes divide by zero on incorrect parameters of `map()` function.
- [`[PR #1254]`](https://github.com/spark/firmware/pull/1254) [`[Fixes #1241]`](https://github.com/spark/firmware/issues/1241) `WiFi.connecting()` was returning `false` while DHCP is resolving, will now remain `true`.
- [`[PR #1296]`](https://github.com/spark/firmware/pull/1296) [`[Fixes #1251]`](https://github.com/spark/firmware/issues/1251) `[Photon/P1]` Default Wi-Fi DNS hostname changed to device ID, to avoid spaces in name which may cause issues.
- [`[PR #1255]`](https://github.com/spark/firmware/pull/1255) [`[Fixes #1136]`](https://github.com/spark/firmware/issues/1136) `[Core]` Interrupts were disabled by default.
- [`[PR #1259]`](https://github.com/spark/firmware/pull/1259) [`[Fixes #1176]`](https://github.com/spark/firmware/issues/1176)  Makes `System.sleep(mode, seconds)` a synchronous operation in multithreaded firmware. This ensures the device is in a well-defined state before entering sleep mode.
- [`[PR #1315]`](https://github.com/spark/firmware/pull/1315) Fixes Particle Publish flag implicit conversion issue. e.g. `Particle.publish("event", "data", NO_ACK);` was previously changing event's TTL instead disabling acknowledgement of the event)
- [`[PR #1316]`](https://github.com/spark/firmware/pull/1316) Fixes LED indication when network credentials are cleared by holding the SETUP button for >10 seconds.
- [`[PR #1270]`](https://github.com/spark/firmware/pull/1270) [`[Fixes #1240]`](https://github.com/spark/firmware/issues/1240) TCP Firmware will not ACK every chunk in Fast OTA mode now.
- [`[PR #1302]`](https://github.com/spark/firmware/pull/1302) [`[Fixes #1282]`](https://github.com/spark/firmware/issues/1282) `[Electron]` `Wire1` was not working correctly.
- [`[PR #1326]`](https://github.com/spark/firmware/pull/1326) Renamed `system_error` enum to `system_error_t` to avoid conflicts with `std::system_error` class.
- [`[PR #1286]`](https://github.com/spark/firmware/pull/1286) Improves stability of TCP server implementation: 1) Update server's list of clients on a client destruction (thanks @tlangmo!), 2) TCPClient now closes underlying socket on destruction.
- [`[PR #1327]`](https://github.com/spark/firmware/pull/1327) [`[Fixes #1098]`](https://github.com/spark/firmware/issues/1098) [Photon/P1] Previously, when entering Sleep-stop mode: `System.sleep(D1, RISING, 60);` while in the process of making a Wi-Fi connection resulted in some parts of the radio still being initialized, consuming about 10-15mA more than normal.
- [`[PR #1336]`](https://github.com/spark/firmware/pull/1336) Fixes an issue with Serial when receiving consecutive multiple 64-byte transmissions from Host
- [`[PR #1337]`](https://github.com/spark/firmware/pull/1337) Fixed system attempting to enter listening mode every 1ms when the SETUP button is pressed.
- [`[PR #1289]`](https://github.com/spark/firmware/pull/1289) Fixes a stack overlap with system-part2 static RAM on Photon/P1
- [`[PR #1289]`](https://github.com/spark/firmware/pull/1289) Fixes a memory leak when Thread is terminated
- [`[PR #1289]`](https://github.com/spark/firmware/pull/1289) Fixes a deadlock in SoftAP, when connection is terminated prematurely
- [`[PR #1340]`](https://github.com/spark/firmware/pull/1340) `[Electron]` Fixes the monolithic build

### INTERNAL

- [`[PR #1313]`](https://github.com/spark/firmware/pull/1313) Compilation fixes for GCC platform
- [`[PR #1323]`](https://github.com/spark/firmware/pull/1323) USB vendor requests should be executed on system thread instead of being processed in ISR.
- [`[PR #1338]`](https://github.com/spark/firmware/pull/1338) Do not read or write feature flags from an ISR

## 0.6.4

### BUGFIXES

- Downgrade bootloader functionality in 0.6.3 would enter an infinite loop after flashing system part 1 0.7.0-rc.X using OTA/serial. `particle flash --usb`/DFU was not affected.

## 0.6.3

### ENHANCEMENTS

- Downgrade bootloader when downgrading from 0.7.0 or newer. [#1416](https://github.com/spark/firmware/pull/1416)

### BUGFIXES

- [KRACK WPA2 security fix](https://github.com/spark/firmware/pull/1419)

## 0.6.2 (same as 0.6.2-rc.2)

### FEATURES

- [[PR #1311]](https://github.com/spark/firmware/pull/1311) `[Implements CH1537] [Electron]` Added support for Twilio SIMs by default in system firmware.

### BUG FIX

- [[PR #1310]](https://github.com/spark/firmware/pull/1310) Fixes a error when `<algorithm>` has already been included before the `math.h` header. Now we only include `math.h` when Arduino compatibility is requested. (math.h was not included in 0.6.0).

## 0.6.2-rc.1

### ENHANCEMENT / BUG FIX

- [[PR #1283]](https://github.com/spark/firmware/pull/1283) [[Implements #1278]](https://github.com/spark/firmware/issues/1278) Restores 0.6.0-style Arduino compatibility by default, full Arduino compatibility when including Arduino.h

## 0.6.1 (same as 0.6.1-rc.2)

### FEATURES

- [[PR #1225]](https://github.com/spark/firmware/pull/1225) [Photon/P1/Electron] Added support for custom LED colors in bootloader v11 (Safe Mode, DFU Mode, Firmware Reset).
- [[PR #1227]](https://github.com/spark/firmware/pull/1227) [[Implements #961]](https://github.com/spark/firmware/issues/961) [Electron] Added new API for hostname IP address lookup `IPAddress ip = Cellular.resolve(hostname)`

### ENHANCEMENTS

- [[PR #1216]](https://github.com/spark/firmware/pull/1216) Improved Arduino Compatibility (now supported by default, added PARTICLE_NO_ARDUINO_COMPATIBILITY=y command line option for disabling)
- [[PR #1217]](https://github.com/spark/firmware/pull/1217) Added Windows, Mac command, & Unix/Linux meta USB keyboard scancode definitions.
- [[PR #1224]](https://github.com/spark/firmware/pull/1224) Allow the compiler to garbage collect USBKeyboard and USBMouse implementations if they are not used in user code, saving flash space.
- [[PR #1225]](https://github.com/spark/firmware/pull/1225) [Photon/P1/Electron] Combined `LEDStatus` and `LEDCustomStatus` into a single class -> `LEDStatus`

### BUG FIXES

- [[PR #1221]](https://github.com/spark/firmware/pull/1221) [[Fixes #1220]](https://github.com/spark/firmware/issues/1220) [Electron] TIM8 PWM pins (B0, B1) did not work correctly in bootloader with `RGB.mirrorTo()`
- [[PR #1222]](https://github.com/spark/firmware/pull/1222) Fixed bug in `String(const char* str, int len)` constructor when the string is longer than the specified length.
- [[PR #1225]](https://github.com/spark/firmware/pull/1225) [Photon/P1/Electron] Fixed LED indication shown during device key generation (blinking white) introduced in 0.6.1-rc.1
- [[PR #1226]](https://github.com/spark/firmware/pull/1226) [[Fixes #1181]](https://github.com/spark/firmware/issues/1181) [Photon/P1/Core] Process TCP `DESCRIBE` properly and return only one response, SYSTEM, APPLICATION, or COMBINED (ALL) describe message.  Was sending separate SYSTEM and APPLICATION previously.
- [[PR #1230]](https://github.com/spark/firmware/pull/1230) Safe Mode event was being published unconditionally introduced in 0.6.1-rc.1
- [[PR #1231]](https://github.com/spark/firmware/pull/1231) [Electron] fixes double newline parser issue on G350 introduced in 428835a 0.6.1-rc.1



## 0.6.1-rc.1

### FEATURES

- [[PR #1190]](https://github.com/spark/firmware/pull/1190) [[Implements #1114]](https://github.com/spark/firmware/issues/1114) Added ability to mirror MODE/SETUP button to any GPIO, available from time of boot, active high or low.
- [[PR #1182]](https://github.com/spark/firmware/pull/1182) [[Fixes #687]](https://github.com/spark/firmware/issues/687) [[Docs]](https://prerelease-docs.particle.io/reference/firmware/electron/#setlistentimeout-) Added `WiFi.set|getListenTimeout()` | `Cellular.set|getListenTimeout()` to override the automatic new Listening Mode timeout (Wi-Fi = no timeout by default, Cellular = 5 minute timeout by default).
- [[PR #1154]](https://github.com/spark/firmware/pull/1154) Added `low_battery` system event, which is generated when low battery condition is detected. This is when the battery falls below the SoC threshold (default 10%, max settable 32%).  The event can only be generated again if the system goes from a non-charging to charing state after the event is generated. The event doesn't carry any data.
- [[PR #1144]](https://github.com/spark/firmware/pull/1144) Added tracking of ACKs for published events (see `WITH_ACK` flag for `Particle.publish()`)
- [[PR #1135]](https://github.com/spark/firmware/pull/1135) [[Fixes #1116]](https://github.com/spark/firmware/issues/1116) [[Fixes #965]](https://github.com/spark/firmware/issues/965) New Time API's! `Time.isValid()` | `Particle.syncTimePending()` | `Particle.syncTimeDone()` | `Particle.timeSyncedLast()`
- [[PR #1127]](https://github.com/spark/firmware/pull/1127) [[PR #1213]](https://github.com/spark/firmware/pull/1213) Added support for runtime logging configuration, which allows to enable logging on already running system via USB control requests. Disabled by default to save flash memory space. (note: this feature is not fully baked with tool support)
- [[PR #1120]](https://github.com/spark/firmware/pull/1120) [[Implements #1059]](https://github.com/spark/firmware/issues/1059) [P1] Added extra spare pin to P1 (P1S6) with GPIO and PWM support.
- [[PR #1204]](https://github.com/spark/firmware/pull/1204) [[Implements #1113]](https://github.com/spark/firmware/issues/1113) RGB LED pins can be mirrored to other PWM capable pins via `RGB.mirrorTo()`. Common Anode/Cathode LED and Bootloader compatible. See PR for usage.
- [[PR #1205]](https://github.com/spark/firmware/pull/1205) [[Closes #569]](https://github.com/spark/firmware/issues/569) [[Closes #976]](https://github.com/spark/firmware/issues/976) [[Closes #1111]](https://github.com/spark/firmware/issues/1111) By implementing a centralized LED service and theme "engine" for system LED signaling, giving users the ability to apply custom LED colors and patterns for system events.

### ENHANCEMENTS

- [[PR #1191]](https://github.com/spark/firmware/pull/1191) Added more Arduino Library compatibility
- [[PR #1188]](https://github.com/spark/firmware/pull/1188) [[Implements #1152]](https://github.com/spark/firmware/issues/1152) Added SPI API's: `SPISettings` | `SPI.beginTransaction()` | `SPI.endTransaction()`
- [[PR #1169]](https://github.com/spark/firmware/pull/1169) Updated system communication logging with new logging API
- [[PR #1160]](https://github.com/spark/firmware/pull/1160) [Electron] Modem USART paused via HW_FLOW_CONTROL (RTS) before going into sleep with SLEEP_NETWORK_STANDBY. Receives and buffers small messages while system sleeping.
- [[PR #1159]](https://github.com/spark/firmware/pull/1159) [[Closes #1085]](https://github.com/spark/firmware/issues/1085) [[Closes #1054]](https://github.com/spark/firmware/issues/1054) Added support for GCC 5.4.x
- [[PR #1151]](https://github.com/spark/firmware/pull/1151) [[Closes #977]](https://github.com/spark/firmware/issues/977) Added System events for cloud/network connection state changes
- [[PR #1122]](https://github.com/spark/firmware/pull/1122) Attach to host even if Serial, USBSerial1 and Keyboard/Mouse are disabled, so that "Control Interface" that receives vendor requests is still accessible.
- [[PR #1097]](https://github.com/spark/firmware/pull/1097) [[Implements #1032]](https://github.com/spark/firmware/issues/1032) When flashing (OTA/YModem) an invalid firmware binary (that the device ignores) it will post an event describing why the binary was not applied.
- [[PR #1203]](https://github.com/spark/firmware/pull/1203) [[PR #1212]](https://github.com/spark/firmware/pull/1212) Automatic bootloader updates have returned to the Electron.  v9 bootloader has been added to firmware release >=0.6.1-rc.1 for Photon/P1/Electron.  After updating your system firmware, a new v9 bootloader will be applied to your device if required.  v9 includes support for SETUP/MODE button and RGB LED mirroring at the bootloader level of operation.  Also included are updates to USB DFU mode so that Windows users do not need to install separate drivers via Zadig.  Bootloader GREEN and WHITE LED flashing speeds (Firmware Reset modes) are faster now as well (you won't see these unless you have loaded user firmware to the Backup location).
- [[PR #1125]](https://github.com/spark/firmware/pull/1125) Breaks on-going network connection when Sleep stop mode is called, thereby speeding up the time to entering sleep when using SYSTEM_THREAD(ENABLED).

### BUGFIX

- [[PR #1186]](https://github.com/spark/firmware/pull/1186) Fixed issue where USB `Serial` might deadlock when interrupts are disabled while using `DEBUG_BUILD=y`
- [[PR #1179]](https://github.com/spark/firmware/pull/1179) [[Fixes #1178]](https://github.com/spark/firmware/issues/1178) [[Fixes #1060]](https://github.com/spark/firmware/issues/1160) [Electron] Bootloader build was failing, fixed and added to CI.
- [[PR #1158]](https://github.com/spark/firmware/pull/1158) [[Fixes #1133]](https://github.com/spark/firmware/issues/1133) [Electron] Before sleeping, now waits for server sent confirmable messages to be acknowledged, in addition to previous behavior of device generated confirmable messages being acknowledged.  Reduces data usage.
- [[PR #1156]](https://github.com/spark/firmware/pull/1156) [[Fixes #1155]](https://github.com/spark/firmware/pull/1156) System.sleep(30) wasn't reapplying power to the network device after set time.
- [[PR #1147]](https://github.com/spark/firmware/pull/1147) [Electron] Fixed approx. -0.1V offset on FuelGauge().getVCell() readings
- [[PR #1145]](https://github.com/spark/firmware/pull/1145) [[Fixes #973]](https://github.com/spark/firmware/issues/973) `Particle.connect()` now blocks `loop()` from running until `Particle.connected()` is `true` in single threaded SEMI_AUTOMATIC mode.
- [[PR #1140]](https://github.com/spark/firmware/pull/1140) [[Fixes #1138]](https://github.com/spark/firmware/issues/1138) [[Fixes #1104]](https://github.com/spark/firmware/issues/1104) [Electron] Fixed modem USART and buffer handling
- [[PR #1130]](https://github.com/spark/firmware/pull/1130) Particle.subscribe() used with same events but changing scope between PUBLIC and PRIVATE or vice versa would potentially result in non-registered subscriptions.  This was also crashing the GCC virtual device with a segfault when subscription checksums were calculated.

### INTERNAL

- [[PR #1196]](https://github.com/spark/firmware/pull/1196) Re-enable GNU extensions for libc globally. Fixes build with ARM GCC 4.9.3 Q1.
- [[PR #1189]](https://github.com/spark/firmware/pull/1189) Typo caused a warning during compilation in wiring/no_fixture Cellular tests.
- [[PR #1184]](https://github.com/spark/firmware/pull/1184) [Electron] moved cellular HAL and its direct dependencies from module 2 to module 3 to free up space (this is system-part3 was reduced in size, while system-part1 was increased)
- [[PR #1167]](https://github.com/spark/firmware/pull/1167) [[Fixes #1036]](https://github.com/spark/firmware/issues/1036) [GCC Virtual Device] workaround for 100% CPU usage problem.
- [[PR #1146]](https://github.com/spark/firmware/pull/1146) [[Closes #1040]](https://github.com/spark/firmware/issues/1040) Added asserts for checking that network calls are run on system thread.
- [[PR #1134]](https://github.com/spark/firmware/pull/1134) [GCC Virtual Device] Error in socket_hal's socket_receive() logic caused random cloud connection errors.


## v0.6.0 (same as v0.6.0-rc.2)

### ENHANCEMENTS

- USB HID enhancements, please see PR: [#1110](https://github.com/spark/firmware/pull/1110) for a list. Closes [#1096](https://github.com/spark/firmware/issues/1096)

### BUGFIX

- Consecutive HID reports were overwriting previous the report before it was delivered to the host. Fixes [#1090](https://github.com/spark/firmware/issues/1090).
- Disabling multiple USB configurations (normal/high power) as this breaks composite driver on Windows. Fixes [#1089](https://github.com/spark/firmware/issues/1089) Serial and USBSerial1 not working at same time on Windows 8.1 Pro.
- Do not run the event loop from delay() when threading is enabled. Fixes [#1055](https://github.com/spark/firmware/issues/1055)
- Cancel current connection attempt before entering the listening mode with WiFi.listen(true) and also WiFi.off(). Fixes [#1013](https://github.com/spark/firmware/issues/1013)

### INTERNAL

- Removed hardcoded server IP that was used when DNS resolution fails. Instead, the cloud connection is failed and the system will have to retry.  This means DNS lookup failure is now consistent with other modes of connection failure.  Addresses #139 Related to #1024


## v0.6.0-rc.1

### BREAKING CHANGES
- `UDP.flush()` and `TCP.flush()`  now conform to the `Stream.flush()` behavior from Arduino 1.0 Wiring. The current (correct) behavior is to wait
  until all data has been transmitted. Previous behavior discarded data in the buffer. [#469](https://github.com/spark/firmware/issues/469)

### FEATURES
- [Logging](https://docs.staging.particle.io/reference/firmware/electron/#logging) library for flexible system and application logging. [Docs](https://docs.staging.particle.io/reference/firmware/electron/#logging)
- [Electron] Reduced data consumption connecting to the cloud with deep sleep. ([See the Docs](https://docs.staging.particle.io/reference/firmware/electron/#optimizing-cellular-data-use-with-cloud-connectivity-on-the-electron) for how to gain the full data reduction.) [#953](https://github.com/spark/firmware/pull/953)
- Can set Claim Code via the Serial interface (for use by the CLI.) [#602](https://github.com/spark/firmware/issues/602)
- Device ID available via dfu-util. [#949](https://github.com/spark/firmware/pull/949)
- [Electron] Firmware Reset now available. [#975](https://github.com/spark/firmware/pull/975) and  [Docs](https://docs.particle.io/guide/getting-started/modes/electron/#firmware-reset)
- System Reset Reason [#403](https://github.com/spark/firmware/issues/403) [Docs](https://docs.staging.particle.io/reference/firmware/electron/#reset-reason)
- [Photon/Electron/P1] Composite USB device driver with HID Mouse & Keyboard implementation for STM32F2 [#902](https://github.com/spark/firmware/pull/902) and [#528](https://github.com/spark/firmware/issues/528)
- Exposes Device ID and Bootloader Version through USB descriptors while in DFU mode, Microsoft WCID support [#1001](https://github.com/spark/firmware/pull/1001)
- USB vendor-specific setup request handling [#1010](https://github.com/spark/firmware/pull/1010)
- [Electron] now allows OTA bootloader updates [#1002](https://github.com/spark/firmware/pull/1002)
- Added Daylight Saving Time support [#1058](https://github.com/spark/firmware/pull/1058) per proposed [#211](https://github.com/spark/firmware/issues/211) [Docs](https://docs.staging.particle.io/reference/firmware/electron/#local-)

### ENHANCEMENTS

- Local build warns if crc32 is not present. [#941](https://github.com/spark/firmware/issues/941)
- [Photon/Core] MAC address is available immediately after `WiFi.on()` [#879](https://github.com/spark/firmware/issues/879)
- [virtual device] support for TCP Server [#1000](https://github.com/spark/firmware/pull/1000)
- [virtual device] support for EEPROM emulation [#1004](https://github.com/spark/firmware/pull/1004)
- Low-level RTOS queues exposed in HAL [#1018](https://github.com/spark/firmware/pull/1018)
- USART LIN bus support. [#930](https://github.com/spark/firmware/pull/930) [Docs](https://docs.staging.particle.io/reference/firmware/electron/#begin--1)
- USART added support for 7E1, 7E2, 7O1, 7O2 modes. [#997](https://github.com/spark/firmware/pull/997) [Docs](https://docs.staging.particle.io/reference/firmware/electron/#begin--1)
- Configurable resolution for analogWrite (PWM and DAC) [#991](https://github.com/spark/firmware/pull/991) [analogWrite() Docs](https://docs.staging.particle.io/reference/firmware/electron/#analogwrite-pwm-) | [analogWriteResolution() Docs](https://docs.staging.particle.io/reference/firmware/electron/#analogwriteresolution-pwm-and-dac-) |  [analogWriteMaxFrequency() Docs](https://docs.staging.particle.io/reference/firmware/electron/#analogwritemaxfrequency-pwm-)
- [System flag](https://docs.particle.io/reference/firmware/core/#system-flags) `SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS` to control if the device resets the network when it cannot connect to the cloud. [#946](https://github.com/spark/firmware/pull/946)
- [Photon] 1KB system backup memory added (same size as Electron) reducing user backup memory to 3KB (3068 bytes) [#1046](https://github.com/spark/firmware/pull/1046)
- Automatically adds vendored libraries from the `lib` directory for extended application projects [#1053](https://github.com/spark/firmware/pull/1053)
- Extended spi_master_slave tests with SPI_MODE0/1/2/3 and MSBFIRST/LSBFIRST testing [#1056](https://github.com/spark/firmware/pull/1056)
- [Electron] System parts reordered from 3,1,2 to 1,2,3 to preserve logical flashing order for OTA/YModem when upgrading. [#1065](https://github.com/spark/firmware/pull/1065)

### BUGFIXES

- SoftAP mode persisting when setup complete if Wi-Fi was off. [#971](https://github.com/spark/firmware/issues/971)
- Free memory allocated for previous system interrupt handler [#951](https://github.com/spark/firmware/pull/951) fixes [#927](https://github.com/spark/firmware/issues/927)
- Fixes to I2C Slave mode implementation with clock stretching enabled [#931](https://github.com/spark/firmware/pull/931)
- `millis()`/`micros()` are now atomic to ensure monotonic values. Fixes [#916](https://github.com/spark/firmware/issues/916), [#925](https://github.com/spark/firmware/issues/925) and [#1042](https://github.com/spark/firmware/issues/1042)
- availableForWrite() was reporting bytes available instead of bytes available for write [#1020](https://github.com/spark/firmware/pull/1020) and [#1017](https://github.com/spark/firmware/issues/1017)
- `digitalRead()` interferes with `analogRead()` [#993](https://github.com/spark/firmware/issues/993)
- USART 9-bit receiving. [#968](https://github.com/spark/firmware/issues/968)
- Fix soft AP suffix broken by the addition of device id in DCT [#1030](https://github.com/spark/firmware/pull/1030)
- WKP pin should not be enabled as a wakeup source unconditionally for STOP mode [#948](https://github.com/spark/firmware/pull/948) and [#938](https://github.com/spark/firmware/issues/938)
- General I2C Improvements and MCP23017 tests [#1047](https://github.com/spark/firmware/pull/1047)
- Rebuilt Wiced_Network_LwIP_FreeRTOS.a WWD_for_SDIO_FreeRTOS.a on OSX [#1057](https://github.com/spark/firmware/pull/1057) fixes Local build stalling on object dump [#1049](https://github.com/spark/firmware/issues/1049)
- Validates that module dependencies would still be satisfied after the module from the "ota_module" location is flashed (via OTA or YMODEM flashing) [#1063](https://github.com/spark/firmware/pull/1063)
- System.sleep SLEEP_MODE_DEEP timing accuracy and sleep STOP mode retains user interrupt handler after resuming [#1051](https://github.com/spark/firmware/pull/1051) fixes [#1043](https://github.com/spark/firmware/issues/1043) and [#1029](https://github.com/spark/firmware/issues/1029)

### INTERNAL

- [Electron] Use floating point arithmetic in PWM to save about 1KB of flash space [#1027](https://github.com/spark/firmware/pull/1027)
- Feature/vendorlibraries [#1009](https://github.com/spark/firmware/pull/1009)
- [Electron] Added a 3rd system module to provide room for additional system firmware [#1035](https://github.com/spark/firmware/pull/1035)
- Remove accidental SYSTEM_MODE(MANUAL) from pwm.cpp in wiring/no_fixture [#1052](https://github.com/spark/firmware/pull/1052)

## v0.5.5

### ENHANCEMENTS

- Downgrade bootloader when downgrading from 0.7.0 or newer. [#1417](https://github.com/spark/firmware/pull/1417)

### BUGFIXES

- [KRACK WPA2 security fix](https://github.com/spark/firmware/pull/1418)

## v0.5.4

### ENHANCEMENTS

- [`[PR #1353]`](https://github.com/spark/firmware/pull/1353) Sticker Rig support added for Electron manufacturing.  6-Digit device setup code added.  Firmware reset enabled on Electron.

## v0.5.3 (same as v0.5.3-rc.3)

### ENHANCEMENTS

- Automatically adds vendored libraries from the `lib` directory for extended application projects [#1053](https://github.com/spark/firmware/pull/1053)

### INTERNAL

- Feature/vendorlibraries [#1009](https://github.com/spark/firmware/pull/1009)


## v0.5.3-rc.2

### FEATURE

- DTR/RTS support (open/closed detection: `Serial.isConnected()`). [#1073](https://github.com/spark/firmware/pull/1073)

### ENHANCEMENTS

- [Electron] System firmware is now aware of system-part3 to allow OTA/YModem upgrade from >=0.5.3-rc.2 to >=0.6.0-rc.1

### BUGFIXES

- added HAL_IsISR() which is used to skip calling the background loop from delay(). fixes [#673](https://github.com/spark/firmware/issue/673)
- Fixes an issue of USB Serial erroneously switching to closed state. [#1073](https://github.com/spark/firmware/pull/1073)
- RTC wakeup time now calculated right before entering SLEEP_MODE_DEEP. Fixes [#1043](https://github.com/spark/firmware/issue/1043)
- STOP mode should retain user interrupt handler. Fixes [#1029](https://github.com/spark/firmware/issue/1029)


## v0.5.3-rc.1

### BUGFIXES

- SoftAP mode persisting when setup complete if Wi-Fi was off. [#971](https://github.com/spark/firmware/issues/971)
- Free memory allocated for previous system interrupt handler [#951](https://github.com/spark/firmware/pull/951) fixes [#927](https://github.com/spark/firmware/issues/927)
- availableForWrite() was reporting bytes available instead of bytes available for write [#1020](https://github.com/spark/firmware/pull/1020) and [#1017](https://github.com/spark/firmware/issues/1017)
- `millis()`/`micros()` are now atomic to ensure monotonic values. Fixes [#916](https://github.com/spark/firmware/issues/916), [#925](https://github.com/spark/firmware/issues/925) and [#1042](https://github.com/spark/firmware/issues/1042)
- Fixes to I2C Slave mode implementation with clock stretching enabled [#931](https://github.com/spark/firmware/pull/931)
- General I2C Improvements and MCP23017 tests [#1047](https://github.com/spark/firmware/pull/1047)
- Rebuilt Wiced_Network_LwIP_FreeRTOS.a WWD_for_SDIO_FreeRTOS.a on OSX [#1057](https://github.com/spark/firmware/pull/1057) fixes Local build stalling on object dump [#1049](https://github.com/spark/firmware/issues/1049)
- `digitalRead()` interfered with `analogRead()` [#1006](https://github.com/spark/firmware/pull/1006) fixes [#993](https://github.com/spark/firmware/issues/993)
- Validates that module dependencies would still be satisfied after the module from the "ota_module" location is flashed (via OTA or YMODEM flashing) [#1063](https://github.com/spark/firmware/pull/1063)


## v0.5.2 (same as v0.5.2-rc.1)

### ENHANCEMENTS

- [Photon/P1] Restores the default WICED country to Japan [#1014](https://github.com/spark/firmware/pull/1014)

### BUGFIXES

- .syncTime() and .unsubscribe() called on the system thread. Prevents issues when multiple threads try to send messages through the cloud connection or manage the network state shared memory. [#1041](https://github.com/spark/firmware/pull/1041)


## v0.5.1 (same as v0.5.1-rc.2)

### FEATURES

- [Electron] Added support in HAL for a SMS received callback handler.


## v0.5.1-rc.1

### FEATURES

- Wi-Fi Country Code can be set to configure the available channels and power transmission. [#942](https://github.com/spark/firmware/pull/942)

### ENHANCEMENTS

- ARM GCC 5.3.1 compiler support

### BUGFIXES

- [Photon/P1] Fix a timing-critical bug in WICED that causes system freeze. [#877](https://github.com/spark/firmware/issues/877)
- Tone not available on A7 after stop-mode sleep. [#938](https://github.com/spark/firmware/issues/938)
- Regression in EEPROM emulation size. [#983](https://github.com/spark/firmware/pull/983)
- [Electron] Wrong bitmask is provided for 4208 setting in power management [#987](https://github.com/spark/firmware/pull/987)


## v0.5.0 (same as v0.5.0-rc.2)

### FEATURES

- Added SYSTEM_FLAG_WIFITEST_OVER_SERIAL1 which is disabled by default. Tinker enables this by default so that the Wi-Fi Tester is available during manufacturing.  Also ensures TX/RX pins are not used for Serial1 by default, in case you want to use these as GPIO. [945](https://github.com/spark/firmware/pull/945)

### ENHANCEMENTS

- Timer::isActive() function added [#950](https://github.com/spark/firmware/pull/950)
- mbedtls headers are private to the communications module now, so user applications can include their own version of mbedtls [](https://github.com/spark/firmware/pull/940)

### BUGFIXES

- Soft AP Claim code fix [#956](https://github.com/spark/firmware/pull/956)
- Variable template fix [#952](https://github.com/spark/firmware/pull/952)
- TCPClient on Electron not receiving all of the data for small files [#896](https://github.com/spark/firmware/issues/896)

## v0.5.0-rc.1

### FEATURES

- [Electron] [SYSTEM_THREAD()](https://docs.particle.io/reference/firmware/electron/#system-thread) is supported (in Beta) [#884](https://github.com/spark/firmware/pull/884)
- [Electron] Cellular [Data Usage API](https://docs.particle.io/reference/firmware/electron/#getdatausage-) [#866](https://github.com/spark/firmware/pull/866)
- [Electron] Configurable keep-alive ping [#913](https://github.com/spark/firmware/pull/913)
- [Electron] Cellular [Band Select API](https://docs.particle.io/reference/firmware/electron/#getbandavailable-) [#891](https://github.com/spark/firmware/pull/891)
- [Electron] Cellular [Local IP API](https://docs.particle.io/reference/firmware/electron/#localip-) [#850](https://github.com/spark/firmware/pull/850)
- [Photon/Electron] Stack overflow detection with SOS code [13-blinks](https://docs.particle.io/guide/getting-started/modes/photon/#red-flash-sos)
- [Photon/Electron] [SPI Slave support](https://docs.particle.io/reference/firmware/photon/#begin-spi_mode-uint16_t-) [#882](https://github.com/spark/firmware/pull/882)
- Atomic writes in [EEPROM emulation](https://docs.particle.io/reference/firmware/electron/#eeprom) [#871](https://github.com/spark/firmware/pull/871)
- Software Watchdog [#860](https://github.com/spark/firmware/pull/860)
- [Serial.availableForWrite()](https://docs.particle.io/reference/firmware/photon/#availableforwrite-) and [Serial.blockOnOverrun()](https://docs.particle.io/reference/firmware/photon/#blockonoverrun-) [#798](https://github.com/spark/firmware/issues/798)
- [Photon] SoftAP HTTP server can serve application pages. [#906](https://github.com/spark/firmware/pull/906)

### ENHANCEMENTS

- Compiler error with variable/function names that are too long. [#883](https://github.com/spark/firmware/pull/883)
- DFU writes are verified [#870](https://github.com/spark/firmware/pull/870)
- [Electron] [NO_ACK flag](https://docs.particle.io/reference/firmware/electron/#particle-publish-) on `Particle.publish()` disables acknoweldgements reducing data use [#862](https://github.com/spark/firmware/pull/862)
- [Electron] Allow session to resume when IP changes. [#848](https://github.com/spark/firmware/pull/848)
- [Electron] Ensure published events are received by the cloud before sleeping. [#909](https://github.com/spark/firmware/pull/909)
- [Electron] [SLEEP_NETWORK_STANDBY on System.sleep()](https://docs.particle.io/reference/firmware/electron/#sleep-sleep-) [#845](https://github.com/spark/firmware/pull/845)
- Serial baudrate to select ymodem mode includes listening mode [#912](https://github.com/spark/firmware/pull/912)
- Wi-Fi connection process forced to timeout after 60 seconds if unsuccessful [#898](https://github.com/spark/firmware/pull/898)
- Added write-verify-retry-fail logic to DFU writes [#870](https://github.com/spark/firmware/pull/870)
- Support for USART (Serial1/2/4/5) [data bits, parity and stop bits](https://docs.particle.io/reference/firmware/electron/#begin-) [#757](https://github.com/spark/firmware/pull/757)

### BUGFIXES

- targets `program-cloud`, `program-dfu` can be used without requiring `all` and will built the firmware correctly. [#899](https://github.com/spark/firmware/issues/899)
- [Electron] Free socket when the socket is closed remotely [#885](https://github.com/spark/firmware/pull/885)
- Extended CAN filters [#857](https://github.com/spark/firmware/pull/857)
- I2C does not ensure a stop condition completes correctly in endTransmission [#856](https://github.com/spark/firmware/pull/856)
- DAC1/2 possible problem with `digitalWrite()` after `analogWrite()` [#855](https://github.com/spark/firmware/pull/855)
- Servo HAL: Do not disable timer if some of its channels are still in use [#839](https://github.com/spark/firmware/pull/839)
- USB driver fixes and Serial.available() not returning values greater than 1 [#812](https://github.com/spark/firmware/pull/812) [#669](https://github.com/spark/firmware/issues/669) [#846](https://github.com/spark/firmware/issues/846) [#923](https://github.com/spark/firmware/issues/923)
- SOS During `WiFi.scan()` [#651](https://github.com/spark/firmware/issues/651)


### INTERNALS

- dynalib: compile-time check for certain types of ABI breaking changes [#895](https://github.com/spark/firmware/pull/895)


## v0.4.9

### FEATURES

- Support for CAN Bus [#790](https://github.com/spark/firmware/pull/790)
- [blockOnOverrun()]((https://docs.particle.io/reference/firmware/photon/#blockonoverrun-)) on hardware serial to allow applications to disable the default flow control.
- [availableForWrite()]((https://docs.particle.io/reference/firmware/photon/#availableforwrite-)) on hardware serial to allow applications to implement flow control. [#798](https://github.com/spark/firmware/issues/798)
- [WiFi.BSSID()](https://docs.particle.io/reference/firmware/photon/#wifi-bssid-) to retrieve the 6-byte MAC address of the connected AP. [#816](https://github.com/spark/firmware/pull/816)
- [attachInterrupt()](https://docs.particle.io/reference/firmware/photon/#attachinterrupt-) configurable interrupt priority [#806](https://github.com/spark/firmware/issues/806)
- [Time.local()](https://docs.particle.io/reference/firmware/photon/#local-) retrieves the current time in the configured timezone. [#783](https://github.com/spark/firmware/issues/783)
- [photon] [WiFi.getCredentials()](https://docs.particle.io/reference/firmware/photon/#wifi-getcredentials-) lists configured credentials.  [#759](https://github.com/spark/firmware/issues/759)
- variable frequency PWM via [analogWrite(pin,value,hz)](https://docs.particle.io/reference/firmware/photon/#analogwrite-) [#756](https://github.com/spark/firmware/pull/756)
- [ATOMIC_BLOCK()](https://docs.particle.io/reference/firmware/photon/#atomic_block-) and [SINGLE_THREADED_BLOCK()](https://docs.particle.io/reference/firmware/photon/#single_threaded_block-) declarations for atomicity and thread scheduling control. [#758](https://github.com/spark/firmware/issues/758)
- [API](https://docs.particle.io/reference/firmware/photon/#synchronizing-access-to-shared-system-resources) for Guarding resources for use between threads.
- System events for individual button clicks and a run of button clicks. [#818](https://github.com/spark/firmware/issues/818)

### ENHANCEMENTS

- [System.freeMemory()](https://docs.particle.io/reference/firmware/photon/#system-freememory-) shows an accurate value for free memory rather than the highwater mark for the heap.
- [threading] Entering listening mode does not block the system thread. [#788](https://github.com/spark/firmware/issues/788)
- [threading] System times out waiting for unresponsive application when attempting to reset. [#763](https://github.com/spark/firmware/issues/763)
- [threading] `Particle.publish()` doesn't block when in listening mode.  [#761](https://github.com/spark/firmware/issues/761)
- [threading]. `delay()`/`Particle.process()` pumps application events.
- Serial, Serial1, SPI and EEPROM global objects guaranteed to be initialized before use. (Prevents White breathing LED if Serial used in a global instance constructor.)
- [Software Timers]((https://docs.particle.io/reference/firmware/photon/#software-timers) have an option for one-shot timers, and support class member function callbacks.

### BUGFIXES

- RSA key generation would sometimes produce invalid keys. [#779](https://github.com/spark/firmware/pull/779)
- Static IP configuration was not being used.
- Interrupt on WKP with class method as an ISR [#819](https://github.com/spark/firmware/issues/819)
- Memory leak configuring WiFi credentials via SoftAP (TCP/HTTP)
- SPI DMA transfer callback invoked too early [#791](https://github.com/spark/firmware/issues/791)
- Unset `wiced_result_t` for tcp clients in `socket_send`.  [#773](https://github.com/spark/firmware/issues/773)
- Update bootloader to support `System.enterSafeMode()`. [#751](https://github.com/spark/firmware/issues/751)
- [threading] `WiFi.listen(false)` remains in listen mode. [#743](https://github.com/spark/firmware/issues/743)
- Factory Reset doesn't clear WiFi credentials until `network.connect()`. [#736](https://github.com/spark/firmware/issues/736)
- Comparison between `IPAddress` objects does not always work.  [#715](https://github.com/spark/firmware/issues/715)
- P1 dfu-util 0.8 does not read/write from External Flash. [#706](https://github.com/spark/firmware/issues/706)
- DFU errors writing to flash silently ignored. [#813](https://github.com/spark/firmware/pull/813)
- [threading] heap allocation not thread-safe. [#826](https://github.com/spark/firmware/issues/826)
- System crash when button interrupt occurs during i2c transmission. [#709](https://github.com/spark/firmware/issues/709)
- [photon] 'analogWrite()` to DAC pins.  [#671](https://github.com/spark/firmware/issues/671)
- [photon] `analogWrite()` to DAC pins requires `pinMode()` each time. [#662](https://github.com/spark/firmware/issues/662)
- [photon] `System.sleep(pin,edge)` doesn't wake.  [#655](https://github.com/spark/firmware/issues/655)


## v0.4.8-rc.1

### FEATURE

- factory firmware uses full modular firmware [#749](https://github.com/spark/firmware/pull/749). No need for UpdateZero.

- `Timer.changePeriod()` [#720](https://github.com/spark/firmware/pull/720)

### BUGFIXES

- [photon] hang when UDP::stop() is called [#742](https://github.com/spark/firmware/issues/742)
- [photon] I2C hangs with no pullup resistors [#713](https://github.com/spark/firmware/issues/713)

## v0.4.7

### FEATURES

 - [Software Timers](https://docs.particle.io/reference/firmware/photon/#software-timers)
 - [pulseIn(pin, value)](https://docs.particle.io/reference/firmware/photon/#pulsein-) now available for all devices.
 - [WiFi.dnsServerIP()](https://docs.particle.io/reference/firmware/core/#wifi-dnsserverip-) and [WiFi.dhcpServerIP()](https://docs.particle.io/reference/firmware/core/#wifi-dhcpserverip-)
 - [serialEvent()](https://docs.particle.io/reference/firmware/core/#serialevent-)
 - [GCC virtual device](https://github.com/spark/firmware/tree/develop/hal/src/gcc#device-configuration)
 - [System.version()](https://docs.particle.io/reference/firmware/photon/#system-version-) to retrieve the version of system firmware [#688](https://github.com/spark/firmware/issues/688)
 - Firmware control of [OTA updates](https://docs.particle.io/reference/firmware/core/#ota-updates) can happen [#375](https://github.com/spark/firmware/issues/375)

### ENHANCEMENTS

 - [multithreading] Application thread continues to run in listening mode
 - [multithreading] `Particle.process()` called from the application thread pumps application messages [#659](https://github.com/spark/firmware/issues/659)
 - `Particle.variable()` supports `String`s [#657](https://github.com/spark/firmware/issues/657)
 - Simplified [Particle.variable()](https://docs.particle.io/reference/firmware/photon/#variables) API - variable type parameter is optional, and variables are passed by reference so  `&`'s are not required.
 - I2C will generate STOP and SW Reset immediately if Slave Acknowledge failure is detected (must use pull-up resistors), instead of taking 100ms. [commit](https://github.com/spark/firmware/commit/893aa523990752a2afcda4ffa5bba3f66e047014)

### BUGFIXES

 - TCPClient unstable [#672](https://github.com/spark/firmware/issues/672)
 - Photon frequently SOS's immediately following cloud re-connect [#663](https://github.com/spark/firmware/issues/663)
 - `String.toLower()` has no affect on string. [#665](https://github.com/spark/firmware/issues/665)
 - SOS due to WICED socket handlers being called when socket is disposed. [#663](https://github.com/spark/firmware/issues/663) [#672](https://github.com/spark/firmware/issues/672)
 - Application constructors executed after RTOS startup so that HAL_Delay_Milliseconds() can be called. This may mean that `STARTUP()` code executes just a little later than before, but
    can safely use all public APIs.
 - Ensure bootloader region is write protected.
 - White breathing LED on exiting listening mode. [#682](https://github.com/spark/firmware/issues/682)
 - WICED not resolving DNS names with 4 parts (it was trying to decode as an IP address.)
 - SoftAP via HTTP would fail on Safari due to request sent as multiple TCP packets. Fixed WICED HTTP server.  [#680](https://github.com/spark/firmware/issues/680)
 - Retained variables are not persisting, even without reset or deep sleep. [#661](https://github.com/spark/firmware/issues/661)
 - Backup RAM enabled for monolithic builds [#667](https://github.com/spark/firmware/issues/667)
 - Pure virtual call on creation of low priority std::thread [#652](https://github.com/spark/firmware/issues/652)


## v0.4.6

### FEATURES
 - [photon] separate [System Thread](https://docs.particle.io/reference/firmware/photon/#system-thread)
 - [core] Hooks to support FreeRTOS (optional library)
 - Variables stored in [Backup RAM](https://docs.particle.io/reference/firmware/photon/#backup-ram)
 - [printf/printlnf](https://docs.particle.io/reference/firmware/core/#printf-) on `Print` classes - `Serial`, `Serial1`, `TCP`, `UDP`
 - `String.format` for printf-style formatting of to as `String`.
 - [Wire.end()](https://docs.particle.io/reference/firmware/photon/#end-) to release the I2C pins. [#597](https://github.com/spark/firmware/issues/597)
 - [Wire.reset()](https://docs.particle.io/reference/firmware/photon/#reset-) to reset the I2C bus. Thanks @pomplesiegel [#598](https://github.com/spark/firmware/issues/598)
 - [System.ticks()](https://docs.particle.io/reference/firmware/core/#system-cycle-counter) to retrieve the current MCU cycle counter for precise timing.
 - [System.enterSafeMode()](https://docs.particle.io/reference/firmware/core/#system-entersafemode-) to restart the device in safe mode.

### ENHANCEMENTS

 - [photon] `WiFi.selectAntenna()` setting is persistent, so the last selected antenna is used when the
device is in safe mode. [#618]
 - Detect when the cloud hasn't been serviced for 15s and disconnect, so device LED state accurately
reflects the connection state when the application loop has stalled. [#626](https://github.com/spark/firmwarwe/issues/626)
 - Compile-time checks for `Particle.variable()` [#619](https://github.com/spark/firmwarwe/issues/619)
- [photon] Increased retry count when connecting to WiFi. [#620](https://github.com/spark/firmware/issues/620)
- Setup button events [#611](https://github.com/spark/firmware/pull/611)

### BUGFIXES

 - `UDP.receivePacket()` would fail if `UDP.setBuffer()` hadn't been called first. Thanks @r2jitu.
 - [photon] Default SS pin for SPI1 now set to D5. [#623](https://github.com/spark/firmware/issues/623)
 - [photon] Long delay entering listening mode. [#566](https://github.com/spark/firmware/issues/566)
 - [photon] Solid green LED when WiFi network cannot be connected to due to invalid key. (The LED now blinks.)
 - [photon] Storing more than 2 Wi-Fi credentials would sometimes give unpredictable results.
 - [photon] TX/RX pins did not work after entering listening mode. [#632](https://github.com/spark/firmware/issues/632)
 - [photon] Improvements to I2C for MCP23017 / Adafruit RGBLCDShield. [#626](https://github.com/spark/firmware/pull/626)


## v0.4.5

### FEATURES
- `SPI.setClockDividerReference`, `SPI.setClockSpeed` to set clock speed in a more portable manner. [#454](https://github.com/spark/firmware/issues/454)
- `WiFi.scan` function to retrieve details of local access points. [#567](https://github.com/spark/firmware/pull/567)
- `UDP.sendPacket`/`UDP.receivePacket` to send/receive a packet directly to an application-supplied buffer. [#452](https://github.com/spark/firmware/pull/452)
- Static IP Support [photon] - [#451](https://github.com/spark/firmware/pull/451)
- [photon] UDP multicast support via `UDP.joinMulticast`/`UDP.leaveMulticast`. Many thanks @stevie67!
- `waitFor(WiFi.ready)` syntax to make it easier to wait for system events. [#415](https://github.com/spark/firmware/issues/415)
- Flexible time output with `Time.format()` [#572](https://github.com/spark/firmware/issues/572)

### ENHANCEMENTS

- [Recipes and Tips](docs/build.md#recipes-and-tips) section in the build documentation.
- `Particle.function`, `Particle.subscribe` and `attachInterrupt` can take a C++ method and instance pointer. [#534](https://github.com/spark/firmware/pull/534) Thanks to @monkbroc!
- `UDP.setBuffer` to set the buffer a UDP instance uses for `read`/`write`. [#224](https://github.com/spark/firmware/pull/224) and [#452](https://github.com/spark/firmware/pull/452)
- `WiFi.setCredentials()` can take a Cipher type to allow full specification of an AP's credentials. [#574](https://github.com/spark/firmware/pull/574)
- TCPClient (from TCPServer) reports remote IP address. [#551](https://github.com/spark/firmware/pull/551)
- Configurable format in `Time.timeStr()`, including ISO 8601. [#455](https://github.com/spark/firmware/issues/455)
- `Servo.trim(adjust)` to allow small adjustments to the stationary point. [#120](https://github.com/spark/firmware/issues/120)
- Time set from the cloud accounts for network latency. [#581](https://github.com/spark/firmware/issues/581)
- `String(Printable)` constructor so any `Printable` can be converted to a string. [example](https://community.particle.io/t/convert-ipaddress-to-string-for-use-with-spark-publish/14885/4?u=mdma)
- Fluent API on `String` - many methods return `*this` so method calls can be chained.
- Small values passed to `delay(1)` result in more accurate delays. [#260](https://github.com/spark/firmware/issues/260)
- Bootloader does not show factory reset modes if a factory reset image is not available. [#557](https://github.com/spark/firmware/issues/557)

### BUGFIXES

- Listening mode re-enters listening mode after credentials are given. [#558](https://github.com/spark/firmware/pull/558)
- String function dtoa() has problems with larger numbers. [#563](https://github.com/spark/firmware/pull/563)
- System doesn't set color of RGB LED when `RGB.control(true)` is called. [#362](https://github.com/spark/firmware/pull/362), [#472](https://github.com/spark/firmware/pull/472) and [#544](https://github.com/spark/firmware/pull/544)
- WiFi.SSID() may not return previous network when switching. [#560](https://github.com/spark/firmware/pull/560)
- [photon] System.sleep(5) not turning Wi-Fi back on after 5 seconds. [#480](https://github.com/spark/firmware/pull/480)
- regression: floating point support in sprintf not compiled in. [#576](https://github.com/spark/firmware/issues/576)
- [photon] SPI1 default clock speed was 7.5MHz, changed to 15MHz, same as for `SPI`.
- TCPClient::connected() doesn't detect when the socket is closed [#542](https://github.com/spark/firmware/issues/542)
- dfu-util: error during downlod get_status msg removed when using :leave option [#599](https://github.com/spark/firmware/issues/599)
- [Core] A0 could not be used as an output [#595](https://github.com/spark/firmware/issues/595)
- Reinstate CFOD handling on the Photon.

## v0.4.4

### FEATURES
 - logging output [documentation](docs/debugging.md)
 - pressing 'v' in SoftAP mode displays the system version. FIRM-128
 - P1: API (compatible with Core) to access the 1MByte external flash. [#498](https://github.com/spark/firmware/pull/498)
 - Arduino compatibility macros for PROGMEM and more.
 - `RGB.onChange` handler receives notification of the current LED color when it changes. Can be used to match an external LED to the onboard led. [#518](https://github.com/spark/firmware/pull/518) Thanks to @monkbroc!
 - Serial2 available on P1 and Photon (note: this also requires above RGB.onChange handler and two resistors would need to be removed on the Photon)
 - `Spark.connected()` et al. is now `Particle.connected()`. The former `Spark` library is still available but is deprecated.
 - `System.freeMemory()` API to determine the amount of available RAM.
 - `STARTUP()` macro to define blocks of code that execute at startup.

### ENHANCEMENTS
 - Retrieve the LED brightness via `RGB.brightness()`
 - More prominent color change on the RGB LED when there is a cloud connection error.
 - System.sleep() - 2nd parameter changed to `InterruptMode` from uint16_t to
 ensure the correct types are used. [#499](https://github.com/spark/firmware/pull/499)
 - Less aggressive exponential backoff when the re-establishing the cloud connection. [FIRM-177]
 - I2C Wire.endTransmission() returns unique values and [I2C docs updated](https://docs.particle.io/reference/firmware/photon/#endtransmission-)
 - Generate I2C STOP after slave addr NACK, I2C software reset all timeouts -  [commit](https://github.com/spark/firmware/commit/53914d809cc17a3802b879fbb4fddcaa7d264680)
 - Improved I2C Master receive method and implemented error handler - [commit](https://github.com/spark/firmware/commit/1bc00ea480ef1fcdbd8ef9ba3df12b121183aeae) -  [commit](https://github.com/spark/firmware/commit/5359f19985756182ff6511217cbcb588b3341a87)
 - `WiFi.selectAntenna()` default antenna is now INTERNAL. Can be called at startup (before WiFi is initialized to select the desired antenna.


### BUGFIXES

 - [Regression] System connects WiFi when Spark.connect() is called after WiFi.on() [#484](https://github.com/spark/firmware/issues/484)
 - [Debug build](https://github.com/spark/firmware/blob/develop/docs/debugging.md) now working.
 - PWM issue fixed - 500Hz output on all channels [#492](https://github.com/spark/firmware/issues/492)
 - Tone issue fixed on D2,D3,RX,TX [#483](https://github.com/spark/firmware/issues/483)
 - SOS when registering more than 2 subscription handlers, and allow 4 subscription handlers to be successfully registered. [#531](https://github.com/spark/firmware/issues/531)
 - SOS on TCPClient.connect() when DNS resolution failed or when connection fails [#490](https://github.com/spark/firmware/issues/490)
 - `TCPClient::stop()` does not work on first connection [#536](https://github.com/spark/firmware/issues/536)
 - `TCPClient::connect()` does not close an existing socket. [#538](https://github.com/spark/firmware/issues/538)
 - TX/RX PWM randomly inverted [#545](https://github.com/spark/firmware/issues/545)
 - UDP.begin/write return values [#552](https://github.com/spark/firmware/issues/552)

## v0.4.3

### FEATURES
 - Half-duplex mode on Serial1 via `Serial1.halfdupliex()`. Thanks to @prices.
 - `WiFi.connect(WIFI_CONNECT_SKIP_LISTEN)` allows application firmware to skip listen mode when there is no credentials.
 - System events

### ENHANCEMENTS
 - I2C methods now use `micros()` for timeouts rather than `millis()`, so I2C functions can be used in an interrupt handler. [#460](https://github.com/spark/firmware/issues/460)
 - `WiFi.listen(false)` to programmatically exit WiFi listening mode.
 - make is verbose by default. To silence, add `-s` to the command line.
 - `WiFi.connect(WIFI_CONNECT_SKIP_LISTEN)` starts connection but does not enter listening mode if no credentials are found.
 - Setup/Mode button now starts listening mode when WiFi is off.
 - `WiFi.listen(false)` can be used to exit listening mode (from an interrupt.)
 - LED flashes high-speed green when requesting an IP address via DHCP.

### BUGFIXES

 - [Photon/TCPServer] - `TCPClient.connected()` was not returning `false` when the socket was asynchronously disconnected.
 - Fix time being reset on wakeup. (removed WICED RTC init code that resets to default preset time in platform_mcu_powersave_init() within photon-wiced repo.) [#440](https://github.com/spark/firmware/issues/440)
 - `TCPClient.connected()` was not returning `false` when the socket was disconnected from the other end.
 - `strdup()` was returning garbage [#457](https://github.com/spark/firmware/issues/457)
 - `attachInterrupt()` should work on all interrupt pins now except D0 & A5. Please note there are shared lines as per the following issue comment : [#443] (https://github.com/spark/firmware/issues/443#issuecomment-114389744)
 - I2C bus lockup when no slave devices are present by issuing a STOP condition after sLave send address fails.
 - `spark/` events not propagated to application handlers. [#481](https://github.com/spark/firmware/issues/481)
 - `sprintf` calls not linking correctly. [#471](https://github.com/spark/firmware/issues/471)
 - Photon/P1 sometimes did not start without hitting reset after a cold boot.
 - Disable LTO compile for user firmware since it causes linking problems (see `sprintf` above.)

## v0.4.2

### FEATURES
 - EEPROM storage of custom data types via `EEPROM.put()` and `EEPROM.get()'
 - When the device is in safe mode, the LED breathes magenta
 - `attachSystemInterrupt()` allows hooking key system interrupts in user code.
 - [DMA-driven SPI master](https://github.com/spark/docs/pull/49)
 - `UDP.sendPacket()` method avoids buffering data when the user can supply the entire buffer at once.
 - [Photon] SoftAP setup can be done over HTTP
 - platform-neutral fast pin access [449](https://github.com/spark/firmware/pull/449)
 - [P1] Serial2 support

### ENHANCEMENTS

 - [Photon] The system firmware updates the bootloader to latest version
 - [Photon] The system write protects the bootloader region.
 - UDP uses dynamically allocated buffers
 - `PRODUCT_ID` and `PRODUCT_VERSION` place these details at a known place in the firmware image
 - DFU mode and serial firmware update can be triggered by setting the line rate.

### BUGFIXES

 - `Serial1.end()` [hangs the system](https://community.particle.io/t/changing-serial-baud-rate-inside-setup-code-causes-core-freezing-afterwards/10314/6)
 - Malformed CoAP acknowledgement message in cloud protocol.
 - `SPARK_WLAN_Loop()` was not linked. (Workaround was to use `Spark.process()`)
 - UDP doesn't send anything to the device until `UDP.write()` [#407](https://github.com/spark/firmware/issues/407)
 - Divide by zero now caught and causes a SOS.
 - Floating-point support for `sprintf()` reinstated
 - Fixed WICED DCT becoming unmodifiable
 - Fix UDP.parsePacket() not receiving any data on the Photon [#468](https://github.com/spark/firmware/issues/468)

## v0.4.1

### ENHANCEMENTS

- Signed Photon USB Driver for use with Windows 8.1


### BUGFIXES

 - `Spark.syncTime()` was not linked. [#426](https://github.com/spark/firmware/issues/426)
 - Wire.setSpeed(CLOCK_SPEED_100KHZ) was not linked. [#432](https://github.com/spark/firmware/issues/432)
 - WiFi.selectAntenna() was not linked.

## v0.4.0

### NEW PLATFORMS
- PHOTON!!!!


### ENHANCEMENTS

 - `loop()` iteration rate increased by 1000 times - from 200 Hz to over 200 kHz!
 - Compiler: Removed all warnings from the compile (and made warnings as errors) so compiler output is minimal.
 - Debugging: SWD Support, thanks to Elco Jacobs. [#337](https://github.com/spark/core-firmware/pull/337)
 - `Spark.publish()` returns a success value - [#388](https://github.com/spark/firmware/issues/388)
 - `Spark.process()` as the public API for running the system loop. [#347](https://github.com/spark/firmware/issues/347)
 - Sleep no longer resets (on the Photon) [#283](https://github.com/spark/firmware/issues/283)
 - Support for application code outside of the firmware repo. [#374](https://github.com/spark/firmware/issues/374)
 - MAC Address available in setup via 'm' key. [#352](https://github.com/spark/firmware/issues/352)
 - SoftAP setup on the Photon
 - `Spark.sleep()` changed to `System.sleep()` and similarly for `deviceID()` [#390](https://github.com/spark/firmware/issues/390)
 - Listening mode uses existing serial connection if already opened. [#384](https://github.com/spark/firmware/issues/384)
 - `Spark.publish("event", PRIVATE)` shorthand - [#376](https://github.com/spark/firmware/issues/376)
 - Improved integrity checks for firmware images
 - Added additional safe/recovery mode in bootloader (> 6.5 sec : restore factory code without clearing wifi credentials)
 - Enabled CRC verification in bootloader before restoring/copying the firmware image from factory reset, ota downloaded area etc.
 - Added 'program-serial' to build target to enter serial ymodem protocol for flashing user firmware (Testing pending...)
 - Cloud string variables can be re-defined [#241](https://github.com/spark/firmware/issues/241)
 - Removed hard-coded limit on number of functions and variables [#111](https://github.com/spark/firmware/issues/111)
 - Parameterized function callbacks, lambda support for functions [#311](https://github.com/spark/firmware/issues/313)
 - C++ STL headers supported
- Can duplicate the onboard RGB LED color in firmware. [#302](https://github.com/spark/firmware/issues/302)
- `WiFi.selectAntenna()` - select between internal (chip) and external (u.FL) antenna on Photon: [#394](https://github.com/spark/firmware/issues/394)
- `WiFi.resolve()` to look up an IP address from a domain name. [#91](https://github.com/spark/firmware/issues/91)
- `System.dfu()` to reboot the core in dfu mode, until next reset or next DFU update is received.


### BUGFIXES

- SOS calling `Spark.publish()` in `SEMI_AUTOMATIC`/`MANUAL` mode
- Subscriptions maintained when cloud disconnected. [#278](https://github.com/spark/firmware/issues/278)
- Fix for events with composite names. [#382](https://github.com/spark/firmware/issues/382)
- `WiFi.ready()` returning true after `WiFi.off()` in manual mode. [#378](https://github.com/spark/firmware/issues/378)
- `Serial.peek()` implemented. [#387](https://github.com/spark/firmware/issues/387)
- Mode button not working in semi-automatic or manual mode. [#343](https://github.com/spark/firmware/issues/343)
- `Time.timeStr()` had a newline at the end. [#336](https://github.com/spark/firmware/issues/336)
- `WiFi.RSSI()` caused panic in some cases. [#377](https://github.com/spark/firmware/issues/377)
- `Spark.publish()` caused SOS when cloud disconnected. [#322](https://github.com/spark/firmware/issues/332)
- `TCPClient.flush()` discards data in the socket layer also. [#416](https://github.com/spark/firmware/issues/416)


### UNDER THE HOOD

 - Platform: hardware dependencies are factored out from wiring into a hardware abstraction layer
 - Repo: all 3 spark repos (core-common-lib, core-communication-lib, core-firmware) are combined into this repo.
 - Modularization: factored common-lib into `platform`, `services` and `hal` modules.
 - Modularization: factored core-firmware into `wiring`, `system`, 'main' and `user` modules.
 - Modularization: user code compiled as a separate library in the 'user' module
 - Build system: fancy new build system - [build/readme.md](build/readme.md)
 - Modularization: modules folder containing dynamically linked modules for the Photon



## v0.3.4

### FEATURES

- Local Build: Specify custom toolchain with `GCC_PREFIX` environment variable ([firmware](https://github.com/spark/firmware/pull/328), [core-common-lib](https://github.com/spark/core-common-lib/pull/39), [core-communication-lib](https://github.com/spark/core-communication-lib/pull/29))

### ENHANCEMENTS

- Wiring: More efficient and reliable `print(String)` (fix issue [#281](https://github.com/spark/firmware/issues/281)) [#305](https://github.com/spark/firmware/pull/305)
- DFU: Add DFU suffix to .bin file [#323](https://github.com/spark/firmware/pull/323)

### BUGFIXES

- I2C: Use I2C polling mode by default [#322](https://github.com/spark/firmware/pull/322)
- Listening Mode: Fix hard fault when Wi-Fi is off [#320](https://github.com/spark/firmware/pull/320)
- LED Interaction: Fix breathing blue that should be blinking green [#315](https://github.com/spark/firmware/pull/315)


## v0.3.3

### FEATURES

 - Cloud: [Secure random seed](https://github.com/spark/core-communication-lib/pull/25). When the spark does a handshake with the cloud, it receives a random number that is set as a seed for `rand()`
 - Wiring: Arduino-compatible `random()` and `randomSeed()` functions. [#289](https://github.com/spark/core-firmware/pull/289)
 - Wiring: Arduino-compatible functions like `isAlpha()` and `toLowerCase()`. [#293](https://github.com/spark/core-firmware/pull/293)

### ENHANCEMENTS

 - Wire: added missing Slave mode using DMA/Interrupts and updated Master mode using DMA. New APIs `Wire.setSpeed()` and `Wire.strechClock()`. [#284](https://github.com/spark/core-firmware/issues/284)
 - Sleep: `Spark.sleep()` supports wakeup on pin change. [#265](https://github.com/spark/core-firmware/issues/265)

### BUGFIXES

 - RGB: calling `RGB.brightness()` doesn't change the LED brightness immediately [#261](https://github.com/spark/core-firmware/issues/261)
 - Wiring: `pinMode()` `INPUT` and `OUTPUT` constants had reversed values compared to Arduino. [#282](https://github.com/spark/core-firmware/issues/282)
 - Wiring: compiler error using `HEX` with `String`. [#210](https://github.com/spark/core-firmware/pull/210)
 - System Mode: MANUAL mode breaks OTA update [#294](https://github.com/spark/core-firmware/issues/294)

## pre v0.3.3 versions

See https://github.com/spark/core-firmware/releases
