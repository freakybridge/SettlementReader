#pragma once
#ifndef _SETTLEMENT_READER_H_
#define _SETTLEMENT_READER_H_


#include "SpreadSheet.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// 成交记录
struct tradeRec
{
	unsigned date;
	unsigned serial;
	string product;
	string instrument;
	string direction;
	float price;
	unsigned lots;
	float pnl;
	float pnl_acc;
};


// 结算单读取类
class SettlementReader
{
public:
	SettlementReader(int, const char **);
	void read();
	void write();

private:
	string account;
	string path_in;
	string path_out;
	vector<string> filelist;
	vector<tradeRec> trade_rec;
};

#endif // !_SETTLEMENT_READER_H_

//#include "SpreadSheet.h"
//
//int main()
//
//{
//
//    CSpreadSheet sheet("sheetfilename.xls", "abc");
//
//    CStringArray headers, row;
//
//    headers.Add("str1");
//
//    headers.Add("str2");
//
//    headers.Add("str3");
//
//    row.Add("hello");
//
//    row.Add("world");
//
//    row.Add("!");
//
//
//
//    sheet.BeginTransaction();
//
//    //似乎必须要有headers，没有headers的话好像就没有输出文件。。。
//
//    sheet.AddHeaders(headers);
//
//    sheet.AddRow(row);
//
//    sheet.Commit();
//
//}
