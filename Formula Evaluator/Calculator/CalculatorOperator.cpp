// ============================================================================
// Formula Evaluator
// ============================================================================
// Calculator operators
// 
// @author Dwoggurd (2023)
// ============================================================================

#include <format>
#include "CalculatorOperator.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// CalculatorOperatorBase definition
// ----------------------------------------------------------------------------
CalculatorOperatorBase::CalculatorOperatorBase()
{
}

// ----------------------------------------------------------------------------
CalculatorOperatorBase::~CalculatorOperatorBase()
{
}


// ----------------------------------------------------------------------------
// CalculatorOperator definition
// ----------------------------------------------------------------------------
CalculatorOperator::CalculatorOperator( const std::string &tag, std::function<void()> cb ) : tag( tag ), cb( cb )
{
}

// ----------------------------------------------------------------------------
CalculatorOperator::~CalculatorOperator()
{
}

// ----------------------------------------------------------------------------
const std::string CalculatorOperator::Dump() const
{
    return tag;
}

// ----------------------------------------------------------------------------
void CalculatorOperator::operator()()
{
    cb();
}


// ----------------------------------------------------------------------------
// CalculatorVariable definition
// ----------------------------------------------------------------------------
CalculatorVariable::CalculatorVariable( const std::string &s, std::function<void(double)> cb ) : name( s ), cb( cb )
{
    Reset();
}

// ----------------------------------------------------------------------------
CalculatorVariable::~CalculatorVariable()
{
}

// ----------------------------------------------------------------------------
void CalculatorVariable::Reset()
{
    value = 0;
}

// ----------------------------------------------------------------------------
void CalculatorVariable::operator()()
{
    cb( value );
}

// ----------------------------------------------------------------------------
const std::string CalculatorVariable::Dump() const
{
    return name;
}

// ----------------------------------------------------------------------------
// CalculatorLiteral definition
// ----------------------------------------------------------------------------
CalculatorLiteral::CalculatorLiteral( const double val, std::function<void(double)> cb ) : value( val ), cb( cb )
{
}

// ----------------------------------------------------------------------------
CalculatorLiteral::~CalculatorLiteral()
{
}

// ----------------------------------------------------------------------------
void CalculatorLiteral::operator()()
{
    cb( value );
}

// ----------------------------------------------------------------------------
const std::string CalculatorLiteral::Dump() const
{
    return std::format( "{:.3f}", value );
}

// ----------------------------------------------------------------------------
// Print stuff for operators
// ----------------------------------------------------------------------------
std::ostream& operator<<( std::ostream& os, const Operator x )
{
    os << x->Dump();

    return os;
}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
