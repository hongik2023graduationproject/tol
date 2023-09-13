#include "parser.h"

Parser::Parser() {
    prefixParseFunctions.insert({TokenType::IDENTIFIER, &Parser::parseIdentifierExpression});
    prefixParseFunctions.insert({TokenType::INTEGER, &Parser::parseIntegerLiteral});
    prefixParseFunctions.insert({TokenType::BANG, &Parser::parsePrefixExpression});
    prefixParseFunctions.insert({TokenType::MINUS, &Parser::parsePrefixExpression});
    prefixParseFunctions.insert({TokenType::TRUE, &Parser::parseBooleanLiteral});
    prefixParseFunctions.insert({TokenType::FALSE, &Parser::parseBooleanLiteral});
    prefixParseFunctions.insert({TokenType::LPAREN, &Parser::parseGroupedExpression});

    infixParseFunctions.insert({TokenType::PLUS, &Parser::parseInfixExpression});
    infixParseFunctions.insert({TokenType::MINUS, &Parser::parseInfixExpression});
    infixParseFunctions.insert({TokenType::ASTERISK, &Parser::parseInfixExpression});
    infixParseFunctions.insert({TokenType::SLASH, &Parser::parseInfixExpression});
    infixParseFunctions.insert({TokenType::EQUAL, &Parser::parseInfixExpression});
    infixParseFunctions.insert({TokenType::NOT_EQUAL, &Parser::parseInfixExpression});
    infixParseFunctions.insert({TokenType::LESS_THAN, &Parser::parseInfixExpression});
}


void Parser::Parse() {
    program.statements.clear();
    setNextToken();
    setNextToken();

    while (currentToken->tokenType != TokenType::NEW_LINE) { // 추후에 END_OF_FILE로 바꿀 것
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
    nextToken = lexer->getToken(); // try 지금은 끝일 경우 에러가 아니라 특정 토큰 반환 중
}

void Parser::skipSpaceToken() {
    if (currentToken->tokenType != TokenType::SPACE) {
        throw invalid_argument("skipSpaceToken: SPACE 토큰이 없습니다.");
    }
    setNextToken();
}

Statement* Parser::parseStatement() {
    if (currentToken->tokenType == TokenType::LET) {
        return parseLetStatement();
    }
    else if (currentToken->tokenType == TokenType::RETURN) {
        return parseReturnStatement();
    }
    else if (currentToken->tokenType == TokenType::INT) {
        return parseIntegerStatement();
    }
    else if (currentToken->tokenType == TokenType::IDENTIFIER && nextToken->tokenType == TokenType::ASSIGN) {
        return parseAssignStatement();
    }
    else if (currentToken->tokenType == TokenType::IF) {
        
    }
    else {
        return parseExpressionStatement();
    }
}

LetStatement* Parser::parseLetStatement() {
    LetStatement* letStatement = new LetStatement;

    if (currentToken->tokenType != TokenType::LET) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 LET이 아닙니다.");
    }
    letStatement->token = currentToken;
    setNextToken();

    skipSpaceToken();

    if (currentToken->tokenType != TokenType::IDENTIFIER) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 IDENTIFIER가 아닙니다.");
    }
    letStatement->name = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    setNextToken();

    skipSpaceToken();

    if (currentToken->tokenType != TokenType::ASSIGN) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 ASSIGN이 아닙니다.");
    }
    setNextToken();

    skipSpaceToken();

    letStatement->expression = parseExpression(Precedence::LOWEST);

    return letStatement;
}

ReturnStatement* Parser::parseReturnStatement() {
    ReturnStatement* returnStatement = new ReturnStatement;
    returnStatement->token = currentToken;
    setNextToken();

    while (currentToken->tokenType != TokenType::NEW_LINE) {
        setNextToken();
    }

    return returnStatement;
}

IntegerStatement* Parser::parseIntegerStatement() {
//    if (currentToken->tokenType != TokenType::INT)
//        throw invalid_argument("parseIntegerStatement: 토큰 타입이 INT가 아닙니다.");
//
//    IntegerStatement* integerStatement = new IntegerStatement;
//    integerStatement->token = currentToken;
//    setNextToken();
//
//    skipSpaceToken();
//
//    integerStatement->identifierExpression = parseIdentifierExpression();
//    setNextToken();
//
//    skipSpaceToken();
//
//    if (currentToken->tokenType != TokenType::ASSIGN)
//        throw invalid_argument("parseIntegerStatement: 토큰 타입이 ASSIGN이 아닙니다.");
//    integerStatement->equal = currentToken;
//    setNextToken();
//
//    skipSpaceToken();
//
//    integerStatement->Expression = parseExpression(Precedence::LOWEST);
//    setNextToken();
//
//    return integerStatement;
}

