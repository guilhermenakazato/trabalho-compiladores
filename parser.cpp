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
	// string vet[26];
    // vet[0] = "UNDEF",
    // vet[1] = "ID",
    // vet[2] = "INTEGER_LITERAL",
    // vet[3] = "OP",
    // vet[4] = "AND",
    // vet[5] = "LT",
    // vet[6] = "GT",
    // vet[7] = "PLUS",
    // vet[8] = "MINUS",
    // vet[9] = "MULT",
    // vet[10] = "DIV",
    // vet[11] = "ATTR",
    // vet[12] = "EQUALS",
    // vet[13] = "NOT",
    // vet[14] = "NEQUALS",
    // vet[15] = "SEP",
    // vet[16] = "LPAREN",
    // vet[17] = "RPAREN",
    // vet[18] = "LBRACK",
    // vet[19] = "RBRACK",
    // vet[20] = "LBRACE",
    // vet[21] = "RBRACE",
    // vet[22] = "SCOLON",
    // vet[23] = "COMMA",
    // vet[24] = "DOT",
    // vet[25] = "END_OF_FILE";

	lToken = scanner->nextToken();
	// if(lToken->name == ID || lToken->name == INTEGER_LITERAL) {
    //     cout << vet[lToken->name] + "(" + lToken->lexeme + ")" + " ";
    // } else if(lToken->name == END_OF_FILE || lToken->name == ATTR) {
    //     cout << vet[lToken->name] + " ";
    // } else if(lToken->name == UNDEF) {
    //     cout << lToken->lexeme << " ";
    // } else {
    //     cout << vet[lToken->attribute] + " ";
    // }
	cout << "Proximo token: " << lToken->name << "," << lToken->attribute << "," << lToken->lexeme << endl;
}

void Parser::match(int t) {
	cout << "Esperado: " << t << endl;
	
	if(t == ID && lToken->isReserved) {
		error("Uso de palavra reservada como variavel");
	}

	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void Parser::match(string lexeme) {
	cout << "Esperado: " << lexeme << endl;

	if(lToken->lexeme == lexeme) 	
		advance();
	else 
		error("palavra reservada nao corresponde ao esperado (" + lexeme + ")");
}

void Parser::match(int t1, int t2) {
	cout << "Esperado (1): " << t1 << endl;
	cout << "Esperado (2): " << t2 << endl;

	if(t1 == ID && lToken->isReserved || t2 == ID && lToken->isReserved) {
		error("Uso de palavra reservada como variavel");
	}

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
	
	match(LBRACE);
	
	while(lToken->lexeme == "int" || lToken->lexeme == "boolean" || (lToken->name == ID && !lToken->isReserved)) {
		varDeclaration();
	}

	while(lToken->lexeme == "public") {
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
	match("public");
	type();
	match(ID);
	match(LPAREN);

	if(lToken->lexeme == "int" || lToken->lexeme == "boolean" || lToken->name == ID) {
		params();
	}

	match(RPAREN);
	match(LBRACE);

	// problemático... statement() e varDeclaration() podem dar muito errado.....
	while(lToken->lexeme == "int" || lToken->lexeme == "boolean" || lToken->name == ID) {
		if(lToken->name == ID && !lToken->isReserved) {
			l2Token = scanner->nextToken();

			if(l2Token->name != ID)
				break;
		}

		varDeclaration();
	}

	if(l2Token)
		cout << "Lookahead 2: " << l2Token->name << "," << l2Token->attribute << endl; 
	while(lToken->attribute == RBRACE || lToken->lexeme == "if" || lToken->lexeme == "while" ||
			lToken->lexeme == "System.out.println" || (lToken->name == ID && !lToken->isReserved)) {
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
			advance();
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

		match(RBRACE);
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
				cout << "Oops!" << endl;
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
		error("Ma formacao de expressao");
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
