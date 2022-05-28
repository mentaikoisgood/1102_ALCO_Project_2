
1102 ALCO Project 2 - 3-bit Histroy Predictor
(高分版)
===

DEMO影片
----

檔案說明
---
* `HistoryPredictor_3bit.cpp` : 低分版
* `HistoryPredictor_3bit_update.cpp`: 高分版

* `README.md`: 說明文件
* `input.txt` : 輸入測資


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
1. `void ReadInstruction()`:
   * 一行一行讀入輸入的Instruction
   * 把每行 Instruction 分段 
   * 依序push_back()到存Instruction的vector裡
   
2. `void Initialize_Inst()`: 
   * 給每行輸入的Instruction一個address
   * 判斷那行Instruction是不是label
   * 如果是label的話address就插入放Label address的Map裡
   * 否則插入放Label address的Map裡
   
3. `void Initialize_Entry()`: 
   * 依照輸入entry數初始化每個entry的3-bit history和該entry 2-bit Counter的狀態
   * 所有的 2-bit Counter的狀態都先預設為Strong Not Taken 

4. `void CAL_Inst(vector<string> inst)`: 依照不同的operation (inst) 更新各register的結果

5. `void UpdateStatus(vector<string> &STATE, int pos, bool branch)`: 依實際結果(branch)更新編號為pos之2-bit Counter的狀態 
6. ` void Output()`:計算 & 輸出結果

程式碼說明(高分版)
---
* `vector<vector<string>> InstructionVec`: 儲存輸入Instruction的二維vector
    
* `vector<bitset<3>> HistoryVecInstructionVec`: 每個entry裡面的 3-bit history

* `vector<vector<string>> AllEntryState`: 儲存每個entry不同的預測STATE
 
* `map<string, int> AllRegisterMap`: 儲存所有Register裡面的值

* `map<string, int> LabelAddrMap`: 儲存Label Address的Map

* `map<int, vector<string>> InstAddrMap`: 儲存instruction Address的Map

輸出結果
---
![](https://i.imgur.com/7vz2TU5.png)



