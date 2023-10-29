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
        exception/exception.h
        evaluator/evaluator.cpp
        evaluator/evaluator.h
        object/object.h
        ast/literals/integerLiteral.h
        ast/literals/literal.h
        ast/literals/booleanLiteral.h
        ast/statements/expressionStatement.h
        ast/statements/letStatement.h
        ast/statements/returnStatement.h
        ast/statements/ifStatement.h
        ast/statements/blockStatement.h
        object/environment.h
        ast/expressions/functionExpression.h
        ast/literals/functionLiteral.h
        code/code.h
        compiler/compiler.h
        compiler/compiler.cpp
        endian/endian.h
        endian/endian.cpp
        code/code.cpp
        vm/virtualMachine.cpp
        vm/virtualMachine.h
        compiler/symbolTable.cpp
        compiler/symbolTable.h
        ast/literals/stringLiteral.h
        object/objectType.h
        ast/literals/arrayLiteral.h
        ast/expressions/indexExpression.h
        ast/statements/loopStatement.h
        ast/statements/classStatement.h
)
