#ifndef TOLELOM_LOOPSTATEMENT_H
#define TOLELOM_LOOPSTATEMENT_H

#include "statement.h"
#include "blockStatement.h"
#include "assignStatement.h"
#include "../expressions/expression.h"

class LoopStatement : public Statement{
public:
	Token* token{};
	Statement* initialization{};
	Expression* condition{};
	AssignStatement* incrementation{};

	BlockStatement* loopBody{};

	string String() override {
		if(initialization != nullptr)
			return "loop: (" + initialization->String() + "; " + condition->String() +
				"; " + incrementation->String() + "{\n" + loopBody->String() + "}";
		else return "loop: " + condition->String() + " " + "{\n" + loopBody->String() + "}";;
	}
};


#endif //TOLELOM_LOOPSTATEMENT_H
