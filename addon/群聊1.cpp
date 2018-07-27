#include <CQEVE_GroupMsg.h>
#include <CQcode.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include <sqlite3.h>
#include <string>
#include<cstdlib>
#include<vector>
#include<future>

using namespace std;
using namespace CQ;
bool debugx = true;
#define DEBUG(x) if(debugx==true)CQ::sendPrivateMsg(295087430, ((string(x)).c_str()));
int calltimes;
long long qqid;
class users {
public: users(long long uid, int uspeak) { id = uid; speak = uspeak; }
		long long id; int speak; string nickname;
};
vector<users>user;
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	calltimes++;
	string qqids = argv[0];
	if (qqids == to_string(qqid)) {
		calltimes = atoi(argv[1])+1;
	}
	
	
	return 0;
}
static int callmax(void *NotUsed, int argc, char **argv, char **azColName) {
	user.push_back(users(atoll(argv[0]), atoi(argv[1])));
	return 0;
}
static int callvoid(void *NotUsed, int argc, char **argv, char **azColName) { return 0; }
EVE_GroupMsg_EX(Group1) {
	long long groupid = 248639786;
	user.clear();
	int rc;
	const char* data = "Callback function called";
	string debuginf;
	extern sqlite3 *db;
	calltimes = 0;
	if (eve.fromGroup != groupid)return;
	 qqid = eve.fromQQ;
	string sql = "select * from rank where id="+to_string(qqid)+" ;";
	char *zErrMsg = 0;
	rc=sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		DEBUG(debuginf="err");
		sqlite3_free(zErrMsg);
	}
	debuginf= "calltimes="+to_string(calltimes);
	DEBUG(debuginf);
	//sqlite3_open("test.db", &db);
	if (calltimes >=1) {
		 sql = "UPDATE rank set SPEAK = " + to_string(calltimes) + " WHERE ID = " + to_string(qqid) + ";";
		DEBUG("更新");
	}else
		if (calltimes == 0) {
			 sql = "INSERT INTO rank (ID,SPEAK) VALUES ("+to_string(qqid)+",1);";
			DEBUG("增添");
		}
	DEBUG(sql);
	rc=sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		debuginf = "err";
		DEBUG(debuginf);
		//sqlite3_free(zErrMsg);
	}

	if (eve.message.find("哪个小可爱最能水群")!=-1) {
		DEBUG("被@" + eve.message);
		sql = "select * from rank order by SPEAK DESC limit 5;";
		rc = sqlite3_exec(db, sql.c_str(), callmax, 0, &zErrMsg);
		string strs = "今日水群排行\n";
		for (auto x : user) {
			GroupMemberInfo i=getGroupMemberInfo(groupid, x.id,true);
			string names = i.名片;
			//DEBUG(names);
			strs += names + " " + to_string(x.speak) + "条\n";
		}
		long long mesid=sendGroupMsg(groupid, strs);
		/*DEBUG("mesid="+to_string(mesid));
		std::future<int> f1 = std::async(std::launch::async,[](long long di) {
			DEBUG(string("开始撤回"));
			Sleep(30 * 1000);
			DEBUG(string("撤回"+to_string(di)));
			deleteMsg(di);
			return 8;
		},mesid);
		*/
	}
	//eve.sendMsg("呵,垃圾");
}