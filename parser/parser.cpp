#include "parser.h"

Program* Parser::run(vector<Token*> inputToken) {
    this->tokens = std::move(inputToken);
    initialization();

    try {
        while (currentReadPoint < tokens.size()) {
            if (tokens[currentReadPoint]->tokenType == TokenType::END_OF_FILE) {
                break;
            }
            if (tokens[currentReadPoint]->tokenType == TokenType::NEW_LINE) {
                setNextToken();
                continue;
            }

                Statement* statement = parseStatement(); // parseStatement가 끝나면 currentToken의 타입은 NEW_LINE이 된다.
                program->statements.push_back(statement);
                setNextToken(); // NEW_LINE 스킵
        }
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return program;
}

void Parser::initialization() {
    program = new Program;
    currentReadPoint = 0;
    nextReadPoint = 1;
    currentToken = tokens[currentReadPoint];
    nextToken = tokens[nextReadPoint];
}

void Parser::setNextToken() {
    currentReadPoint++;
    nextReadPoint++;
    currentToken = nextToken;
    nextToken = nextReadPoint < tokens.size() ? tokens[nextReadPoint] : nullptr;
}

void Parser::skipSpaceToken() {
    if (currentToken->tokenType != TokenType::SPACE) {
        throw invalid_argument("skipSpaceToken: SPACE 토큰이 없습니다.");
    }
    setNextToken();
}

Statement* Parser::parseStatement() {
    if (currentToken->tokenType == TokenType::LBRACKET) {
        return parseLetStatement();
    }
    else if (currentToken->tokenType == TokenType::RETURN) {
        return parseReturnStatement();
    }
//    else if (currentToken->tokenType == TokenType::INT) {
//        return parseIntegerStatement();
//    }
    else if (currentToken->tokenType == TokenType::IDENTIFIER) {
        return parseAssignStatement();
    }
    else {
        return parseExpressionStatement();
    }
}

LetStatement* Parser::parseLetStatement() {
    LetStatement* letStatement = new LetStatement;

    if (currentToken->tokenType != TokenType::LBRACKET) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 LBRACKET이 아닙니다.");
    }
    setNextToken();

    if (currentToken->tokenType != TokenType::INT) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 자료형이 아닙니다.");
    }
    letStatement->token = currentToken;
    setNextToken();

    letStatement->isConst = (currentToken->tokenType == TokenType::BANG);
    if (currentToken->tokenType == TokenType::BANG) {
        setNextToken();
    }

    if (currentToken->tokenType != TokenType::RBRACKET) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 RBRACKET이 아닙니다.");
    }
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

    skipSpaceToken();

    returnStatement->returnValue = parseExpression(Precedence::LOWEST);

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
    AssignStatement* assignStatement = new AssignStatement;

    if (currentToken->tokenType != TokenType::IDENTIFIER) {
        throw invalid_argument("parseAssignStatement: IDENTIFIER가 아닙니다.");
    }
    assignStatement->name = dynamic_cast<IdentifierExpression *>(parseIdentifierExpression());
    setNextToken();

    skipSpaceToken();

    if (currentToken->tokenType != TokenType::ASSIGN) {
        throw invalid_argument("parseAssignStatement: ASSIGN이 아닙니다.");
    }
    assignStatement->token = currentToken;
    setNextToken();

    skipSpaceToken();

    assignStatement->value = parseExpression(Precedence::LOWEST);

    return assignStatement;
}

ExpressionStatement* Parser::parseExpressionStatement() {
    ExpressionStatement* expressionStatement = new ExpressionStatement{currentToken, parseExpression(Precedence::LOWEST)};

//    while (currentToken->tokenType != TokenType::NEW_LINE) {
//        setNextToken();
//    }

    return expressionStatement;
}

BlockStatement* Parser::parseBlockStatement() {
    BlockStatement* blockStatement = new BlockStatement;

    if (currentToken->tokenType != TokenType::STARTBLOCK) {
        throw invalid_argument("parseBlockStatement: STARTBLOCK이 아닙니다.");
    }
    setNextToken();

    while (currentToken->tokenType != TokenType::ENDBLOCK) {
        Statement* statement = parseStatement();
        blockStatement->statements.push_back(statement);
        setNextToken();
        setNextToken();// NEW_LINE 스킵
    }

    return blockStatement;
}

