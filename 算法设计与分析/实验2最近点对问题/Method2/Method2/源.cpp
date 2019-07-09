#include<iostream>
#include<cstdlib>
#include<limits>
#include<random>
#include<cmath>
#include<ctime>
#include<streambuf>
#include<fstream>
#include<iomanip>
#include <chrono>
#define min(a,b) (a<b?a:b)
#define BOUNDS 2000000	//随机数值上界
#define AGAINNUM 20
#define TESTNUM 10
using namespace std;
using namespace chrono;
//运行时间
long long sortCnt[3][10];
//数据规模
int dataSize[] = { 10000,20000,30000,40000,50000,60000,70000,80000,90000,100000 };
double* arrayTmpX;
double* arrayTmpY;

//生成随机坐标
void createPoint(double* arrayX, double* arrayY, int size) {
	//以时间为种子，开启随机数引擎
	std::random_device rd;
	default_random_engine e(rd());
	uniform_real_distribution<double> u(0, BOUNDS);
	for (int i = 0; i < size; i++) {
		arrayX[i] = u(e);
		arrayY[i] = u(e);
	}
}

/*快排部分*/
int setPosition(double* arrayX, double* arrayY, int left, int right) {
	double tmp = arrayX[left];
	double tmp_1 = arrayY[left];
	//直至左边部分小于基准，右部分大于基准
	while (left < right) {
		//从右往左查找第一个小于基准值的元素替换到left位置
		while (left < right&&tmp <= arrayX[right]) {
			right--;
		}
		//替换后left右移一个单位
		if (left < right) {
			arrayX[left] = arrayX[right];
			arrayY[left] = arrayY[right];
			left++;
		}
		//从左向右查找第一个大于基准的元素替换到right位置
		while (left<right&&tmp>arrayX[left]) {
			left++;
		}
		//替换后right左移一个单位
		if (left < right) {
			arrayX[right] = arrayX[left];
			arrayY[right] = arrayY[left];
			right--;
		}
	}
	//放入原基准值
	arrayX[left] = tmp;
	arrayY[left] = tmp_1;
	return left;	//返回基准位置
}
void quickSort(double* arrayX, double* arrayY, int left, int right) {
	if (left < right) {
		//选择基准
		int pos = setPosition(arrayX, arrayY, left, right);
		quickSort(arrayX, arrayY, left, pos - 1);	//对基准左侧进行排序
		quickSort(arrayX, arrayY, pos + 1, right);	//对基准右侧进行排序
	}
}



/*归并求最小距离部分*/
//三个点以内的距离计算
double cal_distance(double* arrayX, double* arrayY, int left, int right) {
	int cnt = right - left + 1;
	if (cnt == 1) {
		return DBL_MAX;
	}if (cnt == 2) {
		//返回唯一确定距离
		return sqrt((arrayX[left] - arrayX[right])*(arrayX[left] - arrayX[right]) + (arrayY[left] - arrayY[right])*(arrayY[left] - arrayY[right]));
	}
	else {
		double d1 = sqrt((arrayX[left] - arrayX[right])*(arrayX[left] - arrayX[right]) + (arrayY[left] - arrayY[right])*(arrayY[left] - arrayY[right]));
		double d2 = sqrt((arrayX[left + 1] - arrayX[right])*(arrayX[left + 1] - arrayX[right]) + (arrayY[left + 1] - arrayY[right])*(arrayY[left + 1] - arrayY[right]));
		double d3 = sqrt((arrayX[left + 1] - arrayX[left])*(arrayX[left + 1] - arrayX[left]) + (arrayY[left + 1] - arrayY[left])*(arrayY[left + 1] - arrayY[left]));
		//返回三距离中的最小值
		return min(min(d1, d2), d3);
	}
}


