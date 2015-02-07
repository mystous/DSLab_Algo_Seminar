
#include <iostream>
#include <string>

using namespace std;
string reverse(string::iterator& it);

int main(){
	int test_count;

	cin>>test_count;

	while(test_count--){
		
		string str;
		cin>>str;

		string::iterator it = str.begin();

		cout<<reverse(it)<<endl;
	}
	
	return 0;
}

string reverse(string::iterator& it)
{
	char front = *it;

	++it;

	if(front == 'w' || front == 'b') return string(1,front);

	string upperLeft = reverse(it);
	string upperRight = reverse(it);
	string lowerLeft = reverse(it);
	string lowerRight = reverse(it);

	return string("x")+lowerLeft+lowerRight+upperLeft+upperRight;
	
}
