#include<iostream>
#include<stack>
#include<set>
#include<chrono>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<streambuf>
#include<memory.h>
#include<Windows.h>
#include <algorithm>
#include<iomanip>
#include<random>
#define VERTICAL true
#define HORIZON false
#define THREE 1
#define FOUR 4
#define FIVE_ORMORE 10
#define DIRECTION 2
#define MAX_DEPTH 20
using namespace std;
using namespace chrono;
int SCORE[] =  0,0,0,THREE,FOUR,FIVE_ORMORE ;

class Chess 
public:
	int** nodes;	
	int* obj;		
	int M, N, K;	
	int Score;		
	int Depth;		

	
	Chess() :nodes(NULL), M(0), N(0), Score(0), Depth(0) 

	
	Chess(int m, int n,int k,bool isRandom)
		if (isRandom) 
			init_random(m,n,k);
		else
			init(m, n, k);
			input();
		
	

	
	Chess(const Chess& Chess_copy) 
		this->M = Chess_copy.M;
		this->N = Chess_copy.N;
		this->K = Chess_copy.K;
		this->Score = Chess_copy.Score;
		this->Depth = Chess_copy.Depth;
		this->obj = NULL;
		this->nodes = new int*[M + 1];
		for (int i = 0; i < M + 1; i++) 
			nodes[i] = new int[N + 1];
			for (int j = 0; j < N + 1; j++) 
				nodes[i][j] = Chess_copy.nodes[i][j];
			
		
	

	~Chess() 
		for (int i = 0; i <= M; i++)delete[]nodes[i];
		delete[]nodes;
		if(obj!=NULL)
		delete[]obj;
	

	
	Chess& operator=(const Chess& Chess_copy) 
		
		if (this->nodes == NULL) 
			init(Chess_copy.M,Chess_copy.N,Chess_copy.K);
		
		this->Score = Chess_copy.Score;
		this->Depth = Chess_copy.Depth;
		this->obj = NULL;
		for (int i = 0; i < M + 1; i++) 
			for (int j = 0; j < N + 1; j++) 
				nodes[i][j] = Chess_copy.nodes[i][j];
			
		
		return *this;
	

	
	void output() 
		cout << "SIZE:" << M << " " << N << " " << K << endl;
		for (int i = 1; i <= M; i++) 
			for (int j = 1; j <= N; j++) 
				cout << nodes[i][j] << " ";
			
			cout << endl;
		
	

	
	bool canDelete_bySwap(int x, int y,bool method) 
		
		if (method == VERTICAL) 

			
			if (x + 1 > M || nodes[x][y] == nodes[x + 1][y] || nodes[x][y] == 0 || nodes[x + 1][y] == 0)
				return false;
			
			swap(nodes[x][y], nodes[x + 1][y]);
			if (canDelete(x, y) || canDelete(x + 1, y)) 
				
				swap(nodes[x][y], nodes[x + 1][y]);
				return true;
			
			swap(nodes[x][y], nodes[x + 1][y]);	
		

		
		if (method == HORIZON) 

			if (y + 1 > N || nodes[x][y] == nodes[x][y + 1] || nodes[x][y] == 0 || nodes[x][y + 1] == 0)
				return false;
			swap(nodes[x][y], nodes[x][y + 1]);
			if (canDelete(x, y)||canDelete(x, y + 1)) 
				
				swap(nodes[x][y], nodes[x][y + 1]);
				return true;
			
			swap(nodes[x][y], nodes[x][y + 1]);
		
		return false;
	

	
	int deleteConnect() 
		int Score = 0;
		int score_onestep = 0;
		while ((score_onestep = delete_connect()) != 0) 
			Score += score_onestep;
		
		return Score;
	

	
	void exchange(int i,int j,bool method) 
		if (method == VERTICAL) 
			swap(nodes[i][j], nodes[i + 1][j]);
		
		if (method == HORIZON) 
			swap(nodes[i][j], nodes[i][j + 1]);
		
	

