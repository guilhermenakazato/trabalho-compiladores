#include "parser.h"

/**
 * name = ID, INTEGER_LITERAL, OP, SEP
 * attribute = resto
 */

Parser::Parser(string input) {
	scanner = new Scanner(input);
}

void Parser::advance() {
	lToken = scanner->nextToken();
}

void Parser::match(int t) {
	if(t == ID && lToken->isReserved) {
		error("Uso de palavra reservada como variável");
	}

	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void Parser::match(string lexeme) {
	if(lToken->lexeme == lexeme) 	
		advance();
	else 
		error("palavra reservada não corresponde ao esperado (" + lexeme + ")");
}

void Parser::run() {
	advance();
	program();

	cout << "Compilação encerrada com sucesso!\n";
}

void Parser::program() {
	mainClass();

	while(lToken->lexeme == "class") {
		classDeclaration();
	}

	match(END_OF_FILE);
}

void Parser::mainClass() {
	match("class");
	match(ID);
	match(LBRACE);
	match("public");
	match("static");
	match("void");
	match("main");
	match(LPAREN);
	match("String");
	match(LBRACK);
	match(RBRACK);
	match(ID);
	match(RPAREN);
	match(LBRACE);
	statement();
	match(RBRACE);
	match(RBRACE);
}

void Parser::classDeclaration() {
	match("class");
	match(ID);
	
	if(lToken->lexeme == "extends") {
		advance();
		match(ID);
	}

	match(RBRACE);

	while(lToken->lexeme == "int" || lToken->lexeme == "boolean" || lToken->name == ID) {
		varDeclaration();
	}

	while(lToken->lexeme == "public") {
		methodDeclaration();
	}
}

void Parser::varDeclaration() {
	type();
	match(ID);
	match(SCOLON);
}

/**
 * TODO: remove ambiguity!!
 */
void Parser::methodDeclaration() {
	match("public");
	type();
	match(ID);
	match(LPAREN);

	if(lToken->lexeme == "int" || lToken->lexeme == "boolean" || lToken->name == ID) {
		params();
	}

	match(RPAREN);
	match(LBRACE);

	while(lToken->lexeme == "int" || lToken->lexeme == "boolean" || lToken->name == ID) {
		varDeclaration();
	}

	while(lToken->attribute == RBRACE || lToken->lexeme == "if" || lToken->lexeme == "while" ||
			lToken->lexeme == "System.out.println" || lToken->name == ID) {
		statement();
	}

	match("return");
	expression();
	match(SCOLON);
	match(RBRACE);
}

void Parser::params() {
	type();
	match(ID);

	while(lToken->attribute == COMMA) {
		advance();
		type();
		match(ID);
	}
}

void Parser::type() {
	if(lToken->lexeme == "int") {
		advance();

		if(lToken->attribute == LBRACK) {
			match(RBRACK);
		}
	} else if(lToken->lexeme == "boolean" || (lToken->name == ID && !lToken->isReserved)) {
		advance();
	} else {
		error("Erro ao especificar o tipo");
	}
}

void Parser::statement() {
	if(lToken->attribute == LBRACE) {
		advance();

		while(lToken->attribute == LBRACE || lToken->lexeme == "if" || lToken->lexeme == "while" ||
		 	lToken->lexeme == "System.out.println" || lToken->name == ID) {
			statement();
		}
	} else if(lToken->lexeme == "if") {
		advance();
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
		match("else");
		statement();
	} else if(lToken->lexeme == "while") {
		advance();
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
	} else if(lToken->lexeme == "System.out.println") {
		advance();
		match(LPAREN);
		expression();
		match(RPAREN);
		match(SCOLON);
	} else if(lToken->name == ID) {
		match(ID);

		if(lToken->attribute == LPAREN) {
			advance();
			expression();
			match(RPAREN);
		}

		match(ATTR);
		expression();
		match(SCOLON);
	} else {
		error("Má formação da declaração");
	}
}

void Parser::expression() {
	if(lToken->name == INTEGER_LITERAL) {
		advance();
		expressionLine();
	} else if(lToken->lexeme == "true") {
		advance();
		expressionLine();
	} else if(lToken->lexeme == "false") {
		advance();
		expressionLine();
	} else if(lToken->lexeme == "this") {
		advance();
		expressionLine();
	} else if(lToken->lexeme == "new") {
		advance();
		expressionFactor();
	} else if(lToken->attribute == NOT) {
		advance();
		expression();
		expressionLine();
	} else if(lToken->name == ID) {
		match(ID);
		expressionLine();
	} else {
		error("Má formação de expressão");
	}
}

void Parser::expressionLine() {
	if(lToken->attribute == AND || lToken->attribute == LT || lToken->attribute == GT || 
		lToken->attribute == EQUALS || lToken->attribute == NEQUALS || lToken->attribute == PLUS ||
		lToken->attribute == MINUS || lToken->attribute == MULT || lToken->attribute == DIV) {
		op();
		expression();
		expressionLine();
	} else if(lToken->attribute == LBRACK) {
		advance();
		expression();
		match(RBRACK);
		expressionLine();
	} else if(lToken->attribute == DOT) {
		advance();
		expressionLineFactor();
	} 
}

void Parser::expressionFactor() {
	if(lToken->lexeme == "int") {
		advance();
		match(LBRACK);
		expression();
		match(RBRACK);
		expressionLine();
	} else if(lToken->name == ID) {
		match(ID);
		match(LPAREN);
		match(RPAREN);
		expressionLine();
	} else {
		error("Má formação de expressão");
	}
}

void Parser::expressionLineFactor() {
	if(lToken->lexeme == "length") {
		advance();
		expressionLine();
	} else if(lToken->name == ID) {
		match(ID);
		match(LPAREN);

		if(lToken->name == INTEGER_LITERAL || lToken->lexeme == "true" || lToken->lexeme == "false" ||
			lToken->lexeme == "this" || lToken->lexeme == "new" || lToken->name == ID || lToken->attribute == NOT) {
			expressionsList();
		}
 
		match(RPAREN);
		expressionLine();
	}
}

void Parser::op() {
	if(lToken->attribute == AND || lToken->attribute == LT || lToken->attribute == GT || lToken->attribute == EQUALS ||
		lToken->attribute == NEQUALS || lToken->attribute == PLUS || lToken->attribute == MINUS || lToken->attribute == MULT || lToken->attribute == DIV) {
		advance();
	} else {
		error("Erro no operador");
	}
}

void Parser::expressionsList() {
	expression();

	while(lToken->attribute == COMMA) {
		expression();
	}
}

void Parser::error(string str) {
	cout << "Linha " << scanner->getLine() << ": " << str << endl;
	exit(EXIT_FAILURE);
}
