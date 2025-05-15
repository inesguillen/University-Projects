// 208 Salvador Ayala Iglesias Inés Guillén Peña
// 100495832@alumnos.uc3m.es 100495752@alumnos.uc3m.es

%{                          // SECCION 1 Declaraciones de C-Yacc

#include <stdio.h>
#include <ctype.h>            // declaraciones para tolower
#include <string.h>           // declaraciones para cadenas
#include <stdlib.h>           // declaraciones para exit ()

#define FF fflush(stdout);    // para forzar la impresion inmediata

int yylex () ;
int yyerror () ;
char *mi_malloc (int) ;
char *gen_code (char *) ;
char *int_to_string (int) ;
char *char_to_string (char) ;

char temp [2048] ;
char current_func[64] = "";

// Abstract Syntax Tree (AST) Node Structure

typedef struct ASTnode t_node ;

struct ASTnode {
    char *op ;
    int type ;		// leaf, unary or binary nodes
    t_node *left ;
    t_node *right ;
} ;


// Definitions for explicit attributes

typedef struct s_attr {
    int value ;    // - Numeric value of a NUMBER 
    char *code ;   // - to pass IDENTIFIER names, and other translations 
    t_node *node ; // - for possible future use of AST
    char function; // to determine if it is a function or a set of variable
} t_attr ;

#define YYSTYPE t_attr

%}

// Definitions for explicit attributes

%token NUMBER        
%token IDENTIF       // Identificador=variable
%token INTEGER       // identifica el tipo entero
%token STRING
%token MAIN          // identifica el comienzo del proc. main
%token WHILE         // identifica el bucle main
%token PUTS          // Para impresiones de tipo "puts"
%token IF
%token ELSE
%token RETURN
%token PRINTF
%token IGUAL
%token DISTINTO
%token FOR
%token MENOR_IGUAL
%token MAYOR_IGUAL
%token AND
%token OR

%left IGUAL DISTINTO
%left '+' '-'                 // menor orden de precedencia
%left '*' '/' '%'             // orden de precedencia intermedio
%left UNARY_SIGN              // mayor orden de precedencia
%left OR AND
%left  MAYOR_IGUAL MENOR_IGUAL '<' '>'
%right '!'

%%                            // Seccion 3 Gramatica - Semantico

axioma:     declVars                 { printf("%s", $1.code); }
            defFunc                  { ; }
            ;

declVars:    INTEGER IDENTIF asignacion cadenaDecl ';' declVars { if (strlen(current_func) > 0)
                                                                      sprintf(temp, "(setq %s_%s %s)%s\n%s", current_func, $2.code, $3.code, $4.code, $6.code);
                                                                  else
                                                                      sprintf(temp, "(setq %s %s)%s\n%s", $2.code, $3.code, $4.code, $6.code);
                                                                  $$.code = gen_code(temp) ;}
            | INTEGER IDENTIF '[' operando ']' ';' declVars     { sprintf (temp, "setq %s (make-array %s)\n%s", $2.code, $4.code, $7.code);
                                                                    $$.code = gen_code (temp) ; }
            | /* lamda */                                       { sprintf (temp, "");
                                                                  $$.code = gen_code (temp) ; }
            ;

cadenaDecl: ',' INTEGER IDENTIF asignacion cadenaDecl   { if (strlen(current_func) > 0)
                                                              sprintf(temp, "(setq %s_%s %s)%s", current_func, $3.code, $4.code, $5.code);
                                                          else
                                                              sprintf(temp, "(setq %s %s)%s", $3.code, $4.code, $5.code);
                                                          $$.code = gen_code (temp) ; }
            | /* lambda */                              { sprintf (temp, "");
                                                          $$.code = gen_code (temp) ; }
            ;

defFunc:    main_func           { printf ("%s\n", $1.code) ; }
            | name_func         { printf ("%s\n", $1.code) ; }
                defFunc         { ; }
            | /* lambda */      { ; }
            ;

