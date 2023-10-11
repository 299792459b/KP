#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <random>
#include <map>
#include <unordered_map>
using namespace std;

int num_actions = 2;
vector<int> cards = {1,2,3};
double utility;

map<string,vector<double>> allinfosets;

unordered_map<string,int> allsets;
vector<vector<double>> regretSum(12,vector<double>(2,0));
vector<double> strategy(2);
vector<vector<double>> strategySum(12,vector<double>(2,0));
int indexinfo;

vector<double> getStrategy(double realizationWeight, int idx)
{
	double normalizingSum = 0;
	for (int a = 0; a < num_actions; a++)
	{
		strategy[a] = regretSum[idx][a] > 0 ? regretSum[idx][a] : 0;
		normalizingSum += strategy[a];
	}
	for (int a = 0; a < num_actions; a++)
	{
		if (normalizingSum > 0) strategy[a] /= normalizingSum;
		else strategy[a] = 1.0 / num_actions;
		strategySum[idx][a] += realizationWeight * strategy[a];
	}
	return strategy;
}

vector<double> getAverageStrategy(int idx)
{
	vector<double> avgStrategy(2);
	double normalizingSum = 0;

	for (int a = 0; a < num_actions; a++) normalizingSum += strategySum[idx][a];
	for (int a = 0; a < num_actions; a++)
	{
		if (normalizingSum > 0) avgStrategy[a] = strategySum[idx][a] / normalizingSum;
		else avgStrategy[a] = 1.0 / num_actions;
	}
	return avgStrategy;
}



double cfr(string hist, double p0, double p1)
{
	int plays = hist.length();
	int player = plays % 2;
	int opponent = 1 - player;
	bool terminalcheck;
	bool terminalbet;
	bool playerhigher;

	if (plays > 1)
	{
		terminalcheck = hist[plays-1] == 'c';
		terminalbet = hist[hist.size()-2] == 'b' && hist[hist.size()-1] == 'b';
		playerhigher = cards[player] > cards[opponent];
		
		if (terminalcheck)
		{
			if (hist == "cc")
			{
				return playerhigher ? 1 : -1;
			}
			else return 1;
		}
		else if (terminalbet) return playerhigher ? 2 : -2;
	}
	
	string infoset = to_string(cards[player]) + hist;
	indexinfo = allsets[infoset]-1;

	allinfosets[infoset] = getAverageStrategy(indexinfo);
	
	vector<double> strat;

	strat = getStrategy(player == 0 ? p0 : p1, indexinfo);
	vector<double> util(2);
	
	double nodeutil = 0;
	for (int i = 0; i < num_actions; i++)
	{
		string nexthis = hist + (i == 0 ? "c" : "b");

		util[i] = player == 0 ? -cfr(nexthis, p0*strat[i], p1) : -cfr(nexthis, p0, p1*strat[i]);
		nodeutil += strat[i] * util[i];
	}
	indexinfo = allsets[infoset]-1;

	for (int i = 0; i < num_actions; i++)
	{
		double regret = util[i] - nodeutil;
		//cerr << "regret: " << regret << endl;
		regretSum[indexinfo][i] += (player == 0 ? p1 : p0)*regret;

	}
	return nodeutil;
}

int main(void)
{
	clock_t tStart = clock();
	vector<double> initializer(2);
	allsets["1"]=1; 
	allsets["1c"]=2; 
	allsets["1cb"]=3; 
	allsets["1b"]=4; 
	allsets["2"]=5; 
	allsets["2c"]=6; 
	allsets["2cb"]=7; 
	allsets["2b"]=8; 
	allsets["3"]=9; 
	allsets["3c"]=10; 
	allsets["3cb"]=11; 
	allsets["3b"]=12; 
	for (auto &thing: allsets)
	{
		allinfosets[thing.first] = initializer;
	}

	srand(time(0));

	for (int i = 0; i < 1000000; i++) //num iterations
	{
		for (int c1 = cards.size() - 1; c1 > 0; c1--)
		{
			int c2 = rand() % (c1+1);
			int tmp = cards[c1];
			cards[c1] = cards[c2];
			cards[c2] = tmp;
		}
		utility += cfr("", 1, 1);
	}
	cerr << "Check/Fold" << " " << "Bet/Call" << endl;
	for (auto &thing: allinfosets)
	{
		cerr << thing.first << ": ";
		for (auto &thing2:thing.second) cerr << thing2 << " ";
		cerr << endl;
	}
	cout << endl;
	cerr << "Calculated EV for Player 1: " << utility / 1000000 << endl;
	cout << endl;
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	return 0;
}