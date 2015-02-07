
#include <iostream>

using namespace std;

int cache[101];

const int MOD = 1000000007;

int tiling(int width){

	if(width <= 1) return 1;
		//메모이제이션

	if(cache[width]!=0) return cache[width];
	int &ret = cache[width];
	//if(ret != -1) return ret;
		
	ret = (tiling(width-2) + tiling(width-1)) % MOD;
	return ret;
	
}

int main()
{
	
	int test_count;
	int width_;

	//int arr_count;

	cin>>test_count;

	while(test_count--){
	cin>>width_;

	cout<<tiling(width_)<<endl;	
	
	}
	return 0;
}

