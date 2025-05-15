/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 4 "trad3.y"
                          // SECCION 1 Declaraciones de C-Yacc

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


#line 115 "trad3.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    IDENTIF = 259,                 /* IDENTIF  */
    INTEGER = 260,                 /* INTEGER  */
    STRING = 261,                  /* STRING  */
    MAIN = 262,                    /* MAIN  */
    WHILE = 263,                   /* WHILE  */
    PUTS = 264,                    /* PUTS  */
    IF = 265,                      /* IF  */
    ELSE = 266,                    /* ELSE  */
    RETURN = 267,                  /* RETURN  */
    PRINTF = 268,                  /* PRINTF  */
    IGUAL = 269,                   /* IGUAL  */
    DISTINTO = 270,                /* DISTINTO  */
    FOR = 271,                     /* FOR  */
    MENOR_IGUAL = 272,             /* MENOR_IGUAL  */
    MAYOR_IGUAL = 273,             /* MAYOR_IGUAL  */
    AND = 274,                     /* AND  */
    OR = 275,                      /* OR  */
    UNARY_SIGN = 276               /* UNARY_SIGN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_IDENTIF = 4,                    /* IDENTIF  */
  YYSYMBOL_INTEGER = 5,                    /* INTEGER  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_MAIN = 7,                       /* MAIN  */
  YYSYMBOL_WHILE = 8,                      /* WHILE  */
  YYSYMBOL_PUTS = 9,                       /* PUTS  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_PRINTF = 13,                    /* PRINTF  */
  YYSYMBOL_IGUAL = 14,                     /* IGUAL  */
  YYSYMBOL_DISTINTO = 15,                  /* DISTINTO  */
  YYSYMBOL_FOR = 16,                       /* FOR  */
  YYSYMBOL_MENOR_IGUAL = 17,               /* MENOR_IGUAL  */
  YYSYMBOL_MAYOR_IGUAL = 18,               /* MAYOR_IGUAL  */
  YYSYMBOL_AND = 19,                       /* AND  */
  YYSYMBOL_OR = 20,                        /* OR  */
  YYSYMBOL_21_ = 21,                       /* '+'  */
  YYSYMBOL_22_ = 22,                       /* '-'  */
  YYSYMBOL_23_ = 23,                       /* '*'  */
  YYSYMBOL_24_ = 24,                       /* '/'  */
  YYSYMBOL_25_ = 25,                       /* '%'  */
  YYSYMBOL_UNARY_SIGN = 26,                /* UNARY_SIGN  */
  YYSYMBOL_27_ = 27,                       /* '<'  */
  YYSYMBOL_28_ = 28,                       /* '>'  */
  YYSYMBOL_29_ = 29,                       /* '!'  */
  YYSYMBOL_30_ = 30,                       /* ';'  */
  YYSYMBOL_31_ = 31,                       /* '['  */
  YYSYMBOL_32_ = 32,                       /* ']'  */
  YYSYMBOL_33_ = 33,                       /* ','  */
  YYSYMBOL_34_ = 34,                       /* '('  */
  YYSYMBOL_35_ = 35,                       /* ')'  */
  YYSYMBOL_36_ = 36,                       /* '{'  */
  YYSYMBOL_37_ = 37,                       /* '}'  */
  YYSYMBOL_38_ = 38,                       /* '@'  */
  YYSYMBOL_39_ = 39,                       /* '='  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_axioma = 41,                    /* axioma  */
  YYSYMBOL_42_1 = 42,                      /* $@1  */
  YYSYMBOL_declVars = 43,                  /* declVars  */
  YYSYMBOL_cadenaDecl = 44,                /* cadenaDecl  */
  YYSYMBOL_defFunc = 45,                   /* defFunc  */
  YYSYMBOL_46_2 = 46,                      /* $@2  */
  YYSYMBOL_set_main = 47,                  /* set_main  */
  YYSYMBOL_main_func = 48,                 /* main_func  */
  YYSYMBOL_set_func = 49,                  /* set_func  */
  YYSYMBOL_name_func = 50,                 /* name_func  */
  YYSYMBOL_argumentos = 51,                /* argumentos  */
  YYSYMBOL_masArgs = 52,                   /* masArgs  */
  YYSYMBOL_sentencias = 53,                /* sentencias  */
  YYSYMBOL_sentencia = 54,                 /* sentencia  */
  YYSYMBOL_retorno = 55,                   /* retorno  */
  YYSYMBOL_posibleElse = 56,               /* posibleElse  */
  YYSYMBOL_llamadaOAsignacion = 57,        /* llamadaOAsignacion  */
  YYSYMBOL_llamadaOExpresion = 58,         /* llamadaOExpresion  */
  YYSYMBOL_argsLlamada = 59,               /* argsLlamada  */
  YYSYMBOL_otroArgLlamada = 60,            /* otroArgLlamada  */
  YYSYMBOL_printArgs = 61,                 /* printArgs  */
  YYSYMBOL_otroPrint = 62,                 /* otroPrint  */
  YYSYMBOL_asignacion = 63,                /* asignacion  */
  YYSYMBOL_expresion = 64,                 /* expresion  */
  YYSYMBOL_termino = 65,                   /* termino  */
  YYSYMBOL_operando = 66                   /* operando  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   221

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  179

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   276


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,     2,     2,    25,     2,     2,
      34,    35,    23,    21,    33,    22,     2,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
      27,    39,    28,     2,    38,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    36,     2,    37,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    26
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    79,    79,    79,    83,    88,    90,    94,    99,   103,
     104,   104,   106,   109,   111,   115,   117,   121,   123,   127,
     129,   133,   135,   139,   147,   149,   151,   153,   155,   157,
     159,   163,   165,   168,   170,   174,   177,   182,   184,   188,
     190,   194,   196,   200,   202,   206,   208,   212,   214,   218,
     219,   221,   223,   225,   227,   229,   231,   233,   235,   237,
     239,   241,   243,   245,   247,   252,   253,   254,   259,   264,
     266
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "IDENTIF",
  "INTEGER", "STRING", "MAIN", "WHILE", "PUTS", "IF", "ELSE", "RETURN",
  "PRINTF", "IGUAL", "DISTINTO", "FOR", "MENOR_IGUAL", "MAYOR_IGUAL",
  "AND", "OR", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY_SIGN", "'<'",
  "'>'", "'!'", "';'", "'['", "']'", "','", "'('", "')'", "'{'", "'}'",
  "'@'", "'='", "$accept", "axioma", "$@1", "declVars", "cadenaDecl",
  "defFunc", "$@2", "set_main", "main_func", "set_func", "name_func",
  "argumentos", "masArgs", "sentencias", "sentencia", "retorno",
  "posibleElse", "llamadaOAsignacion", "llamadaOExpresion", "argsLlamada",
  "otroArgLlamada", "printArgs", "otroPrint", "asignacion", "expresion",
  "termino", "operando", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-126)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-42)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,    11,    21,  -126,   -22,  -126,     4,    22,    80,    -5,
    -126,    -1,  -126,  -126,    15,  -126,  -126,  -126,    80,    43,
      40,    22,    22,    80,   141,  -126,  -126,  -126,    49,    47,
      45,    73,     4,   141,    50,    56,    80,    80,  -126,  -126,
    -126,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    84,     1,  -126,    87,    62,  -126,
    -126,     1,   105,    64,    32,   153,   153,  -126,  -126,    90,
      90,   165,   165,    42,    42,    42,  -126,  -126,    61,  -126,
      59,    70,    68,  -126,  -126,  -126,    80,  -126,    -5,     1,
     101,  -126,     1,    32,  -126,    14,   106,    14,  -126,   -27,
      77,    78,    79,    81,    97,    80,   103,    14,    70,   109,
      80,    80,    80,   104,    80,   144,    80,   147,   163,   127,
    -126,  -126,  -126,    80,   142,   154,   152,   141,  -126,   156,
     159,   160,   164,    61,  -126,   124,  -126,   -29,  -126,   162,
     166,   167,    60,   169,   170,  -126,   171,    14,  -126,    14,
    -126,   141,   164,   172,    80,  -126,   109,   109,  -126,  -126,
     175,   173,   174,   195,  -126,   196,    61,   176,  -126,   178,
      14,   179,   109,    14,   177,   109,  -126,   180,  -126
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       6,     0,     0,     2,    47,     1,    12,     0,     0,     8,
      15,     0,     3,     9,     0,    10,    69,    68,     0,     0,
      68,     0,     0,     0,    48,    38,    49,    65,     0,     0,
       0,    17,    12,     0,    38,     0,     0,    39,    66,    67,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     6,    13,     0,     0,    11,
      70,     6,     0,     0,    38,    58,    59,    63,    62,    56,
      57,    50,    51,    52,    53,    54,    60,    61,    47,     4,
       0,    19,     0,     5,    55,    37,     0,    40,     8,     6,
       0,    18,     6,    38,     7,    21,     0,    21,    42,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    19,    32,
       0,    39,     0,     0,     0,     0,     0,     0,     0,    38,
      14,    22,    20,     0,     0,    38,     0,    36,    23,    38,
       0,    38,    43,    47,    25,     0,    16,     0,    35,     0,
       0,     0,     0,     0,     0,    31,     0,    21,    26,    21,
      46,    45,    43,     0,     0,    24,    32,    32,    44,    30,
      38,     0,     0,     0,    28,    33,    47,     0,    27,     0,
      21,     0,    32,    21,     0,    32,    34,     0,    29
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -126,  -126,  -126,   -41,   118,   184,  -126,  -126,  -126,  -126,
    -126,  -126,   100,   -94,  -126,  -125,  -126,    72,    -7,   107,
     126,    69,  -126,   -76,   -18,  -126,    51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     6,     3,    29,    12,    32,    80,    13,    14,
      15,    58,    91,   106,   107,   124,   168,   113,    33,    63,
      87,   143,   152,     9,    25,    26,    27
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      34,    24,    88,   109,   110,   111,     1,   111,    10,     7,
     112,    11,   112,   121,    79,     4,    40,     8,    99,    64,
      83,     5,   100,   101,   102,    16,    17,   103,    28,    62,
     104,   161,   162,    30,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   174,    95,    31,
     177,    97,   105,   156,    54,   157,    18,   144,    19,    43,
      44,    45,    46,    16,    20,    86,   150,   -41,    93,    52,
      53,    36,    38,    39,    37,    35,   172,    55,    57,   175,
      56,    21,    22,    16,    20,    60,    61,   119,    78,    23,
     169,    81,   125,    64,    18,    89,   129,    82,   131,    85,
       8,    21,    22,    90,    92,   127,    96,    43,    44,    23,
     108,   114,   115,   116,    18,   117,   135,    52,    53,    41,
      42,   123,    43,    44,    45,    46,    47,    48,    49,    50,
      51,   118,    52,    53,   128,   151,   160,    84,    41,    42,
     120,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     130,    52,    53,   132,   145,    41,    42,   134,    43,    44,
      45,    46,    47,    48,    49,    50,    51,   133,    52,    53,
      43,    44,    45,    46,    47,    48,    49,    50,    51,   136,
      52,    53,    43,    44,    45,    46,   137,   138,    49,    50,
      51,   139,    52,    53,   140,   141,   148,   142,   147,   166,
     154,   155,   159,   149,   153,   163,    94,   167,   122,   146,
     164,   165,   170,   171,   176,   173,    59,   178,   126,    98,
       0,   158
};

static const yytype_int16 yycheck[] =
{
      18,     8,    78,    97,    31,    34,     5,    34,     4,    31,
      39,     7,    39,   107,    55,     4,    23,    39,     4,    37,
      61,     0,     8,     9,    10,     3,     4,    13,    33,    36,
      16,   156,   157,    34,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,   172,    89,    34,
     175,    92,    38,   147,     5,   149,    34,   133,     7,    17,
      18,    19,    20,     3,     4,    33,     6,    35,    86,    27,
      28,    31,    21,    22,    34,    32,   170,    30,     5,   173,
      35,    21,    22,     3,     4,    35,    30,   105,     4,    29,
     166,     4,   110,   111,    34,    36,   114,    35,   116,    35,
      39,    21,    22,    33,    36,   112,     5,    17,    18,    29,
       4,    34,    34,    34,    34,    34,   123,    27,    28,    14,
      15,    12,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    34,    27,    28,    30,   142,   154,    32,    14,    15,
      37,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       6,    27,    28,     6,    30,    14,    15,    30,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     4,    27,    28,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    37,
      27,    28,    17,    18,    19,    20,    32,    35,    23,    24,
      25,    35,    27,    28,    35,    35,    30,    33,    36,     4,
      30,    30,    30,    36,    35,    30,    88,    11,   108,   137,
      37,    37,    36,    35,    37,    36,    32,    37,   111,    93,
      -1,   152
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,    41,    43,     4,     0,    42,    31,    39,    63,
       4,     7,    45,    48,    49,    50,     3,     4,    34,    66,
       4,    21,    22,    29,    58,    64,    65,    66,    33,    44,
      34,    34,    46,    58,    64,    32,    31,    34,    66,    66,
      58,    14,    15,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    27,    28,     5,    30,    35,     5,    51,    45,
      35,    30,    58,    59,    64,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,     4,    43,
      47,     4,    35,    43,    32,    35,    33,    60,    63,    36,
      33,    52,    36,    64,    44,    43,     5,    43,    60,     4,
       8,     9,    10,    13,    16,    38,    53,    54,     4,    53,
      31,    34,    39,    57,    34,    34,    34,    34,    34,    64,
      37,    53,    52,    12,    55,    64,    59,    58,    30,    64,
       6,    64,     6,     4,    30,    58,    37,    32,    35,    35,
      35,    35,    33,    61,    63,    30,    57,    36,    30,    36,
       6,    58,    62,    35,    30,    30,    53,    53,    61,    30,
      64,    55,    55,    30,    37,    37,     4,    11,    56,    63,
      36,    35,    53,    36,    55,    53,    37,    55,    37
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    42,    41,    43,    43,    43,    44,    44,    45,
      46,    45,    45,    47,    48,    49,    50,    51,    51,    52,
      52,    53,    53,    54,    54,    54,    54,    54,    54,    54,
      54,    55,    55,    56,    56,    57,    57,    58,    58,    59,
      59,    60,    60,    61,    61,    62,    62,    63,    63,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    65,    65,    65,    66,    66,
      66
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     3,     6,     7,     0,     5,     0,     1,
       0,     3,     0,     0,     8,     1,     9,     0,     3,     0,
       4,     0,     2,     3,     6,     3,     5,     9,     8,    14,
       6,     3,     0,     0,     5,     3,     2,     4,     1,     0,
       2,     0,     3,     0,     3,     1,     1,     0,     2,     1,
       3,     3,     3,     3,     3,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     2,     2,     1,     1,
       3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 79 "trad3.y"
                                     { printf("%s", yyvsp[0].code); }
#line 1340 "trad3.tab.c"
    break;

  case 3: /* axioma: declVars $@1 defFunc  */
