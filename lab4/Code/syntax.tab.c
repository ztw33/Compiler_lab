/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "./syntax.y" /* yacc.c:339  */

    #include "lex.yy.c"
    #include "SyntaxTree.h"

    void yyerror(const char* s);

    int errorNum = 0;
    int lastErrorLineno = 0;

    struct Node* syntaxTreeRootNode = NULL;

    void printError(char errorType, int lineno, char* msg);
    int isNewError(int errorLineno);

#line 81 "./syntax.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "syntax.tab.h".  */
#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ASSIGNOP = 258,
    SEMI = 259,
    COMMA = 260,
    PLUS = 261,
    MINUS = 262,
    STAR = 263,
    DIV = 264,
    AND = 265,
    OR = 266,
    NOT = 267,
    DOT = 268,
    LP = 269,
    RP = 270,
    LB = 271,
    RB = 272,
    LC = 273,
    RC = 274,
    INT = 275,
    FLOAT = 276,
    ID = 277,
    TYPE = 278,
    RELOP = 279,
    IF = 280,
    ELSE = 281,
    WHILE = 282,
    STRUCT = 283,
    RETURN = 284,
    LOWER_THAN_ELSE = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "./syntax.y" /* yacc.c:355  */

    int type_int;
    float type_float;
    char* type_string;
    struct Node* type_pnode;

#line 159 "./syntax.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 190 "./syntax.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   454

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    56,    56,    62,    75,    80,    84,    90,    96,   101,
     112,   124,   129,   138,   145,   151,   159,   165,   179,   186,
     190,   200,   207,   216,   230,   239,   248,   262,   277,   292,
     298,   304,   312,   319,   332,   337,   341,   347,   352,   359,
     367,   376,   384,   395,   407,   419,   433,   446,   461,   475,
     489,   505,   510,   514,   520,   533,   538,   545,   550,   556,
     571,   577,   583,   589,   596,   602,   608,   614,   620,   627,
     633,   639,   648,   657,   664,   672,   679,   686,   693,   706,
     718,   732,   746,   758,   770,   782,   795,   807,   819,   831,
     843,   855,   867,   880,   886
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ASSIGNOP", "SEMI", "COMMA", "PLUS",
  "MINUS", "STAR", "DIV", "AND", "OR", "NOT", "DOT", "LP", "RP", "LB",
  "RB", "LC", "RC", "INT", "FLOAT", "ID", "TYPE", "RELOP", "IF", "ELSE",
  "WHILE", "STRUCT", "RETURN", "LOWER_THAN_ELSE", "$accept", "Program",
  "ExtDefList", "ExtDef", "ExtDecList", "Specifier", "StructSpecifier",
  "OptTag", "Tag", "VarDec", "FunDec", "VarList", "ParamDec", "CompSt",
  "StmtList", "Stmt", "DefList", "Def", "DecList", "Dec", "Exp", "Args", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285
};
# endif

#define YYPACT_NINF -60

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-60)))

