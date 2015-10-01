
#ifndef PRELUDE_HPP
#define PRELUDE_HPP

#include <vector>


struct Type;
struct Boolean_type;
struct Integer_type;
struct Array_type;
struct Tuple_type;


struct Expr;
struct Constant_expr;
struct Tuple_expr;
struct Copy_init;
struct Direct_init;
struct Aggregate_init;


struct Decl;
struct Variable_decl;


using Type_seq = std::vector<Type const*>;
using Expr_seq = std::vector<Expr const*>;
using Decl_seq = std::vector<Decl const*>;


#endif
