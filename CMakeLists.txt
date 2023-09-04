cmake_minimum_required(VERSION 3.26)
project(tolelom)

set(CMAKE_CXX_STANDARD 20)

add_executable(tolelom main.cpp
        repl/repl.cpp
        repl/repl.h
        lexer/lexer.cpp
        lexer/lexer.h
        token/token.h
        token/tokenType.h
        utf8Converter/utf8Converter.cpp
        utf8Converter/utf8Converter.h
        parser/parser.cpp
        parser/parser.h
        ast/node.h
        ast/expressions/expression.h
        ast/statements/statement.h
        ast/statements/assignStatement.h
        ast/expressions/identifierExpression.h
        ast/expressions/prefixExpression.h
        ast/expressions/infixExpression.h
        ast/program.h
        ast/expressions/integerExpression.h
        ast/statements/integerStatement.h
        exception/exception.h
        evaluator/evaluator.cpp
        evaluator/evaluator.h
        object/object.h
        ast/literal/integerLiteral.h
        ast/literal/literal.h
        ast/literal/booleanLiteral.h
)
