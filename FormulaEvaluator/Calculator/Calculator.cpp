// =============================================================================
// Formula Evaluator
// =============================================================================
// Calculator
// 
// @author Dwoggurd (2023-2024)
// =============================================================================

#include <vector>
#include <future>
#include "Calculator.h"
#include "../Utilities/LoggingUtilities.h"

// -----------------------------------------------------------------------------
namespace fe
{
// -----------------------------------------------------------------------------
// Calculator definition
// -----------------------------------------------------------------------------
Calculator::Calculator() : calculateProxy( &Calculator::CalculateST )
{
}

// -----------------------------------------------------------------------------
Calculator::~Calculator()
{
    for ( CalculatorSlot* x : slots.get<SlotAge>() )
    {
        delete x;
    }
}

// -----------------------------------------------------------------------------
void Calculator::EnableParallel()
{
    LOG( 3, "Enabled multi-threading for Calculator" );
    calculateProxy = &Calculator::CalculateMT;
}

// -----------------------------------------------------------------------------
CalculatorSlot* Calculator::CreateSlot( const std::string& name )
{
    CalculatorSlot* newSlot{ nullptr };
    const auto  it = slots.get<SlotName>().find( name );

    if ( it == slots.get<SlotName>().end() )
    {
        slots.get<SlotAge>().emplace_back( ( newSlot = new CalculatorSlot( name ) ) );
    }
    else
    {
        LOG( 5, "Slot already taken: " << (*it)->Name() );
    }

    return newSlot;
}

// -----------------------------------------------------------------------------
void Calculator::Calculate( Dataset set ) const
{
    LOG( 1, "" );
    LOG( 1, "Calculate dataset: " << DumpToStr( set ) );

    (this->*calculateProxy)( set );
}

// -----------------------------------------------------------------------------
void Calculator::CalculateST( Dataset set ) const
{
    for ( CalculatorSlot* const x : slots.get<SlotAge>() )
    {
        x->CalculateSlotST( set );
    }
}

// -----------------------------------------------------------------------------
void Calculator::CalculateMT( Dataset set ) const
{
    std::vector<std::future<void>>  threads;

    threads.reserve( slots.get<SlotAge>().size() );

    for ( CalculatorSlot* const x : slots.get<SlotAge>() )
    {
        threads.emplace_back( std::async( &CalculatorSlot::CalculateSlotMT, x, set ) );
    } 
    for ( auto& x : threads )
    {
        x.wait();
    }
}

// -----------------------------------------------------------------------------
void Calculator::PrintResults() const
{
    LOG( 0, "" );
    LOG( 0, "=============================" );
    for ( const CalculatorSlot* const x : slots.get<SlotAge>() )
    {
        LOG( 5, "    " << x );
        x->PrintResultSlot();
    }
    LOG( 0, "=============================" );
}

// -----------------------------------------------------------------------------
} // namespace fe
