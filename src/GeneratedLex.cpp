
extern "C"
{
	struct CoCoT_Token
	{
		int lexeme;
		const char * start;
		int len;
		void * userData;
	};

	struct LexREDef
	{
		int (*re)(const char *);
		void (*modif)(CoCoT_Token *);
	};

	int _lex_RE_0(const char *); // Blanks
	int _lex_RE_1(const char *); // Identifier
	int _lex_RE_2(const char *); // Modifier
	int _lex_RE_3(const char *); // Regexp

	void CoCoT_Lexer_Ignore(CoCoT_Token * tk);
	void CoCoT_Lexer_Token_Manip(CoCoT_Token * tk);
	void CoCoT_Lexer_Token_RE(CoCoT_Token * tk);

	LexREDef _lex_regexps[] =
	{
		{_lex_RE_0, CoCoT_Lexer_Ignore},
		{_lex_RE_1, 0},
		{_lex_RE_2, CoCoT_Lexer_Token_Manip},
		{_lex_RE_3, CoCoT_Lexer_Token_RE}
	};

	const char * _lex_reNames[] =
	{
		"Blanks",
		"Identifier",
		"Manipulator",
		"Regexp"
	};

	void * CoCoT_Lex_Init(LexREDef lexRE[], int size);

	void * _lex_init()
	{
		return CoCoT_Lex_Init(_lex_regexps, 4);
	}
}
