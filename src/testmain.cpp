#include <iostream>
#include <fstream>

#include <assert.h>

#include "CoCoT_Token.h"

extern "C"
{
	void * _lex_init();
	void CoCoT_Lex_End(void *);
	CoCoT_Token CoCoT_Lex_Run(void *, const char *, int *);

	extern const char * _lex_reNames[];
}

int main()
{
	void * lex = _lex_init();

	std::ifstream ifs("/home/salomon/Code/CoCoT/CoCoT-Lexer.cclex.txt");
	assert(ifs.is_open() && "Could not open file");

	ifs.seekg(0, std::ifstream::end);
	int fileSize = ifs.tellg();
	ifs.seekg(0);

	char * str = new char[fileSize];
	ifs.read(str, fileSize);

	ifs.close();

	std::cout << "=== START ===" << std::endl;

	int consumed;
	for (;;)
	{
		CoCoT_Token tk = CoCoT_Lex_Run(lex, str, &consumed);

		if (tk.lexeme == -1 && consumed == 0)
			break ;

		if (tk.lexeme != -1)
			std::cout << "<(" << _lex_reNames[tk.lexeme] << "):\"" << std::string(tk.start).substr(0, tk.len) << "\">" << std::endl;

		str += consumed;
	}

	std::cout << "==== END ====" << std::endl;

	CoCoT_Lex_End(lex);

	return 0;
}