#define YYTABLE_NINF -36

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      56,   -60,    -3,    22,    85,    56,    73,   -60,    28,    54,
     -60,   -60,   -60,   -60,   -60,   -60,   415,    74,    77,     6,
      56,    86,     2,   -60,    66,    15,    56,   -60,    56,   -60,
      11,    44,    56,   -60,    93,   -60,    66,    95,    87,   -60,
     -60,    94,    97,    46,    46,   109,    53,   111,   112,   -60,
     -60,   -60,   -60,   103,   -60,    56,   -60,   -60,    27,   241,
     245,   257,   -60,   -60,   113,   118,   119,   257,   -60,   110,
      46,    96,   117,   -60,   269,   -60,    66,   -60,   -60,   -60,
     131,     5,   131,     7,   131,   115,   229,   273,   285,   134,
     -60,   -60,   -60,   297,   -60,   301,   313,   325,   329,   341,
     353,   126,   357,   369,   -60,   131,   216,   -60,   -60,   -60,
     131,   -60,   192,    47,   136,   153,   137,   172,   -60,   -60,
     131,   216,   131,     5,   131,     5,   131,     7,   131,     7,
     131,   398,   131,   386,   -60,    37,   204,   131,   438,   257,
     -60,   -60,   403,    69,    69,   403,    69,    69,   -60,   -60,
     -60,   108,   123,   127,   -60,   -60,   -60,    69,    69,    69,
     -60,   -60,   -60
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,    13,    19,     0,     0,     5,     0,    14,    20,     0,
      16,     1,     3,     4,     9,     7,     0,     0,    11,     0,
      52,     0,     0,     6,     0,     0,    52,    10,    52,     8,
       0,     0,    52,    28,    26,    25,     0,     0,    30,    21,
      12,     0,     0,     0,     0,     0,    57,     0,    55,    17,
      15,    51,    27,    31,    24,     0,    23,    22,    52,     0,
       0,     0,    76,    77,    75,     0,     0,     0,    37,     0,
       0,     0,     0,    54,     0,    53,     0,    29,    44,    79,
      90,    69,    91,    70,     0,     0,     0,     0,     0,     0,
      33,    34,    42,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    59,    58,    56,    92,    68,
      81,    72,    94,     0,     0,     0,     0,     0,    43,    38,
      82,    60,    86,    64,    87,    65,    88,    66,    89,    67,
      83,    61,    84,    62,    74,     0,     0,    85,    63,     0,
      80,    71,    79,     0,     0,    79,     0,     0,    78,    73,
      93,    45,    46,    39,    49,    50,    41,     0,     0,     0,
      47,    48,    40
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -60,   -60,   150,   -60,   133,     4,   -60,   -60,   -60,   -25,
     -60,   116,   -60,   146,   -38,   291,    29,   -60,    91,   -60,
     -59,    31
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,    17,    30,     7,     9,    10,    18,
      19,    37,    38,    68,    69,    70,    43,    32,    47,    48,
      71,   113
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      81,    83,    85,    34,     6,    46,    72,    26,    89,     6,
      27,    53,    45,    97,    98,   106,    41,    35,   101,     8,
     101,   102,    11,   102,    28,     1,    36,   112,   115,   117,
       2,    78,    91,    39,   121,    42,   123,   125,   127,   129,
     131,   133,    79,   136,   138,    49,   -18,    58,   140,    31,
       1,    46,    79,    59,   148,     2,    74,    44,    60,    36,
      61,    51,   141,    50,    28,   -35,    62,    63,    64,    25,
      58,    65,    20,    66,    14,    67,    59,    15,    23,     1,
     112,    60,    24,    61,     2,    -2,    12,    28,    39,    62,
      63,    64,    55,    25,    65,    16,    66,    92,    67,    93,
      94,    33,    95,    96,    97,    98,    99,   100,    52,   101,
      54,    56,   102,    73,    57,    75,   108,    76,    93,    25,
     103,    95,    96,    97,    98,    99,   100,    86,   101,    90,
     109,   102,    87,    88,   157,   118,   104,    93,   119,   103,
      95,    96,    97,    98,    99,   100,    79,   101,   134,   158,
     102,   142,   145,   159,   143,    13,    93,    40,   103,    95,
      96,    97,    98,    99,   100,    29,   101,   107,   144,   102,
     150,    77,     0,   146,     0,    93,     0,   103,    95,    96,
      97,    98,    99,   100,     0,   101,     0,   147,   102,     0,
       0,     0,     0,     0,     0,    93,   103,   139,    95,    96,
      97,    98,    99,   100,     0,   101,     0,    93,   102,     0,
      95,    96,    97,    98,    99,   100,   103,   101,     0,    93,
     102,   149,    95,    96,    97,    98,    99,   100,   103,   101,
     110,     0,   102,     0,     0,     0,    59,     0,     0,     0,
     103,    60,    80,    61,   111,     0,    82,     0,    59,    62,
      63,    64,    59,    60,     0,    61,     0,    60,    84,    61,
       0,    62,    63,    64,    59,    62,    63,    64,     0,    60,
     105,    61,     0,     0,   114,     0,    59,    62,    63,    64,
      59,    60,     0,    61,     0,    60,   116,    61,     0,    62,
      63,    64,    59,    62,    63,    64,     0,    60,   120,    61,
       0,     0,   122,     0,    59,    62,    63,    64,    59,    60,
       0,    61,     0,    60,   124,    61,     0,    62,    63,    64,
      59,    62,    63,    64,     0,    60,   126,    61,     0,     0,
     128,     0,    59,    62,    63,    64,    59,    60,     0,    61,
       0,    60,   130,    61,     0,    62,    63,    64,    59,    62,
      63,    64,     0,    60,   132,    61,     0,     0,   135,     0,
      59,    62,    63,    64,    59,    60,     0,    61,     0,    60,
     137,    61,     0,    62,    63,    64,    59,    62,    63,    64,
       0,    60,     0,    61,     0,     0,     0,     0,     0,    62,
      63,    64,    95,    96,    97,    98,    99,     0,     0,   101,
       0,     0,   102,     0,    95,    96,    97,    98,     0,     0,
     103,   101,     0,     0,   102,    60,    21,    61,     0,   -21,
     -21,    28,   103,    62,    63,    64,     0,     0,    65,    22,
      66,   -21,    67,   151,   152,   153,   154,   155,   156,     0,
       0,     0,     0,     0,    95,    96,    97,    98,   160,   161,
     162,   101,     0,     0,   102
};