double north, south, east;
long long cnt = 0;
double merge_solve2(double* arrayX, double* arrayY, int left, int mid, int right, double min_dis) {
	int l, r;
	int top = 0, bound;
	//筛选出位于左侧区间的距arrayX[mid]小于等于min_dis的点
	for (l = mid; l >= left && arrayX[mid] - arrayX[l] <= min_dis; l--) {
		arrayTmpX[top] = arrayX[l];
		arrayTmpY[top] = arrayY[l];
		top++;
	}
	//bound为左侧符合点的上界
	bound = top;
	//筛选出位于右侧区间的距arrayX[mid]小于等于min_dis的点
	for (r = mid + 1; r <= right && arrayX[r] - arrayX[mid] <= min_dis; r++) {
		arrayTmpX[top] = arrayX[r];
		arrayTmpY[top] = arrayY[r];
		top++;
	}
	//对符合点右侧部分进行排序
	cnt += top - bound;
	quickSort(arrayTmpY, arrayTmpX, bound, top - 1);

	//遍历左侧区间的点
	for (int i = 0; i < bound; i++) {
		//以该点确定一个2min_dis*min_dis 的矩形区间
		north = arrayTmpY[i] + min_dis;
		south = arrayTmpY[i] - min_dis;
		east = arrayTmpX[i] + min_dis;
		//遍历右侧没有越出上界的点，更新最小距离
		for (int j = bound; j < top && arrayTmpY[j] <= north; j++) {
			if (arrayTmpY[j] >= south && arrayTmpX[j] <= east) {
				min_dis = min(min_dis, sqrt((arrayTmpX[i] - arrayTmpX[j])*(arrayTmpX[i] - arrayTmpX[j]) + (arrayTmpY[i] - arrayTmpY[j])*(arrayTmpY[i] - arrayTmpY[j])));
			}
		}
	}
	return min_dis;
}

double divide2(double* arrayX, double* arrayY, int left, int right) {
	//solve2 n(logn)^2
	if (right - left + 1 <= 3) {
		return cal_distance(arrayX, arrayY, left, right);
	}
	else {
		int mid = (left + right) / 2;	//分成具有相同数量点的两个部分
		double left_minDis = divide2(arrayX, arrayY, left, mid);		//求解左
		double right_minDis = divide2(arrayX, arrayY, mid + 1, right);	//求解右
		double min_dis = min(left_minDis, right_minDis);		//比较求得两侧的最小距离
																//求合并区域的最小点距
		return merge_solve2(arrayX, arrayY, left, mid, right, min_dis);
	}
}

int main()
{
	double* arrayX;
	double* arrayY;
	double sumTime_1;
	double sumTime_2;
	double res_1, res_2;
	ofstream csvfile;
	//创建csv文件，存储计时结果
	csvfile.open("C:\\Users\\Administrator\\OneDrive\\算法设计与分析\\实验2最近点对问题\\Method2Analysis.csv");
	//统计归并和穷举的运行时间
	for (int p = 0; p < 3; p++) {
		for (int i = 0; i < TESTNUM; i++) {
			cnt = 0;
			int size = dataSize[i] * pow(10, p);
			arrayX = new double[size];
			arrayY = new double[size];
			arrayTmpX = new double[size];
			arrayTmpY = new double[size];
			sumTime_1 = sumTime_2 = 0;
			for (int j = 0; j < AGAINNUM; j++) {
				long long temp = cnt;
				//创建随机点
				createPoint(arrayX, arrayY, size);


				//归并求解计时
				auto startTime_1 = system_clock::now();
				quickSort(arrayX, arrayY, 0, size - 1);
				res_1 = divide2(arrayX, arrayY, 0, size - 1);
				auto endTime_1 = system_clock::now();


				//使用C++11的新计时工具
				//原本的clock只能精确至毫秒级，造成统计误差
				auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
				double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
				runTime_1 *= 1000;	//转成毫秒
				//迭代计算总时间
				sumTime_1 += runTime_1;
				cout << "datasize:" << size << "| sortnum:" << cnt - temp << endl;
			}
			cout << endl;
			delete[]arrayX;
			delete[]arrayY;
			delete[]arrayTmpX;
			delete[]arrayTmpY;
			sortCnt[p][i] = cnt / 20;
		}
	}



	for (int p = 0; p < 3; p++) {
		for (int i = 0; i < TESTNUM; i++) {
			csvfile << dataSize[i]*pow(10,p) << ",";
		}
		cout << endl;
		for (int i = 0; i < TESTNUM; i++) {
			csvfile << sortCnt[p][i] << ",";
		}
		cout << endl;
	}

}
