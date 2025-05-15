// 208 Salvador Ayala Iglesias Inés Guillén Peña
// 100495832@alumnos.uc3m.es 100495752@alumnos.uc3m.es

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define T_NUMBER 	1001
#define T_OPERATOR	1002		
#define T_VARIABLE  1003  

void ParseYourGrammar () ; 		/// Dummy Parser
void ParseAxiom () ;			/// Prototype for forward reference 		

struct s_tokens {
	int token ;					// Here we store the current token/literal 
	int old_token ; 			// Sometimes we need to check the previous token
	int number ;				// The value of the number 
	int old_number ;			// old number value
	char variable_name [8] ;	/// variable name	
	char old_var_name [8] ;		/// old variable name			
	int token_val ;				// the arithmetic operator
	int old_token_val ;			// old arithmetic operator
} ;

struct s_tokens tokens = {0, 0, 0, -1, "", "", 0, -1}; // contains initial values


int line_counter = 1 ;


void update_old_token () 
{					/// Sometimes we need to check the previous token
	tokens.old_token = tokens.token ;
	tokens.old_number = tokens.number ;
	strcpy (tokens.old_var_name, tokens.variable_name) ;	/// Copy variable names			
	tokens.old_token_val = tokens.token_val ;
}


void init_tokens () 
{ 								///  Not really neccesary
    tokens.token = 0;
	tokens.old_token = 0 ;
    tokens.number = 0 ;
    tokens.old_number = -1 ;
	strcpy (tokens.old_var_name, "") ;			/// erase old variable name
	strcpy (tokens.variable_name, "") ;			/// Erase variable name
    tokens.token_val = 0;
    tokens.old_token_val = -1;
}


int rd_lex ()
{
	int c ;
	int cc ;
	
	do {
		c = getchar () ;
		if (c == '\n')
			line_counter++ ;	// info for rd_syntax_error()
	} while (c == ' ' || c == '\r' || c == '\t') ;	/// \r is part of a newline in some Operating Systems
	
	if (isdigit (c)) {			/// Token Number is [Digit]+
		ungetc (c, stdin) ;		/// This returns one character to the standard input stream
		update_old_token () ;
		scanf ("%d", &tokens.number) ;
		tokens.token = T_NUMBER ;
		return (tokens.token) ;	// returns the Token for Number
	}

	if (isalpha(c)) {  /// Token Variable of type Letter[Digit]? 
		cc = getchar () ;

		update_old_token () ;
		if (isdigit (cc)) {									
			sprintf (tokens.variable_name, "%c%c", c, cc) ;		/// This copies the LetterDigit name in the variable name
		} else {											
			ungetc (cc, stdin) ;									
			sprintf (tokens.variable_name, "%c", c) ;			/// This copies the single Letter name in the variable name
		}													

		tokens.token = T_VARIABLE ;
        return (tokens.token) ;	// returns the Token for Variable
    } 

	if (c == '+' || c == '-' || c == '*' || c == '/') {  /// Remember that = is returned as a literal
		update_old_token () ;
		tokens.token_val = c ;
		tokens.token = T_OPERATOR ;
		return (tokens.token) ;		// returns the Token for Arithmetic Operators
	}					

	if (c == EOF){         /// End Of Archive detection for enhanced Batch Processing
        exit (0) ;
    } 

	update_old_token () ;
	tokens.token = c ;
	return (tokens.token) ;		// returns a literal
}


void rd_syntax_error (int expected, int token, char *output) 
{
	fprintf (stderr, "ERROR in line %d ", line_counter) ;
	fprintf (stderr, output, expected, token) ;
	
	exit (0) ;
}


void MatchSymbol (int expected_token)
{
	if (tokens.token != expected_token) {
		rd_syntax_error (expected_token, tokens.token, "token %d expected, but %d was read") ;
		exit (0) ;
	} else {
	 	rd_lex () ; 			/// read next Token
	}
}

void Parse_Number(){ // Imprime el número, si es un número
	printf("%d", tokens.number);
	MatchSymbol(T_NUMBER); // leer siguiente caracter
	return;
}

void Parse_Variable(){ // Imprime la variable, si es una variable
	printf("%s", tokens.variable_name);
	MatchSymbol(T_VARIABLE); // leer siguiente caracter
	return;
}

void Parse_Term(){ // Term ::= Var | Number
	if (tokens.token == T_VARIABLE){
		Parse_Variable();
		return;
	}
	if (tokens.token == T_NUMBER){
		Parse_Number();
		return;
	}
	MatchSymbol(tokens.token); // Caso erróneo, cualquier otra cosa
}

void Parse_Operator(){ // Oper ::= '+' | '-' | '*' | '/'
	MatchSymbol(T_OPERATOR); // No puede imprimir, ya que el operador debe aparecer después del primer termino de la expresion
	return;
}

void Parse_Expr(){ // Other_Expresion ::= '=' Var Expr | Oper Expr Expr
	if (tokens.token == '='){
		// Debería haber un '=', una variable y otra cosa
		MatchSymbol('=');
		Parse_Variable(); // Imprime la variable si no hubo errores
		printf(" = "); // formato "Variable = <algo>"
		ParseYourGrammar(); // Imprimirá, eventualmente, ese algo
		return;
	}
	if (tokens.token == T_OPERATOR){
		// Debería haber un operador y dos expresiones
		Parse_Operator();
		char op = tokens.token_val; // Necesario guardarlo para el formato correcto
		ParseYourGrammar();
		printf(" %c ", op);
		ParseYourGrammar();
		return;
	}
	MatchSymbol(tokens.token); // Caso erroneo, cualquier otra cosa.
}

void ParseYourGrammar (){ // Función principal (Expr ::= Term | Other_Expresion)
	if (tokens.token == '('){ // Hay paréntesis, se imprimen encerrando la expresion
		MatchSymbol('(');
		printf("(");

		Parse_Expr();
		
		MatchSymbol(')');
		printf(")");
		return;
	}
	// No había paréntesis, es un término
	Parse_Term();
	return;
}


void ParseAxiom () 		
{									/// Axiom ::= Expr \n
	ParseYourGrammar () ;							
	if (tokens.token == '\n') { 
		printf ("\n") ; 
		MatchSymbol ('\n') ;
	} else { 
		rd_syntax_error (-1, tokens.token, "-- Unexpected Token (Expected:%d=None, Read:%d) at end of Parsing\n") ;
	}
}


int main (int argc, char **argv) 
{
// Usage :  drLL -s  ==> evaluate a single Input Line
//          drLL     ==> evalute multiple Input Lines until some error appears
/// DO NOT MODIFY THE CODE INSIDE THE MAIN FUNCTION WITHOUT PERMISSION

	int flagMultiple = 1 ;
	
	if (argc >= 2) {
		if (strcmp ("-s", argv [1]) == 0) {
			flagMultiple = 0 ;
		}
	}
	
	rd_lex () ;						/// Read first Token only once
	do {
		ParseAxiom () ;		
//		printf ("\n") ;
	} while (flagMultiple) ;
	
	exit (0) ;
}
