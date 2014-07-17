#include <iostream>
#define FOR(A,B) for(A=0; A<B; A++)

using namespace std;
int main()
{
int n;	
cin>>n;                           //테스트 케이스 입력
if(n>30) return 0;				  //테스트 케이스 체크(<=30)
int i,c,input[30][16] = {0,};
FOR(c,n){						  //시계 초기상태 입력
	FOR(i,16){
		cin>>input[c][i];
	}
}
int test[16],sw[10],cl=0,cl_min=41;
FOR(c,n){
	cl_min=41;
	FOR(i,16)
		test[i]=input[c][i];
	FOR(sw[0],4)
		FOR(sw[1],4)
			FOR(sw[2],4)
				FOR(sw[3],4)
					FOR(sw[4],4)
						FOR(sw[5],4)
							FOR(sw[6],4)
								FOR(sw[7],4)
								   FOR(sw[8],4)
									  FOR(sw[9],4)
										{										
											if((test[0]+sw[0]*3+sw[3]*3+sw[5]*3)%12!=0)			  continue;
											if((test[1]+sw[0]*3+sw[8]*3)%12!=0)					  continue;
											if((test[2]+sw[0]*3+sw[5]*3+sw[8]*3)%12!=0)			  continue;
											if((test[3]+sw[1]*3+sw[6]*3+sw[8]*3+sw[9]*3)%12!=0)	  continue;
											if((test[4]+sw[2]*3+sw[3]*3+sw[7]*3+sw[8]*3+sw[9]*3)%12!=0)  continue;
											if((test[5]+sw[3]*3+sw[7]*3+sw[8]*3+sw[9]*3)%12!=0)   continue;
											if((test[6]+sw[3]*3+sw[4]*3)%12!=0)					  continue;
											if((test[7]+sw[1]*3+sw[3]*3+sw[4]*3+sw[7]*3)%12!=0)   continue;
											if((test[8]+sw[4]*3)%12!=0)							  continue;
											if((test[9]+sw[1]*3+sw[9]*3)%12!=0)					  continue;
											if((test[10]+sw[2]*3+sw[4]*3)%12!=0)				  continue;
											if((test[11]+sw[1]*3)%12!=0)						  continue;
											if((test[12]+sw[4]*3)%12!=0)						  continue;
											if((test[13]+sw[9]*3)%12!=0)						  continue;
											if((test[14]+sw[2]*3+sw[5]*3+sw[6]*3+sw[7]*3)%12!=0)  continue;
											if((test[15]+sw[2]*3+sw[5]*3+sw[6]*3+sw[7]*3)%12!=0)  continue;
											cl=0;
											FOR(i,10)											
												cl+=sw[i];
											if(cl<cl_min)
												cl_min=cl;		
										}
	if(cl_min==41) cout<<"-1"<<endl;
	else cout<<cl_min<<endl;
	}
	return 0;
}
/*
0	0, 1, 2
1	3, 7, 9, 11
2	4, 10, 14, 15
3	0, 4, 5, 6, 7
4	6, 7, 8, 10, 12
5	0, 2, 14, 15
6	3, 14, 15
7	4, 5, 7, 14, 15
8	1, 2, 3, 4, 5
9	3, 4, 5, 9, 13

0, 1, 2
         3,          7, 9,         11
            4,                 10,             14, 15
0,          4, 5, 6, 7
                  6, 7, 8,     10,     12
0,    2,                                       14, 15
         3,                                    14, 15
            4, 5,    7,                        14, 15
   1, 2, 3, 4, 5
         3, 4, 5,           9,             13
		 
*/
