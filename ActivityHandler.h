/*
 */ 


#ifndef ActivityHandler_h_
#define ActivityHandler_h_

enum ActivityType {
	UNKOWN = 0,
	ROTARY_ENCODER_PUSHED = 0x11,
	ROTARY_ENCODER_TURNED = 0x12,	
	
	FRIDGE_DOOR_OPENED = 0x20,
	FRIDGE_DOOR_CLOSED = 0x21,
	// etc.. add other events here
};

/*
 * Receives notification of activity from a source.
 */
class ActivityHandler
{
public:
	virtual void notifyActivity(ActivityType activityType)=0;
};



#endif /* ActivityHandler_h_ */