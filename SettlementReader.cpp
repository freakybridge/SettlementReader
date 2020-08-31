
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

// 读取中英文字符串 / 判断是否为中文 / 读取交易记录
vector<string> split_cn_en(string);
bool isa_cn(char p);
void read_trade();

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
		cout << fileInfo.name << (fileInfo.attrib & _A_SUBDIR ? "[folder]" : "[file]") << endl;
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
}

// 读取每个文件
void SettlementReader::read()
{
	for (auto i : filelist)
	{
		ifstream this_file(i);
		if (this_file)
		{
			string line;
			while (getline(this_file, line))
			{
				auto this_line = split_cn_en(line);
				if (find(this_line.begin(), this_line.end(), "成交记录") != this_line.end())
				{
					// 读取该文件中所有交易记录
					unsigned cnt_row = 0;
					while (getline(this_file, line))
					{
						if (cnt_row < 4)
						{
							// 跳过前四行
							++cnt_row;
							continue;
						}
						else
						{
							// 写入交易记录
							auto this_line = split_cn_en(line);
							tradeRec rec;

							// 写入交易时间
							string pattern{ "-[0-9]+(.[0-9]+)?|[0-9]+(.[0-9]+)?" };
							regex r(pattern);
							smatch results;
							if (regex_search(this_line[0], results, r))
								rec.date = stoi(results.str());

							// 写入交易序号
							static unsigned serial = 0;
							rec.serial = ++serial;

							// 写入品种
							rec.product = this_line[2];

							// 写入合约
							string pattern{ "(-[0-9]+(.[0-9]+)?|[0-9]+(.[0-9]+)?)" };
							pattern = "[[:alpha:]]+" + pattern;
							regex r(pattern);
							smatch results;
							if (regex_search(this_line[0], results, r))
								rec.date = stoi(results.str());

							// 写入方向

							// 写入手数

							// 盈亏 / 



						}


					}
					break;
				}
			}

		}
		this_file.close();
	}
}




vector<string> split_cn_en(string in)
{
	vector<string> out;
	vector<char> temp;
	unsigned curr = 0;
	while (true)
	{
		if (isa_cn(in[curr]))
		{
			// 提取汉字
			for (auto i = curr; curr != in.size(); ++i)
			{
				if (!isa_cn(in[i]))
				{
					curr = i - 1;
					out.push_back(string(temp.begin(), temp.end()));
					temp.clear();
					break;
				}
				else
					temp.push_back(in[i]);
			}

		}
		else
		{
			// 提取其他字符
			if (isalnum(in[curr]))
			{
				for (auto i = curr; curr != in.size(); ++i)
				{
					temp.push_back(in[i]);
					if (isa_cn(in[i]) || in[i] == '\0')
					{
						curr = i - 1;
						out.push_back(string(temp.begin(), temp.end()));
						temp.clear();
						break;
					}
				}
			}
		}

		//退出
		if (curr == in.size() - 1)
			break;
		++curr;
	}
	return out;
}

bool isa_cn(char p)
{

	/*汉字的两个字节的最高为都为1,这里采用判断最高位的方法
	将p字节进行移位运算，右移8位，这样，如果移位后是0，
	则说明原来的字节最高位为0，不是1那么也就不是汉字的一个字节
	*/
	return (p & 0x80) != 0 ? true : false;
}

