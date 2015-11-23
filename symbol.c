#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "symbol.h"

int assingmentType;

//void insert(char * name, int type, int isConst, unsigned int scope)
void predefined(){
	insert("gl_FragColor", VEC4, RESULT, 0,0);
	insert("gl_FragDepth", BOOL, RESULT, 0,0);
	insert("gl_FragCoord", VEC4, RESULT, 0,0);

	insert("gl_TexCoord", VEC4, ATTRIBUTE, 0,0);
	insert("gl_Color", VEC4, ATTRIBUTE, 0,0);
	insert("gl_Secondary", VEC4, ATTRIBUTE, 0,0);
	insert("gl_FogFragCoord", VEC4, ATTRIBUTE, 0,0);

	insert("gl_Light_Half", VEC4, UNIFORM, 0,0);
	insert("gl_Light_Ambient", VEC4, UNIFORM, 0,0);
	insert("gl_Material_Shininess", VEC4, UNIFORM, 0,0);

	insert("env1", VEC4, UNIFORM, 0,0);
	insert("env2", VEC4, UNIFORM, 0,0);
	insert("env3", VEC4, UNIFORM, 0,0);

}

void build_table(node * ast) {

	if(ast==NULL)
		return;

	int kind;
	char * name;

	kind = ast->kind;

	switch(kind){
		case 1:
			scope_count++;
			//printf("ENTER_SCOPE_NODE %d\n", kind);
			build_table(ast->enter_scope.scope);
			scope_count--;
			break;
		case 2:

			//printf("SCOPE_NODE %d\n", kind);
			build_table(ast->scope.declarations);
			build_table(ast->scope.statements);

			break;
		case 3:
			//printf("DECLARATIONS_NODE %d\n", kind);
			build_table(ast->declarations.declarations);
			build_table(ast->declarations.declaration);
			break;
		case 4:
			//printf("STATEMENTS_NODE %d\n", kind);
			build_table(ast->statements.statements);
			build_table(ast->statements.statement);
			break;
		case 5:
			//printf("EXPRESSION_NODE No node %d\n", kind);
			// No EXPRESSION_NODE
			break;
		case 6:
			//printf("PREN_EXPRESSION_NODE %d\n", kind);
			build_table(ast->paren_exp.expression);
			break;
		case 7:
			//printf("UNARY_EXPRESION_NODE %d\n", kind);
			//printf("Operator: %d\n", ast->unary_expr.op);
			build_table(ast->unary_expr.right);
			break;
		case 8:
			//printf("BINARY_EXPRESSION_NODE %d\n", kind);
			//printf("Operator: %d\n", ast->binary_expr.op);
			build_table(ast->binary_expr.left);
			build_table(ast->binary_expr.right);
			break;
		case 9:
			//printf("INT_NODE %d\n", kind);
			//printf("Integer: %d\n",ast->int_literal.right);
			break;
		case 10:
			//printf("FLOAT_NODE %d\n", kind);
			//printf("Float: %f", ast->float_literal.right);
			break;
		case 11:
			//printf("BOOL_NODE %d\n", kind);
			//printf("Bool: %d", ast->bool_literal.right);
			break;
		case 12:
			//printf("IDENT_NODE No node %d\n", kind);
			// No IDENT_NODE
			break;
		case 13:
			//printf("VAR_NODE %d\n", kind);
			//printf("Identifier: %s\n", ast->variable_exp.identifier);
			break;
		case 14:
			//printf("ARRAY_NODE %d\n", kind);
			break;
		case 15:
			//printf("FUNCTION_NODE %d\n", kind);
			build_table(ast->function_exp.arguments);

			if(ast->function_exp.function_name == 2){
				insert("rsq",
						FUNCTION,
						NONCONST,
						ast->function_exp.line,
						scope_count);
			}else if(ast->function_exp.function_name == 0){ //0
				insert("dp3",
						FUNCTION,
						NONCONST,
						ast->function_exp.line,
						scope_count);
			}else if (ast->function_exp.function_name == 1){ //1
				insert("lit",
						FUNCTION,
						NONCONST,
						ast->function_exp.line,
						scope_count);
			}
			break;
		case 16:
			//printf("CONSTRUCTOR_NODE %d\n", kind);
			build_table(ast->constructor_exp.type);
			build_table(ast->constructor_exp.arguments);

			break;
		case 17:
			//printf("TYPE_NODE %d\n", kind);
			break;
		case 18:
			//printf("IF_ELSE_STATEMENT_NODE %d\n", kind);
			build_table(ast->if_else_statement.condition);
			build_table(ast->if_else_statement.else_statement);
			build_table(ast->if_else_statement.then_statement);
			break;
		case 19:
			//printf("IF_STATEMENT_NODE %d\n", kind);
			build_table(ast->if_statement.condition);
			build_table(ast->if_statement.then_statement);
			break;
		case 20:
			//printf("WHILE_STATEMENT_NODE No node %d\n", kind);
			//No WHILE_STATEMENT_NODE
			break;
		case 21:
			//printf("ASSIGNMENT_NODE %d\n", kind);
			build_table(ast->assignment.left);
			// set type of symbol in local var
			name = ast->assignment.left->variable_exp.identifier;

			build_table(ast->assignment.right);
			break;
		case 22:
			//printf("NESTED_SCOPE_NODE No node for %d\n", kind);
			// No NESTED_SCOPE_NODE
			break;
		case 23:
			//printf("DECLARATION_NODE %d\n", kind);
			build_table(ast->declaration.type);

			insert(ast->declaration.iden,
					ast->declaration.type->type.type_name,
					NONCONST,
					ast->declaration.line,
					scope_count);

			break;
		case 24:
			//printf("DECLARATION_ASSIGNMENT_NODE %d\n", kind);
			build_table(ast->declaration_assignment.type);
			build_table(ast->declaration_assignment.value);

			insert(ast->declaration_assignment.iden,
					ast->declaration_assignment.type->type.type_name,
					NONCONST,
					ast->declaration_assignment.line,
					scope_count);

			break;
		case 25:
			//printf("CONST_DECLARATION_ASSIGNMENT_NODE %d\n", kind);
			build_table(ast->const_declaration_assignment.type);
			build_table(ast->const_declaration_assignment.value);

			insert(ast->const_declaration_assignment.iden,
					ast->const_declaration_assignment.type->type.type_name,
					CONST_S,
					ast->const_declaration_assignment.line,
					scope_count);
			break;
		case 26:
			//printf("ARGUMENTS_COMMA_NODE %d\n", kind);
			build_table(ast->arguments_comma.arguments);
			build_table(ast->arguments_comma.expression);
			break;
		case 27:
			//printf("ARGUMENTS_EXPRESSION_NODE %d\n", kind);
			build_table(ast->arguments_expression.expression);
			break;
		default:
			//printf("DEFAULT!!\n");
			break;

	}

}
