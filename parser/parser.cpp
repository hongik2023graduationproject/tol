#include "parser.h"

// statement는 끝까지 처리 (마지막에 EOF 두 번)
// expression은 마지막 처리 x (중위 연산자 처리에 필요)

Program* Parser::run(vector<Token*> inputToken) {
    this->tokens = std::move(inputToken);
    initialization();

    try {
        while (currentReadPoint < tokens.size()) {
            if (currentToken->tokenType == TokenType::END_OF_FILE) {
                break;
            }
            else if (currentToken->tokenType == TokenType::NEW_LINE) {
                skipToken(TokenType::NEW_LINE);
                continue;
            }
            else {
                Statement *statement = parseStatement();
                program->statements.push_back(statement);
                if (currentToken->tokenType == TokenType::NEW_LINE)
                    skipToken(TokenType::NEW_LINE);
            }
        }
    } catch (const CustomException& e) {
        e.print();
    }

    return program;
}

void Parser::initialization() {
    program = new Program;
    currentReadPoint = 0;
    nextReadPoint = 1;
    nextnextReadPoint = 2;
    currentToken = tokens[currentReadPoint];
    nextToken = tokens[nextReadPoint];
    nextnextToken = tokens[nextnextReadPoint];
}

void Parser::setNextToken() {
    currentReadPoint++;
    nextReadPoint++;
    nextnextReadPoint++;
    currentToken = nextToken;
    nextToken = nextnextToken;
    nextnextToken = nextnextReadPoint < tokens.size() ? tokens[nextnextReadPoint] : nullptr;
}

