#include<iostream>
#include<climits>
#include<cstdlib>
#include<ctime>
#include<string>
#include<streambuf>
#include<fstream>
#include<algorithm>
#include<iomanip>
#define MAX_SIZE 50005
using namespace std;
int dataSize[] = { 400000,500000,600000,700000,800000 };
string sortList[] = { "选择排序","冒泡排序","合并排序","快速排序","插入排序" };
//选择排序、冒泡排序、合并排序、快速排序、插入排序算法

void selectSort(int* Array, int n) {
	int tmp;
	for (int i = 0; i < n - 1; i++) {
		int index = i;
		for (int j = i + 1; j < n; j++) {
			if (Array[index] > Array[j])
				index = j;
		}
		tmp = Array[index];
		Array[index] = Array[i];
		Array[i] = tmp;
	}
}

void bubbleSort(int* Array, int n) {
	int tmp;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (Array[j] > Array[j + 1]) {
				tmp = Array[j];
				Array[j] = Array[j + 1];
				Array[j + 1] = tmp;
			}
		}
	}
}

void insertSort(int* Array, int n) {
	for (int i = 1; i < n; i++) {
		int key = Array[i];
		int j = i - 1;
		while (j >= 0 && Array[j] > key) {
			Array[j + 1] = Array[j];
			j--;
		}
		Array[j + 1] = key;
	}
}

int leftArray[MAX_SIZE];
int rightArray[MAX_SIZE];
void merge(int* Array, int left, int mid, int right) {
	
	int left_len = mid - left + 1;
	int right_len = right - mid;
	int i, j;
	i = j = 0;
	for (i = 0; i < left_len; i++) {
		leftArray[i] = Array[left + i];
	}
	for (j = 0; j < right_len; j++) {
		rightArray[j] = Array[mid + j + 1];
	}
	leftArray[left_len] = INT_MAX;
	rightArray[right_len] = INT_MAX;
	i = j = 0;
	for (int k = left; k <= right; k++) {
		if (leftArray[i] <= rightArray[j]) {
			Array[k] = leftArray[i++];
		}
		else {
			Array[k] = rightArray[j++];
		}
	}
}
void mergeSort(int* Array,int left,int right) {
	if (left < right) {
		int mid = (left + right) / 2;
		mergeSort(Array, left, mid);
		mergeSort(Array, mid + 1, right);
		merge(Array, left, mid, right);
	}
}

int setPosition(int* Array, int left, int right) {
	int tmp = Array[left];
	while (left < right) {
		while (left < right&&tmp <= Array[right]) {
			right--;
		}
		if (left < right)Array[left++] = Array[right];

		while (left<right&&tmp>Array[left]) {
			left++;
		}
		if (left < right)Array[right--] = Array[left];
	}
	Array[left] = tmp;
	return left;
}
void quickSort(int* Array, int left, int right) {
	if (left < right) {
		int index = setPosition(Array, left, right);
		quickSort(Array, left, index - 1);
		quickSort(Array, index + 1, right);
	}
}

void generate_randomArray(int* Array, int n) {
	srand(time(0));
	for (int i = 0; i < n; i++) {

		Array[i] = rand()*rand();
	}
}	
int main()
{
	int* Array;
	int op;
	double startTime, endTime, sumTime;
	ofstream csvFile;
	csvFile.open("C:\\Users\\Administrator\\OneDrive\\算法设计与分析\\runningTime1.csv", ios::out | ios::trunc);

	for (int i = 0; i < 5; i++) {
		csvFile << dataSize[i];
		if (i != 4)csvFile << ",";
	}
	csvFile << endl;
	cout << endl;
	/*Array = new int[10];
	for (int i = 0; i < 10; i++)
		cin >> Array[i];*/
	//bubbleSort(Array, n);
	//selectSort(Array, n);
	//mergeSort(Array, 0, n - 1);
	//insertSort(Array, n);
	//quickSort(Array, 0, n - 1);
	//选择排序、冒泡排序、合并排序、快速排序、插入排序算法
	
	while (true) {
		std::cout << "实验1：排序算法性能分析" << endl;
		std::cout << "输入相应排序算法编号，测试算法在不同数据规模下的运行时间" << endl;
		std::cout << "1: 选择排序 | 2：冒泡排序 | 3：合并排序 | 4：快速排序 | 5：插入排序 | 6：退出" << endl << endl;
		
		std::cin >> op;
		if (op > 6)
			continue;
		if (op == 6)
			break;
		csvFile << "[  " << sortList[op - 1] << "   ]" << endl;
		for (int i = 0; i < 5; i++) {
			sumTime = 0;
			Array = new int[dataSize[i]];
			std::cout << "当数据规模为：[" << dataSize[i] << "]时" << endl;
			std::cout << "20组样本运行时间(ms)分别为：";
			for (int j = 0; j < 20; j++) {
				generate_randomArray(Array, dataSize[i]);
				switch (op)
				{
				case 1:
					startTime = double(clock());
					selectSort(Array, dataSize[i]);
					endTime = double(clock());
					break;
				case 2:
					startTime = double(clock());
					bubbleSort(Array, dataSize[i]);
					endTime = double(clock());
					break;
				case 3:
					startTime = double(clock());
					mergeSort(Array, 0, dataSize[i] - 1);
					endTime = double(clock());
					break;
				case 4:
					startTime = double(clock());
					quickSort(Array, 0, dataSize[i] - 1);
					endTime = double(clock());
					break;
				case 5:
					startTime = double(clock());
					insertSort(Array, dataSize[i]);
					endTime = double(clock());
					break;
				default:
					break;
				}
				
				double runningTime = endTime - startTime;
				sumTime += runningTime;
				std::cout << fixed << setprecision(1) << runningTime << " ";
			}
			std::cout << endl << "[" << sortList[op - 1] << "]平均运行时间(ms)：" << fixed << setprecision(2) << sumTime / 20 << endl << endl;
			
			csvFile << fixed << setprecision(2) << sumTime / 20;

			if (i != 4)csvFile << ",";
			delete[]Array;
		}
		csvFile << endl;
	}
	return 0;
}



