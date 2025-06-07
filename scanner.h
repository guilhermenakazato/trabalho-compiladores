#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        int pos;//Posição atual
        int line;
    
    public:
    //Construtor
        Scanner(string);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken(bool reservedPriority = true);        
    
        //Método para manipular erros
        void lexicalError(string);
};
