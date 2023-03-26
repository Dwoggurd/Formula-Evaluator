// ============================================================================
// Formula Evaluator
// ============================================================================
// Dataset type
//
// @author Dwoggurd (2023)
// ============================================================================

#include <sstream>
#include "Dataset.h"

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
std::string  DumpToStr( Dataset set )
{
    std::ostringstream  ss;
    auto                it{ set->cbegin() };

    if ( it != set->cend() )
    {
        ss << it->first << '=' << it->second;
        ++it;
    }
    while ( it != set->cend() )
    {
        ss << ", " << it->first << '=' << it->second;
        ++it;
    }
    
    // Could be implemented by using std::exchange (from #include <utility>)
    // 
    // const char* delim{ "" };
    // for ( const auto& [key, value] : *set )
    // {
    //     ss << std::exchange( delim, "," ) << key << '=' << value;
    // }

    return ss.str();
}

// ----------------------------------------------------------------------------
} // namespace fe