#line 80 "trad3.y"
                                     { ; }
#line 1346 "trad3.tab.c"
    break;

  case 4: /* declVars: INTEGER IDENTIF asignacion cadenaDecl ';' declVars  */
#line 83 "trad3.y"
                                                                { if (strlen(current_func) > 0)
                                                                      sprintf(temp, "(setq %s_%s %s)%s\n%s", current_func, yyvsp[-4].code, yyvsp[-3].code, yyvsp[-2].code, yyvsp[0].code);
                                                                  else
                                                                      sprintf(temp, "(setq %s %s)%s\n%s", yyvsp[-4].code, yyvsp[-3].code, yyvsp[-2].code, yyvsp[0].code);
                                                                  yyval.code = gen_code(temp) ;}
#line 1356 "trad3.tab.c"
    break;

  case 5: /* declVars: INTEGER IDENTIF '[' operando ']' ';' declVars  */
#line 88 "trad3.y"
                                                                { sprintf (temp, "setq %s (make-array %s)\n%s", yyvsp[-5].code, yyvsp[-3].code, yyvsp[0].code);
                                                                    yyval.code = gen_code (temp) ; }
#line 1363 "trad3.tab.c"
    break;

  case 6: /* declVars: %empty  */
#line 90 "trad3.y"
                                                                { sprintf (temp, "");
                                                                  yyval.code = gen_code (temp) ; }
