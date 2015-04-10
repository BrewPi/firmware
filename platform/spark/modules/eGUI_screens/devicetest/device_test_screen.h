#ifndef DEVICE_TEST_SCREEN_H
#define	DEVICE_TEST_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

void SetActuatorButtonState(const D4D_OBJECT* pThis, D4D_BOOL state, uint8_t idx);

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


void ScreenDeviceTest_OnMain();
void ScreenDeviceTest_OnActivate();
void ScreenDeviceTest_OnDeactivate();
Byte ScreenDeviceTest_OnObjectMsg(D4D_MESSAGE* pMsg);

        
#ifdef	__cplusplus
}
#endif

#endif	/* DEVICE_TEST_SCREEN_H */

