#include "parser.h"

/**
 * name = ID, INTEGER_LITERAL, OP, SEP
 * attribute = resto
 */
Token* l2Token;

Parser::Parser(string input) {
	scanner = new Scanner(input);
}

void Parser::advance() {
	lToken = scanner->nextToken();
	cout << "Proximo token: " << lToken->name << "," << lToken->attribute << "," << lToken->lexeme << endl;
}

void Parser::match(int t) {
	cout << "Esperado: " << t << endl;
	
	// if(t == ID && lToken->isReserved) {
	// 	error("Uso de palavra reservada como variavel");
	// }

	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void Parser::match(int t1, int t2) {
	cout << "Esperado (1): " << t1 << endl;
	cout << "Esperado (2): " << t2 << endl;

	// if(t1 == ID && lToken->isReserved || t2 == ID && lToken->isReserved) {
	// 	error("Uso de palavra reservada como variavel");
	// }

	if ((lToken->name == t1 || lToken->attribute == t1) && (l2Token->name == t2 || l2Token->attribute == t2)) {
		delete l2Token;
		l2Token = nullptr;
		advance();
	} else {
		error("Erro inesperado");
	}
}

void Parser::run() {
	advance();
	program();

	cout << "Compilacao encerrada com sucesso!\n";
}

void Parser::program() {
	mainClass();

	while(lToken->attribute == CLASS) {
		classDeclaration();
	}

	match(END_OF_FILE);
}

void Parser::mainClass() {
	match(CLASS);
	match(ID);
	match(LBRACE);
	match(PUBLIC);
	match(STATIC);
	match(VOID);
	match(MAIN);
	match(LPAREN);
	match(STRING);
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
	match(CLASS);
	match(ID);
	
	if(lToken->attribute == EXTENDS) {
		advance();
		match(ID);
	}
	
	match(LBRACE);
	
	while(lToken->attribute == INT || lToken->attribute == BOOLEAN || lToken->name == ID) {
		varDeclaration();
	}

	while(lToken->attribute == PUBLIC) {
		methodDeclaration();
	}

	match(RBRACE);
}

void Parser::varDeclaration() {
	if(l2Token) {
		match(ID, ID);
	} else {
		type();
		match(ID);
	}
	
	match(SCOLON);
}

void Parser::methodDeclaration() {
	match(PUBLIC);
	type();
	match(ID);
	match(LPAREN);

	if(lToken->attribute == INT || lToken->attribute == BOOLEAN || lToken->name == ID) {
		params();
	}

	match(RPAREN);
	match(LBRACE);

	// problemático... statement() e varDeclaration() podem dar muito errado.....
	while(lToken->attribute == INT || lToken->attribute == BOOLEAN || lToken->name == ID) {
		// para resolver a ambiguidade entre VarDeclaration e Statement
		if(lToken->name == ID) {
			l2Token = scanner->nextToken();

			if(l2Token->name != ID)
				break;
		}

		varDeclaration();
	}

	if(l2Token)
		cout << "Lookahead 2: " << l2Token->name << "," << l2Token->attribute << endl; 
	
	while(lToken->attribute == LBRACE || lToken->attribute == IF || lToken->attribute == WHILE ||
			lToken->attribute == SOUT || lToken->name == ID) {
		statement();
	}

	match(RETURN);
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
	if(lToken->attribute == INT) {
		advance();

		if(lToken->attribute == LBRACK) {
			advance();
			match(RBRACK);
		}
	} else if(lToken->attribute == BOOLEAN || lToken->name == ID) {
		advance();
	} else {
		error("Erro ao especificar o tipo");
	}
}

void Parser::statement() {
	if(lToken->attribute == LBRACE) {
		advance();

		while(lToken->attribute == LBRACE || lToken->attribute == IF || lToken->attribute == WHILE ||
		 	lToken->attribute == SOUT || lToken->name == ID) {
			statement();
		}

		match(RBRACE);
	} else if(lToken->attribute == IF) {
		advance();
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
		match(ELSE);
		statement();
	} else if(lToken->attribute == WHILE) {
		advance();
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
	} else if(lToken->attribute == SOUT) {
		advance();
		match(LPAREN);
		expression();
		match(RPAREN);
		match(SCOLON);
	} else if(lToken->name == ID) {
		if(l2Token) {
			if(l2Token->attribute == LBRACK) {
				match(ID, LBRACK);
				expression();
				match(RBRACK);
			} else if(l2Token->attribute == ATTR) {
				match(ID, ATTR);
				expression();
				match(SCOLON);
			} else {
 				error("Ma formacao da declaracao");
			}
		} else {
			match(ID);
		
			if(lToken->attribute == LBRACK) {
				advance();
				expression();
				match(RBRACK);
			}

			match(ATTR);
			expression();
			match(SCOLON);
		}
	} else {
		error("Ma formacao da declaracao");
	}
}

void Parser::expression() {
	if(lToken->name == INTEGER_LITERAL) {
		advance();
		expressionLine();
	} else if(lToken->attribute == TRUE) {
		advance();
		expressionLine();
	} else if(lToken->attribute == FALSE) {
		advance();
		expressionLine();
	} else if(lToken->attribute == THIS) {
		advance();
		expressionLine();
	} else if(lToken->attribute == NEW) {
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
		error("Ma formacao de expressao");
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
	if(lToken->attribute == INT) {
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
		error("Ma formacao de expressao");
	}
}

void Parser::expressionLineFactor() {
	if(lToken->attribute == LENGTH) {
		advance();
		expressionLine();
	} else if(lToken->name == ID) {
		match(ID);
		match(LPAREN);

		if(lToken->name == INTEGER_LITERAL || lToken->attribute == TRUE || lToken->attribute == FALSE ||
			lToken->attribute == THIS || lToken->attribute == NEW || lToken->name == ID || lToken->attribute == NOT) {
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
