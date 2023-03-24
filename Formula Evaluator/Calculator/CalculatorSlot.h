// ============================================================================
// Formula Evalualor
// ============================================================================
// Calculator Slot
// 
// Keeps formula in reverse polish notation
// Calculates formula on stack based machine
// Formula variables values are loaded from datasets (or zero'd if not present in dataset)
// 
// @author Dwoggurd (2023)
// ============================================================================

#ifndef CalculatorSlot_H
#define CalculatorSlot_H

// ----------------------------------------------------------------------------
#include <string>
#include <deque>
#include <iostream>
#include "../Dataset.h"
#include "CalculatorOperator.h"
#include "../Utilities/LoggingUtilities.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// CalculatorSlot declaration
// ----------------------------------------------------------------------------
class CalculatorSlot
{
public:
    explicit CalculatorSlot( const std::string &name );
    CalculatorSlot( const CalculatorSlot& )            = delete;
    CalculatorSlot& operator=( const CalculatorSlot& ) = delete;
    CalculatorSlot( CalculatorSlot&& slot )            = delete;
    CalculatorSlot& operator=( CalculatorSlot&& )      = delete;
    ~CalculatorSlot();
     
    inline std::string  Name() const { return name; };

    void         PushVariable( const std::string &x );
    void         PushLiteral( const double x );
    inline void  PushPos() { operators.emplace_back( pos ); };
    inline void  PushNeg() { operators.emplace_back( neg ); };
    inline void  PushAdd() { operators.emplace_back( add ); };
    inline void  PushSub() { operators.emplace_back( sub ); };
    inline void  PushMul() { operators.emplace_back( mul ); };
    inline void  PushDiv() { operators.emplace_back( div ); };

    void    Reset();
    void    LoadData( Dataset set ) const;
    void    CalculateSlot() const;
    double  ResultSlot() const;
    void    PrintResultSlot() const;

private:
    friend std::ostream& operator<<( std::ostream& os, const CalculatorSlot* const x );

    std::string  Dump() const;
    std::string  DumpVariables() const;

    const std::string  name;

    // Formula in reverse polish notation
    std::vector<Operator>  operators;

    // All variables found in formula
    std::unordered_map<std::string, std::shared_ptr<CalculatorVariable>>  variables;

    // Stack based machine. Using std::deque to allow direct access to the second element.
    std::deque<double>  vm;

    // Reusable operators
    const Operator  pos;
    const Operator  neg;
    const Operator  add;
    const Operator  sub;
    const Operator  mul;
    const Operator  div;
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // CalculatorSlot_H