#line 1370 "trad3.tab.c"
    break;

  case 7: /* cadenaDecl: ',' INTEGER IDENTIF asignacion cadenaDecl  */
#line 94 "trad3.y"
                                                        { if (strlen(current_func) > 0)
                                                              sprintf(temp, "(setq %s_%s %s)%s", current_func, yyvsp[-2].code, yyvsp[-1].code, yyvsp[0].code);
                                                          else
                                                              sprintf(temp, "(setq %s %s)%s", yyvsp[-2].code, yyvsp[-1].code, yyvsp[0].code);
                                                          yyval.code = gen_code (temp) ; }
#line 1380 "trad3.tab.c"
    break;

  case 8: /* cadenaDecl: %empty  */
#line 99 "trad3.y"
                                                        { sprintf (temp, "");
                                                          yyval.code = gen_code (temp) ; }
#line 1387 "trad3.tab.c"
    break;

  case 9: /* defFunc: main_func  */
#line 103 "trad3.y"
                                { printf ("%s\n", yyvsp[0].code) ; }
#line 1393 "trad3.tab.c"
    break;

  case 10: /* $@2: %empty  */
#line 104 "trad3.y"
                                { printf ("%s\n", yyvsp[0].code) ; }
#line 1399 "trad3.tab.c"
    break;

  case 11: /* defFunc: name_func $@2 defFunc  */
