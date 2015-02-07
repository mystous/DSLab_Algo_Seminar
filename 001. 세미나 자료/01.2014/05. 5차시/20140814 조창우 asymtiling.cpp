#include <iostream> 
using namespace std; 
int cache[101]; 
const int MOD = 1000000007; 
 
 int tiling(int width){ 
 	if(width <= 1) return 1; 
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
 	cin>>test_count; 
 	while(test_count--){ 
 	cin>>width_; 
	int output=0;
	if(width_%2==0) output=(tiling(width_)-tiling((int)width_/2)-tiling((int)width_/2-1));	 
	else output=(tiling(width_)-tiling((int)width_/2));
	if(output<0) output+=MOD;
	if(output<0) output+=MOD;
	 cout<<output<<endl;
 	} 
 	return 0; 
 } 
