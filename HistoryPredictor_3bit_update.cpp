#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <bitset>
#include <vector>
#include <map>
using namespace std;

bool BranchOrNot = false; // branch 預設為NT (false)
bool prediction = false;  // 預測結果預設為NT (false)  
int InstNum = 0;          //輸入Instruction的數量 (不含Label) 
int PC = 0;               // Program Counter
int entry = 0;            // entry (預設為0)
   
vector<vector<string>> InstructionVec; // 儲存輸入Instruction的二維vector,  ex:   addi R2, R0, 6   =  Instruction[0][0]   Instruction[0][1],  Instruction[0][2],  Instruction[0][2]   
vector<bitset<3>> HistoryVec;          // 每個entry裡面的 3bit history
vector<vector<string>> AllEntryState;  // 儲存每個entry不同的預測STATE
map<string, int> AllRegisterMap;       // 儲存所有Register裡面的值 ex:   Reg[R1] = AllRegisterMap[R1]								   
map<string, int> LabelAddrMap;		   // Label address -> map
map<int, vector<string>> InstAddrMap;  // key -> address value -> 某行程式

void ReadInstruction();												    // 1. 讀入所有輸入的Instruction 
void Initialize_Inst();													// 2. 給每行輸入的Instruction一個address, 並依照Label 和 Instruction的種類不同, insert 到 LabelAddrMap / InstAddrMap
void Initialize_Entry();								                // 3. 依照輸入的entry數量, 初始化每個entry不同的 預測STATE & 3bit_history
void CAL_Inst(vector<string> inst);										// 4. 依照不同的operation (inst) 更新各register的結果
void UpdateStatus(vector<string> &STATE, int position, bool branch);    // 5. 依實際結果(branch)更新編號為positon之2BC的狀態 : SN -> WN -> WT -> ST
void Output();                                                          // 6. 計算 & 輸出結果

int main() {

	cout << "Please input entry (entry > 0): " << endl;
	cin >> entry;

	cout << "\nPlease input Instruction (^Z to terminate):" << endl;
	ReadInstruction();      // 讀入所有輸入的Instruction 

	Initialize_Inst();      // 給每行輸入的Instruction一個address, 並依照Label 和 Instruction的種類不同, insert 到 LabelAddrMap / InstAddrMap

	Initialize_Entry();     // 依照輸入的entry數量, 初始化每個entry不同的 預測STATE & 3bit_history

	cout << "    PC              Instruction     Entry                             State     Prediction     Outcome     Correct" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------" << endl;

	Output();               // 計算 & 輸出結果

	cout << "------------------------------------------------------------------------------------------------------------------" << endl;
	return 0;
}


void ReadInstruction() {   // 讀入所有輸入的Instruction

	char c;
	string inputStr;
	vector<string> inputLine;

	while (cin.peek() != EOF) {

		c = cin.peek();
		if (c == ' ' || c == '\n' || c == ',') {

			if (c == '\n') {
				if (inputLine.size() > 0) {
					InstructionVec.push_back(inputLine);
				}
				inputLine.clear();
			}
			cin.get();
		}
		else {
			string str = "";

			while (c != '\n') {
				c = cin.peek();
				if (c == ' ' || c == ',' || c == '\n') {
					break;
				}
				str += cin.get();
			}
			inputLine.push_back(str);
		}
	}
}
void Initialize_Inst() {   // 給每行輸入的Instruction一個address, 並依照Label 和 Instruction的種類不同, insert 到 LabelAddrMap / InstAddrMap

	int address = 0;

	for (int i = 0; i < InstructionVec.size(); i++) {                         // i : 第幾行輸入的Instruction 

		// 判斷該行 Instruction 是不是Label : 檢查InstructionVec[0]的最後一個字是否為':' (因為每個Label最後都會加':') 
		int lastPos = InstructionVec[i][0].size() - 1; // 該行InstructionVec[0] (operation 種類) 的最後一個字

		if ( InstructionVec[i][0][lastPos] == ':' ) {    // 如果第i行輸入的Instruction 是 Label 

			//把Label名稱 & address 插入 LabelAddrMap裡
			LabelAddrMap.insert( pair<string, int>(InstructionVec[i][0], address) );   
		} 
		else {  // 如果第i行輸入的Instruction 是 一般Instruction(非Label) 
			InstNum++;
			InstAddrMap.insert( pair<int, vector<string> >(address, InstructionVec[i]) );
			address++;
		} 

	} 
}


void Initialize_Entry() {

	bitset<3> history_3bit;

	for (int i = 0; i < entry; i++) {
		AllEntryState.push_back( { "SN", "SN", "SN", "SN", "SN", "SN", "SN", "SN" } );     // 初始化每個entry的STATE 
		HistoryVec.push_back(history_3bit); // 初始化每個entry的3bit_history
	}

}