set_main: /* vacío */ { strcpy(current_func, "main"); }

main_func:  MAIN '(' ')' set_main '{' declVars sentencias '}'   { sprintf (temp, "(defun main()\n\t%s%s)", $6.code, $7.code) ;
                                                                  $$.code = gen_code (temp) ; }
            ;

set_func: IDENTIF { strcpy(current_func, $1.code); $$ = $1; }

name_func: set_func '(' argumentos ')' '{' declVars sentencias retorno '}'  { sprintf(temp, "(defun %s (%s)\n\t%s %s %s\n)", $1.code, $3.code, $6.code, $7.code, $8.code);
                                                                              $$.code = gen_code(temp); }
            ;

argumentos:   /* lamda */                   { sprintf (temp, "");
                                              $$.code = gen_code (temp) ;}
            | INTEGER IDENTIF masArgs       { sprintf (temp, "%s%s", $2.code, $3.code);
                                              $$.code = gen_code (temp) ;}
            ;

masArgs:      /* lamda */                   { sprintf (temp, "");
                                              $$.code = gen_code (temp) ;}
            | ',' INTEGER IDENTIF masArgs   { sprintf (temp, ",%s%s", $3.code, $4.code);
                                              $$.code = gen_code (temp) ;}
            ;

sentencias:   /* lambda */                  { sprintf (temp, "");
                                              $$.code = gen_code (temp) ; }
            | sentencia sentencias          { sprintf (temp, "%s\n%s", $1.code, $2.code);
                                              $$.code = gen_code (temp) ; }
            ;

sentencia:   IDENTIF llamadaOAsignacion ';'                                                                 { if ($2.function == '0'){
                                                                                                                if (strlen(current_func) > 0)
                                                                                                                  sprintf(temp, "(setf %s_%s %s)", current_func, $1.code, $2.code);
                                                                                                                else
                                                                                                                  sprintf(temp, "(setf %s %s)", $1.code, $2.code);
                                                                                                              }
                                                                                                              else { sprintf(temp, "\t(%s%s)", $1.code, $2.code);}
                                                                                                              $$.code = gen_code (temp) ; }
           | IDENTIF '[' expresion ']' llamadaOAsignacion ';'                                               { sprintf(temp, "(setf (aref %s %s) %s)", $1.code, $3.code, $5.code);
                                                                                                              $$.code = gen_code(temp); }
           | '@' expresion  ';'                                                                             { sprintf (temp, "\t(print %s)", $2.code) ;
                                                                                                              $$.code = gen_code (temp) ; }
           | PUTS '(' STRING ')' ';'                                                                        { sprintf (temp, "\t(print \"%s\")", $3.code) ;
                                                                                                              $$.code = gen_code (temp) ; }
           | IF '(' expresion ')' '{' sentencias retorno '}' posibleElse                                    { sprintf(temp, "(if %s\n\t(progn %s\n%s)%s\t)", $3.code, $6.code, $7.code, $9.code);
                                                                                                              $$.code = gen_code(temp) ; }
           | WHILE '(' expresion ')' '{' sentencias retorno '}'                                             { sprintf (temp, "\t(loop while %s do\n\t%s\n%s)", $3.code, $6.code, $7.code) ;
                                                                                                              $$.code = gen_code (temp) ;}
           | FOR '(' IDENTIF asignacion ';' expresion ';' IDENTIF asignacion ')' '{' sentencias retorno '}' { sprintf (temp, "\t(loop for %s do\n\t%s\n%s", $6.code, $12.code, $13.code) ;
                                                                                                              $$.code = gen_code (temp) ;}
           | PRINTF '(' STRING printArgs ')' ';'                                                            { sprintf(temp, "(%s)", $4.code);
                                                                                                              $$.code = gen_code(temp);}
           ;