static const yytype_int16 yycheck[] =
{
      59,    60,    61,     1,     0,    30,    44,     1,    67,     5,
       4,    36,     1,     8,     9,    74,     1,    15,    13,    22,
      13,    16,     0,    16,    18,    23,    22,    86,    87,    88,
      28,     4,    70,    22,    93,    20,    95,    96,    97,    98,
      99,   100,    15,   102,   103,     1,    18,     1,     1,    20,
      23,    76,    15,     7,    17,    28,     3,    28,    12,    55,
      14,    32,    15,    19,    18,    19,    20,    21,    22,    16,
       1,    25,    18,    27,     1,    29,     7,     4,     4,    23,
     139,    12,     5,    14,    28,     0,     1,    18,    22,    20,
      21,    22,     5,    16,    25,    22,    27,     1,    29,     3,
       4,    15,     6,     7,     8,     9,    10,    11,    15,    13,
      15,    17,    16,     4,    17,     4,     1,     5,     3,    16,
      24,     6,     7,     8,     9,    10,    11,    14,    13,    19,
      15,    16,    14,    14,    26,     1,    19,     3,     4,    24,
       6,     7,     8,     9,    10,    11,    15,    13,    22,    26,
      16,    15,    15,    26,     1,     5,     3,    24,    24,     6,
       7,     8,     9,    10,    11,    19,    13,    76,    15,    16,
     139,    55,    -1,     1,    -1,     3,    -1,    24,     6,     7,
       8,     9,    10,    11,    -1,    13,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    24,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    -1,     3,    16,    -1,
       6,     7,     8,     9,    10,    11,    24,    13,    -1,     3,
      16,    17,     6,     7,     8,     9,    10,    11,    24,    13,
       1,    -1,    16,    -1,    -1,    -1,     7,    -1,    -1,    -1,
      24,    12,     1,    14,    15,    -1,     1,    -1,     7,    20,
      21,    22,     7,    12,    -1,    14,    -1,    12,     1,    14,
      -1,    20,    21,    22,     7,    20,    21,    22,    -1,    12,
       1,    14,    -1,    -1,     1,    -1,     7,    20,    21,    22,
       7,    12,    -1,    14,    -1,    12,     1,    14,    -1,    20,
      21,    22,     7,    20,    21,    22,    -1,    12,     1,    14,
      -1,    -1,     1,    -1,     7,    20,    21,    22,     7,    12,
      -1,    14,    -1,    12,     1,    14,    -1,    20,    21,    22,
       7,    20,    21,    22,    -1,    12,     1,    14,    -1,    -1,
       1,    -1,     7,    20,    21,    22,     7,    12,    -1,    14,
      -1,    12,     1,    14,    -1,    20,    21,    22,     7,    20,
      21,    22,    -1,    12,     1,    14,    -1,    -1,     1,    -1,
       7,    20,    21,    22,     7,    12,    -1,    14,    -1,    12,
       1,    14,    -1,    20,    21,    22,     7,    20,    21,    22,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    20,
      21,    22,     6,     7,     8,     9,    10,    -1,    -1,    13,
      -1,    -1,    16,    -1,     6,     7,     8,     9,    -1,    -1,
      24,    13,    -1,    -1,    16,    12,     1,    14,    -1,     4,
       5,    18,    24,    20,    21,    22,    -1,    -1,    25,    14,
      27,    16,    29,   142,   143,   144,   145,   146,   147,    -1,
      -1,    -1,    -1,    -1,     6,     7,     8,     9,   157,   158,
     159,    13,    -1,    -1,    16
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    23,    28,    32,    33,    34,    36,    37,    22,    38,
      39,     0,     1,    33,     1,     4,    22,    35,    40,    41,
      18,     1,    14,     4,     5,    16,     1,     4,    18,    44,
      36,    47,    48,    15,     1,    15,    36,    42,    43,    22,
      35,     1,    20,    47,    47,     1,    40,    49,    50,     1,
      19,    47,    15,    40,    15,     5,    17,    17,     1,     7,
      12,    14,    20,    21,    22,    25,    27,    29,    44,    45,
      46,    51,    45,     4,     3,     4,     5,    42,     4,    15,
       1,    51,     1,    51,     1,    51,    14,    14,    14,    51,
      19,    45,     1,     3,     4,     6,     7,     8,     9,    10,
      11,    13,    16,    24,    19,     1,    51,    49,     1,    15,
       1,    15,    51,    52,     1,    51,     1,    51,     1,     4,
       1,    51,     1,    51,     1,    51,     1,    51,     1,    51,
       1,    51,     1,    51,    22,     1,    51,     1,    51,     5,
       1,    15,    15,     1,    15,    15,     1,    15,    17,    17,
      52,    46,    46,    46,    46,    46,    46,    26,    26,    26,
      46,    46,    46
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    31,    32,    32,    33,    33,    34,    34,    34,    34,
      34,    35,    35,    36,    36,    37,    37,    37,    38,    38,
      39,    40,    40,    40,    41,    41,    41,    41,    41,    42,
      42,    43,    44,    44,    45,    45,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    47,    47,    48,    48,    49,    49,    50,    50,    50,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    52,    52
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     3,     2,     3,     2,
       3,     1,     3,     1,     1,     5,     2,     5,     1,     0,
       1,     1,     4,     4,     4,     3,     3,     4,     3,     3,
       1,     2,     4,     4,     2,     0,     2,     1,     3,     5,
       7,     5,     2,     3,     2,     5,     5,     7,     7,     5,
       5,     2,     0,     3,     3,     1,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     4,     3,     4,     3,     1,     1,     1,     4,     2,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     3,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 56 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeProgram = createNewNode("Program", NonTerm, (yyloc).first_line);
            buildRel(nodeProgram, 1, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeProgram;
            syntaxTreeRootNode = nodeProgram;
        }
#line 1535 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 62 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Unexpected character");
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeProgram = createNewNode("Program", NonTerm, (yyloc).first_line);
                buildRel(nodeProgram, 2, (yyvsp[-1].type_pnode), nodeError);
                (yyval.type_pnode) = nodeProgram;
                syntaxTreeRootNode = nodeProgram;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1552 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 75 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeExtDefList = createNewNode("ExtDefList", NonTerm, (yyloc).first_line);
            buildRel(nodeExtDefList, 2, (yyvsp[-1].type_pnode), (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExtDefList;
        }
#line 1562 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 80 "./syntax.y" /* yacc.c:1646  */
    {
            (yyval.type_pnode) = NULL;
        }
#line 1570 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 84 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, (yyloc).first_line);
            buildRel(nodeExtDef, 3, (yyvsp[-2].type_pnode), (yyvsp[-1].type_pnode), nodeSEMI);
            (yyval.type_pnode) = nodeExtDef;
        }
#line 1581 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 90 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, (yyloc).first_line);
            buildRel(nodeExtDef, 2, (yyvsp[-1].type_pnode), nodeSEMI);
            (yyval.type_pnode) = nodeExtDef;
        }
