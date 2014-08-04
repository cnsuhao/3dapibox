#include "util.h"
#include <stdio.h>

bool Util_ReadFile(const char *filename, std::string &context)
{
	FILE *fp = fopen(filename, "rt");
	if (fp == NULL)
		return false;

	long len;
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	context.resize(len+1, 0);
	fread((void*)context.data(), len, 1, fp);
	fclose(fp);

	return true;
}

