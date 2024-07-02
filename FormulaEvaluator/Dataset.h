// =============================================================================
// Formula Evaluator
// =============================================================================
// Dataset type
//
// @author Dwoggurd (2023-2024)
// =============================================================================

#ifndef Dataset_H
#define Dataset_H

// -----------------------------------------------------------------------------
#include <memory>
#include <map>
#include <string>

// -----------------------------------------------------------------------------
namespace fe
{
typedef std::map<std::string, double>  DatasetT;
typedef std::shared_ptr<DatasetT>      Dataset;

extern std::string  DumpToStr( Dataset set );
} // namespace fe
// -----------------------------------------------------------------------------
#endif // Dataset_H
