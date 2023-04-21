// ============================================================================
// Formula Evaluator
// ============================================================================
// Data stream parser
// 
// Uses boost::spirit::x3 library for parsing
// 
// Data stream consists of datasets
// 
// Dataset is a comma separated list of pairs: <variable>=<double>
// 
// <variable> : may have letters, digits, underscore and starts with non-digit
// <double>   : double literal
// 
// Datasets allowed to span over multiple lines, spaces are ignored.
// If several variables have the same name, the first one is used.
// Hash symbol ("#") marks a comment until the end of the line
//
// @author Dwoggurd (2023)
// ============================================================================

#ifndef DataParser_H
#define DataParser_H

// ----------------------------------------------------------------------------
#include <string>
#include <fstream>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include "../Dataset.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// DataParser declaration
// ----------------------------------------------------------------------------
class DataParser
{
public:
    DataParser()                               = default;
    DataParser( const DataParser& )            = delete;
    DataParser& operator=( const DataParser& ) = delete;
    DataParser( DataParser&& )                 = delete;
    DataParser& operator=( DataParser&& )      = delete;
    ~DataParser()                              = default;

    void  InitDataSource( const std::string& fname );
    void  StopDataSource();
    void  ParseDataset( Dataset set );

private:
    typedef boost::spirit::istream_iterator  inIter;

    inIter         current{};
    const inIter   end{};
    std::ifstream  dataSource;
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // DataParser_H
