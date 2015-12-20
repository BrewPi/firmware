/*

	Performance timers for Windows and OSX


*/

#ifndef PT_H
#define PT_H

#ifndef PLATFORM_SELECTOR_H
#include "platform_selector.h"
#endif

namespace Chordia
{
    //-----------------------------------------------------------------------------
    // Snap a performance counter tick value
    //-----------------------------------------------------------------------------
#ifdef _WINDOWS_
    typedef int64_t Interval;
#else
    typedef uint64_t Interval;
#endif
    
    class CPTSnap
    {
        Interval m_value;
        
	public:
        
        CPTSnap()
        {
#if defined(_WINDOWS_)
            QueryPerformanceCounter((LARGE_INTEGER*)&m_value);
#elif defined(__MACH__)
            // see http://developer.apple.com/library/mac/#qa/qa1398/_index.html
            m_value = mach_absolute_time();
#else
			m_value = 0;
#endif
        }
        
        CPTSnap(const CPTSnap& arg)
        {
            m_value = arg.value();
        }
        
        explicit CPTSnap(const Interval& arg)
        {
            m_value = arg;
        }
        
        CPTSnap& operator=(const CPTSnap& arg)
        {
            m_value = arg.value();
            return (*this);
        }
        
        //
        Interval value() const { return m_value; }
    };
    
    //-----------------------------------------------------------------------------
    // Defines a time interval between 2 CPTSnap instantiations
    //-----------------------------------------------------------------------------
    
    class CPTInterval
    {
        // the interval value. arbitrary ticks, no time value yet
        Interval m_interval;
        
        // calculate difference in snaps
        Interval Diff(const CPTSnap& pts, const CPTSnap& pte)
        {
            Interval ret = 0;
            ret = pte.value() - pts.value();
            return ret;
        }
        
        //
        enum Unit
        {
            eSeconds,
            eMilliseconds,
            eMicroSeconds,
            eNanoSeconds,
        };
        
        // convert to time value in seconds
        double Scale(Unit unit) const
        {
#if defined(_WINDOWS_)
            LARGE_INTEGER li;
			// what is the frequency ?
            QueryPerformanceFrequency(&li);
            double frequency = static_cast<double>(li.QuadPart);
			double seconds = 0;
            // remember. ticker frequency on Windows is per second ...
            switch (unit)
            {
                case eSeconds:
                    seconds = (m_interval * 1) / frequency;
                    break;
                case eMilliseconds:
                    seconds = (m_interval * 1000) / frequency;
                    break;
                case eMicroSeconds:
                    seconds = (m_interval * 1000000) / frequency;
                    break;
                case eNanoSeconds:
                    seconds = (m_interval * 1000000000) / frequency;
                    break;
            }
            return seconds;
#elif defined(__MACH__)
            // MacOS gives us intervals in nanoseconds ...
			mach_timebase_info_data_t info;
			mach_timebase_info(&info);
            uint64_t nanos = (m_interval * info.numer) / info.denom;
            switch (unit)
            {
                case eSeconds:
                    nanos /= 1000000000;
                    break;
                case eMilliseconds:
                    nanos /= 1000000;
                    break;
                case eMicroSeconds:
                    nanos /= 1000;
                    break;
                case eNanoSeconds:
                    break;
            }
			// hmm ...
            return static_cast<double>(nanos);
#else
			return 0;
#endif
        }
        
	public:
        
        //
        CPTInterval() 	
			: m_interval(0)
        {
        }
        
        //
        CPTInterval(const CPTSnap& pts, const CPTSnap& pte)  
			: m_interval(0)
        {
            // calculate the interval
            m_interval = Diff(pts,pte);
        }
        
        // short cut
        CPTInterval(const CPTSnap& pts) 
			: m_interval(0)
        {
            // calculate the interval
            m_interval = Diff(pts,CPTSnap());
        }
        
        // get the interval in ms
        double ms() const
        {
            // convert to a time period
            return Scale(eMilliseconds);
        }
        
        // get the interval in us
        double us() const
        {
            // convert to a time period
            return Scale(eMicroSeconds);
        }
        
    };	// class
	
}		// namespace

#endif
