#include "SettlementReader.h"
#include <iostream>
using namespace std;


int main(int var_num, const char** var_val)
{
	//SettlementReader reader(var_num, var_val);
	//reader.read();

	string s = "   Ñ¦½òÇÅ    ";
	cout << s << endl;
	s.erase(remove_if(s.begin(), s.end(), [](char a) ->bool {return isspace(a); }), s.end());
	cout << s << endl;
	return 0;
}
