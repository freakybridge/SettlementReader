
#include "SettlementReader.h"
#include <io.h>
#include <iostream>
#include <fstream>
#include <algorithm>
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

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
				cout << line.size() << endl;
				line.erase(
					remove_if(line.begin(), line.end(), [](char a) ->bool {return isspace(a); }),
					line.end());
				cout << line << endl;
			}

		}

		this_file.close();
	}
}