private:
	
	void padding() 
		for (int i = 0; i <= M; i++)nodes[i][0] = -1;
		for (int i = 0; i <= N; i++)nodes[0][i] = -1;
	

	
	void init(int m, int n,int k) 
		this->M = m; this->N = n; this->K = k;
		Score = 0; Depth = 0;
		obj = new int[k];
		for (int i = 0; i < k; i++)obj[i] = i + 1;
		nodes = new int*[M + 1];
		for (int i = 0; i < M + 1; i++) 
			nodes[i] = new int[N + 1];
		
		padding();
	

	
	void init_random(int m,int n,int k) 
		init(m, n, k);
		while (!generate_randomMatrix(0)) 
		
	

	
	bool generate_randomMatrix(int index) 
		if (index >= M * N) 
			return true;
		
		int x = index / N + 1;
		int y = index % N + 1;
		

		
		

		
		static std::random_device rd;
		static default_random_engine e(rd());
		shuffle(obj, obj + K, e);

		for (int i = 0; i < K; i++) 
			nodes[x][y] = obj[i];
			
			if ((y < 3 || !(y >= 3 && (nodes[x][y] == nodes[x][y - 1] && nodes[x][y - 1] == nodes[x][y - 2]))) &&
				((x < 3 || !(x >= 3 && (nodes[x][y] == nodes[x - 1][y] && nodes[x - 1][y] == nodes[x - 2][y]))))) 
				if (generate_randomMatrix(index + 1)) 
					return true;
				
			
		
		return false;
	

	
	void input() 
		for (int i = 1; i <= M; i++) 
			for (int j = 1; j <= N; j++) 
				cin >> nodes[i][j];
			
		
	

	
	bool canDelete(int x, int y) 
		int value = nodes[x][y];

		int cnt = 1;
		
		for (int y_c = y - 1; y_c >= 1 && nodes[x][y_c] == value; y_c--)cnt++;
		for (int y_c = y + 1; y_c <= N && nodes[x][y_c] == value; y_c++)cnt++;

		if (cnt >= 3)
			return true;

		
		cnt = 1;
		for (int x_c = x - 1; x_c >= 1 && nodes[x_c][y] == value; x_c--)cnt++;
		for (int x_c = x + 1; x_c <= M && nodes[x_c][y] == value; x_c++)cnt++;

		if (cnt >= 3)
			return true;

		return false;
	

	
	void flow(bool** will_del) 
		
		for (int j = 1; j <= N; j++) 
			int index = M;
			for (int i = M; i >= 1; i--) 
				
				if (!will_del[i][j] && nodes[i][j] != 0) 
					nodes[index--][j] = nodes[i][j];
				
			
			
			for (; index >= 1; index--)nodes[index][j] = 0;
		
		
		for (int i = 0; i <= M; i++)delete[]will_del[i];
		delete[]will_del;
	

	
	int delete_connect() 
		bool** will_del;	
		will_del = new bool*[M + 1];
		for (int i = 0; i <= M; i++) 
			will_del[i] = new bool[N + 1];
			for (int j = 1; j <= N; j++) 
				will_del[i][j] = false;		
			
		
		int nowObj = 0;	
		int Score = 0;	
		int equal_num = 0;	

		
		for (int i = 1; i <= M; i++) 
			equal_num = 0;	
			for (int j = 1; j <= N; j++) 
				nowObj = nodes[i][j];
				
				if (nowObj == 0) 
					
					if (equal_num >= 3) 

						
						for (int y_c = j - 1; y_c >= 1 && y_c >= j - equal_num; y_c--)will_del[i][y_c] = true;

						
						if (equal_num <= 4)Score += SCORE[equal_num];
						else Score += FIVE_ORMORE;
					
					equal_num = 0;
				
				else 
					
					if (nowObj == nodes[i][j - 1])equal_num++;
					else 
						
						if (equal_num >= 3) 
							
							for (int y_c = j - 1; y_c >= 1 && y_c >= j - equal_num; y_c--)will_del[i][y_c] = true;

							
							if (equal_num <= 4)Score += SCORE[equal_num];
							else Score += FIVE_ORMORE;
						
						equal_num = 1;
					
				
			
			
			if (equal_num >= 3) 
				for (int y_c = N; y_c >= 1 && y_c >= N - equal_num + 1; y_c--)will_del[i][y_c] = true;

				if (equal_num <= 4)Score += SCORE[equal_num];
				else Score += FIVE_ORMORE;
			
		

		
		for (int j = 1; j <= N; j++) 
			equal_num = 0;	
			for (int i = 1; i <= M; i++) 
				nowObj = nodes[i][j];
				
				if (nowObj == 0) 
					
					if (equal_num >= 3) 
						
						for (int x_c = i - 1; x_c >= 1 && x_c >= i - equal_num; x_c--)will_del[x_c][j] = true;

						
						if (equal_num <= 4)Score += SCORE[equal_num];
						else Score += FIVE_ORMORE;
					
					equal_num = 0;	
				
				else 
					
					if (nowObj == nodes[i - 1][j])equal_num++;
					else 
						
						if (equal_num >= 3) 
							
							for (int x_c = i - 1; x_c >= 1 && x_c >= i - equal_num; x_c--)will_del[x_c][j] = true;

							
							if (equal_num <= 4)Score += SCORE[equal_num];
							else Score += FIVE_ORMORE;
						
						equal_num = 1;  
					

				
			
			
			if (equal_num >= 3) 
				for (int x_c = M; x_c >= 1 && x_c >= M - equal_num + 1; x_c--)will_del[x_c][j] = true;

				if (equal_num <= 4)Score += SCORE[equal_num];
				else Score += FIVE_ORMORE;
			
		
		
		flow(will_del);
		
		return Score;
	

