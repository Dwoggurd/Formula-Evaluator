// ============================================================================
// Formula Evaluator
// ============================================================================
// Formulas parser
// 
// @author Dwoggurd (2023)
// ============================================================================

#include <list>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/apply_visitor.hpp>
#include "FormulaParser.h"
#include "../Utilities/LoggingUtilities.h"

// ----------------------------------------------------------------------------
namespace x3 = boost::spirit::x3;

namespace fe
{

// ----------------------------------------------------------------------------
// Formula parser logic is desinged to be locally incapsulated
// Thus, it doesn't use knowledge of external data structures and declarations
// and doesn't export its own structures and declarations
// ----------------------------------------------------------------------------
namespace ast
{

enum class Opcode
{
    Pos,
    Neg,
    Add,
    Sub,
    Mul,
    Div
};

struct Unary;
struct Expression;

struct Operand : x3::variant<
    double
    , std::string
    , x3::forward_ast<Unary>
    , x3::forward_ast<Expression>
    >
{
    using base_type::base_type;
    using base_type::operator=;
};

struct Unary
{
    Opcode   opcode;
    Operand  operand;
};

struct Binary
{
    Opcode   opcode;
    Operand  operand;
};

struct Expression
{
    Operand            operand;
    std::list<Binary>  rest;
};

struct Equation
{
    std::string  name;
    Expression   expression;
};

} // namespace ast

// ----------------------------------------------------------------------------
// BOOST_FUSION_ADAPT_STRUCT macro should be used at global scope
// thus, temporarily leaving "fe" namespace
// ----------------------------------------------------------------------------
} // namespace fe

BOOST_FUSION_ADAPT_STRUCT( fe::ast::Unary,
    opcode, operand
)

BOOST_FUSION_ADAPT_STRUCT( fe::ast::Binary,
    opcode, operand
)

BOOST_FUSION_ADAPT_STRUCT( fe::ast::Expression,
    operand, rest
)

BOOST_FUSION_ADAPT_STRUCT( fe::ast::Equation,
    name, expression
)

// ----------------------------------------------------------------------------
// re-entering "fe" namespace
namespace fe
{

// ----------------------------------------------------------------------------
// AST traverser declaration
// ----------------------------------------------------------------------------
class AstTraverser
{
public:
    explicit AstTraverser( CalculatorSlot* const slot );
    AstTraverser( const AstTraverser& )            = delete;
    AstTraverser& operator=( const AstTraverser& ) = delete;
    AstTraverser( AstTraverser&& )                 = delete;
    AstTraverser& operator=( AstTraverser&& )      = delete;
    ~AstTraverser()                                = default;

