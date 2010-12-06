#include "CoCoT_Token.h"

#include <tbb/tbb.h>

struct LexREDef
{
	int (*re)(const char *);
	void (*modif)(CoCoT_Token *);
};

namespace CoCoT
{
	struct LexerHelper
	{
		LexerHelper(LexREDef RE[], int size) : RE(RE), size(size) { result = new int[size]; }
		~LexerHelper() { delete [] result; }

		void ready(const char * newStr) { index = size; str = newStr; }

		void setMin(int i)
		{
			for (;;)
			{
				int snapshot = index;
				if(snapshot <= i)
					break;
				if(index.compare_and_swap(i, snapshot) == snapshot)
					break;
			}
		}

		const LexREDef * const RE;
		const int size;

		const char * str;
		tbb::atomic<int> index;

		int * result;

	};

	struct TBBTask
	{
		TBBTask(LexerHelper * helper) : helper(helper) {}
		TBBTask(const TBBTask & copy) : helper(copy.helper) {}

		void operator () (const tbb::blocked_range<int> & r) const
		{
			int begin = r.begin();
			int end = r.end();
			for (int i = begin; i < helper->index && i < end; ++i)
			{
				int ret = helper->RE[i].re(helper->str);
				if (ret)
				{
					helper->result[i] = ret;
					helper->setMin(i);
				}
			}
		}

		LexerHelper * helper;
	};
}



extern "C"
{
	void * CoCoT_Lex_Init(LexREDef lexRE[], int size)
	{
		return new CoCoT::LexerHelper(lexRE, size);
	}

	void CoCoT_Lex_End(void * vHelper)
	{
		CoCoT::LexerHelper * helper = (CoCoT::LexerHelper *)vHelper;
		delete helper;
	}

	CoCoT_Token CoCoT_Lex_Run(void * vHelper, const char * str, int * consumed)
	{
		CoCoT::LexerHelper * helper = (CoCoT::LexerHelper *)vHelper;
		helper->ready(str);

		tbb::parallel_for(tbb::blocked_range<int>(0, helper->size), CoCoT::TBBTask(helper));

		CoCoT_Token tk;

		if (consumed)
			*consumed = 0;

		int index = helper->index;
		if (index == helper->size)
			tk.lexeme = -1;
		else
		{
			tk.lexeme = index;
			tk.start = str;

			tk.len = helper->result[index];

			if (consumed)
				*consumed = tk.len;

			if (helper->RE[index].modif)
				helper->RE[index].modif(&tk);
		}

		return tk;
	}
}
