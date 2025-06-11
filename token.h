#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF, //0
    ID, //1
    INTEGER_LITERAL, //2
    OP, //3
    AND, //4
    LT, //5
    GT, //6
    PLUS, //7
    MINUS, //8
    MULT, //9
    DIV, //10
    ATTR, //11
    EQUALS, //12
    NOT, //13
    NEQUALS, //14
    SEP, //15
    LPAREN, //16
    RPAREN, //17
    LBRACK, //18
    RBRACK, //19
    LBRACE, //20
    RBRACE, //21
    SCOLON, //22
    COMMA, //23
    DOT, //24
    END_OF_FILE //25
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
        bool isReserved = false;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }

        Token(int name, string l, bool isReserved)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
            this->isReserved = isReserved;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};
