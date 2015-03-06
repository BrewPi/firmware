#ifndef BREWPI_BOOT_SCREEN_H
#define	BREWPI_BOOT_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

void SetActuatorButtonState(const D4D_OBJECT* pThis, D4D_BOOL state);


D4D_EXTERN_SCREEN(screen_boot);
D4D_EXTERN_OBJECT(scrTest_devices00);
D4D_EXTERN_OBJECT(scrTest_devices01);
D4D_EXTERN_OBJECT(scrTest_devices10);
D4D_EXTERN_OBJECT(scrTest_devices11);
D4D_EXTERN_OBJECT(scrTest_devices20);
D4D_EXTERN_OBJECT(scrTest_devices21);

D4D_EXTERN_OBJECT(scrTest_actuator1)
D4D_EXTERN_OBJECT(scrTest_actuator2)
D4D_EXTERN_OBJECT(scrTest_actuator3)


#ifdef	__cplusplus
}
#endif

#endif	/* BREWPI_BOOT_SCREEN_H */