#line 1592 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 96 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, (yyloc).first_line);
            buildRel(nodeExtDef, 3, (yyvsp[-2].type_pnode), (yyvsp[-1].type_pnode), (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExtDef;
        }
#line 1602 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 101 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \";\"");
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, (yyloc).first_line);
                buildRel(nodeExtDef, 2, (yyvsp[-1].type_pnode), nodeError);
                (yyval.type_pnode) = nodeExtDef;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1618 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 112 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Incomplete definition of function");
                struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, (yyloc).first_line);
                buildRel(nodeExtDef, 3, (yyvsp[-2].type_pnode), (yyvsp[-1].type_pnode), nodeSEMI);
                (yyval.type_pnode) = nodeExtDef;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1634 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 124 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeExtDecList = createNewNode("ExtDecList", NonTerm, (yyloc).first_line);
            buildRel(nodeExtDecList, 1, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExtDecList;
        }
#line 1644 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 129 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExtDecList = createNewNode("ExtDecList", NonTerm, (yyloc).first_line);
            buildRel(nodeExtDecList, 3, (yyvsp[-2].type_pnode), nodeCOMMA, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExtDecList;
        }
#line 1655 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 138 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeTYPE = createNewNode("TYPE", ValToken, (yylsp[0]).first_line);
            nodeTYPE->stringVal = (yyvsp[0].type_string);
            struct Node* nodeSpecifier = createNewNode("Specifier", NonTerm, (yyloc).first_line);
            buildRel(nodeSpecifier, 1, nodeTYPE);
            (yyval.type_pnode) = nodeSpecifier;
        }
#line 1667 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 145 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSpecifier = createNewNode("Specifier", NonTerm, (yyloc).first_line);
            buildRel(nodeSpecifier, 1, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeSpecifier;
        }
#line 1677 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 151 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, (yylsp[-4]).first_line);
            struct Node* nodeLC = createNewNode("LC", NonValToken, (yylsp[-2]).first_line);
            struct Node* nodeRC = createNewNode("RC", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, (yyloc).first_line);           
            buildRel(nodeStructSpecifier, 5, nodeSTRUCT, (yyvsp[-3].type_pnode), nodeLC, (yyvsp[-1].type_pnode), nodeRC);          
            (yyval.type_pnode) = nodeStructSpecifier;
        }
#line 1690 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 159 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, (yylsp[-1]).first_line);           
            struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, (yyloc).first_line);
            buildRel(nodeStructSpecifier, 2, nodeSTRUCT, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeStructSpecifier;
        }
