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
int SCORE[] = { 0,0,0,THREE,FOUR,FIVE_ORMORE };

class Chess {
public:
	int** nodes;	//二维棋盘
	int* obj;		//棋子向量
	int M, N, K;	//行 列 棋子种类
	int Score;		//分数
	int Depth;		//dfs深度

	// 默认构造
	Chess() :nodes(NULL), M(0), N(0), Score(0), Depth(0) {}

	/*初始化构造函数，可指定随机棋盘和读入棋盘*/
	Chess(int m, int n,int k,bool isRandom){
		if (isRandom) {
			init_random(m,n,k);
		}else{
			init(m, n, k);
			input();
		}
	}

	/*拷贝构造函数*/
	Chess(const Chess& Chess_copy) {
		this->M = Chess_copy.M;
		this->N = Chess_copy.N;
		this->K = Chess_copy.K;
		this->Score = Chess_copy.Score;
		this->Depth = Chess_copy.Depth;
		this->obj = NULL;
		this->nodes = new int*[M + 1];
		for (int i = 0; i < M + 1; i++) {
			nodes[i] = new int[N + 1];
			for (int j = 0; j < N + 1; j++) {
				nodes[i][j] = Chess_copy.nodes[i][j];
			}
		}
	}

	~Chess() {
		for (int i = 0; i <= M; i++)delete[]nodes[i];
		delete[]nodes;
		if(obj!=NULL)
		delete[]obj;
	}

	/*重载赋值运算符*/
	Chess& operator=(const Chess& Chess_copy) {
		//没有开空间，先开空间
		if (this->nodes == NULL) {
			init(Chess_copy.M,Chess_copy.N,Chess_copy.K);
		}
		this->Score = Chess_copy.Score;
		this->Depth = Chess_copy.Depth;
		this->obj = NULL;
		for (int i = 0; i < M + 1; i++) {
			for (int j = 0; j < N + 1; j++) {
				nodes[i][j] = Chess_copy.nodes[i][j];
			}
		}
		return *this;
	}

	//输出棋盘
	void output() {
		cout << "SIZE:" << M << " " << N << " " << K << endl;
		for (int i = 1; i <= M; i++) {
			for (int j = 1; j <= N; j++) {
				cout << nodes[i][j] << " ";
			}
			cout << endl;
		}
	}

	/*检查通过指定点与相邻交换后，是否具备消去条件*/
	bool canDelete_bySwap(int x, int y,bool method) {
		// swap (x,y) and (x+1,y)
		if (method == VERTICAL) {

			// 越界或者将交换的两个元素相等或者存在其中一个位置已经为空
			if (x + 1 > M || nodes[x][y] == nodes[x + 1][y] || nodes[x][y] == 0 || nodes[x + 1][y] == 0)
				return false;
			//试探性交换，检查是否存在消除情况
			swap(nodes[x][y], nodes[x + 1][y]);
			if (canDelete(x, y) || canDelete(x + 1, y)) {
				//重置
				swap(nodes[x][y], nodes[x + 1][y]);
				return true;
			}
			swap(nodes[x][y], nodes[x + 1][y]);	//重置
		}

		// swap (x,y) and (x,y+1)
		if (method == HORIZON) {

			if (y + 1 > N || nodes[x][y] == nodes[x][y + 1] || nodes[x][y] == 0 || nodes[x][y + 1] == 0)
				return false;
			swap(nodes[x][y], nodes[x][y + 1]);
			if (canDelete(x, y)||canDelete(x, y + 1)) {
				//重置
				swap(nodes[x][y], nodes[x][y + 1]);
				return true;
			}
			swap(nodes[x][y], nodes[x][y + 1]);
		}
		return false;
	}

	//循环消去连通块，直到不再具有消去的情况，返回得分
	int deleteConnect() {
		int Score = 0;
		int score_onestep = 0;
		while ((score_onestep = delete_connect()) != 0) {
			Score += score_onestep;
		}
		return Score;
	}

	/*  VERTICAL:swap(<i,j>,<i + 1,j>)
		HORIZON :swap(<i,j>,<i,j + 1>)  */
	void exchange(int i,int j,bool method) {
		if (method == VERTICAL) {
			swap(nodes[i][j], nodes[i + 1][j]);
		}
		if (method == HORIZON) {
			swap(nodes[i][j], nodes[i][j + 1]);
		}
	}

private:
	/*边界填充-1函数*/
	void padding() {
		for (int i = 0; i <= M; i++)nodes[i][0] = -1;
		for (int i = 0; i <= N; i++)nodes[0][i] = -1;
	}

