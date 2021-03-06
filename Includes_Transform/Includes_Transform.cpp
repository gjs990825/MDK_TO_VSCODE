// Includes_Transform.cpp : This file contain "main" function. Program execute will start and end here
#include "pch.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LEN 1000		//input maximum lenth
#define SINGLE_MAX_LEN 150	//single line code maximum lenth

#define IS_INCLUDE_LINE(LINENUMBER)	((LINENUMBER >= 340)&&(LINENUMBER <= 342))

const char CodePrefix[] = "\"C:/Keil_v5/ARM/ARMCC/include\",\n";

void Output_Path(const char *str);
char *OpenFile(char *filename);
char *Remove_PrefixAndSuffix(char * str);

int main()
{
	char file_path[MAX_LEN] = { 0 };

	printf("Author: GAOJISHENG Mail: GJS990825@GMAIL.COM\n\n");

	while (true)
	{
		printf("Please input the path of MDK project(contain file name and filename extension):\n");
		scanf_s("%s", &file_path, (unsigned)_countof(file_path));

		Output_Path(Remove_PrefixAndSuffix(OpenFile(file_path)));
	}

	return 0;
}

void Output_Path(const char *str)
{
	char output[SINGLE_MAX_LEN] = {0};
	char *poutput = output;

	printf("\nTransform result:\n\n");

	printf("%s", CodePrefix);

	while (*str != '\0')
	{
		if (*str != ';' && *str != '.')
		{
			(*str == '\\') ? (*poutput++ = '/') : (*poutput++ = *str);
		}
		if ((*str == ';') || (*(str + 1) == '\0'))
		{
			printf("\"${workspaceFolder}%s\"", output);
			memset(output, 0, sizeof(output));
			poutput = output;
			if (*(str + 1) != '\0')
			{
				printf(",\n");
			}
			else
			{
				printf("\n");
			}
		}
		str++;
	}
	printf("\n");
}

char *OpenFile(char *filename)
{
	FILE *stream;
	errno_t err;
	int CurrentIndex = 1;
	int i = 0;
	static char output[MAX_LEN] = { 0 };
	char *begin = NULL;

	memset(output, 0, _countof(output));
	err = fopen_s(&stream, filename, "r");
	if (!err)
	{
		printf("\nFile opened\n\n");

		while (!feof(stream))
		{

			if (IS_INCLUDE_LINE(CurrentIndex))
			{
				fgets(output, MAX_LEN, stream);
				if (strstr(output, "<IncludePath>") != NULL)
				{
					err = fclose(stream);
					err ? printf("CLOSE FAIL\n") : printf("File closed\n");
					return strstr(output, "<IncludePath>");
				}
			}
			else
			{
				fgets(output, MAX_LEN, stream);
			}
			CurrentIndex++;
		}

		err = fclose(stream);
		err ? printf("CLOSE FAIL\n") : printf("CLOSE OK!\n");
		return NULL;
	}
	else
	{
		printf("OPEN FAIL\n");
		return NULL;
	}
}

char *Remove_PrefixAndSuffix(char * str)
{
	int i = 0;
	char *pstr = str;
	while (*pstr != '\0')
	{
		while (*pstr++ != '>');
		while (*(pstr + (++i)) != '<');
		*(pstr + i) = '\0';
		return pstr;
	}
	return NULL;
}