#line 105 "trad3.y"
                                { ; }
#line 1405 "trad3.tab.c"
    break;

  case 12: /* defFunc: %empty  */
#line 106 "trad3.y"
                                { ; }
#line 1411 "trad3.tab.c"
    break;

  case 13: /* set_main: %empty  */
#line 109 "trad3.y"
                       { strcpy(current_func, "main"); }
#line 1417 "trad3.tab.c"
    break;

  case 14: /* main_func: MAIN '(' ')' set_main '{' declVars sentencias '}'  */
#line 111 "trad3.y"
                                                                { sprintf (temp, "(defun main()\n\t%s%s)", yyvsp[-2].code, yyvsp[-1].code) ;
                                                                  yyval.code = gen_code (temp) ; }
#line 1424 "trad3.tab.c"
    break;

  case 15: /* set_func: IDENTIF  */
#line 115 "trad3.y"
                  { strcpy(current_func, yyvsp[0].code); yyval = yyvsp[0]; }
#line 1430 "trad3.tab.c"
    break;

  case 16: /* name_func: set_func '(' argumentos ')' '{' declVars sentencias retorno '}'  */
#line 117 "trad3.y"
                                                                            { sprintf(temp, "(defun %s (%s)\n\t%s %s %s\n)", yyvsp[-8].code, yyvsp[-6].code, yyvsp[-3].code, yyvsp[-2].code, yyvsp[-1].code);
                                                                              yyval.code = gen_code(temp); }
