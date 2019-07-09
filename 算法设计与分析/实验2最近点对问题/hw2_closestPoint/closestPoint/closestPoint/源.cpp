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
double runtime_compare[3][10];
//���ݹ�ģ
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
//��������
void copyArray(double* array, double* array_c, int size) {
	for (int i = 0; i < size; i++) {
		array_c[i] = array[i];
	}
}

/*���Ų���*/
int setPosition(double* arrayX,double* arrayY, int left, int right) {
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

//�ϲ�����С��
double merge_solve1(double* arrayX, double* arrayY, int left, int mid, int right, double min_dis) {

	//������Y�����������ϲ�����������

	//����С��3��ֱ�ӱ������
	if (right - left + 1 <= 3) {
		mergeY(arrayTmpX, arrayTmpY, left, mid, right,min_dis,arrayX[mid],false);
		int cnt = right - left + 1;
		if (cnt == 2) {
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
	else
	{
		
		top = 0;
		mergeY(arrayTmpX, arrayTmpY, left, mid, right,min_dis,arrayX[mid],true);
		//ɸѡ���������ھ�����mindis�ĵ�
		/*for (int i = left; i <= right; i++) {
			if (fabs(arrayTmpX[i] - arrayX[mid]) <= min_dis) {
				X_l[top] = arrayTmpX[i];
				Y_l[top] = arrayTmpY[i];
				top++;
			}
		}*/
		//��ʱY_l����,����ÿһ���㣬ÿ��������ɨ��Y���С��min_dis�ĵ㣬ˢ����С����
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
	//ɸѡ��λ���������ľ�arrayX[mid]С�ڵ���min_dis�ĵ�
	/*for (l = mid; l >= left && arrayX[mid] - arrayX[l] <= min_dis; l--) {
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
	}*/
	for (int i =left; i <= right; i++) {
		if (fabs(arrayX[i] - arrayX[mid])<=min_dis) {
			arrayTmpX[top] = arrayX[i];
			arrayTmpY[top] = arrayY[i];
			top++;
		}
	}
	//�Է��ϵ��Ҳಿ�ֽ�������
	quickSort(arrayTmpY, arrayTmpX, 0, top - 1);

	//�����������ĵ�
	/*for (int i = 0; i < bound; i++) {
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
	}*/
	for (int i = 0; i < top; i++) {
		for (int j = i + 1; j < top&&arrayTmpY[j] - arrayTmpY[i] < min_dis; j++) {
			min_dis = min(min_dis, sqrt((arrayTmpX[i] - arrayTmpX[j])*(arrayTmpX[i] - arrayTmpX[j]) + (arrayTmpY[i] - arrayTmpY[j])*(arrayTmpY[i] - arrayTmpY[j])));
		}
	}
	return min_dis;
}

//���������ģ
double divide1(double* arrayX, double* arrayY, int left, int right) {
	//solve1 nlogn
	if (left < right) {
		int mid = (left + right) / 2;	//�ֳɾ�����ͬ���������������
		double left_minDis = divide1(arrayX, arrayY, left, mid);		//�����
		double right_minDis = divide1(arrayX, arrayY, mid + 1, right);	//�����
		double min_dis = min(left_minDis, right_minDis);		//�Ƚ�����������С����
																//��ϲ��������С���
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
		int mid = (left + right) / 2;	//�ֳɾ�����ͬ���������������
		double left_minDis = divide2(arrayX, arrayY, left, mid);		//�����
		double right_minDis = divide2(arrayX, arrayY, mid + 1, right);	//�����
		double min_dis = min(left_minDis, right_minDis);		//�Ƚ�����������С����
																//��ϲ��������С���
		return merge_solve2(arrayX, arrayY, left, mid, right, min_dis);
	}
}
/*��������С���*/
double bruteforce(double* arrayX, double* arrayY, int left, int right) {
	double min_dis = DBL_MAX;
	//����ÿ�Ե�ľ��룬�����С����
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
	//����csv�ļ����洢��ʱ���
	csvfile.open("C:\\Users\\Administrator\\OneDrive\\�㷨��������\\ʵ��2����������\\analysisRes1.csv");	
	//ͳ�ƹ鲢����ٵ�����ʱ��
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
			//���������
			createPoint(arrayX, arrayY, size);
			//������������
			copyArray(arrayX, arrayX_c, size);
			copyArray(arrayY, arrayY_c, size);

			//slove1�������ƺ�����tmp�������ڹ鲢
			copyArray(arrayX, arrayTmpX, size);
			copyArray(arrayY, arrayTmpY, size);
			quickSort(arrayTmpX, arrayTmpY, 0, size - 1);

			//�鲢����ʱ
			auto startTime_1 = system_clock::now();
			quickSort(arrayX, arrayY, 0, size - 1);
			//tmp���飬���ڹ鲢
			res_1 = divide1(arrayX, arrayY, 0, size - 1);
			auto endTime_1 = system_clock::now();

			//�������ʱ
			auto startTime_2 = system_clock::now();
			res_2 = bruteforce(arrayX_c, arrayY_c, 0, dataSize[i] - 1);
			//res_2 = 0;
			/*quickSort(arrayX_c, arrayY_c, 0, size - 1);
			res_2= divide2(arrayX, arrayY, 0, size - 1);*/
			auto endTime_2 = system_clock::now();

			//ʹ��C++11���¼�ʱ����
			//ԭ����clockֻ�ܾ�ȷ�����뼶�����ͳ�����
			auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
			double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
			runTime_1 *= 1000;	//ת�ɺ���

			auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
			double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
			runTime_2 *= 1000;

			//����������ʱ��
			sumTime_1 += runTime_1;
			sumTime_2 += runTime_2;
			
			std::cout << fixed << setprecision(2) << "���ݹ�ģ[" << size << "][" << j << "] |" << "���η���ʱ��[" << runTime_1 << "]";
			cout << "��ٷ���ʱ��[" << runTime_2 << "] |��С�����������[" << res_1 << "|" << res_2 << "]" << endl;
		}
		cout << endl;

		//д��ͳ�Ʊ��
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

	cout << "------------------------���������ݹ�ģΪ10w-100w�Ĺ鲢�㷨����----------------------------------" << endl;
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
			//���������
			
			//solve1���ڹ鲢��tmp����
			createPoint(arrayX, arrayY, size);

			//�鲢����ʱ
			auto startTime_1 = system_clock::now();
			quickSort(arrayX, arrayY, 0, size - 1);
			//copyArray(arrayX, arrayTmpX, size);
			//copyArray(arrayY, arrayTmpY, size);
			res_1 = divide1(arrayX, arrayY, 0, size - 1);
			auto endTime_1 = system_clock::now();


			auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
			double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
			runTime_1 *= 1000;	//ת�ɺ���
			sumTime_1 += runTime_1;

			cout << fixed << setprecision(2) << "���ݹ�ģ[" << size << "][" << j << "] |" << "���η���ʱ��[" << runTime_1 << "]";
			cout << "| ��С�����������[" << res_1 << "]" << endl;
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
	
	

	//���Ա�ͳ�ƽ��д��csv�ļ�
	csvfile << "[���ݹ�ģ]" << ",";
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
			csvfile << "[�鲢���]"<<",";
		}
		else if(i==1) {
			csvfile << "[������]"<<",";
		}
		else {
			csvfile << "[�鲢���10w-100w]" << ",";
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
