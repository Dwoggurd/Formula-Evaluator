# Formula Evaluator
This is a small exercise project aimed at testing the fundamental concepts of the modern C++ language and STL/Boost libraries.

## What it does
This program implements the (classic) task of parsing textual mathematical formulas and evaluating them on input data.

Input: 
- One or several files with math formulas that support basic operators +,-,*,/,() and variables
- One or several files with datasets that assign values to variables in formulas

Output:
- Evaluates formulas on each dataset and prints out results

## Used features and technogolies

#### C++
- various features up to C++20 (OOP, STL, lambdas, smart pointers, etc)

#### Boost 1.81.0
- spirit x3 (parsing lib)
- options (program options lib)
- multi-index container (STL-like multi-index container)
- log (logging lib)

#### Visual Studio 2022 and CMake

## Usage
`FormulaEvaluator.exe --help` - prints supported program options

`--cfg <pathname>` - config file with options. See [Config/cfg.txt](Config/cfg.txt) for example.

`--formulas <pathname>` - file with named math formulas consisting of basic operators +,-,*,/,unary +-, braces, variables and double precision literals.
Spaces are ignored.
Example:
```
formula1 = (a + b - 2) * (1.1 - 0.3E+2) + +d/c + 4/-2
```
See [Config/formulas.example.txt](Config/formulas.example.txt) for more examples.

`--data <pathname>` - file with datasets that provide values for variables used in formulas. 
Each dataset is a comma separated list of pairs: **"variable=doublevalue"**.
Variables not found in datasets will be zero'd.
Spaces are ignored.
Example:
```
a=1, b=4.1E-1, c=-2.1, d=3E+2
```
See [Config/data.example.txt](Config/data.example.txt) for more examples.

`--log-level <level>` - Sets logging level (0-5).