Expression* Parser::parseIdentifierExpression() {
    if (currentToken->tokenType != TokenType::IDENTIFIER)
        throw invalid_argument("parseIdentifierExpression: 토큰 타입이 IDENTIFIER가 아닙니다.");

    IdentifierExpression* identifierExpression = new IdentifierExpression{currentToken, currentToken->literal};

    return identifierExpression;
}

Expression* Parser::parseIntegerExpression() {
    if (currentToken->tokenType == TokenType::IDENTIFIER) { // identifier type checking eval 단계에서 하기

    }
    if (currentToken->tokenType != TokenType::INTEGER)
        throw invalid_argument("parseIntegerExpression: 토큰 타입이 INTEGER가 아닙니다.");

    IntegerExpression* integerExpression = new IntegerExpression{currentToken, stoll(currentToken->literal)};

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

ExpressionStatement* Parser::parseExpressionStatement() {
    ExpressionStatement* expressionStatement = new ExpressionStatement{currentToken, parseExpression(Precedence::LOWEST)};

    while (currentToken->tokenType != TokenType::NEW_LINE) {
        setNextToken();
    }

    return expressionStatement;
}

Expression* Parser::parseExpression(Precedence precedence) {
    if (prefixParseFunctions.find(currentToken->tokenType) == prefixParseFunctions.end()) {
        throw invalid_argument("parseExpression: 찾는 prefixParseFunction이 존재하지 않습니다.");
    }

    prefixParseFunction prefixFunction = prefixParseFunctions[currentToken->tokenType];
    Expression* leftExpression = (this->*prefixFunction)();

    // infix 연산자가 있을 때는 SPACE가 있다고 가정, infix 연산자가 없을 때 SPACE가 있는 경우는 없는 지 고민할 것
    if (nextToken->tokenType == TokenType::SPACE) { // 버그 발생의 여지가 높은 코드
        setNextToken();
    }

    while (nextToken->tokenType != TokenType::NEW_LINE && precedence < getPrecedence[nextToken->tokenType]) {
        if (infixParseFunctions.find(nextToken->tokenType) == infixParseFunctions.end()) {
            throw invalid_argument("parseExpression: 찾는 infixParseFunction이 존재하지 않습니다.");
        }
        infixParseFunction  infixFunction = infixParseFunctions[nextToken->tokenType];
        setNextToken();

        leftExpression = (this->*infixFunction)(leftExpression);
    }

    return leftExpression;
}

Expression* Parser::parseIntegerLiteral() {
    return new IntegerLiteral{currentToken, stoll(currentToken->literal)};
}

Expression* Parser::parseBooleanLiteral() {
    return new BooleanLiteral{currentToken, (currentToken->tokenType == TokenType::TRUE)};
}

Expression* Parser::parsePrefixExpression() {
    PrefixExpression* prefixExpression = new PrefixExpression;
    prefixExpression->token = currentToken;

    setNextToken();

    prefixExpression->right = parseExpression(Precedence::PREFIX);

    return prefixExpression;
}

Expression* Parser::parseInfixExpression(Expression *left) {
    InfixExpression* infixExpression = new InfixExpression;
    infixExpression->token = currentToken;
    infixExpression->left = left;

    Precedence precedence = getPrecedence[currentToken->tokenType];
    setNextToken();

    skipSpaceToken();

    infixExpression->right = parseExpression(precedence);

    return infixExpression;
}

Expression* Parser::parseGroupedExpression() {
    Expression* expression = new Expression;

    if (currentToken->tokenType != TokenType::LPAREN) {
        throw invalid_argument("parseGroupedExpression: LPAREN이 아닙니다.");
    }
    setNextToken();

    expression = parseExpression(Precedence::LOWEST);

    if (nextToken->tokenType != TokenType::RPAREN) {
        throw invalid_argument("parseGroupedExpression: RPAREN이 아닙니다.");
    }
    setNextToken();

    return expression;
}