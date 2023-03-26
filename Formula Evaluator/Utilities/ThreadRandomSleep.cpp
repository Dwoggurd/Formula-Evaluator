// ============================================================================
// Formula Evaluator
// ============================================================================
// Provides ways to put current thread to sleep for random amount of time
//  
// @author Dwoggurd (2023)
// ============================================================================

#include <thread>
#include <chrono>
#include "ThreadRandomSleep.h"
#include "../FormulaEvaluatorOptions.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
ThreadRandomSleep::ThreadRandomSleep( unsigned int lower, unsigned int upper ) 
    : distrib( lower, upper ), eng( std::random_device{}() )
{
    SleepProxy = programOptions.MTRandomSleeps() ? &ThreadRandomSleep::SleepEnabled : &ThreadRandomSleep::SleepDisabled;
}

// ----------------------------------------------------------------------------
unsigned int  ThreadRandomSleep::SleepEnabled() 
{ 
    unsigned int delay{ distrib( eng ) };

    std::this_thread::sleep_for( std::chrono::milliseconds{ delay } );

    return delay;
}

// ----------------------------------------------------------------------------
unsigned int  ThreadRandomSleep::SleepDisabled()
{
    return 0;
}

// ----------------------------------------------------------------------------
} // namespace fe
