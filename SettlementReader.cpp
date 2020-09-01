
#include "SettlementReader.h"
#include <io.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::regex;
using std::smatch;
using std::to_string;

// 判断是否为中文 / 寻找文件头 / 读取交易记录
bool isa_cn(char p);
bool find_header(string&);
vector<string> read_trade_detail(string&);

// 初始化
SettlementReader::SettlementReader(int num, const char** varlist)
{
	// 保存工作路径
	switch (num)
	{
	case 2:
		account = varlist[1];
		break;
	case 3:
		account = varlist[1];
		path_in = path_out = varlist[2];
		break;
	case 4:
		account = varlist[1];
		path_in = varlist[2];
		path_out = varlist[3];
		break;
	default:
		cerr << "Unexpected Input Argument, Please check !" << endl;
		break;
	}

	// 读取目录中所有文件
	// 初始化
	long hFile = 0;
	struct _finddata_t fileInfo;
	string path_temp = path_in;

	// 搜索第一个文件
	if ((hFile = _findfirst(path_temp.append("\\*").c_str(), &fileInfo)) == -1)
		return;

	// 记录每个文件
	do
	{
		if (!(fileInfo.attrib & _A_SUBDIR))
		{
			filelist.push_back(path_in + "\\" + fileInfo.name);
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	cout << "Find " + to_string(filelist.size() - 1) + " settlement files." << endl;
}

// 读取每个文件
void SettlementReader::read()
{
	for (auto i : filelist)
	{
		ifstream this_file(i);
		string line;
		while (getline(this_file, line))
		{
			// 寻找文件头 / 跳过前四行
			if (!find_header(line))
				continue;
			for (unsigned j = 0; j != 4; ++j)
				getline(this_file, line);

			// 扫描
			while (getline(this_file, line))
			{
				// 判定是否结束输入
				string check = line;
				check.erase(unique(check.begin(), check.end()), check.end());
				if (check.size() == 2 && check == "-\r")
					goto exit_file;
				auto this_line = read_trade_detail(line);

				// 写入交易时间 / 写入交易序号
				tradeRec rec;
				rec.date = stoi(this_line[1]);
				static unsigned serial = 0;
				rec.serial = ++serial;

				// 写入交易品种 / 交易合约 / 交易方向 / 开平
				rec.product = this_line[3];
				rec.instrument = this_line[4];
				rec.direction = this_line[5];
				rec.mark_oc = this_line[10];

				// 写入成交价格 / 手数 / 手续费 / 平仓盈亏 / 累计盈亏
				rec.price = stof(this_line[7]);
				rec.lots = stoi(this_line[8]);
				rec.fee = stof(this_line[11]);
				rec.pnl = stof(this_line[12]);
				rec.pnl_acc = 0;
				trade_rec_raw.push_back(rec);
			}
		}
		exit_file:
		this_file.close();
	}
	cout << "All settlement txt file loaded." << endl;
}

bool find_header(string& in)
{
	for (auto curr = in.begin(), prev = curr; curr != in.end(); ++curr)
	{
		if (isa_cn(*prev) && !isa_cn(*curr) && string(prev, curr) == "成交记录")
			return true;
		if (isa_cn(*curr) && !isa_cn(*prev))
			prev = curr;
	}
	return false;
}

bool isa_cn(char p)
{

	/*汉字的两个字节的最高为都为1,这里采用判断最高位的方法
	将p字节进行移位运算，右移8位，这样，如果移位后是0，
	则说明原来的字节最高位为0，不是1那么也就不是汉字的一个字节
	*/
	return (p & 0x80) != 0 ? true : false;
}

vector<string> read_trade_detail(string& in)
{
	// 分离分隔符
	vector<string> out;
	vector<char> temp;
	for (auto i : in)
	{
		if (i == '|')
		{
			out.push_back(string(temp.begin(), temp.end()));
			temp.clear();
		}
		else
		{
			if (isa_cn(i))
				temp.push_back(i);
			else if (!isspace(i))
				temp.push_back(i);
		}
	}
	return out;
}

void SettlementReader::prepare()
{
	// 入选所有品种
	vector<string> product;
	for (auto i : trade_rec_raw)
		if (find(product.begin(), product.end(), i.product) == product.end())
			product.push_back(i.product);

	// 提取每个品种交易
	for (auto& i : product)
	{

		cout << i << endl;
	}
	
}