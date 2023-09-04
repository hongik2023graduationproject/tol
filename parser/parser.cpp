#include "parser.h"

void Parser::Parse() {
    program.statements.clear();
    setNextToken();
    setNextToken();

    while (currentToken->tokenType != TokenType::END_OF_FILE) {
        try {
            Statement *statement = parseStatement();
            program.statements.push_back(statement);
            setNextToken();
        } catch (const exception& e) {
            cout << e.what() << endl;
        }
    }
}

void Parser::setNextToken() {
    currentToken = nextToken;
    nextToken = lexer->getToken(); // try
}

void Parser::skipSpaceToken() {
    if (currentToken->tokenType != TokenType::SPACE) {
        throw invalid_argument("skipSpaceToken: SPACE 토큰이 없습니다.");
    }
    setNextToken();
}

Statement* Parser::parseStatement() {
    if (currentToken->tokenType == TokenType::INT) {
        return parseIntegerStatement();
    }
    else if (currentToken->tokenType == TokenType::IDENTIFIER && nextToken->tokenType == TokenType::ASSIGN) {
        return parseAssignStatement();
    }

}

IntegerStatement* Parser::parseIntegerStatement() {
    if (currentToken->tokenType != TokenType::INT)
        throw invalid_argument("parseIntegerStatement: 토큰 타입이 INT가 아닙니다.");

    IntegerStatement* integerStatement = new IntegerStatement;
    integerStatement->token = currentToken;
    setNextToken();

    skipSpaceToken();

    integerStatement->identifierExpression = parseIdentifierExpression();
    setNextToken();

    skipSpaceToken();

    if (currentToken->tokenType != TokenType::ASSIGN)
        throw invalid_argument("parseIntegerStatement: 토큰 타입이 ASSIGN이 아닙니다.");
    integerStatement->equal = currentToken;
    setNextToken();

    skipSpaceToken();

    integerStatement->integerExpression = parseIntegerExpression();
    setNextToken();

    return integerStatement;
}

IdentifierExpression* Parser::parseIdentifierExpression() {
    if (currentToken->tokenType != TokenType::IDENTIFIER)
        throw invalid_argument("parseIdentifierExpression: 토큰 타입이 IDENTIFIER가 아닙니다.");

    IdentifierExpression* identifierExpression = new IdentifierExpression;
    identifierExpression->token = currentToken;

    return identifierExpression;
}

IntegerExpression* Parser::parseIntegerExpression() {
    if (currentToken->tokenType == TokenType::IDENTIFIER) { // identifier type checking eval 단계에서 하기

    }
    if (currentToken->tokenType != TokenType::INTEGER)
        throw invalid_argument("parseIntegerExpression: 토큰 타입이 INTEGER가 아닙니다.");

    IntegerExpression* integerExpression = new IntegerExpression;
    integerExpression->token = currentToken;

    return integerExpression;
}

 AssignStatement* Parser::parseAssignStatement() {
//    AssignStatement* assignStatement = new AssignStatement;
//
//    assignStatement->name = parseIdentifierExpression();
//
//
//    assignStatement->token = nextToken;
//
//    return assignStatement;
}


//Expression* Parser::parseExpression(int precedence) {
//    if (prefixParseFunctions.find(currentToken) == prefixParseFunctions.end()) {
//        throw invalid_argument("parseExpression: 찾는 prefixParseFunction이 존재하지 않습니다.");
//    }
//
//    prefixParseFunction prefixFunction = prefixParseFunctions[currentToken];
//    Expression* leftExpression = prefixFunction();
//
//    return leftExpression;
//}