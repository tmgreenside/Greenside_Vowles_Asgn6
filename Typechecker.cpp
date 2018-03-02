#include "Typechecker.h"
#include "ASTNodes.h"


/*
 QUESTIONS FOR SCHROEDER !!!!!!!!!!!!!!!!!!!!!!!!!!!
 
 1. look over existing written methods. Are we doing this right?
 2. Are our existing methods missing anything?
 3. How to use Symbol Table? (see vist(assignmentstatement))
 4. Is visit(literal) one that we need to do, or is that one that
    you completed for us?
 
 */

using namespace std;

void Typechecker::visit(ASTSimpleBoolExpression& simpleBoolExpr) {
    simpleBoolExpr.expression->accept(*this);
    if (currentType != MPLType::BOOL) {
        throw TypecheckerException("Expected boolean expression");
    }
    currentType = MPLType::BOOL;
}

void Typechecker::visit(ASTComplexBoolExpression& complexBoolExpr) {
    // TODO
    currentType = MPLType::BOOL;
    complexBoolExpr.first->accept(*this);
    MPLType firstType = currentType;
    complexBoolExpr.second->accept(*this);
    if (firstType != currentType) {
        throw TypecheckerException("Second and First Types not matching");
    }
    switch (complexBoolExpr.relation) {
        case Token::EQUAL:
        case Token::NOT_EQUAL:
            break;
        default:
            throw TypecheckerException("Invalid operator");
            break;
    }
    if (complexBoolExpr.hasConjunction) {
        complexBoolExpr.remainder->accept(*this);
    }
    currentType = MPLType::BOOL;
}

void Typechecker::visit(ASTStatementList& statementList) {
    table.pushTable();
    for (auto &statement : statementList.statements) {
        statement->accept(*this);
    }
    table.popTable();
}

void Typechecker::visit(ASTBasicIf& basicIf) {
    // TODO
    basicIf.expression->accept(*this);
    basicIf.statementList->accept(*this);
}

void Typechecker::visit(ASTIfStatement& ifStatement) {
    // TODO
    ifStatement.baseIf.accept(*this);
    for (int i = 0; i < ifStatement.elseifs.size(); i++) {
        ifStatement.elseifs[i].accept(*this);
    }
    ifStatement.elseList->accept(*this);
}

void Typechecker::visit(ASTWhileStatement& whileStatement) {
    // TODO
    whileStatement.condition->accept(*this);
    whileStatement.statements->accept(*this);
}

void Typechecker::visit(ASTPrintStatement& printStatement) {
    printStatement.expression->accept(*this);
}

void Typechecker::visit(ASTAssignmentStatement& assignmentStatement) {
    // TODO
    assignmentStatement.identifier->accept(*this);
    
}

void Typechecker::visit(ASTIdentifier& identifier) {
    if (table.doesSymbolExist(identifier.name)) {
        currentType = table.getSymbolType(identifier.name);
    } else {
        throw TypecheckerException("Identifier " + identifier.name + " used before defined");
    }
    if (identifier.indexExpression && currentType != MPLType::ARRAY) {
        throw TypecheckerException("Identifier " + identifier.name + " given an index when not an array");
    }
}

void Typechecker::visit(ASTLiteral& literal) {
    currentType = literal.type;
}

void Typechecker::visit(ASTListLiteral& listLiteral) {
    // TODO
    for (int i = 0; i < listLiteral.expressions.size(); i++) {
        listLiteral.expressions[i]->accept(*this);
    }
}

void Typechecker::visit(ASTReadExpression& readExpression) {
    if (readExpression.isReadInt) {
        currentType = MPLType::INT;
    } else {
        currentType = MPLType::STRING;
    }
}

void Typechecker::visit(ASTComplexExpression& complexExpression) {
    // TODO
    complexExpression.firstOperand->accept(*this);
    MPLType firstType = currentType;
    complexExpression.rest->accept(*this); // change current type for switch
    if (firstType != currentType) {
        throw TypecheckerException("Second and First Types must be the same");
    }
    switch (complexExpression.operation) {
        case Token::MODULUS:
        case Token::MINUS:
        case Token::MULTIPLY:
        case Token::DIVIDE:
            if (firstType == MPLType::STRING) {
                throw TypecheckerException("Cannot perform math operations other than + on string");
            }
        case Token::PLUS:
            break;
        default:
            // we not
            throw TypecheckerException("Expected a Math operator");
            break;
    }
    
}




