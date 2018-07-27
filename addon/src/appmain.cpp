#include <CQEVE.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include "APPINFO.h"

#include <sqlite3.h>
#include <string>
using namespace std;
using namespace CQ;

//请加上static,表示这个logger只有本cpp有效
static Logger logger("酷Q样例应用");

string a = "1";
sqlite3 * db;
EVE_Enable(Enable2)
{
	
	
	
	int res = sqlite3_open("test.db", &db);

	if (res != SQLITE_OK) {

		CQ::sendPrivateMsg(295087430, "数据库失败");

		return 0;

	}

	else {

		CQ::sendPrivateMsg(295087430, "数据库ok");

	}
	
	logger.Info("应用被启用");
	string s = "启动";

	return 0;
}

EVE_Disable(Disable2)
{
	logger.Info("应用被停用");
	CQ::sendPrivateMsg(295087430, "数据库关闭");
	sqlite3_close(db);
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)