#line 1437 "trad3.tab.c"
    break;

  case 17: /* argumentos: %empty  */
#line 121 "trad3.y"
                                            { sprintf (temp, "");
                                              yyval.code = gen_code (temp) ;}
#line 1444 "trad3.tab.c"
    break;

  case 18: /* argumentos: INTEGER IDENTIF masArgs  */
#line 123 "trad3.y"
                                            { sprintf (temp, "%s%s", yyvsp[-1].code, yyvsp[0].code);
                                              yyval.code = gen_code (temp) ;}
#line 1451 "trad3.tab.c"
    break;

  case 19: /* masArgs: %empty  */
#line 127 "trad3.y"
                                            { sprintf (temp, "");
                                              yyval.code = gen_code (temp) ;}
#line 1458 "trad3.tab.c"
    break;

  case 20: /* masArgs: ',' INTEGER IDENTIF masArgs  */
#line 129 "trad3.y"
                                            { sprintf (temp, ",%s%s", yyvsp[-1].code, yyvsp[0].code);
                                              yyval.code = gen_code (temp) ;}
#line 1465 "trad3.tab.c"
    break;

  case 21: /* sentencias: %empty  */
#line 133 "trad3.y"
                                            { sprintf (temp, "");
                                              yyval.code = gen_code (temp) ; }
#line 1472 "trad3.tab.c"
    break;

  case 22: /* sentencias: sentencia sentencias  */
#line 135 "trad3.y"
                                            { sprintf (temp, "%s\n%s", yyvsp[-1].code, yyvsp[0].code);
                                              yyval.code = gen_code (temp) ; }
#line 1479 "trad3.tab.c"
    break;

  case 23: /* sentencia: IDENTIF llamadaOAsignacion ';'  */
#line 139 "trad3.y"
                                                                                                            { if (yyvsp[-1].function == '0'){
                                                                                                                if (strlen(current_func) > 0)
                                                                                                                  sprintf(temp, "(setf %s_%s %s)", current_func, yyvsp[-2].code, yyvsp[-1].code);
                                                                                                                else
                                                                                                                  sprintf(temp, "(setf %s %s)", yyvsp[-2].code, yyvsp[-1].code);
                                                                                                              }
                                                                                                              else { sprintf(temp, "\t(%s%s)", yyvsp[-2].code, yyvsp[-1].code);}
                                                                                                              yyval.code = gen_code (temp) ; }
