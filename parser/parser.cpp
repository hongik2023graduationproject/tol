#include "parser.h"

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
    currentToken = tokens[currentReadPoint];
    nextToken = tokens[nextReadPoint];
}

void Parser::setNextToken() {
    currentReadPoint++;
    nextReadPoint++;
    currentToken = nextToken;
    nextToken = nextReadPoint < tokens.size() ? tokens[nextReadPoint] : nullptr;
}

void Parser::skipToken(TokenType tokenType) {
    if (currentToken->tokenType != tokenType) {
        throw invalid_argument("");
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
    else if (currentToken->tokenType == TokenType::IDENTIFIER && nextToken->tokenType == TokenType::SPACE) { // SPACE는 임시
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

    if (currentToken->tokenType != TokenType::INT ) {
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

    skipToken(TokenType::SPACE);

    if (currentToken->tokenType != TokenType::IDENTIFIER) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 IDENTIFIER가 아닙니다.");
    }
    letStatement->name = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    setNextToken();

    skipToken(TokenType::SPACE);

    if (currentToken->tokenType != TokenType::ASSIGN) {
        throw invalid_argument("parseLetStatement: 토큰 타입이 ASSIGN이 아닙니다.");
    }
    setNextToken();

    skipToken(TokenType::SPACE);

    letStatement->expression = parseExpression(Precedence::LOWEST);

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

    skipToken(TokenType::SPACE);

    if (currentToken->tokenType != TokenType::ASSIGN) {
        throw invalid_argument("parseAssignStatement: ASSIGN이 아닙니다.");
    }
    assignStatement->token = currentToken;
    setNextToken();

    skipToken(TokenType::SPACE);

    assignStatement->value = parseExpression(Precedence::LOWEST);

    return assignStatement;
}

ExpressionStatement* Parser::parseExpressionStatement() {
    ExpressionStatement* expressionStatement = new ExpressionStatement{currentToken, parseExpression(Precedence::LOWEST)};

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
    while ((nextToken->tokenType != TokenType::NEW_LINE && nextToken->tokenType != TokenType::ENDBLOCK && nextToken->tokenType != TokenType::RBRACKET) && precedence < getPrecedence[nextToken->tokenType]) {
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

    skipToken(TokenType::SPACE);

    infixExpression->right = parseExpression(precedence);

    return infixExpression;
}

Expression* Parser::parseGroupedExpression() {
    if (currentToken->tokenType != TokenType::LPAREN) {
        throw invalid_argument("parseGroupedExpression: LPAREN이 아닙니다.");
    }
    setNextToken();

    Expression* expression = parseExpression(Precedence::LOWEST);

    if (nextToken->tokenType != TokenType::RPAREN) {
        throw invalid_argument("parseGroupedExpression: RPAREN이 아닙니다.");
    }
    setNextToken();

    return expression;
}

IfStatement* Parser::parseIfStatement() {
    IfStatement* ifStatement = new IfStatement;

    if (currentToken->tokenType != TokenType::IF) {
        throw invalid_argument("parseIfStatement: IF가 아닙니다.");
    }
	ifStatement->token = currentToken;
    setNextToken();

    if (currentToken->tokenType != TokenType::COLON) {
        throw invalid_argument("parseIfStatement: COLON이 아닙니다.");
    }
    setNextToken();

    skipToken(TokenType::SPACE);

    if (currentToken->tokenType != TokenType::LPAREN) {
        throw invalid_argument("parseIfStatement: LPAREN이 아닙니다.");
    }
    setNextToken();

	ifStatement->condition = parseExpression(Precedence::LOWEST);
    setNextToken();

    if (currentToken->tokenType != TokenType::RPAREN) {
        throw invalid_argument("parseIfStatement: RPAREN이 아닙니다.");
    }
    setNextToken();

//    skipSpaceToken();
//
//    if (currentToken->tokenType != TokenType::END_IF) {
//        throw invalid_argument("parseIfStatement: END_IF가 아닙니다.");
//    }
//    setNextToken();

    if (currentToken->tokenType != TokenType::NEW_LINE) {
        throw invalid_argument("parseIfStatement: NEW_LINE이 아닙니다.");
    }
    setNextToken();

	ifStatement->consequence = parseBlockStatement();

    if (currentToken->tokenType != TokenType::ENDBLOCK) { // ENDBLOCK
        throw invalid_argument("parseIfStatement: ENDBLOCK이 아닙니다.");
    }
//    setNextToken();

    if (nextToken->tokenType == TokenType::ELSE) {
        // 나중에 검사 엄밀히 할 것(필수!)
        setNextToken(); // current: ENDBLOCK, next: ELSE
        setNextToken(); // current: ELSE, next: COLON
        setNextToken(); // current: COLON,     next: NEW_LINE
        setNextToken(); // current: NEW_LINE, next: STARTBLOCK

        ifStatement->alternative = parseBlockStatement();

        if (currentToken->tokenType != TokenType::ENDBLOCK) { // ENDBLOCK
            throw invalid_argument("parseIfStatement: ENDBLOCK이 아닙니다.");
        }
    }

    return ifStatement;
}

LoopStatement* Parser::parseLoopStatement() {
	LoopStatement* loopStatement = new LoopStatement;

	if (currentToken->tokenType != TokenType::LOOP) {
		throw invalid_argument("parseLoopStatement: LOOP가 아닙니다.");
	}
	loopStatement->token = currentToken;
	setNextToken();

	if (currentToken->tokenType != TokenType::COLON) {
		throw invalid_argument("parseLoopStatement: COLON이 아닙니다.");
	}
	setNextToken();

    skipToken(TokenType::SPACE);

	if (currentToken->tokenType != TokenType::LPAREN) {
		throw invalid_argument("parseLoopStatement: LPAREN이 아닙니다.");
	}
	setNextToken();

	Statement* firstStatement = parseStatement(); // for와 while의 분기점
	if(ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(firstStatement)){
		// 첫 Statement가 ExpressionStatement면 while로 처리
		loopStatement->condition = expressionStatement->expression;
	}
	else{ // for
		loopStatement->initialization = firstStatement;

		setNextToken();
		if (currentToken->tokenType != TokenType::SEMICOLON) {
			throw invalid_argument("parseLoopStatement: SEMICOLON이 아닙니다.");
		}
		setNextToken();
        skipToken(TokenType::SPACE);

		loopStatement->condition = parseExpression(Precedence::LOWEST);

		setNextToken();
		if (currentToken->tokenType != TokenType::SEMICOLON) {
			throw invalid_argument("parseLoopStatement: SEMICOLON이 아닙니다.");
		}
		setNextToken();
        skipToken(TokenType::SPACE);

		loopStatement->incrementation = parseStatement();
	}
	setNextToken();

	if (currentToken->tokenType != TokenType::RPAREN) {
		throw invalid_argument("parseLoopStatement: RPAREN이 아닙니다.");
	}
	setNextToken();

	if (currentToken->tokenType != TokenType::NEW_LINE) {
		throw invalid_argument("parseLoopStatement: NEW_LINE이 아닙니다.");
	}
	setNextToken();

	loopStatement->loopBody = parseBlockStatement();

	if (currentToken->tokenType != TokenType::ENDBLOCK) { // ENDBLOCK
		throw invalid_argument("parseLoopStatement: ENDBLOCK이 아닙니다.");
	}

	return loopStatement;
}

Expression* Parser::parseIndexExpression(Expression* left) {
    IndexExpression* indexExpression = new IndexExpression;
    indexExpression->token = currentToken;
    indexExpression->left = left;

    setNextToken();
    indexExpression->index = parseExpression(Precedence::LOWEST);

    setNextToken(); // ']'

    return indexExpression;
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

    skipToken(TokenType::SPACE);

    functionLiteral->parameters = parseFunctionParameters();

    skipToken(TokenType::SPACE);

    functionLiteral->name = dynamic_cast<IdentifierExpression *>(parseIdentifierExpression());
    setNextToken(); // skip Identifier Token
    setNextToken(); // skip DOT token
    skipToken(TokenType::SPACE);

    if (currentToken->tokenType != TokenType::RIGHTARROW) {
        throw invalid_argument("parseFunctionLiteral: RIGHT ARROW가 아닙니다.");
    }
    setNextToken();

    skipToken(TokenType::SPACE);

    if (currentToken->tokenType != TokenType::LBRACKET) {
        throw invalid_argument("parseFunctionLiteral: LBRACKET이 아닙니다.");
    }
    setNextToken();


    // 리턴 타입이 없는 경우도 생각할 것
    if (currentToken->tokenType != TokenType::RBRACKET) {
        if (currentToken->tokenType != TokenType::INT) {
            throw invalid_argument("parseFunctionLiteral: 리턴 타입이 잘못되었습니다.");
        }
        functionLiteral->returnType = currentToken;
        setNextToken();
    }


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
        setNextToken(); // ? ,
        setNextToken(); // , space
        setNextToken(); // space ?
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

        // 여기도 나중에 엄밀히 검증할 것
        setNextToken(); // current: identifier, next: COMMA
        setNextToken(); // current: COMMA, next: SPACE
        setNextToken(); // current: SPACE, next: identifier
    }

    IdentifierExpression* identifier = dynamic_cast<IdentifierExpression*>(parseIdentifierExpression());
    identifiers.push_back(identifier);
    setNextToken(); // current: identifier, next: SPACE

    return identifiers;
}