void Parser::skipToken(TokenType tokenType) {
    if (currentToken->tokenType != tokenType) {
        throw NotFoundToken(currentToken->line, tokenType, currentToken);
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
    else if (currentToken->tokenType == TokenType::IDENTIFIER && nextToken->tokenType == TokenType::SPACE && nextnextToken->tokenType == TokenType::ASSIGN) {
        return parseAssignStatement();
    }
	else if (currentToken->tokenType == TokenType::IF) {
		return parseIfStatement();
	}
	else if (currentToken->tokenType == TokenType::LOOP) {
		return parseLoopStatement();
	}
    else {
        return parseExpressionStatement();
    }
}

LetStatement* Parser::parseLetStatement() {
    LetStatement* letStatement = new LetStatement;

    skipToken(TokenType::LBRACKET); // [

    // type token
    if (currentToken->tokenType != TokenType::INT && currentToken->tokenType != TokenType::STR) {
        throw NotFoundToken(currentToken->line, TokenType::INT ,currentToken);
    }
    letStatement->token = currentToken;
    setNextToken();

    // const check
    letStatement->isConst = (currentToken->tokenType == TokenType::BANG);
    if (currentToken->tokenType == TokenType::BANG) {
        setNextToken();
    }

    skipToken(TokenType::RBRACKET); // ]
    skipToken(TokenType::SPACE); // ' '

    // identifier
    if (currentToken->tokenType != TokenType::IDENTIFIER) {
        throw NotFoundToken(currentToken->line, TokenType::IDENTIFIER, currentToken);
    }
    letStatement->name = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    setNextToken();

    skipToken(TokenType::SPACE);
    skipToken(TokenType::ASSIGN);
    skipToken(TokenType::SPACE);

    letStatement->expression = parseExpression(Precedence::LOWEST);

    setNextToken();
    return letStatement;
}

ReturnStatement* Parser::parseReturnStatement() {
    ReturnStatement* returnStatement = new ReturnStatement;
    returnStatement->token = currentToken;
    setNextToken();

    skipToken(TokenType::SPACE);

    returnStatement->returnValue = parseExpression(Precedence::LOWEST);

    return returnStatement;
}

AssignStatement* Parser::parseAssignStatement() {
    AssignStatement* assignStatement = new AssignStatement;

    if (currentToken->tokenType != TokenType::IDENTIFIER) {
        throw NotFoundToken(currentToken->line, TokenType::IDENTIFIER, currentToken);
    }
    assignStatement->name = dynamic_cast<IdentifierExpression *>(parseIdentifierExpression());
    assignStatement->token = currentToken;
    setNextToken();

    skipToken(TokenType::SPACE);
    skipToken(TokenType::ASSIGN);
    skipToken(TokenType::SPACE);

    assignStatement->value = parseExpression(Precedence::LOWEST);

    setNextToken();
    return assignStatement;
}


IfStatement* Parser::parseIfStatement() {
    IfStatement* ifStatement = new IfStatement;

    if (currentToken->tokenType != TokenType::IF) {
        throw NotFoundToken(currentToken->line, TokenType::IF, currentToken);
    }
    ifStatement->token = currentToken;
    setNextToken();

    skipToken(TokenType::COLON);
    skipToken(TokenType::SPACE);
    skipToken(TokenType::LPAREN);

    ifStatement->condition = parseExpression(Precedence::LOWEST);
    setNextToken();

    skipToken(TokenType::RPAREN);
    skipToken(TokenType::NEW_LINE);

    ifStatement->consequence = parseBlockStatement();

    if (currentToken->tokenType == TokenType::ELSE) {
        skipToken(TokenType::ELSE);
        skipToken(TokenType::COLON);
        skipToken(TokenType::NEW_LINE);

        ifStatement->alternative = parseBlockStatement();
    }

    return ifStatement;
}

LoopStatement* Parser::parseLoopStatement() {
    LoopStatement* loopStatement = new LoopStatement;

    if (currentToken->tokenType != TokenType::LOOP) {
        throw NotFoundToken(currentToken->line, TokenType::LOOP, currentToken);
    }
    loopStatement->token = currentToken;
    setNextToken();

    skipToken(TokenType::COLON);
    skipToken(TokenType::SPACE);
    skipToken(TokenType::LPAREN);

    Statement* firstStatement = parseStatement(); // for와 while의 분기점
    if(ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(firstStatement)){ // while
        loopStatement->condition = expressionStatement->expression;
    }
    else{ // for
        loopStatement->initialization = firstStatement;

        skipToken(TokenType::SEMICOLON);
        skipToken(TokenType::SPACE);

        loopStatement->condition = parseExpression(Precedence::LOWEST);
        setNextToken();

        skipToken(TokenType::SEMICOLON);
        skipToken(TokenType::SPACE);

        loopStatement->incrementation = parseAssignStatement();
    }

    skipToken(TokenType::RPAREN);
    skipToken(TokenType::NEW_LINE);

    loopStatement->loopBody = parseBlockStatement();

    return loopStatement;
}

ExpressionStatement* Parser::parseExpressionStatement() {
    ExpressionStatement* expressionStatement = new ExpressionStatement{currentToken, parseExpression(Precedence::LOWEST)};
    setNextToken();
    return expressionStatement;
}

BlockStatement* Parser::parseBlockStatement() {
    BlockStatement* blockStatement = new BlockStatement;

    if (currentToken->tokenType != TokenType::STARTBLOCK) {
        throw NotFoundToken(currentToken->line, TokenType::STARTBLOCK, currentToken);
    }
    setNextToken();

    while (currentToken->tokenType != TokenType::ENDBLOCK) {
        Statement* statement = parseStatement();
        blockStatement->statements.push_back(statement);
        if (currentToken->tokenType == TokenType::NEW_LINE)
            skipToken(TokenType::NEW_LINE);
    }

    skipToken(TokenType::ENDBLOCK);
    return blockStatement;
}








Expression* Parser::parseIdentifierExpression() {
    if (currentToken->tokenType != TokenType::IDENTIFIER)
        throw NotFoundToken(currentToken->line, TokenType::IDENTIFIER, currentToken);

    IdentifierExpression* identifierExpression = new IdentifierExpression{currentToken, currentToken->literal};
    return identifierExpression;
}

Expression* Parser::parseExpression(Precedence precedence) {
    if (prefixParseFunctions.find(currentToken->tokenType) == prefixParseFunctions.end()) {
        throw NotFoundPrefixFunction(currentToken);
    }

    prefixParseFunction prefixFunction = prefixParseFunctions[currentToken->tokenType];
    Expression* leftExpression = (this->*prefixFunction)();

    // infix 연산자가 있을 때는 SPACE가 있다고 가정
    if (nextToken->tokenType == TokenType::SPACE) { // SPACE가 아니면 NEW_LINE이 와야할 것 (코드 한 줄의 끝에 의미 없는 공백이 오면 안됨)
        setNextToken();
    }

    // RBRACKET은 if문 같은 경우에 해당
    while ((nextToken->tokenType != TokenType::NEW_LINE && nextToken->tokenType != TokenType::ENDBLOCK && nextToken->tokenType != TokenType::RBRACKET) && precedence < getPrecedence[nextToken->tokenType]) {
        if (infixParseFunctions.find(nextToken->tokenType) == infixParseFunctions.end()) {
            throw NotFoundInfixFunction(nextToken);
        }
        infixParseFunction infixFunction = infixParseFunctions[nextToken->tokenType];
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

    skipToken(TokenType::SPACE);

    infixExpression->right = parseExpression(precedence);

    return infixExpression;
}

Expression* Parser::parseGroupedExpression() {
    skipToken(TokenType::LPAREN);
    Expression* expression = parseExpression(Precedence::LOWEST);
    skipToken(TokenType::RPAREN);
    return expression;
}

Expression* Parser::parseIndexExpression(Expression* left) {
    IndexExpression* indexExpression = new IndexExpression;
    indexExpression->token = currentToken;
    indexExpression->left = left;
    setNextToken();

    indexExpression->index = parseExpression(Precedence::LOWEST);
    skipToken(TokenType::RBRACKET);

    return indexExpression;
}

Expression* Parser::parseFunctionExpression() {
    skipToken(TokenType::COLON);

    FunctionExpression* functionExpression = new FunctionExpression;
    functionExpression->arguments = parseFunctionExpressionParameters(); // 파라미터가 없을 경우 생각해야 함

    functionExpression->function = parseExpression(Precedence::LOWEST);

    return functionExpression;
}

vector<Expression*> Parser::parseFunctionExpressionParameters() {
    vector<Expression*> expressions;

    while (true) {
        Expression* expression = parseExpression(Precedence::LOWEST);
        expressions.push_back(expression);
        setNextToken();

        if (currentToken->tokenType != TokenType::COMMA)
            break;
        skipToken(TokenType::COMMA);
        skipToken(TokenType::SPACE);
    }

    return expressions;
}









Expression* Parser::parseIntegerLiteral() {
    return new IntegerLiteral{currentToken, stoll(currentToken->literal)};
}

Expression* Parser::parseStringLiteral() {
    return new StringLiteral{currentToken, currentToken->literal};
}

Expression* Parser::parseBooleanLiteral() {
    return new BooleanLiteral{currentToken, (currentToken->tokenType == TokenType::TRUE)};
}


Expression* Parser::parseArrayLiteral() {
    ArrayLiteral* arrayLiteral = new ArrayLiteral;
    arrayLiteral->token = currentToken; // "{"
    arrayLiteral->elements = parseExpressionList(TokenType::RBRACE);

    return arrayLiteral;
}










vector<Expression*> Parser::parseExpressionList(TokenType endTokenType) {
    vector<Expression*> list;

    if (nextToken->tokenType == endTokenType) {
        setNextToken();
        return list;
    }

    setNextToken();
    list.push_back(parseExpression(Precedence::LOWEST));

    while (nextToken->tokenType != endTokenType) {
        setNextToken();
        skipToken(TokenType::COMMA);
        skipToken(TokenType::SPACE);
        list.push_back(parseExpression(Precedence::LOWEST));
    }

    setNextToken();

    return list;
}


vector<IdentifierExpression*> Parser::parseFunctionParameters() {
    vector<IdentifierExpression*> identifiers;

    if (currentToken->tokenType == TokenType::SPACE) {
        return identifiers;
    }

    while (nextToken->tokenType == TokenType::COMMA) {
        IdentifierExpression* identifier = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
        identifiers.push_back(identifier);
        setNextToken(); // current: identifier, next: COMMA
        skipToken(TokenType::COMMA);
        skipToken(TokenType::SPACE);
    }

    IdentifierExpression* identifier = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    identifiers.push_back(identifier);
    setNextToken(); // current: identifier, next: SPACE

    return identifiers;
}

Expression* Parser::parseFunctionLiteral() {
    FunctionLiteral* functionLiteral = new FunctionLiteral;

    if (currentToken->tokenType != TokenType::FUNCTION) {
        throw invalid_argument("parseFunctionLiteral: FUNCTION이 아닙니다.");
    }
    functionLiteral->token = currentToken;
    setNextToken();

    skipToken(TokenType::COLON);
    skipToken(TokenType::SPACE);

    functionLiteral->parameters = parseFunctionParameters();

    skipToken(TokenType::SPACE);

    functionLiteral->name = dynamic_cast<IdentifierExpression *>(parseIdentifierExpression());
    setNextToken(); // skip Identifier Token
    skipToken(TokenType::SPACE);

    skipToken(TokenType::RIGHTARROW);
    skipToken(TokenType::SPACE);
    skipToken(TokenType::LBRACKET);

    // 리턴 타입이 없는 경우도 생각할 것
    if (currentToken->tokenType != TokenType::RBRACKET) {
        if (currentToken->tokenType != TokenType::INT) {
            throw invalid_argument("parseFunctionLiteral: 리턴 타입이 잘못되었습니다.");
        }
        functionLiteral->returnType = currentToken;
        setNextToken();
    }

    skipToken(TokenType::RBRACKET);
    skipToken(TokenType::NEW_LINE);

    functionLiteral->blockStatement = parseBlockStatement();

    return functionLiteral;
}