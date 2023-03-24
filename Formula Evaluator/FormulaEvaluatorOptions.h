// ============================================================================
// Formula Evalualor
// ============================================================================
// Parse program options.
// 
// Program usage:
// Command line options:
//     --help                 Usage info
//     --cfg arg              Config pathname
//
// Command line or Config file options:
//     --formulas arg         Formulas pathname
//     --data arg             Data pathname
//     --log-level arg(=1)    Log level : 0-5, 0 - minimal logging, 5 - full logging
// 
// @author Dwoggurd (2023)
// ============================================================================

#ifndef FormulaEvaluatorOptions_H
#define FormulaEvaluatorOptions_H

// ----------------------------------------------------------------------------
#include <vector>
#include <string>

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// FormulaEvaluatorOptions declaration
// ----------------------------------------------------------------------------
class FormulaEvaluatorOptions
{
public:
    FormulaEvaluatorOptions();
    FormulaEvaluatorOptions( const FormulaEvaluatorOptions& )            = delete;
    FormulaEvaluatorOptions& operator=( const FormulaEvaluatorOptions& ) = delete;
    FormulaEvaluatorOptions( FormulaEvaluatorOptions&& )                 = delete;
    FormulaEvaluatorOptions& operator=( FormulaEvaluatorOptions&& )      = delete;
    ~FormulaEvaluatorOptions();

    void  ParseOptions( int argc, char* argv[] );

    inline std::vector<std::string>  GetFormulaFiles() const { return formulaFiles; };
    inline std::vector<std::string>  GetDataFiles() const { return dataFiles; };

private:
    std::vector<std::string>  formulaFiles;
    std::vector<std::string>  dataFiles;

    void  SetLogLevel( unsigned int x ) const;
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // FormulaEvaluatorOptions_H