Expression* Parser::parseExpression(Precedence precedence) {
    if (prefixParseFunctions.find(currentToken->tokenType) == prefixParseFunctions.end()) {
        throw invalid_argument("parseExpression: 찾는 prefixParseFunction이 존재하지 않습니다.");
    }

    prefixParseFunction prefixFunction = prefixParseFunctions[currentToken->tokenType];
    Expression* leftExpression = (this->*prefixFunction)();

    // infix 연산자가 있을 때는 SPACE가 있다고 가정, infix 연산자가 없을 때 SPACE가 있는 경우는 없는 지 고민할 것
    if (nextToken->tokenType == TokenType::SPACE) { // SPACE가 아니면 NEW_LINE이 와야할 것 (코드 한 줄의 끝에 의미 없는 공백이 오면 안됨)
        setNextToken();
    }

    // RBRACKET은 if문 같은 경우에 해당
    while ((nextToken->tokenType != TokenType::NEW_LINE && nextToken->tokenType != TokenType::ENDBLOCK) && precedence < getPrecedence[nextToken->tokenType]) {
        if (infixParseFunctions.find(nextToken->tokenType) == infixParseFunctions.end()) {
            throw invalid_argument("parseExpression: 찾는 infixParseFunction이 존재하지 않습니다.");
        }
        infixParseFunction  infixFunction = infixParseFunctions[nextToken->tokenType];
        setNextToken();

        leftExpression = (this->*infixFunction)(leftExpression);
    }

    return leftExpression;
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

Expression* Parser::parseIfExpression() {
    IfExpression* ifExpression = new IfExpression;

    if (currentToken->tokenType != TokenType::IF) {
        throw invalid_argument("parseIfExpression: IF가 아닙니다.");
    }
    ifExpression->token = currentToken;
    setNextToken();

    if (currentToken->tokenType != TokenType::COLON) {
        throw invalid_argument("parseIfExpression: COLON이 아닙니다.");
    }
    setNextToken();

    skipSpaceToken();

    if (currentToken->tokenType != TokenType::LPAREN) {
        throw invalid_argument("parseIfExpression: LPAREN이 아닙니다.");
    }
    setNextToken();

    ifExpression->condition = parseExpression(Precedence::LOWEST);
    setNextToken();

    if (currentToken->tokenType != TokenType::RPAREN) {
        throw invalid_argument("parseIfExpression: RPAREN이 아닙니다.");
    }
    setNextToken();

//    skipSpaceToken();
//
//    if (currentToken->tokenType != TokenType::END_IF) {
//        throw invalid_argument("parseIfExpression: END_IF가 아닙니다.");
//    }
//    setNextToken();

    if (currentToken->tokenType != TokenType::NEW_LINE) {
        throw invalid_argument("parseIfExpression: NEW_LINE이 아닙니다.");
    }
    setNextToken();

    ifExpression->consequence = parseBlockStatement();

    if (currentToken->tokenType != TokenType::ENDBLOCK) { // ENDBLOCK
        throw invalid_argument("parseIfExpression: ENDBLOCK이 아닙니다.");
    }
//    setNextToken();

    if (nextToken->tokenType == TokenType::ELSE) {
        // 나중에 검사 엄밀히 할 것(필수!)
        setNextToken(); // current: ENDBLOCK, next: ELSE
        setNextToken(); // current: ELSE, next: COLON
        setNextToken(); // current: COLON,     next: NEW_LINE
        setNextToken(); // current: NEW_LINE, next: STARTBLOCK

        ifExpression->alternative = parseBlockStatement();

        if (currentToken->tokenType != TokenType::ENDBLOCK) { // ENDBLOCK
            throw invalid_argument("parseIfExpression: ENDBLOCK이 아닙니다.");
        }
    }

    return ifExpression;
}

Expression* Parser::parseIntegerLiteral() {
    return new IntegerLiteral{currentToken, stoll(currentToken->literal)};
}

Expression* Parser::parseBooleanLiteral() {
    return new BooleanLiteral{currentToken, (currentToken->tokenType == TokenType::TRUE)};
}

Expression* Parser::parseFunctionLiteral() {
    FunctionLiteral* functionLiteral = new FunctionLiteral;

    if (currentToken->tokenType != TokenType::FUNCTION) {
        throw invalid_argument("parseFunctionLiteral: FUNCTION이 아닙니다.");
    }
    functionLiteral->token = currentToken;
    setNextToken();

    if (currentToken->tokenType != TokenType::COLON) {
        throw invalid_argument("parseFunctionLiteral: COLON이 아닙니다.");
    }
    setNextToken();

    skipSpaceToken();

    functionLiteral->parameters = parseFunctionParameters();

    skipSpaceToken();

    functionLiteral->name = dynamic_cast<IdentifierExpression *>(parseIdentifierExpression());

    skipSpaceToken();

    if (currentToken->tokenType != TokenType::RIGHTARROW) {
        throw invalid_argument("parseFunctionLiteral: RIGHT ARROW가 아닙니다.");
    }
    setNextToken();

    skipSpaceToken();

    if (currentToken->tokenType != TokenType::LBRACKET) {
        throw invalid_argument("parseFunctionLiteral: LBRACKET이 아닙니다.");
    }
    setNextToken();

    if (currentToken->tokenType != TokenType::INT) {
        throw invalid_argument("parseFunctionLiteral: 리턴 타입이 잘못되었습니다.");
    }
    functionLiteral->returnType = currentToken;
    setNextToken();

    if (currentToken->tokenType != TokenType::RBRACKET) {
        throw invalid_argument("parseFunctionLiteral: RBRACKET이 아닙니다.");
    }
    setNextToken();

    if (currentToken->tokenType != TokenType::NEW_LINE) {
        throw invalid_argument("parseFunctionLiteral: NEW_LINE이 아닙니다.");
    }
    setNextToken();

    functionLiteral->blockStatement = parseBlockStatement();

    if (currentToken->tokenType != TokenType::ENDBLOCK) {
        throw invalid_argument("parseFunctionLiteral: END BLOCK이 아닙니다.");
    }

    return functionLiteral;
}


vector<IdentifierExpression*> Parser::parseFunctionParameters() {
    vector<IdentifierExpression*> identifiers;

    if (currentToken->tokenType == TokenType::SPACE) {
        return identifiers;
    }

    while (nextToken->tokenType != TokenType::COMMA) {
        IdentifierExpression* identifier = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
        identifiers.push_back(identifier);

        // 여기도 나중에 엄밀히 검증할 것
        setNextToken(); // current: identifier, next: COMMA
        setNextToken(); // current: COMMA, next: SPACE
        setNextToken(); // current: SPACE, next: identifier
    }

    return identifiers;
}