#line 1701 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 165 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \"}\"");
                struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, (yylsp[-4]).first_line);
                struct Node* nodeLC = createNewNode("LC", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, (yyloc).first_line);           
                buildRel(nodeStructSpecifier, 5, nodeSTRUCT, (yyvsp[-3].type_pnode), nodeLC, (yyvsp[-1].type_pnode), nodeError);          
                (yyval.type_pnode) = nodeStructSpecifier;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1719 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 179 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[0]).first_line);
            nodeID->stringVal = (yyvsp[0].type_string);
            struct Node* nodeOptTag = createNewNode("OptTag", NonTerm, (yyloc).first_line);
            buildRel(nodeOptTag, 1, nodeID);
            (yyval.type_pnode) = nodeOptTag;
        }
#line 1731 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 186 "./syntax.y" /* yacc.c:1646  */
    {
            (yyval.type_pnode) = NULL;
        }
#line 1739 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 190 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[0]).first_line);
            nodeID->stringVal = (yyvsp[0].type_string);
            struct Node* nodeTag = createNewNode("Tag", NonTerm, (yyloc).first_line);
            buildRel(nodeTag, 1, nodeID);
            (yyval.type_pnode) = nodeTag;
        }
#line 1751 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 200 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[0]).first_line);
            nodeID->stringVal = (yyvsp[0].type_string);
            struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, (yyloc).first_line);
            nodeVarDec->firstChild = nodeID;
            (yyval.type_pnode) = nodeVarDec;
        }
#line 1763 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 207 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeLB = createNewNode("LB", NonValToken, (yylsp[-2]).first_line);
            struct Node* nodeINT = createNewNode("INT", ValToken, (yylsp[-1]).first_line);
            nodeINT->intVal = (yyvsp[-1].type_int);
            struct Node* nodeRB = createNewNode("RB", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, (yyloc).first_line);
            buildRel(nodeVarDec, 4, (yyvsp[-3].type_pnode), nodeLB, nodeINT, nodeRB);
            (yyval.type_pnode) = nodeVarDec;
        }
#line 1777 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 216 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Syntax error between \"[]\"");
                struct Node* nodeLB = createNewNode("LB", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeRB = createNewNode("RB", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, (yyloc).first_line);
                buildRel(nodeVarDec, 4, (yyvsp[-3].type_pnode), nodeLB, nodeError, nodeRB);
                (yyval.type_pnode) = nodeVarDec;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1795 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 230 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-3]).first_line);
            nodeID->stringVal = (yyvsp[-3].type_string);
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-2]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, (yyloc).first_line);
            buildRel(nodeFunDec, 4, nodeID, nodeLP, (yyvsp[-1].type_pnode), nodeRP);
            (yyval.type_pnode) = nodeFunDec;
        }
#line 1809 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 239 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-2]).first_line);
            nodeID->stringVal = (yyvsp[-2].type_string);
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, (yyloc).first_line);
            buildRel(nodeFunDec, 3, nodeID, nodeLP, nodeRP);
            (yyval.type_pnode) = nodeFunDec;
        }
#line 1823 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 248 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \")\"");
                struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-2]).first_line);
                nodeID->stringVal = (yyvsp[-2].type_string);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, (yyloc).first_line);
                buildRel(nodeFunDec, 3, nodeID, nodeLP, nodeError);
                (yyval.type_pnode) = nodeFunDec;
            } else {
            	(yyval.type_pnode) = NULL;
            } 
        }
#line 1842 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 262 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Syntax error between ()");
                struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-3]).first_line);
                nodeID->stringVal = (yyvsp[-3].type_string);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, (yyloc).first_line);
                buildRel(nodeFunDec, 4, nodeID, nodeLP, nodeError, nodeRP);
                (yyval.type_pnode) = nodeFunDec;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1862 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 277 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Missing \"(\"");
                struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-2]).first_line);
                nodeID->stringVal = (yyvsp[-2].type_string);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, (yyloc).first_line);
                buildRel(nodeFunDec, 3, nodeID, nodeError, nodeRP);
                (yyval.type_pnode) = nodeFunDec;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1881 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 292 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeVarList = createNewNode("VarList", NonTerm, (yyloc).first_line);
            buildRel(nodeVarList, 3, (yyvsp[-2].type_pnode), nodeCOMMA, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeVarList;
        }
#line 1892 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 298 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeVarList = createNewNode("VarList", NonTerm, (yyloc).first_line);
            nodeVarList->firstChild = (yyvsp[0].type_pnode);
            (yyval.type_pnode) = nodeVarList;
        }
