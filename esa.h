# if !defined(_ESA_H)

# define _ESA_H

# if defined(_cplusplus)
extern "C"
{
	# endif
	
	typedef struct _EsaRuleList
	{
		int ref;
		int size;
		int *length;
		char **value;
	} EsaRuleList;
	
	typedef struct _Esa
	{
		unsigned char debug_mode;
		int longest_rule_size;
		EsaRuleList rules;
	} Esa;
	
	void esaInit(Esa *esa);
	
	void esaFree(Esa *esa);
	
	void esaSetDebugMode(Esa *esa, unsigned char mode);
	
	char *esaStemToken(Esa *esa, char *token, int length);
	
	# if defined(_cplusplus)
};

# endif

# endif