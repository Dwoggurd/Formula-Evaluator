// ============================================================================
// Formula Evalualor
// ============================================================================
// Data stream parser
// 
// @author Dwoggurd (2023)
// ============================================================================

#include <vector>
#include <exception>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include "DataParser.h"
#include "../Utilities/LoggingUtilities.h"

// ----------------------------------------------------------------------------
namespace x3 = boost::spirit::x3;

namespace fe
{

// ----------------------------------------------------------------------------
// Data stream grammar
// ----------------------------------------------------------------------------
namespace DParser
{
typedef std::string                                        IdentifierType;
typedef double                                             ValueType;
typedef std::vector<std::pair<IdentifierType, ValueType>>  DatasetType;

x3::rule<class DatasetClass, DatasetType> const        dataset( "dataset" );
x3::rule<class IdentifierClass, IdentifierType> const  identifier( "identifier" );
x3::rule<class ValueClass, ValueType> const            value( "value" );
x3::rule<class SkipperClass> const                     skipper( "skipper" );

auto const  value_def = 
    x3::double_;

auto const  identifier_def = 
    x3::raw[x3::lexeme[(x3::ascii::alpha | '_') >> *(x3::ascii::alnum | '_')]];

// Matches a dataset or empty input
auto const  dataset_def = 
    x3::omit[x3::eoi] | (identifier >> '=' >> value) % ',';

auto const  skipper_def = 
    x3::omit[x3::ascii::space | ("#" >> *(x3::ascii::char_ - x3::eol) >> (x3::eol | x3::eoi))];

BOOST_SPIRIT_DEFINE(
    dataset
    , identifier
    , value
    , skipper
);


// ----------------------------------------------------------------------------
} // namespace DParser

// ----------------------------------------------------------------------------
// DataParser definition
// ----------------------------------------------------------------------------
DataParser::DataParser() : current( inIter() ), end( inIter() )
{
}

// ----------------------------------------------------------------------------
DataParser::~DataParser()
{
}

// ----------------------------------------------------------------------------
bool DataParser::InitDataSource( const std::string &fname )
{
    bool  good{ true };

    LOG( 5, "" );
    LOG( 5, "Initializing data source: " << fname );
    dataSource.open( fname );

    if ( good = dataSource.good() )
    {
         current = inIter( dataSource >> std::noskipws );
    }
    else
    {
        LOG( 1, "Failed to open data source: " << fname );
    }

    return good;
}

// ----------------------------------------------------------------------------
void DataParser::StopDataSource()
{
    dataSource.close();
}

// ----------------------------------------------------------------------------
void DataParser::ParseDataset( Dataset set )
{
    if ( current != end )
    {
        try
        {
            if ( !x3::phrase_parse( current, end, DParser::dataset, DParser::skipper, (*set) ) )
            {
                throw std::invalid_argument( "Dataset parsing failed." );
            }
        }
        catch ( [[maybe_unused]] const std::exception& x )
        {
            throw;
        }
    }
    else
    {
        LOG( 5, "Data source exhausted." );
    }
}

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
