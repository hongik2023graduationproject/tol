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
    else if (currentToken->tokenType == TokenType::CLASS) {
        return parseClassStatement();
    }
    else if (currentToken->tokenType == TokenType::FUNCTION) {
        return parseFunctionStatement();
    }
    else {
        return parseExpressionStatement();
    }
}

LetStatement* Parser::parseLetStatement() {
    LetStatement* letStatement = new LetStatement;

    skipToken(TokenType::LBRACKET); // [

    if (currentToken->tokenType != TokenType::INT && currentToken->tokenType != TokenType::STR && currentToken->tokenType != TokenType::IDENTIFIER) {
        throw NotFoundToken(currentToken->line, TokenType::INT ,currentToken);
    }
    letStatement->token = currentToken;
    setNextToken();

    // const check
    letStatement->isConst = (currentToken->tokenType == TokenType::BANG);
    if (currentToken->tokenType == TokenType::BANG) {
        skipToken(TokenType::BANG);
    }

    skipToken(TokenType::RBRACKET); // ]
    skipToken(TokenType::SPACE);

    if (currentToken->tokenType != TokenType::IDENTIFIER) {
        throw NotFoundToken(currentToken->line, TokenType::IDENTIFIER, currentToken);
    }
    letStatement->name = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    skipToken(TokenType::IDENTIFIER);

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
    setNextToken();

    return returnStatement;
}

AssignStatement* Parser::parseAssignStatement() {
    AssignStatement* assignStatement = new AssignStatement;

    if (currentToken->tokenType != TokenType::IDENTIFIER) {
        throw NotFoundToken(currentToken->line, TokenType::IDENTIFIER, currentToken);
    }
    assignStatement->name = dynamic_cast<IdentifierExpression *>(parseIdentifierExpression());
    assignStatement->token = currentToken;
    skipToken(TokenType::IDENTIFIER);

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
    skipToken(TokenType::IF);

    skipToken(TokenType::COLON);
    skipToken(TokenType::SPACE);
    skipToken(TokenType::LPAREN);

    ifStatement->condition = parseExpression(Precedence::LOWEST);
    setNextToken();

    skipToken(TokenType::RPAREN);
    skipToken(TokenType::NEW_LINE);

    ifStatement->consequence = parseBlockStatement();

    // elif
//    if (currentToken->tokenType == TokenType::ELSE && nextToken->tokenType == TokenType::SPACE && nextnextToken->tokenType == TokenType::IF) {
//        skipToken(TokenType::ELSE);
//        skipToken(TokenType::SPACE);
//        ifStatement->alternative = parseBlockStatement();
//    }
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

    Statement* firstStatement = parseStatement();
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
        if (currentToken->tokenType == TokenType::NEW_LINE) // 이게 조건으로 들어갈 이유가 있나?? statement가 연속해서 한 줄에 2개 나오는 경우가?
            skipToken(TokenType::NEW_LINE);
    }

    skipToken(TokenType::ENDBLOCK);
    return blockStatement;
}

ClassStatement* Parser::parseClassStatement() {
    ClassStatement* classStatement = new ClassStatement;

    skipToken(TokenType::CLASS);
    skipToken(TokenType::SPACE);

    classStatement->name = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    skipToken(TokenType::IDENTIFIER);
    skipToken(TokenType::NEW_LINE);


    // block statement 대신에 수정 중
    skipToken(TokenType::STARTBLOCK);
    while (currentToken->tokenType != TokenType::ENDBLOCK) {
        Statement *statement = parseStatement();
        skipToken(TokenType::NEW_LINE);
        if (LetStatement* letStatement = dynamic_cast<LetStatement*>(statement)) {
            classStatement->variables.push_back(letStatement);
        }
        else if (FunctionStatement* functionStatement = dynamic_cast<FunctionStatement*>(statement)) {
            classStatement->functions.push_back(functionStatement);
        }
        else {
            throw invalid_argument("클래스 내 잘못된 statement가 있습니다.");
        }
    }
    skipToken(TokenType::ENDBLOCK);
    // 수정 부분의 끝

//    classStatement->block = parseBlockStatement();

    return classStatement;
}

