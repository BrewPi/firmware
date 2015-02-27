#ifndef BREWPI_BOOT_SCREEN_H
#define	BREWPI_BOOT_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

void SetActuatorButtonState(D4D_OBJECT* pThis, D4D_BOOL state);


D4D_EXTERN_SCREEN(screen_boot);
D4D_EXTERN_OBJECT(scrBoot_devices00);
D4D_EXTERN_OBJECT(scrBoot_devices01);
D4D_EXTERN_OBJECT(scrBoot_devices10);
D4D_EXTERN_OBJECT(scrBoot_devices11);
D4D_EXTERN_OBJECT(scrBoot_devices20);
D4D_EXTERN_OBJECT(scrBoot_devices21);

D4D_EXTERN_OBJECT(scrBoot_actuator1)
D4D_EXTERN_OBJECT(scrBoot_actuator2)
D4D_EXTERN_OBJECT(scrBoot_actuator3)


#ifdef	__cplusplus
}
#endif

#endif	/* BREWPI_BOOT_SCREEN_H */