;


struct swap_op 
	int x_1, y_1;
	int x_2, y_2;
	int Score;
	swap_op() 
	swap_op(int x_1, int y_1, int x_2, int y_2, int Score) 
		this->x_1 = x_1; this->y_1 = y_1;
		this->x_2 = x_2; this->y_2 = y_2;
		this->Score = Score;
	
	swap_op(int x_1, int y_1,bool method, int Score) 
		this->x_1 = x_1; this->y_1 = y_1;
		if (method == VERTICAL) 
			this->x_2 = x_1 + 1; this->y_2 = y_1;
		
		else 
			this->x_2 = x_1; this->y_2 = y_1 + 1;
		
		this->Score = Score;
	
;

class Paths 
public:
	stack<swap_op> opStack;
	void output_bestSolve(Chess init_chess, int max_Score) 

		stack<swap_op> tmp = opStack;
		stack<swap_op> path;
		while (!tmp.empty()) 
			path.push(tmp.top());
			tmp.pop();
		
		cout << "初始化棋盘为：" << endl;
		init_chess.output();

		int stepNum = 0;
		while (!path.empty()) 
			stepNum++;
			swap_op nowStep = path.top();
			path.pop();

			cout << "通过交换:[" << nowStep.x_1 << "," << nowStep.y_1 << "]-[" << nowStep.x_2 << "," << nowStep.y_2 << "] 可获：" << nowStep.Score << "分" << endl;
			cout << "消去连通块并使元素下落后，棋盘为：" << endl;
			swap(init_chess.nodes[nowStep.x_1][nowStep.y_1], init_chess.nodes[nowStep.x_2][nowStep.y_2]);

			init_chess.deleteConnect();
			init_chess.output();
		
		cout << "交换[" << stepNum << "]步,最高得分为：" << max_Score << "分" << endl << endl;
	
	void pushOp(swap_op op) 
		opStack.push(op);
	

	void popOp() 
		opStack.pop();
	

	void clearOp() 
		while (!opStack.empty())opStack.pop();
	