	/*初始化函数，由构造函数调用*/
	void init(int m, int n,int k) {
		this->M = m; this->N = n; this->K = k;
		Score = 0; Depth = 0;
		obj = new int[k];
		for (int i = 0; i < k; i++)obj[i] = i + 1;
		nodes = new int*[M + 1];
		for (int i = 0; i < M + 1; i++) {
			nodes[i] = new int[N + 1];
		}
		padding();
	}

	/*初始化函数，生成随机棋盘*/
	void init_random(int m,int n,int k) {
		init(m, n, k);
		while (!generate_randomMatrix(0)) {}
		//cout << "ok" << endl;
	}

	/*通过DFS生成无消除状态的随机棋盘*/
	bool generate_randomMatrix(int index) {
		if (index >= M * N) {
			return true;
		}
		int x = index / N + 1;
		int y = index % N + 1;
		/*for (int i = 0; i < K; i++) {
			obj[i] = i + 1;
		}*/

		/*unsigned seed = chrono::system_clock::now().time_since_epoch().count();*/
		//shuffle(obj, obj + K, std::default_random_engine());

		//声明成静态，防止多次迭代局部随机种子导致程序崩溃
		static std::random_device rd;
		static default_random_engine e(rd());
		shuffle(obj, obj + K, e);

		for (int i = 0; i < K; i++) {
			nodes[x][y] = obj[i];
			//在保证无消除状态下进入下层放子
			if ((y < 3 || !(y >= 3 && (nodes[x][y] == nodes[x][y - 1] && nodes[x][y - 1] == nodes[x][y - 2]))) &&
				((x < 3 || !(x >= 3 && (nodes[x][y] == nodes[x - 1][y] && nodes[x - 1][y] == nodes[x - 2][y]))))) {
				if (generate_randomMatrix(index + 1)) {
					return true;
				}
			}
		}
		return false;
	}

	//输入棋盘
	void input() {
		for (int i = 1; i <= M; i++) {
			for (int j = 1; j <= N; j++) {
				cin >> nodes[i][j];
			}
		}
	}

	/*检查指定点是否为消去状态*/
	bool canDelete(int x, int y) {
		int value = nodes[x][y];

		int cnt = 1;
		// 行消去检查
		for (int y_c = y - 1; y_c >= 1 && nodes[x][y_c] == value; y_c--)cnt++;
		for (int y_c = y + 1; y_c <= N && nodes[x][y_c] == value; y_c++)cnt++;

		if (cnt >= 3)
			return true;

		// 列消去检查
		cnt = 1;
		for (int x_c = x - 1; x_c >= 1 && nodes[x_c][y] == value; x_c--)cnt++;
		for (int x_c = x + 1; x_c <= M && nodes[x_c][y] == value; x_c++)cnt++;

		if (cnt >= 3)
			return true;

		return false;
	}

	//元素下落，在消去相连元素后调用
	void flow(bool** will_del) {
		/*true need to delete*/
		for (int j = 1; j <= N; j++) {
			int index = M;
			for (int i = M; i >= 1; i--) {
				/*
					index会停留在需要清除的元素位置上
					便于上方元素下降覆盖
					最终落在该列没被清除方块的最上方
				*/
				if (!will_del[i][j] && nodes[i][j] != 0) {
					nodes[index--][j] = nodes[i][j];
				}
			}
			//将空位置填充0
			for (; index >= 1; index--)nodes[index][j] = 0;
		}
		//释放空间
		for (int i = 0; i <= M; i++)delete[]will_del[i];
		delete[]will_del;
	}

