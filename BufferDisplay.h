/*
 * BufferDisplay.h
 *
 * Created: 24/02/2013 22:27:52
 *  Author: mat
 */ 


#ifndef BUFFERDISPLAY_H_
#define BUFFERDISPLAY_H_

class BufferDisplay
{
	public:
	inline void init() { clear(): }
		
	inline void home() {
		_currline = 0;
		_currpos = 0;
	}
		
	inline void begin(uint8_t cols, uint8_t lines) {
		_numlines = lines;
		_currline = 0;
		_currpos = 0;
	}

	inline size_t write(uint8_t value) {
		content[_currline][_currpos] = value;
		_currpos++;
		return 1;
	}

	void setCursor(uint8_t col, uint8_t row)
	{
		if ( row >= _numlines ) {
			row = 0;  //write to first line if out off bounds
		}
		_currline = row;
		_currpos = col;
	}

	private:
	
};



#endif /* BUFFERDISPLAY_H_ */