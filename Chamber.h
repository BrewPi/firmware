/*
 * Chamber.h
 *
 * Created: 20/02/2013 17:58:39
 *  Author: mat
 */ 


#ifndef CHAMBER_H_
#define CHAMBER_H_

/************************************************************************/
/* Multiple chamber support?
* 
* Really need two of everything?
* At present duplicate each arduino, script and web
* Can avoid duplicating hardware by having the script read from a socket
* and having a dispatcher co-ordinate between each logical chamber and each script.                                                                     */
/************************************************************************/
class Chamber
{
	int id;
	TemperatureControl& control;
		
};



#endif /* CHAMBER_H_ */