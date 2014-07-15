class Boggle {
public:
	static int m_direction[8];
	char m_data[26];
	Boggle()
	{
		strcpy(m_data, "URLPMXPRETGIAETXTNZYXOQRS");
	}
	bool DFS_Boggle(int offset , Boggle boggle , char *string);
};

int Boggle::m_direction[8] = {-5 -1,-5, -5 + 1, -1, 1, 5-1 , 5 , 5+1};

bool Boggle::DFS_Boggle(int offset , Boggle boggle , char *string)
{
	if (*string == '\0')
		return true;
	if ( offset < 0 || offset >= 25)
		return false;
	if(boggle.m_data[offset] != *string)
		return false;
	bool retVal =false;
	boggle.m_data[offset] = '*';
	int tmp = 0;
}



#include "boggle.h"
#include <iostream>

int main()
{
	char findString[32];
	printf("input word: ");
	gets(findString);
	
	int len = strlen(findString);
	Boggle boggle;
	for(int i = 0; i < 25; i++)
	{
		if(boggle.DFS_Boggle(i, boggle, findString))
		{
			printf("i = %d	First Char = %c \n", i+1, boggle.m_data[i]);

		}
	}
}