	//消除连接块，记录本次消去所得分数并返回
	int delete_connect() {
		bool** will_del;	//标记矩阵，true表示该元素需删除
		will_del = new bool*[M + 1];
		for (int i = 0; i <= M; i++) {
			will_del[i] = new bool[N + 1];
			for (int j = 1; j <= N; j++) {
				will_del[i][j] = false;		//init
			}
		}
		int nowObj = 0;	//当前元素
		int Score = 0;	//分数
		int equal_num = 0;	//行/列所相等的元素

		/*消去行连接块*/
		for (int i = 1; i <= M; i++) {
			equal_num = 0;	//换行，计数器置0
			for (int j = 1; j <= N; j++) {
				nowObj = nodes[i][j];
				//踩到空元素区
				if (nowObj == 0) {
					//满足消去条件
					if (equal_num >= 3) {

						//更新消去块的标记值，设定为需消去
						for (int y_c = j - 1; y_c >= 1 && y_c >= j - equal_num; y_c--)will_del[i][y_c] = true;

						//更新分数
						if (equal_num <= 4)Score += SCORE[equal_num];
						else Score += FIVE_ORMORE;
					}
					equal_num = 0;//由于踩空，同行计数器置0
				}
				else {
					//当前元素等于上一个元素，计数
					if (nowObj == nodes[i][j - 1])equal_num++;
					else {
						/*
							踩到不相同的元素
							若上一状态达到消去条件
						*/
						if (equal_num >= 3) {
							//更新消去块的标记值，设定为需消去
							for (int y_c = j - 1; y_c >= 1 && y_c >= j - equal_num; y_c--)will_del[i][y_c] = true;

							//更新分数
							if (equal_num <= 4)Score += SCORE[equal_num];
							else Score += FIVE_ORMORE;
						}
						equal_num = 1;//非踩空，计数器从当前元素开始计数，置1
					}
				}
			}
			//特判边缘
			if (equal_num >= 3) {
				for (int y_c = N; y_c >= 1 && y_c >= N - equal_num + 1; y_c--)will_del[i][y_c] = true;

				if (equal_num <= 4)Score += SCORE[equal_num];
				else Score += FIVE_ORMORE;
			}
		}

		/*消去列连接块*/
		for (int j = 1; j <= N; j++) {
			equal_num = 0;	//换列，计数器置0
			for (int i = 1; i <= M; i++) {
				nowObj = nodes[i][j];
				/*踩空*/
				if (nowObj == 0) {
					//满足消去条件
					if (equal_num >= 3) {
						//更新消去块的标记值，设定为需消去
						for (int x_c = i - 1; x_c >= 1 && x_c >= i - equal_num; x_c--)will_del[x_c][j] = true;

						//update score
						if (equal_num <= 4)Score += SCORE[equal_num];
						else Score += FIVE_ORMORE;
					}
					equal_num = 0;	//set zero for null obj
				}
				else {
					//当前元素等于上一个元素，计数
					if (nowObj == nodes[i - 1][j])equal_num++;
					else {
						/*
							踩到不相同的元素
							若上一状态达到消去条件
						*/
						if (equal_num >= 3) {
							//更新消去块的标记值，设定为需消去
							for (int x_c = i - 1; x_c >= 1 && x_c >= i - equal_num; x_c--)will_del[x_c][j] = true;

							//update score
							if (equal_num <= 4)Score += SCORE[equal_num];
							else Score += FIVE_ORMORE;
						}
						equal_num = 1;  //非踩空，计数器从当前元素开始计数，置1
					}

				}
			}
			//特判边缘
			if (equal_num >= 3) {
				for (int x_c = M; x_c >= 1 && x_c >= M - equal_num + 1; x_c--)will_del[x_c][j] = true;

				if (equal_num <= 4)Score += SCORE[equal_num];
				else Score += FIVE_ORMORE;
			}
		}
		/*for (int i = 1; i <= M; i++) {
			for (int j = 1; j <= N; j++) {
				cout << will_del[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;*/
		flow(will_del);
		//output();
		return Score;
	}

};

/*记录交换步*/
struct swap_op {
	int x_1, y_1;
	int x_2, y_2;
	int Score;
	swap_op() {}
	swap_op(int x_1, int y_1, int x_2, int y_2, int Score) {
		this->x_1 = x_1; this->y_1 = y_1;
		this->x_2 = x_2; this->y_2 = y_2;
		this->Score = Score;
	}
	swap_op(int x_1, int y_1,bool method, int Score) {
		this->x_1 = x_1; this->y_1 = y_1;
		if (method == VERTICAL) {
			this->x_2 = x_1 + 1; this->y_2 = y_1;
		}
		else {
			this->x_2 = x_1; this->y_2 = y_1 + 1;
		}
		this->Score = Score;
	}
};
/*	路径类
	以栈结构存储路径	*/
