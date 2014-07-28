#include <iostream>
#include <time.h>
#define MAX(a, b) (a>b ? a:b) 
#define MIN(a, b) (a<b ? a:b)
using namespace std;

int fence(int arr[], int left, int right)
{
	if(left == right) return arr[left];
	int mid = (left + right) / 2;
	//divide
	int ret = MAX(fence(arr,left, mid), fence(arr, mid+1, right));
	int lo = mid;
	int hi = mid + 1;
	int hei = MIN(arr[lo], arr[hi]);

	ret = MAX(ret,hei*2);

	while(left<lo || hi<right)
	{
		if(hi<right&&(lo==left || arr[lo-1]<arr[hi+1]))
		{
			++hi;
			hei = MIN(hei, arr[hi]);
		}
		else
		{
			--lo;
			hei = MIN(hei, arr[lo]);
		}
		ret = MAX(ret, hei*(hi-lo+1));

	}

	return ret;
}
int fence2(int arr[], int lo, int hi)
{
	if (lo == hi) return arr[lo];
	int	mid = (lo + hi) / 2;

	int minHeight = arr[mid];
	int height = arr[mid];
	int maxArea = arr[mid];
	int from = mid;
	for(int i = mid; i>=0; i--)
	{
		minHeight = MIN(minHeight, arr[i]);
		int tempArea = (mid -i + 1) * minHeight;
		if(tempArea > maxArea)
		{
			height = minHeight;
			maxArea = tempArea;
			from = i;
		}
		
	}
	minHeight = height;
	int to = mid + 1;
	for(int i = mid+1; i<=hi; i++)
	{
		minHeight = MIN(minHeight, arr[i]);
		int tempArea = (i-from + 1) * minHeight;
		if(tempArea > maxArea)
		{
			height = minHeight;
			maxArea = tempArea;
			to = i;
		}
	}
	int single = MAX(fence2(arr, lo, mid), fence2(arr, mid + 1, hi));

	return MAX(single, maxArea);
}

int main(void)
{
	int testcase;
	cin>>testcase;
	if(testcase > 50 || testcase <= 0)
		return 0;	

	for(int i=0; i<testcase; i++)
	{	
		int fenceNum;
		cin>>fenceNum;
		int *h;
		h = new int[fenceNum];
		
		for(int j=0; j<fenceNum; j++)
		{
			cin>>h[j];
		}
		cout<<fence(h, 0, fenceNum-1)<<endl;
		delete[] h;
	}
	return 0;
}
//int main(void)
//{
//	clock_t begin, end;
//	begin = clock();
//	int testcase = 1;
//	int fenceNum = 7;
//	//int h[7] = {7, 1, 5, 9, 6, 7, 3};
//	int h[7] = {10, 10, 4, 11, 12, 1, 1};
//	cout<<"max: "<<fence(h, 0, fenceNum-1)<<endl;
//
//	end = clock();
//	cout<<"time : "<<(double)(end-begin) / CLOCKS_PER_SEC<<endl;
//	return 0;
//}


//int main(void)
//{
//	int testcase;
//	cin>>testcase;
//	if(testcase > 50 || testcase <= 0)
//		return 0;
//	int *result;
//	result = new int[testcase];
//
//	for(int i=0; i<testcase; i++)
//	{
//		int fenceNum;
//		cin>>fenceNum;
//		int *height;
//		height = new int[fenceNum];
//		
//		for(int j=0; j<fenceNum; j++)
//		{
//			cin>>height[j];
//		}
//		result[i] = fence2(height, 0, fenceNum-1);
//
//		delete[] height;
//	}
//	/*for(int i = 0; i<testcase; i++)
//	{
//		cout<<result[i]<<endl;
//	}*/
//	delete[] result;
//	return 0;
//}


//int fence(int left, int right)
//{
//	if(left == right) return h[left];
//	int mid = (left + right) / 2;
//	//divide
//	int ret = MAX(fence(left, mid), fence(mid+1, right));
//	int lo = mid;
//	int hi = mid + 1;
//	int hei = MIN(h[lo], h[hi]);
//
//	ret = MAX(ret,hei*2);
//
//	while(left<lo || hi<right)
//	{
//		if(hi<right&&(lo==left || h[lo-1]<h[hi+1]))
//		{
//			++hi;
//			hei = MIN(hei, h[hi]);
//		}
//		else
//		{
//			--lo;
//			hei = MIN(hei, h[lo]);
//		}
//		ret = MAX(ret, hei*(hi-lo+1));
//
//	}
//
//	return ret;
//}


//int main(void)
//{
//	int testcase;
//	//cout<<"testcase:";
//	cin>>testcase;
//	if(testcase > 50 || testcase <= 0)
//		return 0;
//
//	for(int i=0; i<testcase; i++)
//	{
//		int fenceNum;
//		cout<<"fenceNum ["<<i<<"]";
//		cin>>fenceNum;
//		int *height;
//		height = new int[fenceNum];
//		
//		for(int j=0; j<fenceNum; j++)
//		{
//			cout<<"fence height["<<j<<"]: ";
//			cin>>height[j];
//		}
//		cout<<fence2(height, 0, fenceNum-1)<<endl;
//
//		delete[] height;
//	}
//	return 0;
//}