#line 1902 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 304 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeParamDec = createNewNode("ParamDec", NonTerm, (yyloc).first_line);
            buildRel(nodeParamDec, 2, (yyvsp[-1].type_pnode), (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeParamDec;
        }
#line 1912 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 312 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeLC = createNewNode("LC", NonValToken, (yylsp[-3]).first_line);
            struct Node* nodeRC = createNewNode("RC", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeCompSt = createNewNode("CompSt", NonTerm, (yyloc).first_line);
            buildRel(nodeCompSt, 4, nodeLC, (yyvsp[-2].type_pnode), (yyvsp[-1].type_pnode), nodeRC);
            (yyval.type_pnode) = nodeCompSt;
        }
#line 1924 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 319 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-3]).first_line)) {
                printError('B', (yylsp[-3]).first_line, "Missing \"{\"");
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-3]).first_line);
                struct Node* nodeRC = createNewNode("LC", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeCompSt = createNewNode("CompSt", NonTerm, (yyloc).first_line);
                buildRel(nodeCompSt, 4, nodeError, (yyvsp[-2].type_pnode), (yyvsp[-1].type_pnode), nodeRC);
                (yyval.type_pnode) = nodeCompSt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 1941 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 332 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeStmtList = createNewNode("StmtList", NonTerm, (yyloc).first_line);
            buildRel(nodeStmtList, 2, (yyvsp[-1].type_pnode), (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeStmtList;
        }
#line 1951 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 337 "./syntax.y" /* yacc.c:1646  */
    {
            (yyval.type_pnode) = NULL;
        }
#line 1959 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 341 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
            buildRel(nodeStmt, 2, (yyvsp[-1].type_pnode), nodeSEMI);
            (yyval.type_pnode) = nodeStmt;
        }
#line 1970 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 347 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
            nodeStmt->firstChild = (yyvsp[0].type_pnode);
            (yyval.type_pnode) = nodeStmt;
        }
#line 1980 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 352 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeRETURN = createNewNode("RETURN", NonValToken, (yylsp[-2]).first_line);
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
            buildRel(nodeStmt, 3, nodeRETURN, (yyvsp[-1].type_pnode), nodeSEMI);
            (yyval.type_pnode) = nodeStmt;
        }
#line 1992 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 359 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeIF = createNewNode("IF", NonValToken, (yylsp[-4]).first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-3]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
            buildRel(nodeStmt, 5, nodeIF, nodeLP, (yyvsp[-2].type_pnode), nodeRP, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeStmt;
        }
#line 2005 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 367 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeIF = createNewNode("IF", NonValToken, (yylsp[-6]).first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-5]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[-3]).first_line);
            struct Node* nodeELSE = createNewNode("ELSE", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
            buildRel(nodeStmt, 7, nodeIF, nodeLP, (yyvsp[-4].type_pnode), nodeRP, (yyvsp[-2].type_pnode), nodeELSE, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeStmt;
        }
#line 2019 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 376 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeWHILE = createNewNode("WHILE", NonValToken, (yylsp[-4]).first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-3]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
            buildRel(nodeStmt, 5, nodeWHILE, nodeLP, (yyvsp[-2].type_pnode), nodeRP, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeStmt;
        }
#line 2032 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 384 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \";\"");
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);                
                buildRel(nodeStmt, 2, (yyvsp[-1].type_pnode), nodeError);
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2048 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 395 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \";\"");
                struct Node* nodeRETURN = createNewNode("RETURN", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);                
                buildRel(nodeStmt, 3, nodeRETURN, (yyvsp[-1].type_pnode), nodeError);
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2065 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 407 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Syntax error in Exp");
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);                
                struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
                buildRel(nodeStmt, 2, nodeError, nodeSEMI);
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2082 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 419 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-2]).first_line)) {
                printError('B', (yylsp[-2]).first_line, "Syntax error in Exp");
                struct Node* nodeIF = createNewNode("IF", NonValToken, (yylsp[-4]).first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-3]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-2]).first_line);                
                struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
                buildRel(nodeStmt, 5, nodeIF, nodeLP, nodeError, nodeRP, (yyvsp[0].type_pnode));
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2101 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 433 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Missing \")\"");
                struct Node* nodeIF = createNewNode("IF", NonValToken, (yylsp[-4]).first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-3]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);                
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
                buildRel(nodeStmt, 5, nodeIF, nodeLP, (yyvsp[-2].type_pnode), nodeError, (yyvsp[0].type_pnode));
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2119 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 446 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-4]).first_line)) {
                printError('B', (yylsp[-4]).first_line, "Syntax error in Exp");
                struct Node* nodeIF = createNewNode("IF", NonValToken, (yylsp[-6]).first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-5]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-4]).first_line);                
                struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[-3]).first_line);
                struct Node* nodeELSE = createNewNode("ELSE", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
                buildRel(nodeStmt, 7, nodeIF, nodeLP, nodeError, nodeRP, (yyvsp[-2].type_pnode), nodeELSE, (yyvsp[0].type_pnode));
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2139 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 461 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-3]).first_line)) {
                printError('B', (yylsp[-3]).first_line, "Missing \")\"");
                struct Node* nodeIF = createNewNode("IF", NonValToken, (yylsp[-6]).first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-5]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-3]).first_line);                
                struct Node* nodeELSE = createNewNode("ELSE", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
                buildRel(nodeStmt, 7, nodeIF, nodeLP, (yyvsp[-4].type_pnode), nodeError, (yyvsp[-2].type_pnode), nodeELSE, (yyvsp[0].type_pnode));
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2158 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 475 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-2]).first_line)) {
                printError('B', (yylsp[-2]).first_line, "Syntax error in Exp");
                struct Node* nodeWHILE = createNewNode("WHILE", NonValToken, (yylsp[-4]).first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-3]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
                buildRel(nodeStmt, 5, nodeWHILE, nodeLP, nodeError, nodeRP, (yyvsp[0].type_pnode));
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2177 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 489 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Missing \")\"");
                struct Node* nodeWHILE = createNewNode("WHILE", NonValToken, (yylsp[-4]).first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-3]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, (yyloc).first_line);
                buildRel(nodeStmt, 5, nodeWHILE, nodeLP, (yyvsp[-2].type_pnode), nodeError, (yyvsp[0].type_pnode));
                (yyval.type_pnode) = nodeStmt;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2195 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 505 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeDefList = createNewNode("DefList", NonTerm, (yyloc).first_line);
            buildRel(nodeDefList, 2, (yyvsp[-1].type_pnode), (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeDefList;
        }
#line 2205 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 510 "./syntax.y" /* yacc.c:1646  */
    {
            (yyval.type_pnode) = NULL;
        }
#line 2213 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 514 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeDef = createNewNode("Def", NonTerm, (yyloc).first_line);
            buildRel(nodeDef, 3, (yyvsp[-2].type_pnode), (yyvsp[-1].type_pnode), nodeSEMI);
            (yyval.type_pnode) = nodeDef;
        }
