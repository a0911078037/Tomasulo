#include <iostream>
#include <string>
#include<vector>
#include <iomanip>
#include<sstream>
#include<fstream>
#define rs_alu_size 3
#define rs_mul_size 2
#define ADD_cycle 2
#define SUB_cycle 2
#define DIV_cycle 40
#define MUL_cycle 10
using namespace std;
class RS_ALU_buffer {
public:
	string inst = "";
	int pos = -1;
	int cycle = -1;
	bool full() {
		if (inst == "" && pos == -1 && cycle == -1) {
			return false;
		}
		else {
			return true;
		}
	}
	void clear() {
		inst = "";
		pos = -1;
		cycle = -1;
	}
};
class RS_MUL_buffer {
public:
	string inst = "";
	int pos = -1;
	int cycle = -1;
	bool full() {
		if (inst == "" && pos == -1 && cycle == -1) {
			return false;
		}
		else {
			return true;
		}
	}
	void clear() {
		inst = "";
		pos = -1;
		cycle = -1;
	}
};
class inst
{
public:
	string operation;
	string ins;
	string val1;
	string val2;
	string val3;
	bool full = false;
	void clear() {
		operation = "";
		ins = "";
		val1 = "";
		val2 = "";
		val3 = "";
		full = false;
	}
};

class RS_ALU {
public:
	string rs_name = "";
	string operation = "";
	string val1 = "";
	string val2 = "";
	bool new_inst = false;

	void clear() {
		operation = "";
		val1 = "";
		val2 = "";
	}
	bool full() {
		if (operation == "" && val1 == "" && val2 == "") {
			return false;
		}
		else {
			return true;
		}
	}
	bool check_rs_alu() {
		int pos1 = (val1).find("RS");
		int pos2 = (val2).find("RS");
		if (pos1 == -1 && pos2 == -1) {
			return true;
		}
		return false;
	}
	int return_cycle() {
		if (operation == "+") {
			return ADD_cycle;
		}
		else {
			return SUB_cycle;
		}
	}
};
class RS_MUL {
public:
	string rs_name = "";
	string operation = "";
	string val1 = "";
	string val2 = "";
	bool new_inst = false;

