// ============================================================================
// Formula Evaluator
// ============================================================================
// Calculator
// 
// @author Dwoggurd (2023)
// ============================================================================

#include "Calculator.h"
#include "../Utilities/LoggingUtilities.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// Calculator definition
// ----------------------------------------------------------------------------
Calculator::Calculator()
{
}

// ----------------------------------------------------------------------------
Calculator::~Calculator()
{
    for ( CalculatorSlot* x : slots.get<SlotAge>() )
    {
        delete x;
    }
}

// ----------------------------------------------------------------------------
CalculatorSlot*  Calculator::CreateSlot( const std::string &name )
{
    CalculatorSlot  *newSlot{ nullptr };
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

// ----------------------------------------------------------------------------
void Calculator::Reset() const
{
    LOG( 5, "Calculator reset." );
    for ( CalculatorSlot* const x : slots.get<SlotAge>() )
    {
        x->Reset();
    }
}

// ----------------------------------------------------------------------------
void Calculator::LoadData( Dataset set ) const
{
    LOG( 1, "" );
    LOG( 1, "Loading data set:" );
    for ( const auto &x : *set )
    {
        LOG( 1, "    " << x.first << "=" << x.second );
    }

    for ( CalculatorSlot* const x : slots.get<SlotAge>() )
    {
        x->LoadData( set );
    }
}

// ----------------------------------------------------------------------------
void Calculator::Calculate() const
{
    for ( CalculatorSlot* const x : slots.get<SlotAge>() )
    {
        x->CalculateSlot();
    }
}

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