#line 2224 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 520 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Syntax error in DecList");
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeDef = createNewNode("Def", NonTerm, (yyloc).first_line);
                buildRel(nodeDef, 3, (yyvsp[-2].type_pnode), nodeError, nodeSEMI);
                (yyval.type_pnode) = nodeDef;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2241 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 533 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeDecList = createNewNode("DecList", NonTerm, (yyloc).first_line);
            nodeDecList->firstChild = (yyvsp[0].type_pnode);
            (yyval.type_pnode) = nodeDecList;
        }
#line 2251 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 538 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeDecList = createNewNode("DecList", NonTerm, (yyloc).first_line);
            buildRel(nodeDecList, 3, (yyvsp[-2].type_pnode), nodeCOMMA, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeDecList;
        }
#line 2262 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 545 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeDec = createNewNode("Dec", NonTerm, (yyloc).first_line);
            nodeDec->firstChild = (yyvsp[0].type_pnode);
            (yyval.type_pnode) = nodeDec;
        }
#line 2272 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 550 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeDec = createNewNode("Dec", NonTerm, (yyloc).first_line);
            buildRel(nodeDec, 3, (yyvsp[-2].type_pnode), nodeASSIGNOP, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeDec;
        }
#line 2283 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 556 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeDec = createNewNode("Dec", NonTerm, (yyloc).first_line);
                buildRel(nodeDec, 3, (yyvsp[-2].type_pnode), nodeASSIGNOP, nodeError);
                (yyval.type_pnode) = nodeDec;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2300 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 571 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeASSIGNOP, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2311 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 577 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeAND = createNewNode("AND", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeAND, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2322 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 583 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeOR = createNewNode("OR", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeOR, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2333 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 589 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeRELOP = createNewNode("RELOP", NonValToken, (yylsp[-1]).first_line);
            nodeRELOP->stringVal = (yyvsp[-1].type_string);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeRELOP, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2345 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 596 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodePLUS = createNewNode("PLUS", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodePLUS, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2356 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 602 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeMINUS, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2367 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 608 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeSTAR = createNewNode("STAR", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeSTAR, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2378 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 614 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeDIV = createNewNode("DIV", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeDIV, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2389 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 620 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-2]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, nodeLP, (yyvsp[-1].type_pnode), nodeRP);
            (yyval.type_pnode) = nodeExp;
        }
#line 2401 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 627 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 2, nodeMINUS, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2412 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 633 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeNOT = createNewNode("NOT", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 2, nodeNOT, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeExp;
        }
#line 2423 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 639 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-3]).first_line);
            nodeID->stringVal = (yyvsp[-3].type_string);
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-2]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 4, nodeID, nodeLP, (yyvsp[-1].type_pnode), nodeRP);
            (yyval.type_pnode) = nodeExp;
        }
#line 2437 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 648 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-2]).first_line);
            nodeID->stringVal = (yyvsp[-2].type_string);
            struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, nodeID, nodeLP, nodeRP);
            (yyval.type_pnode) = nodeExp;
        }
#line 2451 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 657 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeLB = createNewNode("LB", NonValToken, (yylsp[-2]).first_line);
            struct Node* nodeRB = createNewNode("RB", NonValToken, (yylsp[0]).first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 4, (yyvsp[-3].type_pnode), nodeLB, (yyvsp[-1].type_pnode), nodeRB);
            (yyval.type_pnode) = nodeExp;
        }
