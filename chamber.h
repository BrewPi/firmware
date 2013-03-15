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

typedef uint8_t chamber_id;

class Chamber
{
	public:
	Chamber(TempSensor& fridge, TempSensor& beer) : state(fridge, beer) { }
		
	void apply();
	void retract();
	
	private:
	TempControlState state;
	
	Chamber(const Chamber&);
};


class ChamberManager
{
public: 
	ChamberManager(Chamber** _chambers, int _count) 
		: chambers(_chambers), current(0), chamber_count(_count) { }
	
	void init()
	{
		getChamber(0).apply();
		current = 0;
	}		
		
	chamber_id switchChamber(chamber_id id)
	{
		chamber_id prev = current;
		getChamber(current).retract();
		getChamber(id).apply();
		current = id;
		return prev;
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