class Paths {
public:
	stack<swap_op> opStack;
	void output_bestSolve(Chess init_chess, int max_Score) {

		stack<swap_op> tmp = opStack;
		stack<swap_op> path;
		while (!tmp.empty()) {
			path.push(tmp.top());
			tmp.pop();
		}
		cout << "初始化棋盘为：" << endl;
		init_chess.output();

		int stepNum = 0;
		while (!path.empty()) {
			stepNum++;
			swap_op nowStep = path.top();
			path.pop();

			cout << "通过交换:[" << nowStep.x_1 << "," << nowStep.y_1 << "]-[" << nowStep.x_2 << "," << nowStep.y_2 << "] 可获：{" << nowStep.Score << "}分" << endl;
			cout << "消去连通块并使元素下落后，棋盘为：" << endl;
			swap(init_chess.nodes[nowStep.x_1][nowStep.y_1], init_chess.nodes[nowStep.x_2][nowStep.y_2]);

			init_chess.deleteConnect();
			init_chess.output();
		}
		cout << "交换[" << stepNum << "]步,最高得分为：{" << max_Score << "}分" << endl << endl;
	}
	void pushOp(swap_op op) {
		opStack.push(op);
	}

	void popOp() {
		opStack.pop();
	}

	void clearOp() {
		while (!opStack.empty())opStack.pop();
	}
};

int X;
Paths currentPath;		//记录当前路径
Paths maxScore_Path;	//记录最佳路径
int maxScore;			//最佳分数
int crossLevel_num[MAX_DEPTH];		//记录当前层次的遍历次数
int levelSum[MAX_DEPTH];			//当前层的

/*一步交换*/
void Solve_of_oneStep(Chess init_chess) {
	Paths path;
	path.pushOp(swap_op(0, 0, 0, 0, 0));	//填充空值
	int maxScore = -1;
	for (int i = 1; i <= init_chess.M; i++) {
		for (int j = 1; j <= init_chess.N; j++) {
			int score_hang, score_lie;
			score_hang = score_lie = 0;
			int nowScore;	//消去某相连块获得的分数

			/*垂直交换可消去*/
			if (init_chess.canDelete_bySwap(i, j, VERTICAL)) {
				Chess tmp_lie = init_chess;		//备份棋盘
				swap(tmp_lie.nodes[i][j], tmp_lie.nodes[i + 1][j]);		//进行交换
				/*计算本次交换所得分数*/
				while ((nowScore = tmp_lie.deleteConnect()) != 0) {
					score_lie += nowScore;
				}
			}
			/*平行交换可消去*/
			if (init_chess.canDelete_bySwap(i, j, HORIZON)) {
				Chess tmp_hang = init_chess;	//备份棋盘
				swap(tmp_hang.nodes[i][j], tmp_hang.nodes[i][j + 1]);	//进行交换
				/*cout << "del" << endl;
				tmp_hang.output();*/
				/*计算本次交换所得分数*/
				while ((nowScore = tmp_hang.deleteConnect()) != 0) {
					score_hang += nowScore;
				}
			}
			//为方便计算，得分情况为0都简化为-1
			if (!score_hang)score_hang--;
			if (!score_lie)score_lie--;

			//本轮没有得分，无需更新
			if (score_hang == -1 && score_lie == -1)continue;


			//获取垂直交换和平行交换所得最高分
			nowScore = score_hang > score_hang ? score_hang : score_lie;
			int hang_flag = 1;		//hang_flag=1平行交换获得最高分
			if (score_hang < score_lie)hang_flag = 0;


			//更新最高一步交换得分
			if (hang_flag) {
				if (maxScore < score_hang) {
					maxScore = score_hang;
					path.popOp();
					path.pushOp(swap_op(i, j, i, j + 1, maxScore));
				}
			}
			else {
				if (maxScore < score_lie) {
					maxScore = score_lie;
					path.popOp();
					path.pushOp(swap_op(i, j, i + 1, j, maxScore));
				}
			}
		}
	}
	path.output_bestSolve(init_chess, maxScore);
}


