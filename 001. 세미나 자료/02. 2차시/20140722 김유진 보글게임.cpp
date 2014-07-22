#include <stdio.h>
#include <string.h>


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
	printf("%d	%c\n", offset, boggle.m_data[offset]);
	if (*string == '\0')
		return true;
	if ( offset < 0 || offset >= 25)
		return false;
	if(boggle.m_data[offset] != *string)
		return false;
	bool retVal =false;
	boggle.m_data[offset]= '*';
	
	int tmp = 0;
}

int main()
{

	char findString[6][10]= { "PRETTY" , "GIRL", "REPEAT", "KARA", "PANDORA" ,"GIAZAPX"};
	//char findString[6]
	//int len = strlen(findString);
	Boggle boggle;
	
	for(int j = 0; j<6; j++)
	{
		printf("%s	", findString[j]);
		for(int i = 0; i < 25; i++)
		{		
				
				if(boggle.DFS_Boggle(i, boggle, findString[j]))
				{
					printf("YES\n");
				}
		}
	}
	
}

