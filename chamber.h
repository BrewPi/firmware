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

class Chamber
{
	public:
	Chamber(TempSensor& fridge, TempSensor& beer) : state(fridge, beer) { }
		
	void init() {
		state.applyInit();
	}				
		
	void apply()
	{
		state.apply();
	}	

	void retract()
	{
		state.retract();
	}	

	const TempControlState& getState() const { return state; }

	
	private:
	TempControlState state;
	
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
		
	chamber_id switchChamber(chamber_id id)
	{
		if (id==current)
			return id;
		DEBUG_MSG(PSTR("switch from chamber %d to %d"), current, id);
		chamber_id prev = current;
		getChamber(current).retract();
		getChamber(id).apply();
		current = id;
		return prev;
	}
	
	void initChamber(chamber_id id) {
		current = id;
		getChamber(id).init();		
		tempControl.loadSettingsAndConstants(); //read previous settings from EEPROM
		tempControl.init();
		tempControl.updatePID();
		tempControl.updateState();
		getChamber(id).retract();
	}
	
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