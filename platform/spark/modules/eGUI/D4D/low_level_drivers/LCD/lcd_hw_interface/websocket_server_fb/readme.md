This display driver implements a websocket server that
pushes pixels to connected clients. 

On non-embedded devices, it uses a screen buffer. This is used only to capture the state before the client connects. This is
necessary when D4D is in calibration mode, because the calibration
screen isn't a true screen with components and cannot be re-rendered (without changing the flow in that routine.)