retorno: RETURN llamadaOExpresion ';'   { sprintf(temp, "return-from-%s %s", current_func, $2.code);
                                  $$.code = gen_code(temp); }
        | /* lambda */          { ; }
        ;

posibleElse:  /* lambda */              { sprintf(temp, "");
                                          $$.code = gen_code(temp); }
            | ELSE '{' sentencias retorno '}'   { sprintf(temp, "\n\t(progn %s\n%s)", $3.code, $4.code);
                                          $$.code = gen_code(temp); }
            ;

llamadaOAsignacion: '(' argsLlamada ')'  { sprintf(temp, " %s", $2.code);
                                           $$.code = gen_code(temp);
                                           $$.function = '1'; }
                | '=' llamadaOExpresion  { sprintf(temp, "%s", $2.code);
                                           $$.code = gen_code(temp);
                                           $$.function = '0';}
                ;

llamadaOExpresion:    IDENTIF '(' argsLlamada ')' { sprintf(temp, "(%s %s)", $1.code, $3.code);
                                                    $$.code = gen_code(temp); }
                    | expresion                   { sprintf(temp, "%s", $1.code);
                                                    $$.code = gen_code(temp); }
                    ;

argsLlamada:      /* lamda */               { sprintf(temp, "");
                                              $$.code = gen_code(temp); }
                | expresion otroArgLlamada  { sprintf(temp, "%s%s", $1.code, $2.code);
                                              $$.code = gen_code(temp); }
                ;

otroArgLlamada:   /* lamda */                   { sprintf(temp, "");
                                                  $$.code = gen_code(temp); }
                | ',' expresion otroArgLlamada  { sprintf(temp, " %s%s", $2.code, $3.code);
                                                  $$.code = gen_code(temp); }
                ;

printArgs:    /*lamda*/                 { sprintf(temp, "");
                                          $$.code = gen_code(temp);}
            | ',' otroPrint printArgs   { sprintf(temp, "(princ %s) %s", $2.code, $3.code);
                                          $$.code = gen_code(temp);}
            ;

otroPrint:   llamadaOExpresion  { sprintf(temp, "%s", $1.code);
                          $$.code = gen_code(temp);}
            | STRING    { sprintf(temp, "\"%s\"", $1.code);
                          $$.code = gen_code(temp);}
            ;

asignacion: /* lamda */       { sprintf (temp, "0") ;
                                $$.code = gen_code (temp) ;}
            | '=' llamadaOExpresion   { sprintf (temp, "%s", $2.code) ;
                                $$.code = gen_code (temp) ; }
            ;

