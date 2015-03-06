#ifndef BREWPI_BOOT_SCREEN_H
#define	BREWPI_BOOT_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

void SetActuatorButtonState(D4D_OBJECT* pThis, D4D_BOOL state);


D4D_EXTERN_SCREEN(screen_devicetest);
D4D_EXTERN_OBJECT(scrDeviceTest_devices00);
D4D_EXTERN_OBJECT(scrDeviceTest_devices01);
D4D_EXTERN_OBJECT(scrDeviceTest_devices10);
D4D_EXTERN_OBJECT(scrDeviceTest_devices11);
D4D_EXTERN_OBJECT(scrDeviceTest_devices20);
D4D_EXTERN_OBJECT(scrDeviceTest_devices21);

D4D_EXTERN_OBJECT(scrDeviceTest_actuator1)
D4D_EXTERN_OBJECT(scrDeviceTest_actuator2)
D4D_EXTERN_OBJECT(scrDeviceTest_actuator3)


#ifdef	__cplusplus
}
#endif

#endif	/* BREWPI_BOOT_SCREEN_H */