	void clear() {
		operation = "";
		val1 = "";
		val2 = "";
	}
	bool full() {
		if (operation == "" && val1 == "" && val2 == "") {
			return false;
		}
		else {
			return true;
		}
	}
	bool check_rs_mul() {
		int pos1 = (val1).find("RS");
		int pos2 = (val2).find("RS");
		if (pos1 == -1 && pos2 == -1) {
			return true;
		}
		return false;
	}
	int return_cycle() {
		if (operation == "*") {
			return MUL_cycle;
		}
		else {
			return DIV_cycle;
		}
	}

};
class RF {
public:
	string rf_name = "";
	int val = 0;
};
class RAT {
public:
	string rat_name = "";
	string val = "";
};
void output(int cycle, vector<RS_ALU>RS_ALU, vector<RS_MUL>RS_MUL, vector<RAT>RAT, vector<RF>RF, RS_ALU_buffer RS_ALU_buffer, RS_MUL_buffer RS_MUL_buffer) {
	cout << "Cycle:" << cycle << endl;
	cout << "   -RF--" << '\t' << "   -RAT-" << endl;
	for (int i = 0; i < RF.size(); i++)
	{
		cout << setw(3) << RF[i].rf_name << "|" << setw(3) << RF[i].val << "|" << '\t';
		cout << setw(3) << RAT[i].rat_name << "|" << setw(3) << RAT[i].val << "|" << endl;
	}
	cout << "   -----" << '\t' << "   -----" << endl << endl;
	cout << "   ----RS_ALU---" << '\t' << "   ----RS_MUL---" << endl;
	for (int i = 0; i < RS_ALU.size(); i++)
	{
		if (i != RS_MUL.size()) {
			cout << RS_ALU[i].rs_name << "|" << setw(3) << RS_ALU[i].operation << "|" << setw(3) << RS_ALU[i].val1 << "|" << setw(3) << RS_ALU[i].val2 << "|" << '\t' << RS_MUL[i].rs_name << "|" << setw(3) << RS_MUL[i].operation << "|" << setw(3) << RS_MUL[i].val1 << "|" << setw(3) << RS_MUL[i].val2 << "|" << endl;
		}
		else
		{
			cout << RS_ALU[i].rs_name << "|" << setw(3) << RS_ALU[i].operation << "|" << setw(3) << RS_ALU[i].val1 << "|" << setw(3) << RS_ALU[i].val2 << "|" << endl;
		}
	}
	cout << "   -------------" << '\t' << "   -------------" << endl;
	if (!RS_ALU_buffer.full()) {
		cout << "BUFFER: empty" << '\t' << '\t';
	}
	else {
		cout << "BUFFER: " << RS_ALU_buffer.inst << '\t';
	}
	if (!RS_MUL_buffer.full()) {
		cout << "BUFFER: empty" << endl << endl;
	}
	else {
		cout << "BUFFER: " << RS_MUL_buffer.inst << endl << endl;
	}
}
inst de_code(string ins) {
	inst temp;
	vector <string>res;
	string result;
	stringstream input(ins);
	while (input >> result)
		res.push_back(result);
	//-----去除逗號
	res[1].pop_back();
	res[2].pop_back();
	temp.ins = res[0];
	temp.val1 = res[1];
	temp.val2 = res[2];
	temp.val3 = res[3];
	if (temp.ins == "ADDI" || temp.ins == "ADD") {
		temp.operation = "+";
		temp.full = true;
	}
	else if (temp.ins == "SUB") {
		temp.operation = "-";
		temp.full = true;
	}
	else if (temp.ins == "MUL") {
		temp.operation = "*";
		temp.full = true;
	}
	else if (temp.ins == "DIV") {
		temp.operation = "/";
		temp.full = true;
	}
	else {
		cout << "inst not support or incorrect!";
		system("pause");
	}
	return temp;
}
void inst_into_RSRAT(inst& inst, vector<RS_ALU>& RS_ALU, vector<RS_MUL>& RS_MUL, vector<RAT>& RAT, vector<RF>& RF) {
	if (inst.operation == "+" || inst.operation == "-") {
		for (int i = 0; i < rs_alu_size; i++)
		{
			if (!RS_ALU[i].full()) {
				//放入RS
				RS_ALU[i].operation = inst.operation;
				RS_ALU[i].val1 = inst.val2;
				RS_ALU[i].val2 = inst.val3;
				RS_ALU[i].new_inst = true;
				//確認數值能否抓回來
				//從RAT抓值
				for (int j = 0; j < RAT.size(); j++)
				{
					if (RAT[j].rat_name == RS_ALU[i].val1) {
						if (RAT[j].val != "") {
							RS_ALU[i].val1 = RAT[j].val;
						}
					}
					if (RAT[j].rat_name == RS_ALU[i].val2) {
						if (RAT[j].val != "") {
							RS_ALU[i].val2 = RAT[j].val;
						}
					}
				}
				//從RF抓值
				for (int j = 0; j < RF.size(); j++)
				{
					if (RS_ALU[i].val1.find("RS") == -1)
					{
						if (RF[j].rf_name == RS_ALU[i].val1) {
							RS_ALU[i].val1 = to_string(RF[j].val);
						}
					}
					if (RS_ALU[i].val2.find("RS") == -1)
					{
						if (RF[j].rf_name == RS_ALU[i].val2) {
							RS_ALU[i].val2 = to_string(RF[j].val);
						}
					}
				}
				//更新RAT
				for (int j = 0; j < RAT.size(); j++)
				{
					if (RAT[j].rat_name == inst.val1) {
						RAT[j].val = RS_ALU[i].rs_name;
						break;
					}
				}
				inst.clear();
				break;
			}
		}
	}
	else {
		for (int i = 0; i < rs_mul_size; i++)
		{
			if (!RS_MUL[i].full()) {
				//放入RS
				RS_MUL[i].operation = inst.operation;
				RS_MUL[i].val1 = inst.val2;
				RS_MUL[i].val2 = inst.val3;
				RS_MUL[i].new_inst = true;

				//確認數值能否抓回來
				//從RAT抓值
				for (int j = 0; j < RAT.size(); j++)
				{
					if (RAT[j].rat_name == RS_MUL[i].val1) {
						if (RAT[j].val != "") {
							RS_MUL[i].val1 = RAT[j].val;
						}
					}
					if (RAT[j].rat_name == RS_MUL[i].val2) {
						if (RAT[j].val != "") {
							RS_MUL[i].val2 = RAT[j].val;
						}
					}
				}
				//從RF抓值
				for (int j = 0; j < RF.size(); j++)
				{
					if (RS_MUL[i].val1.find("RS") == -1)
					{
						if (RF[j].rf_name == RS_MUL[i].val1) {
							RS_MUL[i].val1 = to_string(RF[j].val);
						}
					}
					if (RS_MUL[i].val2.find("RS") == -1)
					{
						if (RF[j].rf_name == RS_MUL[i].val2) {
							RS_MUL[i].val2 = to_string(RF[j].val);
						}
					}
				}
				//更新RAT
				for (int j = 0; j < RAT.size(); j++)
				{
					if (RAT[j].rat_name == inst.val1) {
						RAT[j].val = RS_MUL[i].rs_name;
						break;
					}
				}
				inst.clear();
				break;
			}
		}
	}


}

