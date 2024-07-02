// =============================================================================
// Formula Evaluator
// =============================================================================
// main() function
// 
// @author Dwoggurd (2023-2024)
// =============================================================================
#include <memory>
#include <exception>
#include <iostream>
#include "FormulaEvaluatorOptions.h"
#include "FormulaEvaluator.h"

// -----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
    std::cout << "--------------------------------\n"
              << "|       Formula Evaluator      |\n"
              << "--------------------------------" << std::endl;
    try
    {
        fe::programOptions.ParseOptions( argc, argv );

        auto formulaEvaluator{ std::make_unique<fe::FormulaEvaluator>() };

        formulaEvaluator->Init();
        formulaEvaluator->Start();
    }
    catch ( [[maybe_unused]] const std::exception& e )
    {
        return 1;
    }

    return 0;
}

// -----------------------------------------------------------------------------