#line 1492 "trad3.tab.c"
    break;

  case 24: /* sentencia: IDENTIF '[' expresion ']' llamadaOAsignacion ';'  */
#line 147 "trad3.y"
                                                                                                            { sprintf(temp, "(setf (aref %s %s) %s)", yyvsp[-5].code, yyvsp[-3].code, yyvsp[-1].code);
                                                                                                              yyval.code = gen_code(temp); }
#line 1499 "trad3.tab.c"
    break;

  case 25: /* sentencia: '@' expresion ';'  */
#line 149 "trad3.y"
                                                                                                            { sprintf (temp, "\t(print %s)", yyvsp[-1].code) ;
                                                                                                              yyval.code = gen_code (temp) ; }
#line 1506 "trad3.tab.c"
    break;

  case 26: /* sentencia: PUTS '(' STRING ')' ';'  */
#line 151 "trad3.y"
                                                                                                            { sprintf (temp, "\t(print \"%s\")", yyvsp[-2].code) ;
                                                                                                              yyval.code = gen_code (temp) ; }
#line 1513 "trad3.tab.c"
    break;

  case 27: /* sentencia: IF '(' expresion ')' '{' sentencias retorno '}' posibleElse  */
#line 153 "trad3.y"
                                                                                                            { sprintf(temp, "(if %s\n\t(progn %s\n%s)%s\t)", yyvsp[-6].code, yyvsp[-3].code, yyvsp[-2].code, yyvsp[0].code);
                                                                                                              yyval.code = gen_code(temp) ; }
#line 1520 "trad3.tab.c"
    break;

  case 28: /* sentencia: WHILE '(' expresion ')' '{' sentencias retorno '}'  */
#line 155 "trad3.y"
                                                                                                            { sprintf (temp, "\t(loop while %s do\n\t%s\n%s)", yyvsp[-5].code, yyvsp[-2].code, yyvsp[-1].code) ;
                                                                                                              yyval.code = gen_code (temp) ;}
#line 1527 "trad3.tab.c"
    break;

  case 29: /* sentencia: FOR '(' IDENTIF asignacion ';' expresion ';' IDENTIF asignacion ')' '{' sentencias retorno '}'  */
#line 157 "trad3.y"
                                                                                                            { sprintf (temp, "\t(loop for %s do\n\t%s\n%s", yyvsp[-8].code, yyvsp[-2].code, yyvsp[-1].code) ;
                                                                                                              yyval.code = gen_code (temp) ;}
#line 1534 "trad3.tab.c"
    break;

  case 30: /* sentencia: PRINTF '(' STRING printArgs ')' ';'  */
#line 159 "trad3.y"
                                                                                                            { sprintf(temp, "(%s)", yyvsp[-2].code);
                                                                                                              yyval.code = gen_code(temp);}
#line 1541 "trad3.tab.c"
    break;

  case 31: /* retorno: RETURN llamadaOExpresion ';'  */
#line 163 "trad3.y"
                                        { sprintf(temp, "return-from-%s %s", current_func, yyvsp[-1].code);
                                  yyval.code = gen_code(temp); }
#line 1548 "trad3.tab.c"
    break;

  case 32: /* retorno: %empty  */
#line 165 "trad3.y"
                                { ; }
#line 1554 "trad3.tab.c"
    break;

  case 33: /* posibleElse: %empty  */
#line 168 "trad3.y"
                                        { sprintf(temp, "");
                                          yyval.code = gen_code(temp); }
#line 1561 "trad3.tab.c"
    break;

  case 34: /* posibleElse: ELSE '{' sentencias retorno '}'  */
#line 170 "trad3.y"
                                                { sprintf(temp, "\n\t(progn %s\n%s)", yyvsp[-2].code, yyvsp[-1].code);
                                          yyval.code = gen_code(temp); }
#line 1568 "trad3.tab.c"
    break;

  case 35: /* llamadaOAsignacion: '(' argsLlamada ')'  */
#line 174 "trad3.y"
                                         { sprintf(temp, " %s", yyvsp[-1].code);
                                           yyval.code = gen_code(temp);
                                           yyval.function = '1'; }
#line 1576 "trad3.tab.c"
    break;

  case 36: /* llamadaOAsignacion: '=' llamadaOExpresion  */
