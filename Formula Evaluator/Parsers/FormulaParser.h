// =============================================================================
// Formula Evaluator
// =============================================================================
// Formulas parser 
// 
// Uses boost::spirit::x3 library for parsing
// 
// Formulas have syntax : <name>=<expression>
// 
// <name>       : may have letters, digits, underscore and starts with non-digit
// <expression> : may have variables, (double)literals, braces
//     allowed operators: unary+ , unary- , binary + - * /
// 
// Formula allowed to span over multiple lines, spaces are ignored.
// If several formulas have the same name, the first one is used.
// Hash symbol ("#") marks a comment until the end of the line
//
// @author Dwoggurd (2023-2024)
// =============================================================================

#ifndef FormulaParser_H
#define FormulaParser_H

// -----------------------------------------------------------------------------
#include <string>
#include <string_view>
#include "../Calculator/Calculator.h"

// -----------------------------------------------------------------------------
namespace fe
{
// -----------------------------------------------------------------------------
// FormulaParser declaration
// -----------------------------------------------------------------------------
class FormulaParser
{
public:
    explicit FormulaParser( Calculator& calc );
    FormulaParser( const FormulaParser& )            = delete;
    FormulaParser& operator=( const FormulaParser& ) = delete;
    FormulaParser( FormulaParser&& )                 = delete;
    FormulaParser& operator=( FormulaParser&& )      = delete;
    ~FormulaParser()                                 = default;

    void  Parse( const std::string_view s );

private:
    Calculator&  calculator;

    void  UploadFormula( const auto& f );
};

// -----------------------------------------------------------------------------
} // namespace fe
// -----------------------------------------------------------------------------
#endif // FormulaParser_H
