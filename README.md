# Tomasulo
Tomasulo algorithm 
# 如何使用
將要測試的input放在資料夾裡，並檔名也取為input.txt<br>
會直接輸出每個 cycle 的狀態
# 條件
目前inst.只支援簡單的 (add，addi，sub，div，mul)<br>
在程式碼的最上方define裡可以設定個別的execute cycle
# 程式流程
* 從檔案讀出各別的inst，並放進IQ裡
* 當RS有空位開始分割 inst 並 issue ，並且每個cycle都會搜索準備好的數值
* 數值準備好了之後進去buufer，開始execute
* execute cycle 為0之後開始write result，並放回RF、RAT以及RS裡
* 當檢查到 IQ RS 都為空後，代表程式執行完畢
# 程式解說
```c++
  //設定RS大小
  #define rs_alu_size 3
  #define rs_mul_size 2
  //設定execute cucle
  #define ADD_cycle 2
  #define SUB_cycle 2
  #define DIV_cycle 40
  #define MUL_cycle 10
```
### 各別buffer RS RAT等等設定
```c++
class RS_ALU_buffer {       //RS buffer的設定 MUL的也一樣
public:
	string inst = "";         //儲存inst的地方
	int pos = -1;             //儲存在RS 的位置
	int cycle = -1;           //儲存execute cycle
	bool full() {             //檢查當前的位置是否有資料 ex RS_ALU_buffer[i]
		if (inst == "" && pos == -1 && cycle == -1) {
			return false;
		}
		else {
			return true;
		}
	}
	void clear() {            //刪除當前位置的buffer
		inst = "";
		pos = -1;
		cycle = -1;
	}
};
```
```c++
class RS_ALU {        //RS 設定
public:
	string rs_name = "";    //儲存 inst. name ex:add，addi
	string operation = "";  //儲存運算符號
	string val1 = "";       //儲存數值
	string val2 = "";       //儲存數值
	bool new_inst = false;  //確認是否新 issue 的指令

	void clear() {        //清空當前位置的數值
		operation = "";
		val1 = "";
		val2 = "";
	}
	bool full() {         //檢查是否已有資料
		if (operation == "" && val1 == "" && val2 == "") {
			return false;
		}
		else {
			return true;
		}
	}
	bool check_rs_alu() {       //檢查當前的值是否ready 是否數值全部準備好了
		int pos1 = (val1).find("RS");
		int pos2 = (val2).find("RS");
		if (pos1 == -1 && pos2 == -1) {
			return true;
		}
		return false;
	}
	int return_cycle() {          //看運算符號來回傳 execute cycle
		if (operation == "+") {
			return ADD_cycle;
		}
		else {
			return SUB_cycle;
		}
	}
};
```
```c++
class RF {            //RF設定
public:
	string rf_name = "";  //RF名稱 ex:F1,F2等等
	int val = 0;            //RF數值
};
```
```c++
class RAT {         //RAT設定
public:
	string rat_name = "";
	string val = "";
};
```
