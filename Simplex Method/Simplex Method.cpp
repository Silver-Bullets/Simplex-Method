// Simplex Method.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

const int MAXN = 20;
const int MAXM = 10;

const int FOUND_OPTIMAL_SOLUTION = 0;
const int UNBOUNDED = 1;

int n, m;	//m个方程，n个未知数
Fraction matrix[MAXM][MAXN];	//增广矩阵
Fraction target[MAXN];	//目标函数的系数
Fraction sigma[MAXN];	//检验数
Fraction theta[MAXM];	//用于计算换出变量
bool hasTheta[MAXM];	//theta是否有意义
int X[MAXM];			//当前解
bool minz;	//是否要求取最小值

inline Fraction& b(int i)	//增广矩阵最后一列记录b
{
	return matrix[i][n];
}

inline void showSplitLine(int length = 100, char c = '-')
{
	for (int i = 0; i < length; i++)
	{
		cout << c;
	}
	cout << endl;
}

void inputMatrix();
int work();			//完整算法
void findFirstX();		//寻找单位阵（初始解）
void getSigma();		//计算检验数
void getTheta(int column);		//计算指定列计算theta
void transfer(int row, int column);	//将指定位置变换为1，并更换解
void throwError(string str);		//向命令行输出异常提示并终止程序
void showMatrix();			//输出单纯形表

int main(int argn, char** argv)
{
	ifstream fin("in.txt");
	ofstream fout("out.txt");
	auto consoleIn = cin.rdbuf();
	auto consoleOut = cout.rdbuf();
	cin.rdbuf(fin.rdbuf());		//重定向cin,cout至文件
	cout.rdbuf(fout.rdbuf());
	inputMatrix();
	work();
	system("pause");
	return 0;
}

void inputMatrix()
{
	cin >> n >> m;
	if (n <= 0 || m <= 0)
	{
		throwError("未知数数量与方程数量为0！");
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			cin >> matrix[i][j];
		}
	}
	string targetType;
	cin >> targetType;
	if (targetType == "min")
	{
		minz = true;
	}
	for (int i = 0; i < n; i++)
	{
		cin >> target[i];
	}
}

void throwError(string str)
{
	cout << "ERROR: " << str << endl;
	exit(-1);
}

int work()
{
	findFirstX();
	while (true)
	{
		getSigma();

		bool isOptimalSolution = true;
		int swapin = -1;				//换入变量
		Fraction maxSigma = 0;
		for (int i = 0; i < n; i++)
		{
			if (sigma[i] > maxSigma)
			{
				isOptimalSolution = false;
				maxSigma = sigma[i];
				swapin = i;
			}
		}
		if (isOptimalSolution)
		{
			//计算终止
			for (int i = 0; i < m; i++)
				hasTheta[i] = false;
			showMatrix();
			return FOUND_OPTIMAL_SOLUTION;
		}

		getTheta(swapin);
		showMatrix();

		int swapout = -1;		//换出变量在X中的下标（换出变量所在矩阵行）
		Fraction minTheta = 0;
		for (int i = 0; i < m; i++)
		{
			if (hasTheta[i])
			{
				if (theta[i] < minTheta || swapout == -1)
				{
					swapout = i;
					minTheta = theta[i];
				}
			}
		}

		X[swapout] = swapin;
		transfer(swapout, swapin);
	}
}


void findFirstX()
{
	for (int i = 0; i < m; i++)
	{
		bool flag = false;	//该行初始解是否寻找成功
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == 1)
			{
				bool valid = true;	//该列是否合法（除该行是1，其余全是0）
				for (int k = 0; k < m; k++)
				{
					if (k != i && matrix[k][j] != 0)
					{
						valid = false;
						break;
					}
				}
				if (valid)
				{
					X[i] = j;
					flag = true;
					break;
				}
			}
		}
		if (!flag)
		{
			throwError("找不到初始单位阵！");
		}
	}
}

void getSigma()
{
	for (int i = 0; i < n; i++)
	{
		Fraction zi = 0;
		for (int j = 0; j < m; j++)
		{
			zi += target[X[j]] * matrix[j][i];
		}
		sigma[i] = target[i] - zi;
		if (minz)
		{
			sigma[i] = -sigma[i];	//如果要求最小值，sigma取反，保证判别方法为<=
		}
	}
}

void getTheta(int column)
{
	for (int i = 0; i < m; i++)
	{
		if (matrix[i][column] <= 0)
		{
			hasTheta[i] = false;
		}
		else
		{
			hasTheta[i] = true;
			theta[i] = b(i) / matrix[i][column];
		}
	}
}

void transfer(int row, int column)
{
	Fraction tmp = matrix[row][column];
	for (int i = 0; i <= n; i++)	//行变换将该位置化为一
	{
		matrix[row][i] /= tmp;
	}

	for (int i = 0; i < m; i++)			//将该列其它位置置0
	{
		Fraction tmp = matrix[i][column];
		for (int j = 0; j <= n; j++)
		{
			if (i != row)
			{
				matrix[i][j] = matrix[i][j] - tmp * matrix[row][j];
			}
		}
	}
}

void showMatrix()
{
	showSplitLine(38 + 8 * n, '=');

	cout << setw(20)<<"cj -->";
	for (int i = 0; i < n; i++)
	{
		cout<<setw(8) << target[i];
	}
	cout << endl;

	showSplitLine(28 + 8 * n);

	cout << setw(8) << "CB" << setw(4) << "XB" << setw(8) << "b";
	for (int i = 0; i < n; i++)
	{
		cout << setw(8) << "x" + to_string(i+1);
	}
	cout << setw(8) << "theta" << endl;

	showSplitLine(28 + 8 * n);

	for (int i = 0; i < m; i++)
	{
		cout << setw(8) << target[X[i]] << setw(4) << "x" + to_string(X[i]+1) << setw(8) << b(i);
		for (int j = 0; j < n; j++)
		{
			cout << setw(8) << matrix[i][j];
		}
		if (hasTheta[i])
			cout << setw(8) << theta[i];
		else
			cout << setw(8) << "-";
		cout << endl;
	}

	showSplitLine(28 + 8 * n);

	cout << setw(20) << "cj - zj";
	for (int i = 0; i < n; i++)
	{
		if (minz)
		{
			cout << setw(8) << -sigma[i];
		}
		else
		{
			cout << setw(8) << sigma[i];
		}
	}
	cout << endl;
	showSplitLine(38 + 8 * n, '=');
	cout << endl;
}
