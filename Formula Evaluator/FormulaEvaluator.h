// ============================================================================
// Formula Evalualor
// ============================================================================
// Root class
// 
// @author Dwoggurd (2023)
// ============================================================================

#ifndef FormulaEvaluator_H
#define FormulaEvaluator_H

// ----------------------------------------------------------------------------
#include <string>
#include <queue>
#include "Dataset.h"
#include "FormulaEvaluatorOptions.h"
#include "Parsers/FormulaParser.h"
#include "Parsers/DataParser.h"
#include "Calculator/Calculator.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// FormulaEvaluator declaration
// ----------------------------------------------------------------------------
class FormulaEvaluator
{
public:
    FormulaEvaluator();
    FormulaEvaluator( const FormulaEvaluator& )            = delete;
    FormulaEvaluator& operator=( const FormulaEvaluator& ) = delete;
    FormulaEvaluator( FormulaEvaluator&& )                 = delete;
    FormulaEvaluator& operator=( FormulaEvaluator&& )      = delete;
    ~FormulaEvaluator();

    void  Init( int argc, char* argv[] );
    void  Start();

private:
    void         Run();
    void         ParseFormulas();
    void         ParseData();
    std::string  LoadFileToStr( const std::string& fn ) const;

    FormulaEvaluatorOptions  options;
    Calculator               calculator;     // Must be declared before "formulaParser"
    DataParser               dataParser;
    FormulaParser            formulaParser;  // Constructed with "calculator" reference

    // Data stream queue
    std::queue<Dataset>  dataQ;
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // FormulaEvaluator_H
