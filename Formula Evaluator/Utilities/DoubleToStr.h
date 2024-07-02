// =============================================================================
// Formula Evaluator
// =============================================================================
// Wrapper function that converts double to std::string
// Intended as workaround until std::format is fully supported
// 
// @author Dwoggurd (2023-2024)
// =============================================================================

#ifndef DoubleToStr_H
#define DoubleToStr_H

// -----------------------------------------------------------------------------
#include <string>

// -----------------------------------------------------------------------------
namespace fe
{
extern std::string DoubleToStr( const double value, const unsigned int precision );
} // namespace fe
// -----------------------------------------------------------------------------
#endif // DoubleToStr_H
