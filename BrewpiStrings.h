#pragma once



extern const char STR_FMT_S[];
extern const char STR_FMT_U[];
extern const char STR_FMT_D[];
extern const char STR_SPACES[];

#define indexOf_inline 0

#if indexOf_inline
inline int8_t indexOf(const char* s, char c)
{
	char c2;
	int8_t idx = -1;
	while ((c2=s[++idx]))
	{
		if (c==c2)
			return idx;
	}
	return -1;
}
#else
int8_t indexOf(const char* s, char c);
#endif
