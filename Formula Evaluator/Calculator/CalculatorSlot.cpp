// ============================================================================
// Formula Evalualor
// ============================================================================
// Calculator Slot
// 
// @author Dwoggurd (2023)
// ============================================================================

#include <sstream>
#include <format>
#include "CalculatorSlot.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// CalculatorSlot definition
// ----------------------------------------------------------------------------
CalculatorSlot::CalculatorSlot( const std::string &name ) : 
    name( name ),
    pos( std::make_shared<CalculatorOperator>( "#+", []() { /* do nothing */ } ) ),
    neg( std::make_shared<CalculatorOperator>( "#-", [this]() { this->vm.front() = -this->vm.front(); } ) ),
    add( std::make_shared<CalculatorOperator>( "+",  [this]() { this->vm[1] += this->vm.front(); this->vm.pop_front(); } ) ),
    sub( std::make_shared<CalculatorOperator>( "-",  [this]() { this->vm[1] -= this->vm.front(); this->vm.pop_front(); } ) ),
    mul( std::make_shared<CalculatorOperator>( "*",  [this]() { this->vm[1] *= this->vm.front(); this->vm.pop_front(); } ) ),
    div( std::make_shared<CalculatorOperator>( "/",  [this]() { this->vm[1] /= this->vm.front(); this->vm.pop_front(); } ) )
{
    LOG( 5, "Created calculator slot: " << name );
}

// ----------------------------------------------------------------------------
CalculatorSlot::~CalculatorSlot()
{
}

// ----------------------------------------------------------------------------
void  CalculatorSlot::PushVariable( const std::string &x )
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

// ----------------------------------------------------------------------------
void  CalculatorSlot::PushLiteral( const double x )
{
    operators.emplace_back( std::make_shared<CalculatorLiteral>( x, [this]( const double v ) { this->vm.push_front( v ); } ) );
}

// ----------------------------------------------------------------------------
void  CalculatorSlot::Reset()
{
    vm.clear();
    for ( const auto& x : variables )
    {
        x.second->Reset();
    }
}

// ----------------------------------------------------------------------------
void  CalculatorSlot::LoadData( Dataset set ) const
{
    for ( const auto& x : *set )
    {
        if ( const auto it{ variables.find( x.first ) }; it != variables.end() )
        {
            it->second->SetValue( x.second );
        }
    }
}

// ----------------------------------------------------------------------------
void  CalculatorSlot::CalculateSlot() const
{
    for ( const Operator x : operators )
    {
        (*x)();
    }
    return;
}

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
void  CalculatorSlot::PrintResultSlot() const
{
    LOG( 0, Name() << " = " << std::format( "{:.3f}", ResultSlot() ) );
}

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
std::string  CalculatorSlot::DumpVariables() const
{
    std::ostringstream os;

    for ( const auto& x : variables )
    {
        os << " " << x.second->Name() << "(" << x.second->GetValue() << ")";
    }

    return os.str();
}

// ----------------------------------------------------------------------------
// Print stuff for calculator slot
// ----------------------------------------------------------------------------
std::ostream& operator<<( std::ostream& os, const CalculatorSlot* const x )
{
    os << x->Dump();

    return os;
}

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
