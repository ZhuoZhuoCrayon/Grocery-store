#include<iostream>
#include<ctime>
#include<algorithm>
#define MAX_SIZE 10000000
using namespace std;
int Array[MAX_SIZE];
void generate_randomArray() {
	srand(time(0));
	for (int i = 0; i < MAX_SIZE; i++) {

		Array[i] = rand()*rand();
	}
}
int setPosition(int* Array,int left, int right) {
	int p = Array[left];
	while (left < right) {
		while (left < right&&p >= Array[right]) {
			right--;
		}
		if (left < right) {
			Array[left++] = Array[right];
		}
		while (left < right&&p < Array[left]) {
			left++;
		}
		if (left < right) {
			Array[right--] = Array[left];
		}
	}
	Array[left] = p;
	return left;
}
int pre_array[1010];
int top = 0;
int pre_len = 10;
void quick_halfSort(int* Array,int left, int right) {
	if (pre_len == 0)
		return;
	int p = setPosition(Array,left, right);
	if (p > pre_len) {
		quick_halfSort(Array, left, p - 1);
	}
	else if (p == pre_len){
		pre_len -= 10;
		for (int i = 0; i < 10; i++)
			pre_array[top++] = Array[i];
	}
	else {
		for (int i = 0; i <= p, pre_len > 0; i++){
			pre_array[top++] = Array[i];
			pre_len--;
		}
		int max = 0;
		int j;
		for (int k = 1; k <= pre_len; k++) {
			max = 0;
			j = right;
			for (int i = right; i >= p + 1; i--) {
				if (Array[i] > max) {
					max = Array[i];
					j = i;
				}
			}
			Array[j] = 0;
			pre_array[top++] = max;
		}
		pre_len = 0;
	}
}

int main()
{
	double startTime, endTime, sumTime = 0;
	for (int i = 0; i < 100; i++) {
		generate_randomArray();
		startTime = double(clock());
		quick_halfSort(Array, 0, MAX_SIZE - 1);
		endTime = double(clock());
		sumTime += endTime - startTime;
		pre_len = 10;
	}
	startTime = double(clock());
	quick_halfSort(pre_array, 0, 1000 - 1);
	endTime = double(clock());
	sumTime += endTime - startTime;
	cout << "在10亿数据中取出10个最大数所花费的时间：";
	cout << sumTime <<"ms"<< endl;
	for (int i = 1000; i < 1010; i++)
	{
		cout << pre_array[i] << " ";
		if (i == 1004)
			cout << endl;
	}
	cout << endl;
}