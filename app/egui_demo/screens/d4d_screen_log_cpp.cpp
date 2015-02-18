/* Define functions that use C++ here and link them with extern "C" */

#include "screen_common.h"
#include "application.h"

extern "C" {
    
void Log_GetTimeString(char charArrray[32]){
    String timeString = Time.timeStr();
    timeString.toCharArray(charArrray, 32);    
}

}
