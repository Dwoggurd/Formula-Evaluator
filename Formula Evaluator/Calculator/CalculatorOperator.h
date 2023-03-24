// ============================================================================
// Formula Evaluator
// ============================================================================
// Calculator operators
//
// Implements functors that operate on calculator slot stack based machine
// 
// @author Dwoggurd (2023)
// ============================================================================

#ifndef CalculatorOperator_H
#define CalculatorOperator_H

// ----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <functional>

// ----------------------------------------------------------------------------
namespace fe
{

// ----------------------------------------------------------------------------
// CalculatorOperatorBase declaration
// ----------------------------------------------------------------------------
class CalculatorOperatorBase;
// ----------------------------------------------------------------------------
typedef class std::shared_ptr<CalculatorOperatorBase>  Operator;
// ----------------------------------------------------------------------------
class CalculatorOperatorBase
{
public:
    CalculatorOperatorBase();
    CalculatorOperatorBase( const CalculatorOperatorBase& )            = delete;
    CalculatorOperatorBase& operator=( const CalculatorOperatorBase& ) = delete;
    CalculatorOperatorBase( CalculatorOperatorBase&& )                 = delete;
    CalculatorOperatorBase& operator=( CalculatorOperatorBase&& )      = delete;
    virtual ~CalculatorOperatorBase();

    virtual void  operator()() = 0;

protected:
    friend std::ostream& operator<<( std::ostream& os, const Operator x );

    virtual const std::string  Dump() const = 0;
};


// ----------------------------------------------------------------------------
// CalculatorOperator declaration
// ----------------------------------------------------------------------------
class CalculatorOperator : public CalculatorOperatorBase
{
public:
    explicit CalculatorOperator( const std::string &tag, std::function<void()> cb );
    CalculatorOperator( const CalculatorOperator& )            = delete;
    CalculatorOperator& operator=( const CalculatorOperator& ) = delete;
    CalculatorOperator( CalculatorOperator&& )                 = delete;
    CalculatorOperator& operator=( CalculatorOperator&& )      = delete;
    ~CalculatorOperator() final;

    void operator()() final;

protected:
    const std::string  Dump() const final;

private:
    const std::string            tag;
    const std::function<void()>  cb;
};

// ----------------------------------------------------------------------------
// CalculatorVariable declaration
// ----------------------------------------------------------------------------
class CalculatorVariable : public CalculatorOperatorBase
{
public:
    explicit CalculatorVariable( const std::string &name, std::function<void(double)> cb );
    CalculatorVariable( const CalculatorVariable& )            = delete;
    CalculatorVariable& operator=( const CalculatorVariable& ) = delete;
    CalculatorVariable( CalculatorVariable&& )                 = delete;
    CalculatorVariable& operator=( CalculatorVariable&& )      = delete;
    ~CalculatorVariable() final;

    void operator()() final;
    void Reset();

    inline std::string  Name() const { return name; };
    inline double       GetValue() const { return value; }
    inline void         SetValue( const double x ) { value = x; }

protected:
    const std::string  Dump() const final;

private:
    const std::string            name;
    double                       value;
    std::function<void(double)>  cb;
};

// ----------------------------------------------------------------------------
// CalculatorLiteral declaration
// ----------------------------------------------------------------------------
class CalculatorLiteral : public CalculatorOperatorBase
{
public:
    explicit CalculatorLiteral( const double val, std::function<void(double)> cb );
    CalculatorLiteral( const CalculatorLiteral& )            = delete;
    CalculatorLiteral& operator=( const CalculatorLiteral& ) = delete;
    CalculatorLiteral( CalculatorLiteral&& )                 = delete;
    CalculatorLiteral& operator=( CalculatorLiteral&& )      = delete;
    ~CalculatorLiteral() final;

    void operator()() final;

protected:
    const std::string  Dump() const final;

private:
    const double                 value;
    std::function<void(double)>  cb;
};

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
#endif // CalculatorOperator_H
