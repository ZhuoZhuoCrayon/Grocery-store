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
double runtime_compare[3][10];
//数据规模
int dataSize[] = { 10000,20000,30000,40000,50000,60000,70000,80000,90000,100000 };
double* arrayTmpX;
double* arrayTmpY;
double* X_l;
double* X_r;
double* Y_l;
double* Y_r;
double X_l1[1000000];
double X_r1[1000000];
double Y_l1[1000000];
double Y_r1[1000000];
int top;

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
//拷贝数组
void copyArray(double* array, double* array_c, int size) {
	for (int i = 0; i < size; i++) {
		array_c[i] = array[i];
	}
}

/*快排部分*/
int setPosition(double* arrayX,double* arrayY, int left, int right) {
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


void mergeY(double* arrayX, double* arrayY, int left, int mid, int right,double d,double m,bool op) {

	int left_len = mid - left + 1;
	int right_len = right - mid;
	int i, j;
	i = j = 0;
	for (i = 0; i < left_len; i++) {
		X_l1[i] = arrayTmpX[left + i];
		Y_l1[i] = arrayTmpY[left + i];
	}
	for (j = 0; j < right_len; j++) {
		X_r1[j] = arrayTmpX[mid + j + 1];
		Y_r1[j] = arrayTmpY[mid + j + 1];
	}
	Y_l1[left_len] = DBL_MAX;
	Y_r1[right_len] = DBL_MAX;
	i = j = 0;
	for (int k = left; k <= right; k++) {
		if (Y_l1[i] < Y_r1[j]) {
			arrayTmpX[k] = X_l1[i];
			arrayTmpY[k] = Y_l1[i];
			i++;
		}
		else {
			arrayTmpX[k] = X_r1[j];
			arrayTmpY[k] = Y_r1[j];
			j++;
		}
		if (fabs(arrayTmpX[k] - m) <= d&&op) {
			X_l[top] = arrayTmpX[k];
			Y_l[top] = arrayTmpY[k];
			top++;
		}
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

//合并求最小距
double merge_solve1(double* arrayX, double* arrayY, int left, int mid, int right, double min_dis) {

	//将左右Y轴有序的数组合并成有序数组

	//点数小于3，直接暴力求解
	if (right - left + 1 <= 3) {
		mergeY(arrayTmpX, arrayTmpY, left, mid, right,min_dis,arrayX[mid],false);
		int cnt = right - left + 1;
		if (cnt == 2) {
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
	else
	{
		
		top = 0;
		mergeY(arrayTmpX, arrayTmpY, left, mid, right,min_dis,arrayX[mid],true);
		//筛选出该区间内距中线mindis的点
		/*for (int i = left; i <= right; i++) {
			if (fabs(arrayTmpX[i] - arrayX[mid]) <= min_dis) {
				X_l[top] = arrayTmpX[i];
				Y_l[top] = arrayTmpY[i];
				top++;
			}
		}*/
		//此时Y_l有序,遍历每一个点，每个点向上扫描Y轴距小于min_dis的点，刷新最小距离
		for (int i = 0; i < top; i++) {
			for (int j = i + 1; j < top&&Y_l[j] - Y_l[i] < min_dis; j++) {
				min_dis = min(min_dis, sqrt((X_l[i] - X_l[j])*(X_l[i] - X_l[j]) + (Y_l[i] - Y_l[j])*(Y_l[i] - Y_l[j])));
			}
		}
		return min_dis;
	}
}

double north, south, east;
double merge_solve2(double* arrayX, double* arrayY, int left, int mid, int right, double min_dis) {
	int l, r;
	int top = 0, bound;
	//筛选出位于左侧区间的距arrayX[mid]小于等于min_dis的点
	/*for (l = mid; l >= left && arrayX[mid] - arrayX[l] <= min_dis; l--) {
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
	}*/
	for (int i =left; i <= right; i++) {
		if (fabs(arrayX[i] - arrayX[mid])<=min_dis) {
			arrayTmpX[top] = arrayX[i];
			arrayTmpY[top] = arrayY[i];
			top++;
		}
	}
	//对符合点右侧部分进行排序
	quickSort(arrayTmpY, arrayTmpX, 0, top - 1);

	//遍历左侧区间的点
	/*for (int i = 0; i < bound; i++) {
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
	}*/
	for (int i = 0; i < top; i++) {
		for (int j = i + 1; j < top&&arrayTmpY[j] - arrayTmpY[i] < min_dis; j++) {
			min_dis = min(min_dis, sqrt((arrayTmpX[i] - arrayTmpX[j])*(arrayTmpX[i] - arrayTmpX[j]) + (arrayTmpY[i] - arrayTmpY[j])*(arrayTmpY[i] - arrayTmpY[j])));
		}
	}
	return min_dis;
}

//降低问题规模
double divide1(double* arrayX, double* arrayY, int left, int right) {
	//solve1 nlogn
	if (left < right) {
		int mid = (left + right) / 2;	//分成具有相同数量点的两个部分
		double left_minDis = divide1(arrayX, arrayY, left, mid);		//求解左
		double right_minDis = divide1(arrayX, arrayY, mid + 1, right);	//求解右
		double min_dis = min(left_minDis, right_minDis);		//比较求得两侧的最小距离
																//求合并区域的最小点距
		return merge_solve1(arrayX, arrayY, left, mid, right, min_dis);
	}
	else {
		return DBL_MAX;
	}
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
/*穷举求解最小点距*/
double bruteforce(double* arrayX, double* arrayY, int left, int right) {
	double min_dis = DBL_MAX;
	//遍历每对点的距离，求出最小距离
	for (int i = 0; i < right; i++) {
		for (int j = i + 1; j <= right; j++) {
			double dis = sqrt((arrayX[i] - arrayX[j])*(arrayX[i] - arrayX[j]) + (arrayY[i] - arrayY[j])*(arrayY[i] - arrayY[j]));
			min_dis = min(dis, min_dis);
		}
	}
	return min_dis;
}

int main()
{
	double* arrayX;
	double* arrayY;
	double* arrayX_c;
	double* arrayY_c;
	double sumTime_1;
	double sumTime_2;
	double res_1, res_2;
	ofstream csvfile;
	//创建csv文件，存储计时结果
	csvfile.open("C:\\Users\\Administrator\\OneDrive\\算法设计与分析\\实验2最近点对问题\\analysisRes1.csv");	
	//统计归并和穷举的运行时间
	for (int i = 0; i < TESTNUM; i++) {
		int size = dataSize[i]*10;
		arrayX = new double[size];
		arrayY = new double[size];
		arrayX_c = new double[size];
		arrayY_c = new double[size];
		arrayTmpX = new double[size];
		arrayTmpY = new double[size];
		X_l = new double[size];
		X_r = new double[size];
		Y_l = new double[size];
		Y_r = new double[size];
		sumTime_1 = sumTime_2 = 0;
		for (int j = 0; j < AGAINNUM; j++) {
			//创建随机点
			createPoint(arrayX, arrayY, size);
			//复制坐标数组
			copyArray(arrayX, arrayX_c, size);
			copyArray(arrayY, arrayY_c, size);

			//slove1用来复制和排序tmp数组用于归并
			copyArray(arrayX, arrayTmpX, size);
			copyArray(arrayY, arrayTmpY, size);
			quickSort(arrayTmpX, arrayTmpY, 0, size - 1);

			//归并求解计时
			auto startTime_1 = system_clock::now();
			quickSort(arrayX, arrayY, 0, size - 1);
			//tmp数组，用于归并
			res_1 = divide1(arrayX, arrayY, 0, size - 1);
			auto endTime_1 = system_clock::now();

			//穷举求解计时
			auto startTime_2 = system_clock::now();
			res_2 = bruteforce(arrayX_c, arrayY_c, 0, dataSize[i] - 1);
			//res_2 = 0;
			/*quickSort(arrayX_c, arrayY_c, 0, size - 1);
			res_2= divide2(arrayX, arrayY, 0, size - 1);*/
			auto endTime_2 = system_clock::now();

			//使用C++11的新计时工具
			//原本的clock只能精确至毫秒级，造成统计误差
			auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
			double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
			runTime_1 *= 1000;	//转成毫秒

			auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
			double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
			runTime_2 *= 1000;

			//迭代计算总时间
			sumTime_1 += runTime_1;
			sumTime_2 += runTime_2;
			
			std::cout << fixed << setprecision(2) << "数据规模[" << size << "][" << j << "] |" << "分治法用时：[" << runTime_1 << "]";
			cout << "穷举法耗时：[" << runTime_2 << "] |最小距离计算结果：[" << res_1 << "|" << res_2 << "]" << endl;
		}
		cout << endl;

		//写入统计表格
		runtime_compare[0][i] = sumTime_1 / 20;
		runtime_compare[1][i] = sumTime_2 / 20;

		delete[]arrayX;
		delete[]arrayY;
		delete[]arrayX_c;
		delete[]arrayY_c;
		delete[]arrayTmpX;
		delete[]arrayTmpY;
		delete[]X_l;
		delete[]X_r;
		delete[]Y_l;
		delete[]Y_r;
	}

	cout << "------------------------以下是数据规模为10w-100w的归并算法测试----------------------------------" << endl;
	for (int i = 0; i < TESTNUM; i++) {
		int size = dataSize[i] * 100;
		arrayX = new double[size];
		arrayY = new double[size];
		arrayTmpX = new double[size];
		arrayTmpY = new double[size];
		X_l = new double[size];
		X_r = new double[size];
		Y_l = new double[size];
		Y_r = new double[size];
		sumTime_1 = 0;
		for (int j = 0; j < AGAINNUM; j++) {
			//创建随机点
			
			//solve1用于归并的tmp数组
			createPoint(arrayX, arrayY, size);

			//归并求解计时
			auto startTime_1 = system_clock::now();
			quickSort(arrayX, arrayY, 0, size - 1);
			//copyArray(arrayX, arrayTmpX, size);
			//copyArray(arrayY, arrayTmpY, size);
			res_1 = divide1(arrayX, arrayY, 0, size - 1);
			auto endTime_1 = system_clock::now();


			auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
			double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
			runTime_1 *= 1000;	//转成毫秒
			sumTime_1 += runTime_1;

			cout << fixed << setprecision(2) << "数据规模[" << size << "][" << j << "] |" << "分治法用时：[" << runTime_1 << "]";
			cout << "| 最小距离计算结果：[" << res_1 << "]" << endl;
		}
		cout << endl;

		runtime_compare[2][i] = sumTime_1 / 20;
		delete[]arrayX;
		delete[]arrayY;
		delete[]arrayTmpX;
		delete[]arrayTmpY;
		delete[]X_l;
		delete[]X_r;
		delete[]Y_l;
		delete[]Y_r;
	}
	
	

	//将对比统计结果写入csv文件
	csvfile << "[数据规模]" << ",";
	for (int i = 0; i < TESTNUM; i++) {
		csvfile << dataSize[i];
		if (i != TESTNUM - 1) {
			csvfile << ",";
		}
		else
			csvfile << endl;
	}
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			csvfile << "[归并求解]"<<",";
		}
		else if(i==1) {
			csvfile << "[穷举求解]"<<",";
		}
		else {
			csvfile << "[归并求解10w-100w]" << ",";
		}
		for (int j = 0; j < TESTNUM; j++) {
			csvfile << runtime_compare[i][j];
			if (j != TESTNUM - 1) {
				csvfile << ",";
			}
			else
				csvfile << endl;
		}
	}
	
}
