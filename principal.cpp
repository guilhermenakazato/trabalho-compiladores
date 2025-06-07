#include "scanner.h"

string* vet;
void print(Token* token);
void allocVetor();

int main(int argc, char* argv[]) {
    //Verifica se foi executado corretamente
    //Essa main espera receber o nome do arquivo a ser
    //executado na linha de comando.
    if (argc != 2) {
        cout << "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }

    Scanner* scanner = new Scanner(argv[1]);
    Token* token;
    allocVetor();
    
    do {
        token = scanner->nextToken();
        print(token);        
    } while(token->name != END_OF_FILE);
    cout << endl;

    delete[] vet;
    delete scanner;
    return 0;
}

void allocVetor() {
    vet = new string[26];
    vet[0] = "UNDEF",
    vet[1] = "ID",
    vet[2] = "INTEGER_LITERAL",
    vet[3] = "OP",
    vet[4] = "AND",
    vet[5] = "LT",
    vet[6] = "GT",
    vet[7] = "PLUS",
    vet[8] = "MINUS",
    vet[9] = "MULT",
    vet[10] = "DIV",
    vet[11] = "ATTR",
    vet[12] = "EQUALS",
    vet[13] = "NOT",
    vet[14] = "NEQUALS",
    vet[15] = "SEP",
    vet[16] = "LPAREN",
    vet[17] = "RPAREN",
    vet[18] = "LBRACK",
    vet[19] = "RBRACK",
    vet[20] = "LBRACE",
    vet[21] = "RBRACE",
    vet[22] = "SCOLON",
    vet[23] = "COMMA",
    vet[24] = "DOT",
    vet[25] = "END_OF_FILE";
}

void freeVetor() {
    delete[] vet;

}

void print(Token* token) {
    if(token->name == ID || token->name == INTEGER_LITERAL) {
        cout << vet[token->name] + "(" + token->lexeme + ")" + " ";
    } else if(token->name == END_OF_FILE || token->name == ATTR) {
        cout << vet[token->name] + " ";
    } else if(token->name == UNDEF) {
        cout << token->lexeme << " ";
    } else {
        cout << vet[token->attribute] + " ";
    }
}