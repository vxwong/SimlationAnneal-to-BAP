#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <iomanip>
using namespace std;

#define max_time 31
#define max_berth 13
#define max_ship 25

struct Ship{
	int arrival_time, 
		service_time, 
		occupies;
};


struct Management{
	int start_berth[max_ship],
		start_time[max_ship],
		vess,
		departure,
		waiting;
};

struct Managements
{	
	int arrang_order[max_ship]; //排船顺序
	int gamemap[max_berth][max_time]; //游戏地图
	Management ma;	
	int weight;
};


Ship ship[max_ship];
Managements ans;	//答案
Managements ways[10];
int bestsol = 999;	
double decline = 0.99;
int case_time, case_berth, case_ship;
int get_weight (int vess, int waiting, int departure);
void initial (Managements &a);
void readfile ();
void greedy (Managements &a);  //贪心寻找位置 
bool check (int maptime, int mapspace, int shiptime, int shipspace);	//检查能不能放入
void moor(int maptime, int mapspace, int shiptime, int shipspace);		//把船放入地图
void simulateAnneal();	//模拟退火 
void printAns();
string name;

void initial (Managements &a)
{
	memset (a.gamemap, 0, sizeof(a.gamemap));
	memset (a.ma.start_berth, -1, sizeof(a.ma.start_berth));
	memset (a.ma.start_time, -1, sizeof(a.ma.start_time));
	a.ma.vess = case_ship;
	a.ma.waiting = 0;
	a.ma.departure = 0;
}

bool check (Managements a, int maptime, int mapspace, int shiptime, int shipspace)
{
	bool flag = true;
	for (int i=0; i<shiptime; i++)
		for (int j=0; j<shipspace; j++)
			if (a.gamemap[mapspace+j][maptime+i] != 0)
				flag = false;
	return flag;
}

void printmap(Managements a)
{
	for (int j=0; j<case_berth; j++)
	{	for (int i=0; i<case_time; i++)
		{
			cout << fixed << setw(3) << a.gamemap[j][i] ;				
		}
		cout << endl;
	}		
}

void moor(Managements &a, int index, int maptime, int mapspace, int shiptime, int shipspace)
{
	for (int i=0; i<shiptime; i++)
		for (int j=0; j<shipspace; j++)
		{
			a.gamemap[mapspace+j][maptime+i] = index+1;
		}
}

void greedy (Managements &a)   
{
	initial(a);
	for (int i=0; i<case_ship; i++)
	{
		bool posited = false;
		for (int k=ship[a.arrang_order[i]].arrival_time; k+ship[a.arrang_order[i]].service_time<=case_time; k++)
		{
			for (int j=0; j+ship[a.arrang_order[i]].occupies<=case_berth; j++)
			{
				if (check(a,k,j,ship[a.arrang_order[i]].service_time,ship[a.arrang_order[i]].occupies))
				{
					moor(a,a.arrang_order[i],k,j,ship[a.arrang_order[i]].service_time,ship[a.arrang_order[i]].occupies);
					a.ma.start_berth[a.arrang_order[i]] = j;
					a.ma.start_time[a.arrang_order[i]] = k;
					a.ma.vess --;
					a.ma.waiting += k-ship[a.arrang_order[i]].arrival_time;
					a.ma.departure = max(a.ma.departure, k+ship[a.arrang_order[i]].service_time);
					posited = true;
				}
				if (posited) break;
			}
			if (posited) break;
		}
	}

	a.weight = get_weight(a.ma.vess, a.ma.waiting, a.ma.departure);
	if (a.weight < bestsol)
	{
		bestsol = a.weight;
		ans = a;
	}

}

int get_weight (int vess, int waiting, int departure)
{
	return vess*100 + waiting*2 + departure;
}

void readfile ()
{
	int i = 0;
//	string name = "10.txt";
	cout << "Please input test name \n";
	cin >> name;

	if (name[0] == '7' || name[1] != '.')		decline = 0.9999;   //复杂情况，把降温系数改大 
	fstream in(name.c_str());
	while (!in.is_open())
	{
		cout << "Opne failed! Please try again\n";
		cin >> name;
		in.open(name.c_str());
	}
	in >> case_time >> case_berth >> case_ship ;
	while (in >> ship[i].arrival_time >> ship[i].service_time >> ship[i].occupies)
	{
		i ++;
	}
}

void simulateAnneal()
{
	double initial_tem = 1;
	double end_tem = 1e-50;
//	double decline = 0.99;
	while (initial_tem > end_tem)
	{
		for (int i=0; i<5; i++)
		{
			Managements tmp = ways[i];
			int u = rand() % (case_ship-1) + 1;
			int v = rand() % (case_ship-1) + 1;
			while (u == v)
			{
				u = rand() % (case_ship-1) + 1;
			}
			swap(tmp.arrang_order[u], tmp.arrang_order[v]);
			greedy(tmp);
			if (tmp.weight < ways[i].weight || exp((ways[i].weight-tmp.weight) / initial_tem) > rand()/RAND_MAX)
				ways[i] = tmp;
		}
		initial_tem *= decline;
	}
}

void printAns()
{
	cout << "The weight is : " << bestsol << endl;
	cout << "The number of unassigned vessels is : " << ans.ma.vess << endl;
	cout << "The total waiting time is : " << ans.ma.waiting << endl;
	cout << "The last departure time is : " << ans.ma.departure << endl; 
	cout << "The solution is : \n";	
	for (int i=0; i<case_ship; i++)
		cout << ans.ma.start_berth[i] << ',' << ans.ma.start_time[i] << ';';	
	
	cout << endl << endl;
	printmap(ans); 
	cout << endl << endl; 
} 

int main()
{
	int cnt = 1;
	while (1)
	{
		readfile();
		for (int i=0; i<case_ship; i++)
			ways[0].arrang_order[i] = i;
			
		greedy(ways[0]);
		
		for (int i=1; i<10; i++)
			ways[i] = ways[0];
	
		simulateAnneal();
		cout << "end\n";
		printAns();			
		//system("pause");
		cnt++;
	}
} 
