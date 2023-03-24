// ============================================================================
// Formula Evalualor
// ============================================================================
// Root class
// 
// @author Dwoggurd (2023)
// ============================================================================

#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <exception>
#include "FormulaEvaluator.h"
#include "Utilities/LoggingUtilities.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// FormulaEvaluator definition
// ----------------------------------------------------------------------------
FormulaEvaluator::FormulaEvaluator() : formulaParser( calculator )
{
}

// ----------------------------------------------------------------------------
FormulaEvaluator::~FormulaEvaluator()
{
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::Init( int argc, char* argv[] )
{
    LOG( 0, "--------------------------------" );
    LOG( 0, "|       Formula Evaluator      |" );
    LOG( 0, "--------------------------------" );

    options.ParseOptions( argc, argv );
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::Start()
{
    try
    {
        ParseFormulas();
        ParseData();
    }
    catch ( const std::exception& e )
    {
        LOG( 0, e.what() );
        LOG( 0, "Stopping..." );
        throw;
    }

    Run();
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::Run()
{
    LOG( 1, "" );
    LOG( 1, "Starting Calculator..." );
    LOG( 1, "=============================" );

    while ( !dataQ.empty() )
    {
        calculator.Reset();
        calculator.LoadData( dataQ.front() );
        calculator.Calculate();
        calculator.PrintResults();

        dataQ.pop();
    }
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::ParseFormulas()
{
    for ( const auto &fn : options.GetFormulaFiles() )
    {
        LOG( 5, "" );
        LOG( 5, "Formulas from: " << fn );

        std::string s = LoadFileToStr( fn );

        LOG( 5, "---------------------------------------------------\n" << s );
        LOG( 5, "---------------------------------------------------" );

        formulaParser.Parse( s );
    }
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::ParseData()
{
    for ( const auto& fn : options.GetDataFiles() )
    {
        if ( dataParser.InitDataSource( fn ) )
        {
            while ( true )
            {
                auto set = std::make_shared<DatasetT>();

                try
                {
                    LOG( 5, "-------------------------" );

                    dataParser.ParseDataset( set );

                    if ( set->size() == 0 )
                    {
                        break;
                    }

                    dataQ.push( set );

                    LOG( 5, "Parsed dataset:" );
                    for ( const auto& [key, value] : *set )
                    {
                        LOG( 5, "    " << key << '=' << value );
                    }
                }
                catch ( [[maybe_unused]] const std::exception& e )
                {
                    dataParser.StopDataSource();
                    throw;
                }
            }
            dataParser.StopDataSource();
        }
    }
}

// ----------------------------------------------------------------------------
std::string FormulaEvaluator::LoadFileToStr( const std::string &fn ) const
{
    std::ifstream f( fn );

    if ( f )
    {
        std::ostringstream ss;
        ss << f.rdbuf();
        return ss.str();
    }
    else
    {
        LOG( 1, "Failed to open formulas source: " << fn );
        return "";
    }
}

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------