/*回溯不剪*/
void backTracing(Chess& chess, int score, int depth) {

	//更新最高分及最高分路径
	if (score > maxScore) {
		maxScore_Path = currentPath;
		maxScore = score;
	}

	//大于设定交换步数，返回
	if (depth > X)return;

	/*两种交换状态，垂直交换和平行交换*/
	for (int dir = 0; dir < DIRECTION ; dir++) {

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	//设置交换状态

		//遍历棋盘
		for (int i = 1; i <= chess.M; i++) {
			for (int j = 1; j <= chess.N; j++) {

				//可交换状态
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) {
					Chess tmpchess = chess;					//备份棋盘
					tmpchess.exchange(i, j, Swap_Dir);		//元素交换
					int nowScore = tmpchess.deleteConnect();	//获取分数


					if (!nowScore)continue;		//分数为0，不需要往下走
					currentPath.pushOp(swap_op(i, j,Swap_Dir, nowScore));	//路径压栈
					backTracing(tmpchess, score + nowScore, depth + 1);		//进行下一状态的交换
					currentPath.popOp();	//出栈路径
				}
			}
		}
	}
	/*
	for (int j = 1; j <= chess.N; j++) {
		for (int i = 1; i < chess.M; i++) {
			if (chess.canDelete_bySwap(i, j, VERTICAL)) {
				Chess tmpchess = chess;
				tmpchess.exchange(i, j, VERTICAL);
				nowScore = tmpchess.deleteConnect();
				if (!nowScore)continue;

				currentPath.pushOp(swap_op(i, j, i + 1, j, nowScore));
				backTracing(tmpchess, score + nowScore, depth + 1);
				currentPath.popOp();
			}
		}
	}
	*/
}
int Solve1_of_XiaoxiaoLe(Chess init_chess) {
	//初始化路径和分数
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	backTracing(init_chess, 0, 1);
	return maxScore;
}

/*纵向剪枝*/
void backTracing_depth(Chess& chess, int score, int depth,double parms) {

	//更新最高分及最高分路径
	if (score > maxScore) {
		maxScore_Path = currentPath;
		maxScore = score;
	}

	//大于设定交换步数，返回
	if (depth > X)return;

	/*
		纵向剪枝
		如果剩下层数需获的平均层分大于已获最大分数的平均分*parms，直接返回，不往下搜索
	*/
	double averageScore = maxScore * 1.0 / X;
	int level_left = X - depth + 1;
	double averScore_nextlevels = (maxScore - score)*1.0 / level_left;
	if (averScore_nextlevels > averageScore*parms)
		return;

	/*两种交换状态，垂直交换和平行交换*/
	for (int dir = 0; dir < DIRECTION; dir++) {

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	//设置交换状态

														//遍历棋盘
		for (int i = 1; i <= chess.M; i++) {
			for (int j = 1; j <= chess.N; j++) {

				//可交换状态
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) {
					Chess tmpchess = chess;					//备份棋盘
					tmpchess.exchange(i, j, Swap_Dir);		//元素交换
					int nowScore = tmpchess.deleteConnect();	//获取分数


					if (!nowScore)continue;		//分数为0，不需要往下走
					currentPath.pushOp(swap_op(i, j, Swap_Dir, nowScore));	//路径压栈
					backTracing_depth(tmpchess, score + nowScore, depth + 1,parms);		//进行下一状态的交换
					currentPath.popOp();	//出栈路径
				}
			}
		}
	}
}
int Solve2_reduceDepth(Chess init_chess,double parms) {
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	backTracing_depth(init_chess, 0, 1, parms);
	return maxScore;
}


/*横向剪枝*/
void backTracing_width(Chess& chess, int score, int depth, int maxWidth) {

	//更新最高分及最高分路径
	if (score > maxScore) {
		maxScore_Path = currentPath;
		maxScore = score;
	}

	//大于设定交换步数，返回
	if (depth > X)return;

	/*
		横向剪枝
		记录每一层得分总和，及每一层经过的次数（宽度）
		在已有一定搜索宽度的情况下，如果当前层得分小于当前层的宽度平均分，则直接返回
	*/
	crossLevel_num[depth]++;
	levelSum[depth] += score;
	if (crossLevel_num[depth] >= maxWidth) {
		double averSum = levelSum[depth] * 1.0 / crossLevel_num[depth];
		if (score < averSum)return;
	}

	/*两种交换状态，垂直交换和平行交换*/
	for (int dir = 0; dir < DIRECTION; dir++) {

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	//设置交换状态

														//遍历棋盘
		for (int i = 1; i <= chess.M; i++) {
			for (int j = 1; j <= chess.N; j++) {

				//可交换状态
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) {
					Chess tmpchess = chess;					//备份棋盘
					tmpchess.exchange(i, j, Swap_Dir);		//元素交换
					int nowScore = tmpchess.deleteConnect();	//获取分数


					if (!nowScore)continue;		//分数为0，不需要往下走
					currentPath.pushOp(swap_op(i, j, Swap_Dir, nowScore));	//路径压栈
					backTracing_width(tmpchess, score + nowScore, depth + 1, maxWidth);		//进行下一状态的交换
					currentPath.popOp();	//出栈路径
				}
			}
		}
	}
}
int Solve3_reduceWidth(Chess init_chess, int maxWidth) {
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	memset(crossLevel_num, 0, sizeof(MAX_DEPTH));
	memset(levelSum, 0, sizeof(MAX_DEPTH));
	backTracing_width(init_chess, 0, 1, maxWidth);
	return maxScore;
}