#line 2463 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 664 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeDOT = createNewNode("DOT", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[0]).first_line);
            nodeID->stringVal = (yyvsp[0].type_string);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeDOT, nodeID);
            (yyval.type_pnode) = nodeExp;
        }
#line 2476 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 672 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[0]).first_line);
            nodeID->stringVal = (yyvsp[0].type_string);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            nodeExp->firstChild = nodeID;
            (yyval.type_pnode) = nodeExp;
        }
#line 2488 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 679 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeINT = createNewNode("INT", ValToken, (yylsp[0]).first_line);
            nodeINT->intVal = (yyvsp[0].type_int);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            nodeExp->firstChild = nodeINT;
            (yyval.type_pnode) = nodeExp;
        }
#line 2500 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 686 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeFLOAT = createNewNode("FLOAT", ValToken, (yylsp[0]).first_line);
            nodeFLOAT->floatVal = (yyvsp[0].type_float);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
            nodeExp->firstChild = nodeFLOAT;
            (yyval.type_pnode) = nodeExp;
        }
#line 2512 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 693 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Syntax error between \"[]\"");
                struct Node* nodeLB = createNewNode("LB", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeRB = createNewNode("RB", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 4, (yyvsp[-3].type_pnode), nodeLB, nodeError, nodeRB);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2530 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 706 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[-1]).first_line)) {
                printError('B', (yylsp[-1]).first_line, "Missing \"(\"");
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, (yylsp[0]).first_line);
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 2, nodeError, nodeRP);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2547 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 718 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \")\"");
                struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-3]).first_line);
                nodeID->stringVal = (yyvsp[-3].type_string);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 4, nodeID, nodeLP, (yyvsp[-1].type_pnode), nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2566 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 732 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \")\"");
                struct Node* nodeID = createNewNode("ID", ValToken, (yylsp[-2]).first_line);
                nodeID->stringVal = (yyvsp[-2].type_string);
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, nodeID, nodeLP, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2585 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 746 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeASSIGNOP, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2602 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 758 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeAND = createNewNode("AND", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeAND, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2619 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 770 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeOR = createNewNode("OR", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeOR, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2636 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 782 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeRELOP = createNewNode("RELOP", NonValToken, (yylsp[-1]).first_line);
                nodeRELOP->stringVal = (yyvsp[-1].type_string);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeRELOP, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2654 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 795 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodePLUS = createNewNode("PLUS", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodePLUS, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2671 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 807 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeMINUS, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2688 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 819 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeSTAR = createNewNode("STAR", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeSTAR, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2705 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 831 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeDIV = createNewNode("DIV", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, (yyvsp[-2].type_pnode), nodeDIV, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2722 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 843 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 2, nodeMINUS, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2739 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 855 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Syntax error in Exp");
                struct Node* nodeNOT = createNewNode("NOT", NonValToken, (yylsp[-1]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 2, nodeNOT, nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2756 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 867 "./syntax.y" /* yacc.c:1646  */
    {
            if (isNewError((yylsp[0]).first_line)) {
                printError('B', (yylsp[0]).first_line, "Missing \")\"");
                struct Node* nodeLP = createNewNode("LP", NonValToken, (yylsp[-2]).first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, (yylsp[0]).first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, (yyloc).first_line);
                buildRel(nodeExp, 3, nodeLP, (yyvsp[-1].type_pnode), nodeError);
                (yyval.type_pnode) = nodeExp;
            } else {
            	(yyval.type_pnode) = NULL;
            }
        }
#line 2773 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 880 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, (yylsp[-1]).first_line);
            struct Node* nodeArgs = createNewNode("Args", NonTerm, (yyloc).first_line);
            buildRel(nodeArgs, 3, (yyvsp[-2].type_pnode), nodeCOMMA, (yyvsp[0].type_pnode));
            (yyval.type_pnode) = nodeArgs;
        }
#line 2784 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 886 "./syntax.y" /* yacc.c:1646  */
    {
            struct Node* nodeArgs = createNewNode("Args", NonTerm, (yyloc).first_line);
            nodeArgs->firstChild = (yyvsp[0].type_pnode);
            (yyval.type_pnode) = nodeArgs;
        }
#line 2794 "./syntax.tab.c" /* yacc.c:1646  */
    break;


#line 2798 "./syntax.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 893 "./syntax.y" /* yacc.c:1906  */

void yyerror(const char* s) { }

void printError(char errorType, int lineno, char* msg) {
    fprintf(stderr, "Error type %c at Line %d: %s.\n", errorType, lineno, msg);
}

int isNewError(int errorLineno) {
    if (lastErrorLineno != errorLineno) {
        errorNum++;
        lastErrorLineno = errorLineno;
        return 1;
    } else {
        return 0;
    }
}
