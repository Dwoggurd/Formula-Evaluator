#------------------------------------------------------------------------------
# Formula Evaluator
#------------------------------------------------------------------------------
# CMake project for Formula Evaluator
#
# @author Dwoggurd (2023-2024)
#------------------------------------------------------------------------------

if (MSVC)
  message("MSVC=${MSVC}")
  set (BOOST_ROOT "C:/Program Files/boost/boost_1_81_0/")
  find_package(Boost REQUIRED)
else()
  message("UNIX=${UNIX}")
  find_package(Boost 1.70 REQUIRED COMPONENTS program_options log)
endif()
link_directories(${Boost_LIBRARY_DIRS})

# Add source to this project's executable.
add_executable (FormulaEvaluator
    main.cpp
    FormulaEvaluator.h FormulaEvaluator.cpp
    FormulaEvaluatorOptions.h FormulaEvaluatorOptions.cpp
    Dataset.h Dataset.cpp
    Parsers/FormulaParser.h Parsers/FormulaParser.cpp
    Parsers/DataParser.h Parsers/DataParser.cpp
    Calculator/Calculator.h Calculator/Calculator.cpp
    Calculator/CalculatorOperator.h Calculator/CalculatorOperator.cpp
    Calculator/CalculatorSlot.h Calculator/CalculatorSlot.cpp
    Utilities/LoggingUtilities.h 
    Utilities/ThreadRandomSleep.h Utilities/ThreadRandomSleep.cpp 
    Utilities/DoubleToStr.h Utilities/DoubleToStr.cpp)
    
target_include_directories(FormulaEvaluator PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(FormulaEvaluator ${Boost_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FormulaEvaluator PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