/*组合剪枝*/
void backTracing_combine(Chess& chess, int score, int depth, int maxWidth,double parms) {

	//更新最高分及最高分路径
	if (score > maxScore) {
		maxScore_Path = currentPath;
		maxScore = score;
	}

	//大于设定交换步数，返回
	if (depth > X)return;

	/*
		横向剪枝
		记录每一层得分总和，及每一层经过的次数（宽度）
		在已有一定搜索宽度的情况下，如果当前层得分小于当前层的宽度平均分，则直接返回
	*/
	crossLevel_num[depth]++;
	levelSum[depth] += score;
	if (crossLevel_num[depth] >= maxWidth) {
		double averSum = levelSum[depth] * 1.0 / crossLevel_num[depth];
		if (score < averSum)return;
	}

	/*
		纵向剪枝
		如果剩下层数需获的平均层分大于已获最大分数的平均分*parms，直接返回，不往下搜索
	*/
	double averageScore = maxScore * 1.0 / X;
	int level_left = X - depth + 1;
	double averScore_nextlevels = (maxScore - score)*1.0 / level_left;
	if (averScore_nextlevels > averageScore*parms)
		return;


	/*两种交换状态，垂直交换和平行交换*/
	for (int dir = 0; dir < DIRECTION; dir++) {

		bool Swap_Dir = dir == 0 ? HORIZON : VERTICAL;	//设置交换状态

														//遍历棋盘
		for (int i = 1; i <= chess.M; i++) {
			for (int j = 1; j <= chess.N; j++) {

				//可交换状态
				if (chess.canDelete_bySwap(i, j, Swap_Dir)) {
					Chess tmpchess = chess;					//备份棋盘
					tmpchess.exchange(i, j, Swap_Dir);		//元素交换
					int nowScore = tmpchess.deleteConnect();	//获取分数


					if (!nowScore)continue;		//分数为0，不需要往下走
					currentPath.pushOp(swap_op(i, j, Swap_Dir, nowScore));	//路径压栈
					backTracing_combine(tmpchess, score + nowScore, depth + 1, maxWidth,parms);		//进行下一状态的交换
					currentPath.popOp();	//出栈路径
				}
			}
		}
	}
}
int Solve4_combine(Chess init_chess, int maxWidth,double parms) {
	maxScore = 0;
	currentPath.clearOp();
	maxScore_Path.clearOp();
	memset(crossLevel_num, 0, sizeof(MAX_DEPTH));
	memset(levelSum, 0, sizeof(MAX_DEPTH));
	backTracing_combine(init_chess, 0, 1, maxWidth,parms);
	return maxScore;
}



void write(double table[][7], int* data, int* x,int data_len,int x_len,double p,ofstream& csvfile) {
	csvfile << "parms = " << p << endl;
	csvfile << "交换步数/规模" << ",";
	for (int i = 1; i <= data_len; i++) {
		csvfile << data[i] << ",";
	}
	csvfile << endl;
	for (int i = 1; i <= x_len; i++) {
		for (int j = 1; j <= data_len; j++) {
			if (j == 1)csvfile << x[i] << ",";
			csvfile << table[i][j] << ",";
		}
		csvfile << endl;
	}
	csvfile << endl;
}