ClassInitStatement* Parser::parseClassInitStatement() {
    ClassInitStatement* classInitStatement = new ClassInitStatement;
    classInitStatement->token = currentToken;

    skipToken(TokenType::DOT);
    classInitStatement->name = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    skipToken(TokenType::IDENTIFIER);
    skipToken(TokenType::COLON);
    skipToken(TokenType::SPACE);
    classInitStatement->value = parseExpression(Precedence::LOWEST);
    setNextToken();

    return classInitStatement;
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
    if (nextToken->tokenType == TokenType::COMMA || nextToken->tokenType == TokenType::SPACE)
        functionExpression->arguments = parseFunctionExpressionParameters(); // 파라미터가 없을 경우 생각해야 함

    functionExpression->function = parseExpression(Precedence::LOWEST);
    skipToken(TokenType::IDENTIFIER);

    return functionExpression;
}

Expression* Parser::parseClassExpression() {
    ClassExpression* classExpression = new ClassExpression;
    classExpression->token = currentToken;
    skipToken(TokenType::LBRACE);
    skipToken(TokenType::NEW_LINE);

    if (currentToken->tokenType == TokenType::STARTBLOCK) {
        skipToken(TokenType::STARTBLOCK);
        while (currentToken->tokenType != TokenType::ENDBLOCK) {
            ClassInitStatement *statement = parseClassInitStatement();
            classExpression->statements.push_back(statement);
            skipToken(TokenType::NEW_LINE);
        }
        skipToken(TokenType::ENDBLOCK);
    }

    return classExpression;
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







Expression* Parser::parseFloatLiteral() {
	return new FloatLiteral{currentToken, stod(currentToken->literal)};
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
    arrayLiteral->token = currentToken; // "["
    arrayLiteral->elements = parseExpressionList(TokenType::RBRACKET);

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

//    if (currentToken->tokenType == TokenType::SPACE) {
//        return identifiers;
//    }

    while (nextToken->tokenType == TokenType::COMMA) { // 중요: 이 루프에서 타입도 같이 받아서 넘겨야 함
//        skipToken(TokenType::LBRACKET);
        // ??????????? 타입 넘기는 부분
//        skipToken(TokenType::RBRACKET);
//        skipToken(TokenType::SPACE);
        IdentifierExpression* identifier = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
        identifiers.push_back(identifier);
        skipToken(TokenType::IDENTIFIER);
        skipToken(TokenType::COMMA);
        skipToken(TokenType::SPACE);
    }

    IdentifierExpression* identifier = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    identifiers.push_back(identifier);
    skipToken(TokenType::IDENTIFIER);

    return identifiers;
}

FunctionStatement* Parser::parseFunctionStatement() {
    FunctionStatement* functionStatement = new FunctionStatement;

    if (currentToken->tokenType != TokenType::FUNCTION) {
        throw invalid_argument("parseFunctionStatement: FUNCTION이 아닙니다.");
    }
    functionStatement->token = currentToken;
    setNextToken();

    skipToken(TokenType::COLON);
    skipToken(TokenType::SPACE);

    if (nextToken->tokenType != TokenType::DOT) {
        functionStatement->parameters = parseFunctionParameters();
        skipToken(TokenType::SPACE);
    }

    functionStatement->name = dynamic_cast<IdentifierExpression *>(parseIdentifierExpression());
    skipToken(TokenType::IDENTIFIER);
    skipToken(TokenType::DOT);
    skipToken(TokenType::SPACE);

    skipToken(TokenType::RIGHTARROW);
    skipToken(TokenType::SPACE);
    skipToken(TokenType::LBRACKET);

    // 리턴 타입이 없는 경우도 생각할 것
    if (currentToken->tokenType != TokenType::RBRACKET) {
        if (currentToken->tokenType != TokenType::INT && currentToken->tokenType != TokenType::STR) {
            throw invalid_argument("parseFunctionStatement: 리턴 타입이 잘못되었습니다.");
        }
        functionStatement->returnType = currentToken;
        setNextToken();
    }

    skipToken(TokenType::RBRACKET);
    skipToken(TokenType::NEW_LINE);

    functionStatement->blockStatement = parseBlockStatement();

    return functionStatement;
}