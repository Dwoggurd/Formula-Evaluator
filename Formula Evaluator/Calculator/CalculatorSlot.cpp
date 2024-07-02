// =============================================================================
// Formula Evaluator
// =============================================================================
// Calculator Slot
// 
// @author Dwoggurd (2023-2024)
// =============================================================================

#include <sstream>
#include <thread>
#include "../Utilities/DoubleToStr.h"
#include "CalculatorSlot.h"
#include "../Utilities/LoggingUtilities.h"
#include "../Utilities/ThreadRandomSleep.h"

// -----------------------------------------------------------------------------
namespace fe
{
// -----------------------------------------------------------------------------
// CalculatorSlot definition
// -----------------------------------------------------------------------------
CalculatorSlot::CalculatorSlot( const std::string& name ) : name( name )
{
    LOG( 5, "Created calculator slot: " << name );
}

// -----------------------------------------------------------------------------
void  CalculatorSlot::PushVariable( const std::string& x )
{
    if ( const auto it{ variables.find( x ) }; it != variables.end() )
    {
        operators.emplace_back( it->second );
    }
    else
    {
        const auto p = std::make_shared<CalculatorVariable>( x, [this]( const double v ) { this->vm.push_front( v ); } );
        variables.emplace( x, p );
        operators.emplace_back( p );
    }
}

// -----------------------------------------------------------------------------
void  CalculatorSlot::PushLiteral( const double x )
{
    operators.emplace_back( std::make_shared<CalculatorLiteral>( x, [this]( const double v ) { this->vm.push_front( v ); } ) );
}

// -----------------------------------------------------------------------------
void  CalculatorSlot::Reset()
{
    vm.clear();
    // Don't need to reset variables, they will be initialized by LoadData()
}

// -----------------------------------------------------------------------------
void  CalculatorSlot::LoadData( Dataset set ) const
{
    for ( const auto& [name, var] : variables )
    {
        const auto it{ set->find( name ) };
        var->SetValue( it == set->end() ? 0 : it->second );
    }
 }

// -----------------------------------------------------------------------------
void  CalculatorSlot::CalculateSlotST( Dataset set )
{
    Reset();
    LoadData( set );

    for ( Operator x : operators )
    {
        (*x)();
    }
}

// -----------------------------------------------------------------------------
void  CalculatorSlot::CalculateSlotMT( Dataset set )
{
    LOG( 5, "Starting async for: " << Name() );

    Reset();
    LoadData( set );

    for ( Operator x : operators )
    {
        (*x)();
    }

    // MT debug. Slow down threads by random time.
    ThreadRandomSleep pause( 5, 200 );
    unsigned int x = pause.Sleep();   // Returns zero when "random sleeps" are disabled

    LOG( 5, "CalculateSlot finished: " << Name()
            << "(0x" << std::hex << std::this_thread::get_id() << ")" 
            << std::dec << " slept=" << x );
}

// -----------------------------------------------------------------------------
double  CalculatorSlot::ResultSlot() const
{
    if ( vm.size() == 1 )
    {
        return vm.front();
    }
    else
    {
        LOG( 0, "Slot: " << Name() << " doesnt have valid result." );
        return 0;
    }
}

// -----------------------------------------------------------------------------
void  CalculatorSlot::PrintResultSlot() const
{
    LOG( 0, Name() << " = " << DoubleToStr( ResultSlot(), 3 ) );
}

// -----------------------------------------------------------------------------
std::string  CalculatorSlot::Dump() const
{
    std::ostringstream os;

    // Print formula in reverse polish notation
    os << "==>  " << Name() << " =";
    for ( const Operator x : operators )
    {
        os << " " << x;
    }

    // Print all found variables in formula
    os << "  <== " << DumpVariables();

    return os.str();
}

// -----------------------------------------------------------------------------
std::string  CalculatorSlot::DumpVariables() const
{
    std::ostringstream os;

    for ( const auto& x : variables )
    {
        os << " " << x.second->Name() << "(" << x.second->GetValue() << ")";
    }

    return os.str();
}

// -----------------------------------------------------------------------------
// Print stuff for calculator slot
// -----------------------------------------------------------------------------
std::ostream& operator<<( std::ostream& os, const CalculatorSlot* const x )
{
    os << x->Dump();

    return os;
}

// -----------------------------------------------------------------------------
} // namespace fe