;

int X;
Paths currentPath;		
Paths maxScore_Path;	
int maxScore;			
int crossLevel_num[MAX_DEPTH];		
int levelSum[MAX_DEPTH];			


void Solve_of_oneStep(Chess init_chess) 
	Paths path;
	path.pushOp(swap_op(0, 0, 0, 0, 0));	
	int maxScore = -1;
	for (int i = 1; i <= init_chess.M; i++) 
		for (int j = 1; j <= init_chess.N; j++) 
			int score_hang, score_lie;
			score_hang = score_lie = 0;
			int nowScore;	

			
			if (init_chess.canDelete_bySwap(i, j, VERTICAL)) 
				Chess tmp_lie = init_chess;		
				swap(tmp_lie.nodes[i][j], tmp_lie.nodes[i + 1][j]);		
				
				while ((nowScore = tmp_lie.deleteConnect()) != 0) 
					score_lie += nowScore;
				
			
			
			if (init_chess.canDelete_bySwap(i, j, HORIZON)) 
				Chess tmp_hang = init_chess;	
				swap(tmp_hang.nodes[i][j], tmp_hang.nodes[i][j + 1]);	
				
				
				while ((nowScore = tmp_hang.deleteConnect()) != 0) 
					score_hang += nowScore;
				
			
			
			if (!score_hang)score_hang--;
			if (!score_lie)score_lie--;

			
			if (score_hang == -1 && score_lie == -1)continue;


			
			nowScore = score_hang > score_hang ? score_hang : score_lie;
			int hang_flag = 1;		
			if (score_hang < score_lie)hang_flag = 0;


			
			if (hang_flag) 
				if (maxScore < score_hang) 
					maxScore = score_hang;
					path.popOp();
					path.pushOp(swap_op(i, j, i, j + 1, maxScore));
				
			
			else 
				if (maxScore < score_lie) 
					maxScore = score_lie;
					path.popOp();
					path.pushOp(swap_op(i, j, i + 1, j, maxScore));
				
			
		
	
	path.output_bestSolve(init_chess, maxScore);




void backTracing(Chess& chess, int score, int depth) 

	
	if (score > maxScore) 
		maxScore_Path = currentPath;
		maxScore = score;
	

	
	if (depth > X)return;

	
	for (int dir = 0; dir < DIRECTION ; dir++) 

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	

		
		for (int i = 1; i <= chess.M; i++) 
			for (int j = 1; j <= chess.N; j++) 

				
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) 
					Chess tmpchess = chess;					
					tmpchess.exchange(i, j, Swap_Dir);		
					int nowScore = tmpchess.deleteConnect();	


					if (!nowScore)continue;		
					currentPath.pushOp(swap_op(i, j,Swap_Dir, nowScore));	
					backTracing(tmpchess, score + nowScore, depth + 1);		
					currentPath.popOp();	
				
			
		
	
	

int Solve1_of_XiaoxiaoLe(Chess init_chess) 
	
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	backTracing(init_chess, 0, 1);
	return maxScore;



void backTracing_depth(Chess& chess, int score, int depth,double parms) 

	
	if (score > maxScore) 
		maxScore_Path = currentPath;
		maxScore = score;
	

	
	if (depth > X)return;

	
	double averageScore = maxScore * 1.0 / X;
	int level_left = X - depth + 1;
	double averScore_nextlevels = (maxScore - score)*1.0 / level_left;
	if (averScore_nextlevels > averageScore*parms)
		return;

	
	for (int dir = 0; dir < DIRECTION; dir++) 

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	

														
		for (int i = 1; i <= chess.M; i++) 
			for (int j = 1; j <= chess.N; j++) 

				
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) 
					Chess tmpchess = chess;					
					tmpchess.exchange(i, j, Swap_Dir);		
					int nowScore = tmpchess.deleteConnect();	


					if (!nowScore)continue;		
					currentPath.pushOp(swap_op(i, j, Swap_Dir, nowScore));	
					backTracing_depth(tmpchess, score + nowScore, depth + 1,parms);		
					currentPath.popOp();	
				
			
		
	

