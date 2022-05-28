
1102 ALCO Project 2 - 3-bit Histroy Predictor
(高分版)
===
   
   

2-bit Counter State Diagram
---
![](https://i.imgur.com/KXcaJnG.png)

                    
3-bit Histroy Predictor
---
共有$2^3$個2-bit Counter

![](https://i.imgur.com/S3LQ251.png)




Input Example 
---
* Please input entry (entry > 0): 
```gherkin=
4
```
* Please input Instruction (^Z to terminate):
```gherkin=
	    addi R0, R0, 0
	    addi R2, R0, 4 
    Loop:
	    beq R1, R2, End
	    addi R2, R2, -1 
	    beq R0, R0, Loop
    End:
```
![](https://i.imgur.com/JqADKfJ.png)


流程說明(高分版)
---
1. `void ReadInstruction()`:讀入所有輸入的Instruction 
2. `void Initialize_Inst()`: 給每行輸入的Instruction一個address, 並依照Label 和 Instruction的種類不同, insert 到 LabelAddrMap or InstAddrMap
3. `void Initialize_Entry()`: 依照輸入的entry數量, 初始化每個entry不同的 預測STATE & 3bit_history
4. `void CAL_Inst(vector<string> inst)`: 依照不同的operation (inst) 更新各register的結果
5. `void UpdateStatus(vector<string> &STATE, int position, bool branch)`: 依實際結果(branch)更新編號為positon之2BC的狀態 
6. ` void Output()`:計算 & 輸出結果

程式碼說明(高分版)
---
* `vector<vector<string>> InstructionVec`: 儲存輸入Instruction的二維vector
    
* `vector<bitset<3>> HistoryVecInstructionVec`: 每個entry裡面的 3bit history

* `vector<vector<string>> AllEntryState`: 儲存每個entry不同的預測STATE
 
* `map<string, int> AllRegisterMap`: 儲存所有Register裡面的值

* `map<string, int> LabelAddrMap`: Label address -> map

* `map<int, vector<string>> InstAddrMap`: key -> address value -> 某行程式

輸出結果
---
![](https://i.imgur.com/7vz2TU5.png)


