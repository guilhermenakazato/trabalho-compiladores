#include "scanner.h"    

// lexema número: 3, 4, 5
// lexema palavra: 0, 1, 2

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input) {
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open()) {
        while (getline(inputFile,line)) {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    // cout << this->input;
}

int Scanner::getLine() {
    return line;
}

//Método que retorna o próximo token da entrada
Token* Scanner::nextToken(bool reservedPriority) {
    int state = 0;
    int blockCommentLineCount = 0;
    string lexeme;

    while(true) {
        switch(state) {
            case 0: // 0, 3, 6, 22, 36, 41
                if(input[pos] == '\0')
                    return new Token(END_OF_FILE);

                if(isalpha(input[pos])) {
                    lexeme += input[pos];
                    state = 1;
                } else if(isdigit(input[pos])) {
                    lexeme += input[pos];
                    state = 4;
                } else if(input[pos] == '&') {
                    state = 7;
                } else if(input[pos] == '<') {
                    state = 9;
                } else if(input[pos] == '>') {
                    state = 10;
                } else if(input[pos] == '+') {
                    state = 11;
                } else if(input[pos] == '-') {
                    state = 12;
                } else if(input[pos] == '*') {
                    state = 13;
                } else if(input[pos] == '/') {
                    state = 14;
                } else if(input[pos] == '=') {
                    state = 16;
                } else if(input[pos] == '!') {
                    state = 19;
                } else if(input[pos] == '(') {
                    state = 23;
                } else if(input[pos] == ')') {
                    state = 24;
                } else if(input[pos] == '[') {
                    state = 25;
                } else if(input[pos] == ']') {
                    state = 26;
                } else if(input[pos] == '{') {
                    state = 27;
                } else if(input[pos] == '}') {
                    state = 28;
                } else if(input[pos] == ';') {
                    state = 29;
                } else if(input[pos] == ',') {
                    state = 30;
                } else if(input[pos] == '.') {
                    state = 42;
                } else if(input[pos] == '\n') {
                    state = 32;
                } else if(isspace(input[pos])) {
                    state = 37;
                } else {
                    lexicalError("token inválido! :(");
                }

                pos++;
                break;
            case 1:
                if(isalpha(input[pos]) || isdigit(input[pos]) || input[pos] == '_') {
                    lexeme += input[pos];
                } else if(input[pos] == '.' && lexeme == "System" && reservedPriority) {
                    lexeme += input[pos];
                    state = 39;
                } else {
                    state = 2;
                }

                pos++;
                break;
            case 2:
                pos--;

                if(lexeme == "boolean") {
                    return new Token(RESERVED, BOOLEAN);
                } else if(lexeme == "class") {
                    return new Token(RESERVED, CLASS);
                } else if(lexeme == "else") {
                    return new Token(RESERVED, ELSE);
                } else if(lexeme == "extends") {    
                    return new Token(RESERVED, EXTENDS);
                } else if(lexeme == "false") {
                    return new Token(RESERVED, FALSE);
                } else if(lexeme == "if") {
                    return new Token(RESERVED, IF);
                } else if(lexeme == "int") {
                    return new Token(RESERVED, INT);
                } else if(lexeme == "length") {
                    return new Token(RESERVED, LENGTH);
                } else if(lexeme == "main") {
                    return new Token(RESERVED, MAIN);
                } else if(lexeme == "new") {
                    return new Token(RESERVED, NEW);
                } else if(lexeme == "public") {
                    return new Token(RESERVED, PUBLIC);
                } else if(lexeme == "return") {
                    return new Token(RESERVED, RETURN);
                } else if(lexeme == "static") {
                    return new Token(RESERVED, STATIC);
                } else if(lexeme == "String") {
                    return new Token(RESERVED, STRING);
                } else if(lexeme == "true") {
                    return new Token(RESERVED, TRUE);
                } else if(lexeme == "void") {
                    return new Token(RESERVED, VOID);
                } else if(lexeme == "while") {
                    return new Token(RESERVED, WHILE);
                }

                return new Token(ID, lexeme);                 
            // case 3:
            case 4:
                if(isdigit(input[pos])) {
                    lexeme += input[pos];
                } else {
                    state = 5;
                }
                
                pos++;
                break;
            case 5:
                pos--;
                return new Token(INTEGER_LITERAL, lexeme);
            // case 6:
            case 7:
                if(input[pos] == '&') {
                    state = 8;
                    pos++;
                } else {
                    lexicalError("má formação do operador &&");
                }

                break;
            case 8:
                return new Token(OP, AND);
            case 9:
                return new Token(OP, LT);
            case 10:
                return new Token(OP, GT);
            case 11:
                return new Token(OP, PLUS);
            case 12:
                return new Token(OP, MINUS);
            case 13:
                return new Token(OP, MULT);
            case 14:
                if(input[pos] == '/') {
                    state = 31;
                } else if(input[pos] == '*') {
                    state = 33;
                } else {
                    state = 15;
                }

                pos++;
                break;
            case 15:
                pos--;
                return new Token(OP, DIV);
            case 16:
                if(input[pos] == '=') {
                    state = 18;
                } else {
                    state = 17;
                }

                pos++;
                break;
            case 17:
                pos--;
                return new Token(OP, ATTR);
            case 18:
                return new Token(OP, EQUALS);
            case 19:
                if(input[pos] == '=') {
                    state = 21;
                } else {
                    state = 20;
                }

                pos++;
                break;
            case 20:
                pos--;
                return new Token(OP, NOT);
            case 21:
                return new Token(OP, NEQUALS);
            // case 22:
            case 23:
                return new Token(SEP, LPAREN);
            case 24:   
                return new Token(SEP, RPAREN);
            case 25:
                return new Token(SEP, LBRACK);
            case 26:
                return new Token(SEP, RBRACK);
            case 27:
                return new Token(SEP, LBRACE);
            case 28:
                return new Token(SEP, RBRACE);
            case 29:
                return new Token(SEP, SCOLON);
            case 30:
                return new Token(SEP, COMMA);
            case 31:
                if(input[pos] == '\n') {
                    state = 32;
                } 

                pos++;
                break;
            case 32:
                if(input[pos] == '\0') {
                    state = 43;
                } else {
                    pos++;
                    state = 44;
                }

                break;
            case 33:
                if(input[pos] == '*') {
                    state = 34;
                } else if(input[pos] == '\n') {
                    blockCommentLineCount++;
                } else if(input[pos] == '\0') {
                    lexicalError("comentário não finalizado.");
                }

                pos++;
                break;
            case 34:
                if(input[pos] == '/') {
                    state = 35;
                } else {
                    state = 33;
                }

                pos++;
                break;
            case 35:
                line += blockCommentLineCount;
                blockCommentLineCount = 0;
                state = 0;
                break;
            // case 36:
            case 37:
                if(!isspace(input[pos])) {
                    state = 38;
                }

                pos++;
                break;
            case 38:
                pos--;
                state = 0;
                break;
            case 39:
                if(isalpha(input[pos]) || input[pos] == '.') {
                    lexeme += input[pos];
                } else {
                    state = 40;
                }

                pos++;
                break;
            case 40:
                if(lexeme == "System.out.println") {
                    pos--;
                    return new Token(RESERVED, SOUT);
                } else {
                    pos -= (lexeme.length() + 1);
                    return nextToken(false);
                }
            // case 41:
            case 42:
                return new Token(SEP, DOT);
            case 43: 
                return new Token(END_OF_FILE);
            case 44: 
                line++;
                pos--;
                state = 0;
                break;       
        }
    }
}

void Scanner::lexicalError(string msg) {
    cout << "\nLinha "<< line << ": " << msg << endl;    
    exit(EXIT_FAILURE);
}