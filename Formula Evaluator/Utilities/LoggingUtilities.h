// ============================================================================
// Formula Evalualor
// ============================================================================
// Macros for logging.
// 
// Define DISABLE_LOGGING to remove logging code at compile time
// 
// @author Dwoggurd (2023)
// ============================================================================

#ifndef LoggingUtilities_H
#define LoggingUtilities_H

// ----------------------------------------------------------------------------
// TODO:
// Logging is a bit hacky for now.
// Implement more complex logging scheme.
// Using boost trivial logging as a starting point.
// "fatal" level mimics program console output
// Lower log levels are intended for tracing
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

// ----------------------------------------------------------------------------
// #define DISABLE_LOGGING

#ifdef DISABLE_LOGGING
#define LOG(...)
#else
// boost::log::trivial defines these severity levels:
// 
// enum severity_level
// {
//     trace,
//     debug,
//     info,
//     warning,
//     error,
//     fatal
// };

// Supports 0-5 levels (0 - minimal logging, 5 - full logging)
#define LOG( LVL, message )                                                               \
        BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),                \
        (::boost::log::keywords::severity = static_cast<boost::log::trivial::severity_level>(boost::log::trivial::fatal - LVL))) \
        << message
#endif

// ----------------------------------------------------------------------------
#endif // LoggingUtilities_H
