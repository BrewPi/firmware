/*
 * chamber.h
 *
 * Created: 15/03/2013 02:19:45
 *  Author: mat
 */ 

#ifndef CHAMBER_H_
#define CHAMBER_H_

#include "TempControl.h"
#include "TempSensor.h"
#include "piLink.h"

typedef uint8_t chamber_id;

/*
* The chamber class encapsulates how we switch between chambers.
* Since TemperatureController has only static fields, this implementation copies the state in and out of these static fields.
* If TemperatureController fields are made non-static then this class can simply copy references. 
*/
class Chamber : public TempControlState
{
	public:
	Chamber(TempSensor& _fridgeSensor, TempSensor& _beerSensor, Actuator& _cooler, Actuator& _heater, Actuator& _light, Sensor<boolean>& _door);
		
	void init() {
		applyInit();
	}				
	
	private:
	Chamber(const Chamber&);	
};


class ChamberManager
{
public: 
	ChamberManager(Chamber** _chambers, int _count) 
		: chambers(_chambers), current(0), chamber_count(_count) 
		{			
		}
	
	void init()
	{
		/*
			DEBUG_MSG(PSTR("Initialized with %d chambers"), chamberCount());
			for (chamber_id i=0; i<chamberCount(); i++) {
				DEBUG_MSG(PSTR("Chamber %d sensor pins %d and %d"), i, getChamber(i).getState().beerSensor.pinNr, 
					getChamber(i).getState().fridgeSensor.pinNr);
			}
		*/
		current = 0;
	}		
		
	chamber_id switchChamber(chamber_id id);
	
	void initChamber(chamber_id id);
	
	chamber_id chamberCount() const
	{
		return chamber_count;
	}
	
	chamber_id currentChamber() const
	{
		return current;
	}
	
private:
	Chamber& getChamber(chamber_id id) const {
		return *chambers[id];
	}
	
private:
	Chamber** chambers;
	chamber_id current;
	chamber_id chamber_count;
};

extern ChamberManager chamberManager;



#endif /* CHAMBER_H_ */