#line 177 "trad3.y"
                                         { sprintf(temp, "%s", yyvsp[0].code);
                                           yyval.code = gen_code(temp);
                                           yyval.function = '0';}
#line 1584 "trad3.tab.c"
    break;

  case 37: /* llamadaOExpresion: IDENTIF '(' argsLlamada ')'  */
#line 182 "trad3.y"
                                                  { sprintf(temp, "(%s %s)", yyvsp[-3].code, yyvsp[-1].code);
                                                    yyval.code = gen_code(temp); }
#line 1591 "trad3.tab.c"
    break;

  case 38: /* llamadaOExpresion: expresion  */
#line 184 "trad3.y"
                                                  { sprintf(temp, "%s", yyvsp[0].code);
                                                    yyval.code = gen_code(temp); }
#line 1598 "trad3.tab.c"
    break;

  case 39: /* argsLlamada: %empty  */
#line 188 "trad3.y"
                                            { sprintf(temp, "");
                                              yyval.code = gen_code(temp); }
#line 1605 "trad3.tab.c"
    break;

  case 40: /* argsLlamada: expresion otroArgLlamada  */
#line 190 "trad3.y"
                                            { sprintf(temp, "%s%s", yyvsp[-1].code, yyvsp[0].code);
                                              yyval.code = gen_code(temp); }
#line 1612 "trad3.tab.c"
    break;

  case 41: /* otroArgLlamada: %empty  */
#line 194 "trad3.y"
                                                { sprintf(temp, "");
                                                  yyval.code = gen_code(temp); }
#line 1619 "trad3.tab.c"
    break;

  case 42: /* otroArgLlamada: ',' expresion otroArgLlamada  */
#line 196 "trad3.y"
                                                { sprintf(temp, " %s%s", yyvsp[-1].code, yyvsp[0].code);
                                                  yyval.code = gen_code(temp); }
#line 1626 "trad3.tab.c"
    break;

  case 43: /* printArgs: %empty  */
#line 200 "trad3.y"
                                        { sprintf(temp, "");
                                          yyval.code = gen_code(temp);}
#line 1633 "trad3.tab.c"
    break;

  case 44: /* printArgs: ',' otroPrint printArgs  */
#line 202 "trad3.y"
                                        { sprintf(temp, "(princ %s) %s", yyvsp[-1].code, yyvsp[0].code);
                                          yyval.code = gen_code(temp);}
#line 1640 "trad3.tab.c"
    break;

  case 45: /* otroPrint: llamadaOExpresion  */
#line 206 "trad3.y"
                                { sprintf(temp, "%s", yyvsp[0].code);
                          yyval.code = gen_code(temp);}
#line 1647 "trad3.tab.c"
    break;

  case 46: /* otroPrint: STRING  */
#line 208 "trad3.y"
                        { sprintf(temp, "\"%s\"", yyvsp[0].code);
                          yyval.code = gen_code(temp);}
#line 1654 "trad3.tab.c"
    break;

  case 47: /* asignacion: %empty  */
#line 212 "trad3.y"
                              { sprintf (temp, "0") ;
                                yyval.code = gen_code (temp) ;}
#line 1661 "trad3.tab.c"
    break;

  case 48: /* asignacion: '=' llamadaOExpresion  */
#line 214 "trad3.y"
                                      { sprintf (temp, "%s", yyvsp[0].code) ;
                                yyval.code = gen_code (temp) ; }
#line 1668 "trad3.tab.c"
    break;

  case 49: /* expresion: termino  */
#line 218 "trad3.y"
                                                { yyval = yyvsp[0] ; }
#line 1674 "trad3.tab.c"
    break;

  case 50: /* expresion: llamadaOExpresion '+' llamadaOExpresion  */