    void operator()( const ast::Equation &x ) const;
    void operator()( const ast::Expression& x ) const;
    void operator()( const ast::Unary& x ) const;
    void operator()( const ast::Binary& x ) const;
    void operator()( const std::string& x ) const;
    void operator()( const double& x ) const;

private:
    CalculatorSlot* const  slot;
};

// ----------------------------------------------------------------------------
// AST traverser definition
// ----------------------------------------------------------------------------
AstTraverser::AstTraverser( CalculatorSlot* const slot ) : slot( slot )
{

}

// ----------------------------------------------------------------------------
void AstTraverser::operator()( const ast::Equation& x ) const
{
    (*this)( x.expression );
}

// ----------------------------------------------------------------------------
void AstTraverser::operator()( const ast::Expression& x ) const
{
    boost::apply_visitor( *this, x.operand );
    for ( const auto& op : x.rest )
    {
        (*this)( op );
    }
}

// ----------------------------------------------------------------------------
void AstTraverser::operator()( const ast::Unary& x ) const
{
    boost::apply_visitor( *this, x.operand );
    switch ( x.opcode )
    {
        case ast::Opcode::Pos: slot->PushPos(); break;
        case ast::Opcode::Neg: slot->PushNeg(); break;
        default: break;
    }
}

// ----------------------------------------------------------------------------
void AstTraverser::operator()( const ast::Binary& x ) const
{
    boost::apply_visitor( *this, x.operand );
    switch ( x.opcode )
    {
        case ast::Opcode::Add: slot->PushAdd(); break;
        case ast::Opcode::Sub: slot->PushSub(); break;
        case ast::Opcode::Mul: slot->PushMul(); break;
        case ast::Opcode::Div: slot->PushDiv(); break;
        default: break;
    }
}

// ----------------------------------------------------------------------------
void AstTraverser::operator()( const std::string& x ) const
{
    slot->PushVariable( x );
}

// ----------------------------------------------------------------------------
void AstTraverser::operator()( const double& x ) const
{
    slot->PushLiteral( x );
}

// ----------------------------------------------------------------------------
// Formula grammar
// ----------------------------------------------------------------------------
namespace FParser
{
namespace ascii = boost::spirit::x3::ascii;

x3::symbols<ast::Opcode>  unaryOpcode;
x3::symbols<ast::Opcode>  additiveOpcode;
x3::symbols<ast::Opcode>  multiplicativeOpcode;

typedef std::string  VariableType;
typedef double       LiteralType;

x3::rule<class VariableClass, VariableType> const       variable( "variable" );
x3::rule<class LiteralClass, LiteralType> const         literal( "literal" );
x3::rule<class TermClass, ast::Expression> const        term( "term" );
x3::rule<class FactorClass, ast::Operand> const         factor( "factor" );
x3::rule<class ExpressionClass, ast::Expression> const  expression( "expression" );
x3::rule<class EquationClass, ast::Equation> const      equation( "equation" );
x3::rule<class SkipperClass> const                      skipper( "skipper" );

auto const  literal_def =
    x3::double_;

auto const  variable_def =
    x3::raw[x3::lexeme[(ascii::alpha | '_') >> *(ascii::alnum | '_')]];

auto const  term_def =
    factor >> *(multiplicativeOpcode > factor);

auto const  expression_def =
    term >> *(additiveOpcode > term);

// Matches a formula or empty input
auto const  equation_def =
    x3::omit[x3::eoi] | variable >> '=' >> expression;

auto const  factor_def =
    literal
    | variable
    | (unaryOpcode > factor)
    | '(' > expression > ')';

auto const  skipper_def =
    x3::omit[ascii::space | ("#" >> *(ascii::char_ - x3::eol) >> (x3::eol | x3::eoi))];

BOOST_SPIRIT_DEFINE(
    variable
    , literal
    , term
    , factor
    , expression
    , equation
    , skipper
);

// ----------------------------------------------------------------------------
void  BuildOpcodeTables()
{
    static bool  built{ false };

    if ( !built )
    {
        unaryOpcode.add
            ( "+", ast::Opcode::Pos )
            ( "-", ast::Opcode::Neg );

        additiveOpcode.add
            ( "+", ast::Opcode::Add )
            ( "-", ast::Opcode::Sub );

        multiplicativeOpcode.add
            ( "*", ast::Opcode::Mul )
            ( "/", ast::Opcode::Div) ;

        built = true;
    }
}

// ----------------------------------------------------------------------------
auto const&  parser()
{
    BuildOpcodeTables();
    return equation;
}

// ----------------------------------------------------------------------------
} // namespace FParser

// ----------------------------------------------------------------------------
// FormulaParser definition
// ----------------------------------------------------------------------------
FormulaParser::FormulaParser( Calculator &calc ) : calculator( calc )
{
}

// ----------------------------------------------------------------------------
void FormulaParser::Parse( const std::string_view s )
{
    auto        current{ s.cbegin() };
    const auto  end{ s.cend() };

    try
    {
        while ( current != end )
        {
            ast::Equation  res;

            if ( x3::phrase_parse( current, end, FParser::parser(), FParser::skipper, res ) )
            {
                if ( res.name.size() > 0 )
                {
                    LOG( 5, "Formula parsing succeeded: " << res.name );
                    UploadFormula( res );
                }
            }
            else
            {
                throw std::invalid_argument( "Formula parsing failed." );
            }
        }
    }
    catch( [[maybe_unused]] const std::exception &e )
    {
        throw;
    }
}

// ----------------------------------------------------------------------------
void FormulaParser::UploadFormula( const auto &f )
{
    CalculatorSlot* const  slot{ calculator.CreateSlot( f.name ) };

    if ( slot )
    {
        LOG( 5, "Uploading formula: " << f.name );

        AstTraverser  traverser{ slot };
        traverser( f );

        LOG( 5, slot );
    }
    LOG( 5, "" );
}

// ----------------------------------------------------------------------------
} // namespace fe
// ----------------------------------------------------------------------------
