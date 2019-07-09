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
#define BOUNDS 2000000	//�����ֵ�Ͻ�
#define AGAINNUM 20
#define TESTNUM 10
using namespace std;
using namespace chrono;
//����ʱ��
long long sortCnt[3][10];
//���ݹ�ģ
int dataSize[] = { 10000,20000,30000,40000,50000,60000,70000,80000,90000,100000 };
double* arrayTmpX;
double* arrayTmpY;

//�����������
void createPoint(double* arrayX, double* arrayY, int size) {
	//��ʱ��Ϊ���ӣ��������������
	std::random_device rd;
	default_random_engine e(rd());
	uniform_real_distribution<double> u(0, BOUNDS);
	for (int i = 0; i < size; i++) {
		arrayX[i] = u(e);
		arrayY[i] = u(e);
	}
}

/*���Ų���*/
int setPosition(double* arrayX, double* arrayY, int left, int right) {
	double tmp = arrayX[left];
	double tmp_1 = arrayY[left];
	//ֱ����߲���С�ڻ�׼���Ҳ��ִ��ڻ�׼
	while (left < right) {
		//����������ҵ�һ��С�ڻ�׼ֵ��Ԫ���滻��leftλ��
		while (left < right&&tmp <= arrayX[right]) {
			right--;
		}
		//�滻��left����һ����λ
		if (left < right) {
			arrayX[left] = arrayX[right];
			arrayY[left] = arrayY[right];
			left++;
		}
		//�������Ҳ��ҵ�һ�����ڻ�׼��Ԫ���滻��rightλ��
		while (left<right&&tmp>arrayX[left]) {
			left++;
		}
		//�滻��right����һ����λ
		if (left < right) {
			arrayX[right] = arrayX[left];
			arrayY[right] = arrayY[left];
			right--;
		}
	}
	//����ԭ��׼ֵ
	arrayX[left] = tmp;
	arrayY[left] = tmp_1;
	return left;	//���ػ�׼λ��
}
void quickSort(double* arrayX, double* arrayY, int left, int right) {
	if (left < right) {
		//ѡ���׼
		int pos = setPosition(arrayX, arrayY, left, right);
		quickSort(arrayX, arrayY, left, pos - 1);	//�Ի�׼����������
		quickSort(arrayX, arrayY, pos + 1, right);	//�Ի�׼�Ҳ��������
	}
}



/*�鲢����С���벿��*/
//���������ڵľ������
double cal_distance(double* arrayX, double* arrayY, int left, int right) {
	int cnt = right - left + 1;
	if (cnt == 1) {
		return DBL_MAX;
	}if (cnt == 2) {
		//����Ψһȷ������
		return sqrt((arrayX[left] - arrayX[right])*(arrayX[left] - arrayX[right]) + (arrayY[left] - arrayY[right])*(arrayY[left] - arrayY[right]));
	}
	else {
		double d1 = sqrt((arrayX[left] - arrayX[right])*(arrayX[left] - arrayX[right]) + (arrayY[left] - arrayY[right])*(arrayY[left] - arrayY[right]));
		double d2 = sqrt((arrayX[left + 1] - arrayX[right])*(arrayX[left + 1] - arrayX[right]) + (arrayY[left + 1] - arrayY[right])*(arrayY[left + 1] - arrayY[right]));
		double d3 = sqrt((arrayX[left + 1] - arrayX[left])*(arrayX[left + 1] - arrayX[left]) + (arrayY[left + 1] - arrayY[left])*(arrayY[left + 1] - arrayY[left]));
		//�����������е���Сֵ
		return min(min(d1, d2), d3);
	}
}


double north, south, east;
long long cnt = 0;
double merge_solve2(double* arrayX, double* arrayY, int left, int mid, int right, double min_dis) {
	int l, r;
	int top = 0, bound;
	//ɸѡ��λ���������ľ�arrayX[mid]С�ڵ���min_dis�ĵ�
	for (l = mid; l >= left && arrayX[mid] - arrayX[l] <= min_dis; l--) {
		arrayTmpX[top] = arrayX[l];
		arrayTmpY[top] = arrayY[l];
		top++;
	}
	//boundΪ�����ϵ���Ͻ�
	bound = top;
	//ɸѡ��λ���Ҳ�����ľ�arrayX[mid]С�ڵ���min_dis�ĵ�
	for (r = mid + 1; r <= right && arrayX[r] - arrayX[mid] <= min_dis; r++) {
		arrayTmpX[top] = arrayX[r];
		arrayTmpY[top] = arrayY[r];
		top++;
	}
	//�Է��ϵ��Ҳಿ�ֽ�������
	cnt += top - bound;
	quickSort(arrayTmpY, arrayTmpX, bound, top - 1);

	//�����������ĵ�
	for (int i = 0; i < bound; i++) {
		//�Ըõ�ȷ��һ��2min_dis*min_dis �ľ�������
		north = arrayTmpY[i] + min_dis;
		south = arrayTmpY[i] - min_dis;
		east = arrayTmpX[i] + min_dis;
		//�����Ҳ�û��Խ���Ͻ�ĵ㣬������С����
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
		int mid = (left + right) / 2;	//�ֳɾ�����ͬ���������������
		double left_minDis = divide2(arrayX, arrayY, left, mid);		//�����
		double right_minDis = divide2(arrayX, arrayY, mid + 1, right);	//�����
		double min_dis = min(left_minDis, right_minDis);		//�Ƚ�����������С����
																//��ϲ��������С���
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
	//����csv�ļ����洢��ʱ���
	csvfile.open("C:\\Users\\Administrator\\OneDrive\\�㷨��������\\ʵ��2����������\\Method2Analysis.csv");
	//ͳ�ƹ鲢����ٵ�����ʱ��
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
				//���������
				createPoint(arrayX, arrayY, size);


				//�鲢����ʱ
				auto startTime_1 = system_clock::now();
				quickSort(arrayX, arrayY, 0, size - 1);
				res_1 = divide2(arrayX, arrayY, 0, size - 1);
				auto endTime_1 = system_clock::now();


				//ʹ��C++11���¼�ʱ����
				//ԭ����clockֻ�ܾ�ȷ�����뼶�����ͳ�����
				auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
				double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
				runTime_1 *= 1000;	//ת�ɺ���
				//����������ʱ��
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