void run(int M,int N,int K,int x) {
	X = x;
	Chess test(M, N, K, true);
	auto startTime_1 = system_clock::now();
	int ans1 = Solve1_of_XiaoxiaoLe(test);
	auto endTime_1 = system_clock::now();
	auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
	double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
	runTime_1 *= 1000;	//转成毫秒
	cout << runTime_1 << endl;
	cout << ans1 << endl;
	//maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_2 = system_clock::now();
	int ans2 = Solve2_reduceDepth(test, 1.3);
	auto endTime_2 = system_clock::now();
	auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
	double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
	runTime_2 *= 1000;	//转成毫秒
	cout << runTime_2 << endl;
	cout << ans2 << endl;
	//maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_3 = system_clock::now();
	int ans3 = Solve3_reduceWidth(test, 12);
	auto endTime_3 = system_clock::now();
	auto duration_3 = duration_cast<microseconds>(endTime_3 - startTime_3);
	double runTime_3 = double(duration_3.count()) * microseconds::period::num / microseconds::period::den;
	runTime_3 *= 1000;	//转成毫秒
	cout << runTime_3 << endl;
	cout << ans3 << endl;
	//maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_4 = system_clock::now();
	int ans4 = Solve4_combine(test, 12, 1.3);
	auto endTime_4 = system_clock::now();
	auto duration_4 = duration_cast<microseconds>(endTime_4 - startTime_4);
	double runTime_4 = double(duration_4.count()) * microseconds::period::num / microseconds::period::den;
	runTime_4 *= 1000;
	cout << runTime_4 << endl;
	cout << ans4 << endl;
	cout << endl;
}
int main() {

	/*Solve_of_oneStep(test);*/
	/*cout << "X:1-5----------------" << endl;
	for (int x = 1; x <= 5; x++) {
		run(8, 8, 4, x);
	}
	cout << endl;

	cout << "K:3-8-------------" << endl;
	for (int k = 3; k <= 8; k++) {
		run(8, 8, k, 4);
	}
	cout << endl;

	cout << "M:6-12----------------" << endl;
	for (int m = 6; m <= 12; m++) {
		run(m, 6, 4, 5);
	}
	cout << endl;

	cout << "N:6-12" << endl;
	for (int n = 6; n <= 12; n++) {
		run(6, n, 4, 5);
	}
	cout << endl;*/

	int m, n, k, x;
	cin >> n >> m>> k >> x;
	X = x;
	Chess test(m, n, k, false);
	auto startTime_1 = system_clock::now();
	int ans1=Solve1_of_XiaoxiaoLe(test);
	auto endTime_1 = system_clock::now();
	auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
	double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
	runTime_1 *= 1000;	//转成毫秒
	cout << runTime_1 << endl;
	cout << ans1 << endl << endl;
	maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_2 = system_clock::now();
	int ans2 = Solve2_reduceDepth(test, 1.3);
	auto endTime_2 = system_clock::now();
	auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
	double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
	runTime_2 *= 1000;	//转成毫秒
	cout << runTime_2 << endl;
	cout << ans2 << endl << endl;
	maxScore_Path.output_bestSolve(test, maxScore);

	auto startTime_3 = system_clock::now();
	int ans3 = Solve3_reduceWidth(test,12);
	auto endTime_3 = system_clock::now();
	auto duration_3 = duration_cast<microseconds>(endTime_3 - startTime_3);
	double runTime_3 = double(duration_3.count()) * microseconds::period::num / microseconds::period::den;
	runTime_3 *= 1000;	//转成毫秒
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
	/*
	X = 10;
	auto startTime_5 = system_clock::now();
	int ans5 = Solve4_combine(test, 12, 1.3);
	auto endTime_5 = system_clock::now();
	auto duration_5 = duration_cast<microseconds>(endTime_5 - startTime_5);
	double runTime_5 = double(duration_5.count()) * microseconds::period::num / microseconds::period::den;
	runTime_5 *= 1000;
	cout << runTime_5 << endl;
	cout << ans5 << endl;
	*/

	//maxScore_Path.output_bestSolve(test, maxScore);
	/*ofstream csvfile;
	//创建csv文件，存储计时结果
	csvfile.open("C:\\Users\\Administrator\\OneDrive\\算法设计与分析\\实验3消消乐-回溯法\\find_width.csv");*/
	int TEST_NUM = 10;
	int data_len = 6;
	int x_len = 4;
	int dataSize[] = { 0,5,6,7,8,9,10 };
	int X_size[] = { 0,1,2,3,4};
	double time_rate[5][7];
	double ans_rate[5][7];
	double total_weight[5][7];
	double parms = 1.0;
	double add = 0.1;

	//parms调参
	for (int k = 1; k <= TEST_NUM; k++) {
		double parms_c = parms + add * k;

		for (int i = 1; i <= data_len; i++) {

			Chess init_chess(dataSize[i], dataSize[i], 3, true);

			for (int j = 1; j <= x_len; j++) {
				X = X_size[j];

				auto startTime_1 = system_clock::now();
				int ans_1 = Solve1_of_XiaoxiaoLe(init_chess);
				auto endTime_1 = system_clock::now();

				auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
				double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
				runTime_1 *= 1000;	//转成毫秒


				auto startTime_2 = system_clock::now();
				int ans_2 = Solve2_reduceDepth(init_chess, parms_c);
				auto endTime_2 = system_clock::now();

				auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
				double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
				runTime_2 *= 1000;	//转成毫秒

				time_rate[j][i] = runTime_2 / runTime_1;
				ans_rate[j][i] = ans_2 * 1.0 / ans_1;;
				total_weight[j][i] = time_rate[j][i] / ans_rate[j][i];

				cout << fixed << setprecision(2);
				cout << "SIZE {" << dataSize[i] << "} |DEPTH {" << X << "} |Parms {" << parms_c << "}" << endl;
				cout << "回溯法耗时：{" << runTime_1 << "}" << " |最高分数：{" << ans_1 <<"} "<< endl;
				cout << "回溯法(剪枝调参）耗时：{" << runTime_2 << "}" << " |最高分数：{" << ans_2 << "} " << endl;
			}
		}
		write(time_rate, dataSize, X_size, data_len, x_len, parms_c, csvfile);
		write(ans_rate, dataSize, X_size, data_len, x_len, parms_c, csvfile);
		write(total_weight, dataSize, X_size, data_len, x_len, parms_c, csvfile);
	}


	//树宽阀值调参  5-12
	int chess_size = 10;
	int T = 10;
	int width[] = { 0,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
	X = 5;
	double time_rate1[27] = { 0 };
	double ans_rate1[27] = { 0 };
	while (T--) {

		Chess init_chess(10, 6, 3, true);
		auto startTime_1 = system_clock::now();
		int ans_1 = Solve1_of_XiaoxiaoLe(init_chess);
		auto endTime_1 = system_clock::now();

		auto duration_1 = duration_cast<microseconds>(endTime_1 - startTime_1);
		double runTime_1 = double(duration_1.count()) * microseconds::period::num / microseconds::period::den;
		runTime_1 *= 1000;	//转成毫秒

		for (int i = 1; i <= 26; i++) {
			auto startTime_2 = system_clock::now();
			int ans_2 = Solve3_reduceWidth(init_chess, width[i]);
			auto endTime_2 = system_clock::now();

			auto duration_2 = duration_cast<microseconds>(endTime_2 - startTime_2);
			double runTime_2 = double(duration_2.count()) * microseconds::period::num / microseconds::period::den;
			runTime_2 *= 1000;	//转成毫秒

			time_rate1[i] += runTime_2 / runTime_1;
			ans_rate1[i] += ans_2 * 1.0 / ans_1;

			cout << fixed << setprecision(2);
			cout << "SIZE {10 * 6} |DEPTH {5} |width {" << width[i] << "}" << endl;
			cout << "回溯法耗时：{" << runTime_1 << "}" << " |最高分数：{" << ans_1 << "} " << endl;
			cout << "回溯法(剪枝调参）耗时：{" << runTime_2 << "}" << " |最高分数：{" << ans_2 << "} " << endl;
		}
	}

	csvfile << "树宽规模" << ",";
	for (int i = 1; i <= 26; i++) {
		csvfile << width[i] << ",";
	}
	csvfile << endl;
	for (int i = 1; i <= 26; i++) {
		if (i == 1)csvfile << "效率比" << ",";
		csvfile << time_rate1[i] / 10 << ",";
	}
	csvfile << endl;
	for (int i = 1; i <= 26; i++) {
		if (i == 1)csvfile << "准确率" << ",";
		csvfile << ans_rate1[i] / 10 << ",";
	}
	csvfile << endl;


}