void bordcast(vector<RS_ALU>& RS_ALU, vector<RS_MUL>& RS_MUL, RS_MUL_buffer& RS_MUL_buffer, RS_ALU_buffer& RS_ALU_buffer) {
	if (!RS_ALU_buffer.full()) {
		for (int i = 0; i < RS_ALU.size(); i++)
		{
			if (RS_ALU[i].full() && RS_ALU[i].new_inst == false)
			{
				if (RS_ALU[i].check_rs_alu()) { //檢查數值是否ready
					//將inst 放入buffur
					RS_ALU_buffer.inst = "(" + RS_ALU[i].rs_name + ") " + RS_ALU[i].val1 + RS_ALU[i].operation + RS_ALU[i].val2;
					RS_ALU_buffer.cycle = RS_ALU[i].return_cycle();
					RS_ALU_buffer.pos = i;
					break;
				}
			}

		}
	}
	if (!RS_MUL_buffer.full())
	{
		for (int i = 0; i < RS_MUL.size(); i++)
		{
			if (RS_MUL[i].full() && RS_MUL[i].new_inst == false)
			{
				if (RS_MUL[i].check_rs_mul()) {
					RS_MUL_buffer.inst = "(" + RS_MUL[i].rs_name + ") " + RS_MUL[i].val1 + RS_MUL[i].operation + RS_MUL[i].val2;
					RS_MUL_buffer.cycle = RS_MUL[i].return_cycle();
					RS_MUL_buffer.pos = i;
					break;
				}
			}
		}
	}
}
void execute(RS_MUL_buffer& RS_MUL_buffer, RS_ALU_buffer& RS_ALU_buffer, vector<RS_ALU>& RS_ALU, vector<RS_MUL>& RS_MUL, vector<RAT>& RAT, vector<RF>& RF) {
	if (RS_ALU_buffer.full()) {
		if (RS_ALU_buffer.cycle == 0) {
			// write back
			int pos = RS_ALU_buffer.pos;
			int val = -1;
			if (RS_ALU[pos].operation == "+") {
				val = stoi(RS_ALU[pos].val1) + stoi(RS_ALU[pos].val2);
			}
			else {
				val = stoi(RS_ALU[pos].val1) - stoi(RS_ALU[pos].val2);
			}
			for (int i = 0; i < RAT.size(); i++)
			{
				if (RAT[i].val == RS_ALU[pos].rs_name) {
					RAT[i].val = "";
					RF[i].val = val;
					break;
				}
			}
			//將數值 forwading 回去到RS裡面
			for (int i = 0; i < RS_ALU.size(); i++)
			{
				if (i==pos)
				{
					continue;
				}
				if (RS_ALU[i].val1==RS_ALU[pos].rs_name)
				{
					RS_ALU[i].val1 = to_string(val);
				}
				if (RS_ALU[i].val2 == RS_ALU[pos].rs_name)
				{
					RS_ALU[i].val2 = to_string(val);
				}
			}
			for (int i = 0; i < RS_MUL.size(); i++)
			{
				if (RS_MUL[i].val1 == RS_ALU[pos].rs_name)
				{
					RS_MUL[i].val1 = to_string(val);
				}
				if (RS_MUL[i].val2 == RS_ALU[pos].rs_name)
				{
					RS_MUL[i].val2 = to_string(val);
				}
			}
			RS_ALU_buffer.clear();
			RS_ALU[pos].clear();
		}
	}
	if (RS_MUL_buffer.full()) {
		if (RS_MUL_buffer.cycle == 0) {
			// write back
			int pos = RS_MUL_buffer.pos;
			int val = -1;
			if (RS_MUL[pos].operation == "*") {
				val = stoi(RS_MUL[pos].val1) * stoi(RS_MUL[pos].val2);
			}
			else {
				val = stoi(RS_MUL[pos].val1) / stoi(RS_MUL[pos].val2);
			}
			for (int i = 0; i < RAT.size(); i++)
			{
				if (RAT[i].val == RS_MUL[pos].rs_name) {
					RAT[i].val = "";
					RF[i].val = val;
					break;
				}
			}
			//將數值 forwading 回去到RS裡面
			for (int i = 0; i < RS_MUL.size(); i++)
			{
				if (i == pos)
				{
					continue;
				}
				if (RS_MUL[i].val1 == RS_MUL[pos].rs_name)
				{
					RS_MUL[i].val1 = to_string(val);
				}
				if (RS_MUL[i].val2 == RS_MUL[pos].rs_name)
				{
					RS_MUL[i].val2 = to_string(val);
				}
			}
			for (int i = 0; i < RS_ALU.size(); i++)
			{
				if (RS_ALU[i].val1 == RS_MUL[pos].rs_name)
				{
					RS_ALU[i].val1 = to_string(val);
				}
				if (RS_ALU[i].val2 == RS_MUL[pos].rs_name)
				{
					RS_ALU[i].val2 = to_string(val);
				}
			}
			RS_MUL_buffer.clear();
			RS_MUL[pos].clear();
		}
	}
}
bool done_check(vector<RS_ALU>& RS_ALU, vector<RS_MUL>& RS_MUL, vector<string>& IQ,int ins_pointer) {
	if (ins_pointer!=IQ.size()) {
		return false;
	}
	for (int i = 0; i < RS_ALU.size(); i++)
	{
		if (RS_ALU[i].full()) {
			return false;
		}
	}
	for (int i = 0; i < RS_MUL.size(); i++)
	{
		if (RS_MUL[i].full()) {
			return false;
		}
	}
	return true;
}
void run_code(vector<string>& IQ, vector<RS_ALU>& RS_ALU, vector<RS_MUL>& RS_MUL, vector<RAT>& RAT, vector<RF>& RF, RS_MUL_buffer& RS_MUL_buffer, RS_ALU_buffer& RS_ALU_buffer) {
	inst inst;
	int cycle = 1;
	int inst_pointer = 0;
	while (!done_check(RS_ALU,RS_MUL,IQ,inst_pointer))
	{
		if (IQ.size()!=inst_pointer)
		{
			if (inst.full == false) {  //如果還沒issue
			//------取得inst
				string ins = IQ[inst_pointer];
				inst_pointer++;
				//-------將inst decode 並將結果回傳到變數儲存
				inst = de_code(ins);
			}
		}
		// 尋找inst是否有空位放入RS
		execute(RS_MUL_buffer, RS_ALU_buffer, RS_ALU, RS_MUL, RAT, RF);
		inst_into_RSRAT(inst, RS_ALU, RS_MUL, RAT, RF);
		bordcast(RS_ALU, RS_MUL, RS_MUL_buffer, RS_ALU_buffer);
		execute(RS_MUL_buffer, RS_ALU_buffer, RS_ALU, RS_MUL, RAT, RF);
		output(cycle, RS_ALU, RS_MUL, RAT, RF, RS_ALU_buffer, RS_MUL_buffer);
		//更新inst狀態
		for (int i = 0; i < RS_ALU.size(); i++)
		{
			if (RS_ALU[i].full()&&RS_ALU[i].new_inst==true)
			{
				RS_ALU[i].new_inst = false;
			}
		}
		for (int i = 0; i < RS_MUL.size(); i++)
		{
			if (RS_MUL[i].full() && RS_MUL[i].new_inst == true)
			{
				RS_MUL[i].new_inst = false;
			}
		}
		if (RS_ALU_buffer.full()) {
			RS_ALU_buffer.cycle--;
		}
		if (RS_MUL_buffer.full())
		{
			RS_MUL_buffer.cycle--;
		}
		cycle++;
	}
}
int main() {
	vector<string> IQ;
	vector<RS_ALU>RS_ALU;
	vector<RS_MUL>RS_MUL;
	vector<RAT>RAT;
	vector<RF>RF;
	RS_ALU_buffer RS_ALU_buffer;
	RS_MUL_buffer RS_MUL_buffer;
	RS_ALU.resize(rs_alu_size);
	RS_MUL.resize(rs_mul_size);
	//------------初始化RS
	for (int i = 0; i < rs_alu_size; i++)
	{
		RS_ALU[i].rs_name = "RS" + to_string(i + 1);
	}
	for (int i = 0; i < rs_mul_size; i++)
	{
		RS_MUL[i].rs_name = "RS" + to_string(rs_alu_size + i + 1);
	}
	//------------讀取inst 到 IQ
	fstream file;
	file.open("input.txt", ios::in);
	if (!file) {
		cout << "file could not open" << endl;
		return -1;
	}
	string inst_temp;
	while (getline(file, inst_temp))
	{
		IQ.push_back(inst_temp);
	}
	//------------尋找inst 所需要的register 編號
	int found = 1;
	int size = 1;
	while (found != -1)
	{
		string count = to_string(size);
		for (int i = 0; i < IQ.size(); i++)
		{
			found = IQ[i].find(count);
			if (found != -1) {
				size++;
				break;
			}
		}
	}
	//-------初始化RAT&RF
	RAT.resize(size - 1);
	RF.resize(size - 1);
	int count = 0;
	for (int i = 0; i < size - 1; i++)
	{
		RAT[i].rat_name = "F" + to_string(i + 1);
		RF[i].rf_name = "F" + to_string(i + 1);
		RF[i].val = count;
		count += 2;
	}
	run_code(IQ, RS_ALU, RS_MUL, RAT, RF, RS_MUL_buffer, RS_ALU_buffer);
}