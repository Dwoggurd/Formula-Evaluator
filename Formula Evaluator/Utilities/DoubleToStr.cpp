// ============================================================================
// Formula Evaluator
// ============================================================================
// Wrapper function that converts double to std::string
// Intended as workaround until std::format is fully supported
// 
// @author Dwoggurd (2023)
// ============================================================================

#include "DoubleToStr.h"

// ----------------------------------------------------------------------------
#if __has_include(<format>) && defined(__cpp_lib_format)
#include <format>
std::string fe::DoubleToStr( const double value, const unsigned int precision )
{
    return std::format( "{:.{}f}", value, precision );
}
// ----------------------------------------------------------------------------
#else
#include <sstream>
#include <iomanip>
std::string fe::DoubleToStr( const double value, const unsigned int precision )
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision( precision ) << value;
    return ss.str();
}
#endif
// ----------------------------------------------------------------------------
