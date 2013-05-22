/*
 * Debug.cpp
 *
 * Created: 22-5-2013 17:53:42
 *  Author: Elco
 */ 

#include "Brewpi.h"
#include "Debug.h"
#include "PiLink.h"
#include "TemperatureFormats.h"
#include "JsonKeys.h"

void Logger::logMessage(DEBUG_ID_TYPE errorID){
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.sendJsonClose();
}

void Logger::logMessage(DEBUG_ID_TYPE errorID, uint8_t val){
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.sendJsonPair(JSONKEY_logString, val);
	piLink.sendJsonClose();
}

void Logger::logMessage(DEBUG_ID_TYPE errorID, char * infoString){
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.sendJsonPair(JSONKEY_logString, infoString);
	piLink.sendJsonClose();
}

void Logger::logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 val){
	
}

//static Logger logger;