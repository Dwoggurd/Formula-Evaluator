// ============================================================================
// Formula Evaluator
// ============================================================================
// Provides mechanism to put current thread to sleep for random amount of time.
// 
// This can make multi-threading execution less deterministic 
// and gives more cases for debugging.
// 
// @author Dwoggurd (2023)
// ============================================================================

#ifndef ThreadRandomSleep_H
#define ThreadRandomSleep_H

// ----------------------------------------------------------------------------
#include <random>

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// ThreadRandomSleep declaration
// ----------------------------------------------------------------------------
class ThreadRandomSleep
{
public:
    explicit ThreadRandomSleep( unsigned int lower, unsigned int upper );
    ThreadRandomSleep( const ThreadRandomSleep& )            = delete;
    ThreadRandomSleep& operator=( const ThreadRandomSleep& ) = delete;
    ThreadRandomSleep( ThreadRandomSleep&& )                 = delete;
    ThreadRandomSleep& operator=( ThreadRandomSleep&& )      = delete;
    ~ThreadRandomSleep()                                     = default;

    // When "random sleeps" are enabled puts current thread to sleep and returns delay.
    // Returns zero when "random sleeps" are disabled.
    constexpr unsigned int  Sleep() { return (this->*SleepProxy)(); };

private:
    unsigned int  (ThreadRandomSleep::*SleepProxy)();
    unsigned int  SleepEnabled();
    unsigned int  SleepDisabled();

    std::mt19937_64                              eng{ std::random_device{}() };
    std::uniform_int_distribution<unsigned int>  distrib;
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // ThreadRandomSleep_H
