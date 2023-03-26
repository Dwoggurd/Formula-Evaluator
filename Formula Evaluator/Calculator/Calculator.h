// ============================================================================
// Formula Evaluator
// ============================================================================
// Calculator
//
// Handles calculator slots with formulas
// 
// @author Dwoggurd (2023)
// ============================================================================

#ifndef Calculator_H
#define Calculator_H

// ----------------------------------------------------------------------------
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include "CalculatorSlot.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// Calculator declaration
// ----------------------------------------------------------------------------
class Calculator
{
public:
    Calculator();
    Calculator( const Calculator& )            = delete;
    Calculator& operator=( const Calculator& ) = delete;
    Calculator( Calculator&& )                 = delete;
    Calculator& operator=( Calculator&& )      = delete;
    ~Calculator();

    CalculatorSlot* CreateSlot( const std::string &name );

    void  Calculate( Dataset set ) const;
    void  PrintResults() const;
    void  EnableParallel();

private:
    void  (Calculator::*calculateProxy)( Dataset set ) const;  // CalculateST or CalculateMT
    void  CalculateST( Dataset set ) const;
    void  CalculateMT( Dataset set ) const;

    // Using multi-index container with two indices:
    // - sequenced : to preserve formula insertion order
    // - ordered   : to handle uniquess and searches
    struct SlotAge {};
    struct SlotName {};

    boost::multi_index::multi_index_container<
        CalculatorSlot*,
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<boost::multi_index::tag<SlotAge>>,
            boost::multi_index::ordered_unique<boost::multi_index::tag<SlotName>, boost::multi_index::const_mem_fun<CalculatorSlot, std::string, &CalculatorSlot::Name>>
        >
    >  slots;
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // Calculator_H
