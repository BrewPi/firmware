/*
 * NoDisplay.cpp
 *
 * Created: 24/02/2013 22:19:55
 *  Author: mat
 */ 

/**
 * A display that provides only buffering of the content.
 */


void BufferDisplay::clear()
{
	for(uint8_t i = 0; i<4; i++){
		for(uint8_t j = 0; j<20; j++){
			content[i][j]=' '; // initialize on all spaces
		}
		content[i][20]='\0'; // NULL terminate string
	}
}


void BufferDisplay::setCursor(uint8_t col, uint8_t row)
{
	if ( row >= _numlines ) {
		row = 0;  //write to first line if out off bounds
	}
	_currline = row;
	_currpos = col;	
}

void BufferDisplay::getLine(uint8_t lineNumber, char * buffer){
	for(uint8_t i =0;i<20;i++){
		if(content[lineNumber][i] == 0b11011111){
			buffer[i] = 0xB0; // correct degree sign
		}
		else{
			buffer[i] = content[lineNumber][i]; // copy to string buffer
		}
	}
	buffer[20] = '\0'; // NULL terminate string
}