int Solve2_reduceDepth(Chess init_chess,double parms) 
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	backTracing_depth(init_chess, 0, 1, parms);
	return maxScore;




void backTracing_width(Chess& chess, int score, int depth, int maxWidth) 

	
	if (score > maxScore) 
		maxScore_Path = currentPath;
		maxScore = score;
	

	
	if (depth > X)return;

	
	crossLevel_num[depth]++;
	levelSum[depth] += score;
	if (crossLevel_num[depth] >= maxWidth) 
		double averSum = levelSum[depth] * 1.0 / crossLevel_num[depth];
		if (score < averSum)return;
	

	
	for (int dir = 0; dir < DIRECTION; dir++) 

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	

														
		for (int i = 1; i <= chess.M; i++) 
			for (int j = 1; j <= chess.N; j++) 

				
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) 
					Chess tmpchess = chess;					
					tmpchess.exchange(i, j, Swap_Dir);		
					int nowScore = tmpchess.deleteConnect();	


					if (!nowScore)continue;		
					currentPath.pushOp(swap_op(i, j, Swap_Dir, nowScore));	
					backTracing_width(tmpchess, score + nowScore, depth + 1, maxWidth);		
					currentPath.popOp();	
				
			
		
	

int Solve3_reduceWidth(Chess init_chess, int maxWidth) 
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	memset(crossLevel_num, 0, sizeof(MAX_DEPTH));
	memset(levelSum, 0, sizeof(MAX_DEPTH));
	backTracing_width(init_chess, 0, 1, maxWidth);
	return maxScore;



void backTracing_combine(Chess& chess, int score, int depth, int maxWidth,double parms) 

	
	if (score > maxScore) 
		maxScore_Path = currentPath;
		maxScore = score;
	

	
	if (depth > X)return;

	
	crossLevel_num[depth]++;
	levelSum[depth] += score;
	if (crossLevel_num[depth] >= maxWidth) 
		double averSum = levelSum[depth] * 1.0 / crossLevel_num[depth];
		if (score < averSum)return;
	

	
	double averageScore = maxScore * 1.0 / X;
	int level_left = X - depth + 1;
	double averScore_nextlevels = (maxScore - score)*1.0 / level_left;
	if (averScore_nextlevels > averageScore*parms)
		return;


	
	for (int dir = 0; dir < DIRECTION; dir++) 

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	

														
		for (int i = 1; i <= chess.M; i++) 
			for (int j = 1; j <= chess.N; j++) 

				
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) 
					Chess tmpchess = chess;					
					tmpchess.exchange(i, j, Swap_Dir);		
					int nowScore = tmpchess.deleteConnect();	


					if (!nowScore)continue;		
					currentPath.pushOp(swap_op(i, j, Swap_Dir, nowScore));	
					backTracing_combine(tmpchess, score + nowScore, depth + 1, maxWidth,parms);		
					currentPath.popOp();	
				
			
		
	

int Solve4_combine(Chess init_chess, int maxWidth,double parms) 
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	memset(crossLevel_num, 0, sizeof(MAX_DEPTH));
	memset(levelSum, 0, sizeof(MAX_DEPTH));
	backTracing_combine(init_chess, 0, 1, maxWidth,parms);
	return maxScore;




void write(double table[][7], int* data, int* x,int data_len,int x_len,double p,ofstream& csvfile) 
	csvfile << "parms = " << p << endl;
	csvfile << "交换步数/规模" << ",";
	for (int i = 1; i <= data_len; i++) 
		csvfile << data[i] << ",";
	
	csvfile << endl;
	for (int i = 1; i <= x_len; i++) 
		for (int j = 1; j <= data_len; j++) 
			if (j == 1)csvfile << x[i] << ",";
			csvfile << table[i][j] << ",";
		
		csvfile << endl;
	
	csvfile << endl;