expresion:    termino                           { $$ = $1 ; }
            | llamadaOExpresion '+' llamadaOExpresion           { sprintf (temp, "(+ %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ; }
            | llamadaOExpresion '-' llamadaOExpresion           { sprintf (temp, "(- %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ; }
            | llamadaOExpresion '*' llamadaOExpresion           { sprintf (temp, "(* %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ; }
            | llamadaOExpresion '/' llamadaOExpresion           { sprintf (temp, "(/ %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ; }
            | llamadaOExpresion '%' llamadaOExpresion           { sprintf (temp, "(mod %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ; }
            | IDENTIF '[' llamadaOExpresion ']'         { sprintf (temp, "(aref %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ; }
            | llamadaOExpresion AND llamadaOExpresion           { sprintf (temp, "(and %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | llamadaOExpresion OR llamadaOExpresion            { sprintf (temp, "(or %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | llamadaOExpresion IGUAL llamadaOExpresion         { sprintf (temp, "(= %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | llamadaOExpresion DISTINTO llamadaOExpresion      { sprintf (temp, "(/= %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | llamadaOExpresion '<' llamadaOExpresion           { sprintf (temp, "(< %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | llamadaOExpresion '>' llamadaOExpresion           { sprintf (temp, "(> %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | llamadaOExpresion MAYOR_IGUAL llamadaOExpresion   { sprintf (temp, "(>= %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | llamadaOExpresion MENOR_IGUAL llamadaOExpresion   { sprintf (temp, "(<= %s %s)", $1.code, $3.code) ;
                                                  $$.code = gen_code (temp) ;}
            | '!' llamadaOExpresion                     { sprintf (temp, "(not %s)", $2.code) ;
                                                  $$.code = gen_code (temp) ;}
            ;


termino:        operando                           { $$ = $1 ; }                          
            |   '+' operando %prec UNARY_SIGN      { $$ = $1 ; }
            |   '-' operando %prec UNARY_SIGN      { sprintf (temp, "(- %s)", $2.code) ;
                                                     $$.code = gen_code (temp) ; }

            ;

operando:       IDENTIF                  { if (strlen(current_func) > 0)
                                               sprintf(temp, "%s_%s", current_func, $1.code);
                                           else
                                               sprintf(temp, "%s", $1.code);
                                           $$.code = gen_code (temp) ; }
            |   NUMBER                   { sprintf (temp, "%d", $1.value) ;
                                           $$.code = gen_code (temp) ; }
            |   '(' expresion ')'        { $$ = $2 ; }
            ;


%%                            // SECCION 4    Codigo en C

int n_line = 1 ;

int yyerror (mensaje)
char *mensaje ;
{
    fprintf (stderr, "%s en la linea %d\n", mensaje, n_line) ;
    printf ( "\n") ;	// bye
}

char *int_to_string (int n)
{
    sprintf (temp, "%d", n) ;
    return gen_code (temp) ;
}

char *char_to_string (char c)
{
    sprintf (temp, "%c", c) ;
    return gen_code (temp) ;
}

char *my_malloc (int nbytes)       // reserva n bytes de memoria dinamica
{
    char *p ;
    static long int nb = 0;        // sirven para contabilizar la memoria
    static int nv = 0 ;            // solicitada en total

    p = malloc (nbytes) ;
    if (p == NULL) {
        fprintf (stderr, "No queda memoria para %d bytes mas\n", nbytes) ;
        fprintf (stderr, "Reservados %ld bytes en %d llamadas\n", nb, nv) ;
        exit (0) ;
    }
    nb += (long) nbytes ;
    nv++ ;

    return p ;
}


/***************************************************************************/
/********************** Seccion de Palabras Reservadas *********************/
/***************************************************************************/

typedef struct s_keyword { // para las palabras reservadas de C
    char *name ;
    int token ;
} t_keyword ;

t_keyword keywords [] = { // define las palabras reservadas y los
    "main",         MAIN,           // y los token asociados
    "int",          INTEGER,
    "puts",         PUTS,
    "if",           IF,
    "else",         ELSE,
    "return",       RETURN,
    "printf",       PRINTF,
    "==",           IGUAL,
    "while",        WHILE,
    "!=",           DISTINTO,
    "for",          FOR,
    "<=",           MENOR_IGUAL,
    ">=",           MAYOR_IGUAL,
    "&&",           AND,
    "||",           OR,
    NULL,           0               // para marcar el fin de la tabla
} ;

t_keyword *search_keyword (char *symbol_name)
{                                  // Busca n_s en la tabla de pal. res.
                                   // y devuelve puntero a registro (simbolo)
    int i ;
    t_keyword *sim ;

    i = 0 ;
    sim = keywords ;
    while (sim [i].name != NULL) {
	    if (strcmp (sim [i].name, symbol_name) == 0) {
		                             // strcmp(a, b) devuelve == 0 si a==b
            return &(sim [i]) ;
        }
        i++ ;
    }

    return NULL ;
}

 
/***************************************************************************/
/******************* Seccion del Analizador Lexicografico ******************/
/***************************************************************************/

char *gen_code (char *name)     // copia el argumento a un
{                                      // string en memoria dinamica
    char *p ;
    int l ;
	
    l = strlen (name)+1 ;
    p = (char *) my_malloc (l) ;
    strcpy (p, name) ;
	
    return p ;
}


int yylex ()
{
// NO MODIFICAR ESTA FUNCION SIN PERMISO
    int i ;
    unsigned char c ;
    unsigned char cc ;
    char ops_expandibles [] = "!<=|>%&/+-*" ;
    char temp_str [256] ;
    t_keyword *symbol ;

    do {
        c = getchar () ;

        if (c == '#') {	// Ignora las lineas que empiezan por #  (#define, #include)
            do {		//	OJO que puede funcionar mal si una linea contiene #
                c = getchar () ;
            } while (c != '\n') ;
        }

        if (c == '/') {	// Si la linea contiene un / puede ser inicio de comentario
            cc = getchar () ;
            if (cc != '/') {   // Si el siguiente char es /  es un comentario, pero...
                ungetc (cc, stdin) ;
            } else {
                c = getchar () ;	// ...
                if (c == '@') {	// Si es la secuencia //@  ==> transcribimos la linea
                    do {		// Se trata de codigo inline (Codigo embebido en C)
                        c = getchar () ;
                        putchar (c) ;
                    } while (c != '\n') ;
                } else {		// ==> comentario, ignorar la linea
                    while (c != '\n') {
                        c = getchar () ;
                    }
                }
            }
        } else if (c == '\\') c = getchar () ;
		
        if (c == '\n')
            n_line++ ;

    } while (c == ' ' || c == '\n' || c == 10 || c == 13 || c == '\t') ;

    if (c == '\"') {
        i = 0 ;
        do {
            c = getchar () ;
            temp_str [i++] = c ;
        } while (c != '\"' && i < 255) ;
        if (i == 256) {
            printf ("AVISO: string con mas de 255 caracteres en linea %d\n", n_line) ;
        }		 	// habria que leer hasta el siguiente " , pero, y si falta?
        temp_str [--i] = '\0' ;
        yylval.code = gen_code (temp_str) ;
        return (STRING) ;
    }

    if (c == '.' || (c >= '0' && c <= '9')) {
        ungetc (c, stdin) ;
        scanf ("%d", &yylval.value) ;
//         printf ("\nDEV: NUMBER %d\n", yylval.value) ;        // PARA DEPURAR
        return NUMBER ;
    }

    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        i = 0 ;
        while (((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '_') && i < 255) {
            temp_str [i++] = tolower (c) ;
            c = getchar () ;
        }
        temp_str [i] = '\0' ;
        ungetc (c, stdin) ;

        yylval.code = gen_code (temp_str) ;
        symbol = search_keyword (yylval.code) ;
        if (symbol == NULL) {    // no es palabra reservada -> identificador antes vrariabre
//               printf ("\nDEV: IDENTIF %s\n", yylval.code) ;    // PARA DEPURAR
            return (IDENTIF) ;
        } else {
//               printf ("\nDEV: OTRO %s\n", yylval.code) ;       // PARA DEPURAR
            return (symbol->token) ;
        }
    }

    if (strchr (ops_expandibles, c) != NULL) { // busca c en ops_expandibles
        cc = getchar () ;
        sprintf (temp_str, "%c%c", (char) c, (char) cc) ;
        symbol = search_keyword (temp_str) ;
        if (symbol == NULL) {
            ungetc (cc, stdin) ;
            yylval.code = NULL ;
            return (c) ;
        } else {
            yylval.code = gen_code (temp_str) ; // aunque no se use
            return (symbol->token) ;
        }
    }

//    printf ("\nDEV: LITERAL %d #%c#\n", (int) c, c) ;      // PARA DEPURAR
    if (c == EOF || c == 255 || c == 26) {
//         printf ("tEOF ") ;                                // PARA DEPURAR
        return (0) ;
    }

    return c ;
}


int main ()
{
    yyparse () ;
}
