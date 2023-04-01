// ============================================================================
// Formula Evaluator
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
#include "Utilities/ThreadRandomSleep.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// FormulaEvaluator definition
// ----------------------------------------------------------------------------
FormulaEvaluator::FormulaEvaluator() : formulaParser( calculator ), dataQeoi( false )
{
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::Init()
{
    if ( programOptions.Parallel() )
    {
        LOG( 3, "Enabled multi-threading. hardware_concurrency() = " << std::thread::hardware_concurrency() );
        calculator.EnableParallel();
    }
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::Start()
{
    try
    {
        ParseFormulas();
    }
    catch ( const std::exception& e )
    {
        LOG( 0, e.what() );
        LOG( 0, "Stopping..." );
        throw;
    }

    LOG( 1, "" );
    LOG( 1, "Starting Calculator..." );
    LOG( 1, "=============================" );

    if ( programOptions.Parallel() )
    {
        RunMT();
    }
    else
    {
        RunST();
    }
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::RunST()
{
    ParseDataST();

    while ( !dataQ.empty() )
    {
        calculator.Calculate( std::move( dataQ.front() ) );
        dataQ.pop();
        calculator.PrintResults();
    }
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::RunMT()
{
    ThreadRandomSleep pause( 10, 2000 );

    std::thread dataThread( &FormulaEvaluator::ParseDataMT, this );

    std::unique_lock<std::mutex> lock( dataQmutex );
    while ( true )
    {
        if ( dataQ.empty() )
        {
            if ( dataQeoi )
            {
                break;
            }

            dataQcv.wait( lock, [this]() { return !this->dataQ.empty() || dataQeoi; } );

            // Finishing if wait() stopped on dataQeoi but still no new data
            if ( dataQ.empty() && dataQeoi )
            {
                break;
            }
        }
        Dataset set{ std::move( dataQ.front() ) };
        dataQ.pop();
        lock.unlock();
        calculator.Calculate( std::move( set ) );
        calculator.PrintResults();
        pause.Sleep();
        lock.lock();
    }
    lock.unlock();
    dataThread.join();
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::ParseFormulas()
{
    for ( const auto &fn : programOptions.GetFormulaFiles() )
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
void FormulaEvaluator::ParseDataST()
{
    try
    {
        for ( const auto& fn : programOptions.GetDataFiles() )
        {
            dataParser.InitDataSource( fn );
            while ( true )
            {
                auto set = std::make_shared<DatasetT>();

                dataParser.ParseDataset( set );

                if ( set->size() == 0 )
                {
                    break;
                }

                LOG( 3, "Parsed dataset: " << DumpToStr( set ) );
                dataQ.push( std::move( set ) );
            }
            dataParser.StopDataSource();
        }
    }
    catch ( [[maybe_unused]] const std::exception& e )
    {
        dataParser.StopDataSource();
        LOG( 0, e.what() );
    }
    LOG( 5, "No more data." );
}

// ----------------------------------------------------------------------------
void FormulaEvaluator::ParseDataMT()
{
    ThreadRandomSleep pause( 200, 2000 );

    try
    {
        std::unique_lock<std::mutex> lock( dataQmutex );
        lock.unlock();

        for ( const auto& fn : programOptions.GetDataFiles() )
        {
            dataParser.InitDataSource( fn );
            while ( true )
            {
                auto set = std::make_shared<DatasetT>();

                dataParser.ParseDataset( set );

                if ( set->size() == 0 )
                {
                    break;
                }

                LOG( 3, "+++ Parsed dataset: " << DumpToStr( set ) );
                lock.lock();
                dataQ.push( std::move( set ) );
                lock.unlock();
                dataQcv.notify_one();
                pause.Sleep();
            }
            dataParser.StopDataSource();
        }
    }
    catch ( [[maybe_unused]] const std::exception& e )
    {
        dataParser.StopDataSource();
        LOG( 0, e.what() );
    }
    LOG( 5, "No more data." );
    dataQeoi = true;
    dataQcv.notify_one();
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
