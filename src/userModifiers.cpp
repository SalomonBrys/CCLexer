#include "CoCoT_Token.h"

extern "C"
{
	void CoCoT_Lexer_Ignore(CoCoT_Token * tk)
	{
		tk->lexeme = -1;
	}

	void CoCoT_Lexer_Token_Manip(CoCoT_Token * tk)
	{
		tk->len -= 2;
		tk->start += 1;
	}

	void CoCoT_Lexer_Token_RE(CoCoT_Token * tk)
	{
		tk->len -= 2;
		tk->start += 1;
	}
}

