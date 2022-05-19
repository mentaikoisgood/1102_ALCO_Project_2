// TTTTNNTNTNTNTNTNT
/********************************************************************************************************/
/********************************************************************************************************/
/***  000         SN         SN         SN         SN         SN         SN         SN         SN    ***/
/*** hisory     STATE[0]   STATE[1]   STATE[2]   STATE[3]   STATE[4]   STATE[5]   STATE[6]   STATE[7] ***/ 
/********************************************************************************************************/
/********************************************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <bitset>
#include <cmath>
using namespace std;

int main()
{

	string STATE_2BC[4]= { "SN", "WN", "WT", "ST" };
	int STATE[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	string inputStr;

	cout << "Enter input: " << endl;
	cin >> inputStr;
	cout << endl;

	vector<char> outcome(3, 'N');

	for (char c:inputStr)
	{
		outcome.push_back(c);
	}

	cout << "                                           State     Prediction     Outcome     Correct" << endl;

	int position = 3;
	while (position < outcome.size())
	{
		cout <<"Round" << setw(2) << position-2 << "     ";
		int history = 0;
		char prediction = ' ';
		if (outcome[position - 1] == 'T')
		{
			history = history + pow(2, 0);
		}
		if (outcome[position - 2] == 'T')
		{
			history = history + pow(2, 1);
		}
		if (outcome[position - 3] == 'T')
		{
			history = history + pow(2, 2);
		}

		if (STATE[history] == 0 || STATE[history] == 1)
		{
			prediction = 'N';
		}
		else
		{
			prediction = 'T';
		}


		cout << " " <<bitset<3>(history) << "  ";
		
		
		cout << STATE_2BC[STATE[0]] << " ";
		cout << STATE_2BC[STATE[1]] << " ";
		cout << STATE_2BC[STATE[2]] << " ";
		cout << STATE_2BC[STATE[3]] << " ";
		cout << STATE_2BC[STATE[4]] << " ";
		cout << STATE_2BC[STATE[5]] << " ";
		cout << STATE_2BC[STATE[6]] << " ";
		cout << STATE_2BC[STATE[7]] << " ";

		cout << setw(13) << prediction << setw(12) << outcome[position];

		prediction == outcome[position] ? (cout << setw(12) << "O" << endl ) : (cout << setw(12) << "X" << endl);


		cout << endl;
		
		if (outcome[position] == 'T' && STATE[history] < 3)
		{
			STATE[history]++;
		}
		if (outcome[position] == 'N' && STATE[history] > 0)
		{
			STATE[history]--;
		}
		

		position++;
	}
}