#line 219 "trad3.y"
                                                                { sprintf (temp, "(+ %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ; }
#line 1681 "trad3.tab.c"
    break;

  case 51: /* expresion: llamadaOExpresion '-' llamadaOExpresion  */
#line 221 "trad3.y"
                                                                { sprintf (temp, "(- %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ; }
#line 1688 "trad3.tab.c"
    break;

  case 52: /* expresion: llamadaOExpresion '*' llamadaOExpresion  */
#line 223 "trad3.y"
                                                                { sprintf (temp, "(* %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ; }
#line 1695 "trad3.tab.c"
    break;

  case 53: /* expresion: llamadaOExpresion '/' llamadaOExpresion  */
#line 225 "trad3.y"
                                                                { sprintf (temp, "(/ %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ; }
#line 1702 "trad3.tab.c"
    break;

  case 54: /* expresion: llamadaOExpresion '%' llamadaOExpresion  */
#line 227 "trad3.y"
                                                                { sprintf (temp, "(mod %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ; }
#line 1709 "trad3.tab.c"
    break;

  case 55: /* expresion: IDENTIF '[' llamadaOExpresion ']'  */
#line 229 "trad3.y"
                                                        { sprintf (temp, "(aref %s %s)", yyvsp[-3].code, yyvsp[-1].code) ;
                                                  yyval.code = gen_code (temp) ; }
#line 1716 "trad3.tab.c"
    break;

  case 56: /* expresion: llamadaOExpresion AND llamadaOExpresion  */
#line 231 "trad3.y"
                                                                { sprintf (temp, "(and %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1723 "trad3.tab.c"
    break;

  case 57: /* expresion: llamadaOExpresion OR llamadaOExpresion  */
#line 233 "trad3.y"
                                                                { sprintf (temp, "(or %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1730 "trad3.tab.c"
    break;

  case 58: /* expresion: llamadaOExpresion IGUAL llamadaOExpresion  */
#line 235 "trad3.y"
                                                                { sprintf (temp, "(= %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1737 "trad3.tab.c"
    break;

  case 59: /* expresion: llamadaOExpresion DISTINTO llamadaOExpresion  */
#line 237 "trad3.y"
                                                                { sprintf (temp, "(/= %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1744 "trad3.tab.c"
    break;

  case 60: /* expresion: llamadaOExpresion '<' llamadaOExpresion  */
#line 239 "trad3.y"
                                                                { sprintf (temp, "(< %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1751 "trad3.tab.c"
    break;

  case 61: /* expresion: llamadaOExpresion '>' llamadaOExpresion  */
#line 241 "trad3.y"
                                                                { sprintf (temp, "(> %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1758 "trad3.tab.c"
    break;

  case 62: /* expresion: llamadaOExpresion MAYOR_IGUAL llamadaOExpresion  */
#line 243 "trad3.y"
                                                                { sprintf (temp, "(>= %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1765 "trad3.tab.c"
    break;

  case 63: /* expresion: llamadaOExpresion MENOR_IGUAL llamadaOExpresion  */
#line 245 "trad3.y"
                                                                { sprintf (temp, "(<= %s %s)", yyvsp[-2].code, yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1772 "trad3.tab.c"
    break;

  case 64: /* expresion: '!' llamadaOExpresion  */
#line 247 "trad3.y"
                                                        { sprintf (temp, "(not %s)", yyvsp[0].code) ;
                                                  yyval.code = gen_code (temp) ;}
#line 1779 "trad3.tab.c"
    break;

  case 65: /* termino: operando  */
#line 252 "trad3.y"
                                                   { yyval = yyvsp[0] ; }
#line 1785 "trad3.tab.c"
    break;

  case 66: /* termino: '+' operando  */
#line 253 "trad3.y"
                                                   { yyval = yyvsp[-1] ; }
#line 1791 "trad3.tab.c"
    break;

  case 67: /* termino: '-' operando  */
#line 254 "trad3.y"
                                                   { sprintf (temp, "(- %s)", yyvsp[0].code) ;
                                                     yyval.code = gen_code (temp) ; }
#line 1798 "trad3.tab.c"
    break;

  case 68: /* operando: IDENTIF  */
#line 259 "trad3.y"
                                         { if (strlen(current_func) > 0)
                                               sprintf(temp, "%s_%s", current_func, yyvsp[0].code);
                                           else
                                               sprintf(temp, "%s", yyvsp[0].code);
                                           yyval.code = gen_code (temp) ; }
#line 1808 "trad3.tab.c"
    break;

  case 69: /* operando: NUMBER  */
#line 264 "trad3.y"
                                         { sprintf (temp, "%d", yyvsp[0].value) ;
                                           yyval.code = gen_code (temp) ; }
#line 1815 "trad3.tab.c"
    break;

  case 70: /* operando: '(' expresion ')'  */
#line 266 "trad3.y"
                                         { yyval = yyvsp[-1] ; }
#line 1821 "trad3.tab.c"
    break;


#line 1825 "trad3.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 270 "trad3.y"
                            // SECCION 4    Codigo en C

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
