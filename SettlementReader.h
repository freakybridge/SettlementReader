#pragma once
#ifndef _SETTLEMENT_READER_H_
#define _SETTLEMENT_READER_H_


#include "SpreadSheet.h"
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

// �ɽ���¼
struct tradeRec
{
	unsigned date;
	unsigned serial;
	string product;
	string instrument;
	string direction;
	string mark_oc;
	float price;
	unsigned lots;
	float fee;
	float pnl;
	float pnl_acc;
};


// ���㵥��ȡ��
class SettlementReader
{
public:
	SettlementReader(int, const char **);
	void read();
	void prepare();
	void write();

private:
	string account;
	string path_in;
	string path_out;
	vector<string> filelist;
	vector<tradeRec> trade_rec_raw;
	map<string, tradeRec> trade_rec_sorted;
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
//    //�ƺ�����Ҫ��headers��û��headers�Ļ������û������ļ�������
//
//    sheet.AddHeaders(headers);
//
//    sheet.AddRow(row);
//
//    sheet.Commit();
//
//}
