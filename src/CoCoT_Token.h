
#ifndef COCOT_TOKEN_H_
#define COCOT_TOKEN_H_

extern "C"
{
	struct CoCoT_Token
	{
		int lexeme;
		const char * start;
		int len;
		void * userData;
	};
}

#endif /* COCOT_TOKEN_H_ */