void run(int M,int N,int K,int x) 
	X = x;
	Chess test(M, N, K, true);
	auto startTime_1 = system_clock::now();
	int ans1 = Solve1_of_XiaoxiaoLe(test);
	auto endTime_1 = system_clock::now();
	auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
	double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
	runTime_1 *= 1000;	
	cout << runTime_1 << endl;
	cout << ans1 << endl;
	

	auto startTime_2 = system_clock::now();
	int ans2 = Solve2_reduceDepth(test, 1.3);
	auto endTime_2 = system_clock::now();
	auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
	double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
	runTime_2 *= 1000;	
	cout << runTime_2 << endl;
	cout << ans2 << endl;
	

	auto startTime_3 = system_clock::now();
	int ans3 = Solve3_reduceWidth(test, 12);
	auto endTime_3 = system_clock::now();
	auto duration_3 = duration_cast<microseconds>(endTime_3 - startTime_3);
	double runTime_3 = double(duration_3.count()) * microseconds::period::num / microseconds::period::den;
	runTime_3 *= 1000;	
	cout << runTime_3 << endl;
	cout << ans3 << endl;
	

	auto startTime_4 = system_clock::now();
	int ans4 = Solve4_combine(test, 12, 1.3);
	auto endTime_4 = system_clock::now();
	auto duration_4 = duration_cast<microseconds>(endTime_4 - startTime_4);
	double runTime_4 = double(duration_4.count()) * microseconds::period::num / microseconds::period::den;
	runTime_4 *= 1000;
	cout << runTime_4 << endl;
	cout << ans4 << endl;
	cout << endl;

