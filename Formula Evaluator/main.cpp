// ============================================================================
// Formula Evaluator
// ============================================================================
// main() function
// 
// @author Dwoggurd (2023)
// ============================================================================

#include <memory>
#include <exception>
#include "FormulaEvaluatorOptions.h"
#include "FormulaEvaluator.h"

// ----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
    auto formulaEvaluator = std::make_unique<fe::FormulaEvaluator>();

    try
    {
        fe::programOptions.ParseOptions( argc, argv );

        formulaEvaluator->Init();
        formulaEvaluator->Start();
    }
    catch ( [[maybe_unused]] const std::exception &e )
    {
        return 1;
    }

    return 0;
}

// ----------------------------------------------------------------------------
