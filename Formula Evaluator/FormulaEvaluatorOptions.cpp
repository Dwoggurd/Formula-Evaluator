// ============================================================================
// Formula Evaluator
// ============================================================================
// Parse program options
// 
// @author Dwoggurd (2023)
// ============================================================================

#include <exception>
#include <boost/program_options.hpp>
#include "FormulaEvaluatorOptions.h"
#include "Utilities/LoggingUtilities.h"

// ----------------------------------------------------------------------------
namespace po = boost::program_options;

namespace fe
{
FormulaEvaluatorOptions  programOptions;

// ----------------------------------------------------------------------------
FormulaEvaluatorOptions::FormulaEvaluatorOptions() : parallel( false ), mtRandomSleeps( false )
{
    SetLogLevel( 1 );
}

// ----------------------------------------------------------------------------
void  FormulaEvaluatorOptions::ParseOptions( int argc, char* argv[] )
{
    po::options_description  config( "Command line or Config file options" );
    config.add_options()
        ( "formulas",         po::value<std::vector<std::string>>()->composing()->required(), "Formulas pathname" )
        ( "data",             po::value<std::vector<std::string>>()->composing()->required(), "Data pathname" )
        ( "log-level",        po::value<unsigned int>()->default_value( 1 ), "Log level: 0-5, 0 - minimal logging, 5 - full logging" )
        ( "parallel",         po::value<bool>( &parallel )->default_value( true ), "Enable multi-threading (true/false)" )
        ( "mt-random-sleeps", po::value<bool>( &mtRandomSleeps )->default_value( false ), "Add random sleeps for threads (true/false)" )
        ;

    po::options_description  cmdline( "Command line options" );
    cmdline.add_options()
        ( "help", "Usage info" )
        ( "cfg", po::value<std::vector<std::string>>()->composing(), "Config pathname" )
        ;
    cmdline.add( config );

    po::variables_map  vm;

    try
    {
        po::store( po::parse_command_line( argc, argv, cmdline ), vm );

        SetLogLevel( vm["log-level"].as<unsigned int>() );

        LOG( 5, "Parsing command line options..." );

        if ( vm.count( "cfg" ) )
        {
            for ( const auto  &fname : vm["cfg"].as<std::vector<std::string>>() )
            {
                LOG( 5, "Parsing config file: " << fname );

                po::store( po::parse_config_file( fname.c_str(), config ), vm );
            }
        }

        // Special case: handle "--help" option before notify(vm)
        // This is to avoid exceptions from required options (which could be ommitted when --help is used) 
        if ( vm.count( "help" ) )
        {
            throw std::exception( "Program usage:" );
        }
        notify( vm );

        SetLogLevel( vm["log-level"].as<unsigned int>() );

        formulaFiles = vm["formulas"].as<std::vector<std::string>>();
        dataFiles    = vm["data"].as<std::vector<std::string>>();

        // Print pathnames for input sources
        LOG( 1, "Formulas from:" );
        for ( const auto& fname : GetFormulaFiles() )
        {
            LOG( 1, "    " << fname );
        }

        LOG( 1, "Data from:" );
        for ( const auto& fname : GetDataFiles() )
        {
            LOG( 1, "    " << fname );
        }
    }
    catch( [[maybe_unused]] std::exception &e )
    {
        LOG( 0, e.what() );
        LOG( 0, cmdline );
        throw;
    }
}

// ----------------------------------------------------------------------------
void  FormulaEvaluatorOptions::SetLogLevel( unsigned int x ) const
{
    if ( x > 5 )
    {
        LOG( 3, "Supported log levels: 0-5. Provided: " << x << ". Setting to 5." );
        x = 5;
    }

    boost::log::core::get()->set_filter(
        [x]( const boost::log::attribute_value_set& attrs )
        { 
            auto level = attrs["Severity"].extract<boost::log::trivial::severity_level>();
            return level && *level >= static_cast<boost::log::trivial::severity_level>(boost::log::trivial::fatal - x);
        } );
}

// ----------------------------------------------------------------------------
} // namespace fe
