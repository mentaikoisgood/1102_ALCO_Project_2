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
	//宣告 2-bit Counter State  
	string STATE_2BC[4]= { "SN", "WN", "WT", "ST" };

	//宣告 8個 2-bit Counter
	int STATE[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	//輸入字串:  TTTTNNTNTNTNTNTNT
	string inputStr;
	cout << "Enter input: " << endl;
	cin >> inputStr;
	cout << endl;

	//宣告一個vector 儲存 inputStr 
	//預設先存3個N進去 , 這樣才可以從inputStr的第1個字母開始預測
	//也就是說第1個字母的history 是 'NNN'
	vector<char> outcome(3, 'N');
	for (char c:inputStr)
	{
		outcome.push_back(c);
	}

	//輸出結果
	cout << "                                    State     Prediction     Outcome     Correct" << endl;

	//從outcome[3] (inputStr的第1個字母) 開始預測, 因為outcome[0]-outcome[2]是之前預設的3個N
	int position = 3;

	//預測從inputStr的第1個字母 ~ 最後一個字母
	//while loop共執行 inputStr.size() 次
	while (position < outcome.size())
	{
		cout <<"Round" << setw(2) << position-2 << "     ";             //預測第幾個字母 ex: round 1 = 現在正在預測第1個字母 
		int history = 0;                                                //history表示前3個字母  ex: history = 101 , 前3個字母為TNT
		char prediction = ' ';                                          //prediction表示預測結果
		if (outcome[position - 1] == 'T')                               //更新history , 若前面1位字母為T, 則history += 2^0
		{
			history = history + pow(2, 0);
		}
		if (outcome[position - 2] == 'T')                               //更新history , 若前面2位字母為T, 則history += 2^1
		{
			history = history + pow(2, 1);
		}
		if (outcome[position - 3] == 'T')                               //更新history , 若前面3位字母為T, 則history += 2^2
		{
			history = history + pow(2, 2);
		}

		if (STATE[history] == 0 || STATE[history] == 1)                 //預測時要以編號為history的2-bit Counter為準
		{                                                               //如果STATE[history]的2BC 狀態為WN or SN, 預測結果為N
			prediction = 'N';
		}
		else
		{
			prediction = 'T';                                           //否則STATE[history]的2BC 狀態為WT or ST, 預測結果為T
		}


		cout << " " <<bitset<3>(history) << "  ";                       //將history編號以2進位表示, 長度3bit 
		
		cout << STATE_2BC[STATE[0]] << " ";                             //依序輸出2BC的狀態
		cout << STATE_2BC[STATE[1]] << " ";
		cout << STATE_2BC[STATE[2]] << " ";
		cout << STATE_2BC[STATE[3]] << " ";
		cout << STATE_2BC[STATE[4]] << " ";
		cout << STATE_2BC[STATE[5]] << " ";
		cout << STATE_2BC[STATE[6]] << " ";
		cout << STATE_2BC[STATE[7]] << " ";

		cout << setw(14) << prediction << setw(12) << outcome[position];        //輸出本次預測結果 & 實際結果

		//如果預測結果 = 實際結果, 輸出O ; 如果預測結果 != 實際結果, 輸出X
		prediction == outcome[position] ? (cout << setw(12) << "O" << endl ) : (cout << setw(12) << "X" << endl);  
        cout << endl; 
		
		//依實際結果更新編號為history之2BC的狀態, 若實際結果為T則增加state狀態編號 (編號越高 , 預測結果為T的可能性越大)
		if (outcome[position] == 'T' && STATE[history] < 3)
		{
			STATE[history]++;
		}//依實際結果更新編號為history之2BC的狀態, 若實際結果為N則減少state狀態編號 (編號越低 , 預測結果為N的可能性越大)
		if (outcome[position] == 'N' && STATE[history] > 0)
		{
			STATE[history]--;
		}
		
	    //換下一個字母
		position++;
	}
}