void CAL_Inst(vector<string> inst) {     // 依照不同的operation計算各register的結果

	if (inst[0] == "add") {    // ex:    add        R1         R0            R2
							   //      inst[0]    inst[1]     inst[2]      inst[3]
							   //                   reg1       reg2          reg3
							   // Reg[R1] = Reg[R0] + Reg[R2] 
		AllRegisterMap[inst[1]] = AllRegisterMap[inst[2]] + AllRegisterMap[inst[3]];
	}
	if (inst[0] == "addi") {   // ex:    addi        R1         R0            5
							   //      inst[0]    inst[1]     inst[2]      inst[3]
							   //                   reg1       reg2       immediate
							   // Reg[R1] = Reg[R2] + immediate
		AllRegisterMap[inst[1]] = AllRegisterMap[inst[2]] + stoi(inst[3]);
	}
	if (inst[0] == "beq") {    // ex:    beq        R1          R5           Loop
							   //      inst[0]    inst[1]     inst[2]      inst[3]
							   //                   reg1       reg2          Label
							   // if ( Reg[R1] = Reg[R5] ), GOTO  Label ( 發生 branch )
		( AllRegisterMap[inst[1]] == AllRegisterMap[inst[2]] ) ? BranchOrNot = true : BranchOrNot = false; 
	}
	if (inst[0] == "bne") {    // ex:    bne        R1          R5           Loop
							   //      inst[0]    inst[1]     inst[2]      inst[3]
							   //                   reg1       reg2          Label
							   // if ( Reg[R1] != Reg[R5] ), GOTO  Label ( 發生 branch )
		( AllRegisterMap[inst[1]] != AllRegisterMap[inst[2]] ) ? BranchOrNot = true : BranchOrNot = false;
	}
}

void UpdateStatus(vector<string> &STATE, int pos, bool branch) {   // 依實際結果更新編號為positon之2BC的狀態 : SN -> WN -> WT -> ST

	if (branch) {   // 若發生branch, 該positon 的 state 增加 ( 預測結果為T的可能性大 )
		if (STATE[pos] == "SN") {
			STATE[pos] = "WN";
		}
		else if (STATE[pos] == "WN") {
			STATE[pos] = "WT";
		}
		else if (STATE[pos] == "WT") {
			STATE[pos] = "ST";
		}
	}
	else {        // 若branch not taken, 該positon 的 state 下降 ( 預測結果為N的可能性大 )
		if (STATE[pos] == "ST") {
			STATE[pos] = "WT";
		}
		else if (STATE[pos] == "WT") {
			STATE[pos] = "WN";
		}
		else if (STATE[pos] == "WN") {
			STATE[pos] = "SN";
		}
	}
}


void Output() {   // 計算 & 輸出結果

	while ( PC < InstNum ) {

		//輸出順序:   PC  ->   Instruction  ->   Entry   ->    history   ->   STATE[0] ~ STATE[7]  ->   Prediction  ->   Outcome  ->   O or X 

		//把10進制PC轉成16進位 並輸出PC
		cout << "0x" << setw(4) << setfill('0') << hex << PC * 4 << "          " << setfill(' ');
	

		// 輸出現在的 instruction
		//ex:      addi            R1           R2            R3  
		//      curInst[0]     curInst[1]   curInst[2]    curInst[3]
		vector<string> curInst = InstAddrMap[PC];
		cout << setw(4) << curInst[0] << setw(3) << curInst[1] << setw(3) << curInst[2] << setw(5) << curInst[3];


		// 依照不同的operation (curInst[0]) 更新各register的結果
		CAL_Inst(curInst);

		// -----------------開始預測----------------------------------
		// 要更新哪個entry   
		int curEntry = PC % entry;  

		// 輸出該entry
		cout << setw(10) << curEntry;

		// 輸出該entry 的 3 bit history
		cout << setw(10) << HistoryVec[curEntry] << " ";

		// 輸出該entry的 STATE[0] ~ STATE[7] 
		for (string str : AllEntryState[curEntry]) {
			cout << str << " ";
		}

		// to_ulong() : 把 3-bit history 的 bitset值 變成int, 然後回傳該int值 
		int history_dec = HistoryVec[curEntry].to_ulong();

		// 如果prediction預測為T
		if (AllEntryState[curEntry][history_dec][1] == 'T') {
			prediction = true;
			cout << setw(14) << 'T';
		}
		else {
			prediction = false;
			cout << setw(14) << 'N';
		}

		// 更新該entry 的 STATE[]
		UpdateStatus(AllEntryState[curEntry], history_dec, BranchOrNot);

		// 是否發生branch? 如果有發生的話輸出T  :否則輸出N 
		BranchOrNot ? (cout << setw(12) << 'T') : (cout << setw(12) << 'N');

		// 預測結果正確嗎? 如果預測正確的話輸出O  :否則輸出X 
		BranchOrNot == prediction ? (cout << setw(12) << 'O') : (cout << setw(12) << 'X');

		// 更新該entry 的 3-bit history
		HistoryVec[curEntry] = HistoryVec[curEntry] << 1;

		//更新PC
		if (BranchOrNot) {
			HistoryVec[curEntry][0] = 1;
			PC = LabelAddrMap[curInst[3] + ':'];
		}
		else {
			PC++;
		}
		cout << endl << endl;
	}

}