#include "Typechecker.h"
#include "ASTNodes.h"

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
}

void Typechecker::visit(ASTIfStatement& ifStatement) {
    // TODO
}

void Typechecker::visit(ASTWhileStatement& whileStatement) {
    // TODO
}

void Typechecker::visit(ASTPrintStatement& printStatement) {
    printStatement.expression->accept(*this);
}

void Typechecker::visit(ASTAssignmentStatement& assignmentStatement) {
    // TODO
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
}