int main() 

	
	

	int m, n, k, x;
	cin >> n >> m>> k >> x;
	X = x;
	Chess test(m, n, k, false);
	auto startTime_1 = system_clock::now();
	int ans1=Solve1_of_XiaoxiaoLe(test);
	auto endTime_1 = system_clock::now();
	auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
	double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
	runTime_1 *= 1000;	
	cout << runTime_1 << endl;
	cout << ans1 << endl << endl;
	maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_2 = system_clock::now();
	int ans2 = Solve2_reduceDepth(test, 1.3);
	auto endTime_2 = system_clock::now();
	auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
	double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
	runTime_2 *= 1000;	
	cout << runTime_2 << endl;
	cout << ans2 << endl << endl;
	maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_3 = system_clock::now();
	int ans3 = Solve3_reduceWidth(test,12);
	auto endTime_3 = system_clock::now();
	auto duration_3 = duration_cast<microseconds>(endTime_3 - startTime_3);
	double runTime_3 = double(duration_3.count()) * microseconds::period::num / microseconds::period::den;
	runTime_3 *= 1000;	
	cout << runTime_3 << endl;
	cout << ans3 << endl << endl;
	maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_4 = system_clock::now();
	int ans4 = Solve4_combine(test, 12, 1.3);
	auto endTime_4 = system_clock::now();
	auto duration_4 = duration_cast<microseconds>(endTime_4 - startTime_4);
	double runTime_4 = double(duration_4.count()) * microseconds::period::num / microseconds::period::den;
	runTime_4 *= 1000;
	cout << runTime_4 << endl;
	cout << ans4 << endl << endl;

	cout << "ans1:" << ans1 << " |time1:" << runTime_1 << endl;
	cout << "ans2:" << ans1 << " |time2:" << runTime_2 << endl;
	cout << "ans3:" << ans1 << " |time3:" << runTime_3 << endl;
	cout << "ans4:" << ans1 << " |time4:" << runTime_4 << endl;
	

	
	
	int TEST_NUM = 10;
	int data_len = 6;
	int x_len = 4;
	int dataSize[] =  0,5,6,7,8,9,10 ;
	int X_size[] =  0,1,2,3,4;
	double time_rate[5][7];
	double ans_rate[5][7];
	double total_weight[5][7];
	double parms = 1.0;
	double add = 0.1;

	
	for (int k = 1; k <= TEST_NUM; k++) 
		double parms_c = parms + add * k;

		for (int i = 1; i <= data_len; i++) 

			Chess init_chess(dataSize[i], dataSize[i], 3, true);

			for (int j = 1; j <= x_len; j++) 
				X = X_size[j];

				auto startTime_1 = system_clock::now();
				int ans_1 = Solve1_of_XiaoxiaoLe(init_chess);
				auto endTime_1 = system_clock::now();

				auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
				double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
				runTime_1 *= 1000;	


				auto startTime_2 = system_clock::now();
				int ans_2 = Solve2_reduceDepth(init_chess, parms_c);
				auto endTime_2 = system_clock::now();

				auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
				double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
				runTime_2 *= 1000;	

				time_rate[j][i] = runTime_2 / runTime_1;
				ans_rate[j][i] = ans_2 * 1.0 / ans_1;;
				total_weight[j][i] = time_rate[j][i] / ans_rate[j][i];

				cout << fixed << setprecision(2);
				cout << "SIZE " << dataSize[i] << " |DEPTH " << X << " |Parms " << parms_c << "" << endl;
				cout << "回溯法耗时：" << runTime_1 << "" << " |最高分数：" << ans_1 <<" "<< endl;
				cout << "回溯法(剪枝调参）耗时：" << runTime_2 << "" << " |最高分数：" << ans_2 << " " << endl;
			
		
		write(time_rate, dataSize, X_size, data_len, x_len, parms_c, csvfile);
		write(ans_rate, dataSize, X_size, data_len, x_len, parms_c, csvfile);
		write(total_weight, dataSize, X_size, data_len, x_len, parms_c, csvfile);
	


	
	int chess_size = 10;
	int T = 10;
	int width[] =  0,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30;
	X = 5;
	double time_rate1[27] =  0 ;
	double ans_rate1[27] =  0 ;
	while (T--) 

		Chess init_chess(10, 6, 3, true);
		auto startTime_1 = system_clock::now();
		int ans_1 = Solve1_of_XiaoxiaoLe(init_chess);
		auto endTime_1 = system_clock::now();

		auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
		double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
		runTime_1 *= 1000;	

		for (int i = 1; i <= 26; i++) 
			auto startTime_2 = system_clock::now();
			int ans_2 = Solve3_reduceWidth(init_chess, width[i]);
			auto endTime_2 = system_clock::now();

			auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
			double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
			runTime_2 *= 1000;	

			time_rate1[i] += runTime_2 / runTime_1;
			ans_rate1[i] += ans_2 * 1.0 / ans_1;

			cout << fixed << setprecision(2);
			cout << "SIZE 10 * 6 |DEPTH 5 |width " << width[i] << "" << endl;
			cout << "回溯法耗时：" << runTime_1 << "" << " |最高分数：" << ans_1 << " " << endl;
			cout << "回溯法(剪枝调参）耗时：" << runTime_2 << "" << " |最高分数：" << ans_2 << " " << endl;
		
	

	csvfile << "树宽规模" << ",";
	for (int i = 1; i <= 26; i++) 
		csvfile << width[i] << ",";
	
	csvfile << endl;
	for (int i = 1; i <= 26; i++) 
		if (i == 1)csvfile << "效率比" << ",";
		csvfile << time_rate1[i] / 10 << ",";
	
	csvfile << endl;
	for (int i = 1; i <= 26; i++) 
		if (i == 1)csvfile << "准确率" << ",";
		csvfile << ans_rate1[i] / 10 << ",";
	
	csvfile << endl;


