// ============================================================================
// Formula Evaluator
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
#include <mutex>
#include <condition_variable>
#include <atomic>
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
    FormulaEvaluator()                                     = default;
    FormulaEvaluator( const FormulaEvaluator& )            = delete;
    FormulaEvaluator& operator=( const FormulaEvaluator& ) = delete;
    FormulaEvaluator( FormulaEvaluator&& )                 = delete;
    FormulaEvaluator& operator=( FormulaEvaluator&& )      = delete;
    ~FormulaEvaluator()                                    = default;

    void  Init();
    void  Start();

private:
    void         RunST();
    void         RunMT();
    void         ParseFormulas();
    void         ParseDataST();
    void         ParseDataMT();
    std::string  LoadFileToStr( const std::string& fn ) const;

    Calculator               calculator;     // Must be declared before "formulaParser"
    DataParser               dataParser;
    FormulaParser            formulaParser{ calculator };  // Constructed with "calculator" reference

    // Data stream queue
    std::queue<Dataset>      dataQ;

    std::mutex               dataQmutex;
    std::condition_variable  dataQcv;
    std::atomic_bool         dataQeoi{ false };
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // FormulaEvaluator_H
