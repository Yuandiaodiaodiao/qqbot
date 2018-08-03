#include <CQEVE_GroupMsg.h>
#include <CQcode.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include<Windows.h>
#include <string>
#include<cstdlib>
#include<vector>
#include<future>
#include<map>
#include<queue>
#include<fstream>
#include<sstream>
#include<random>
#include <CQEVE.h>
#include "APPINFO.h"
#include <CQEVE_RequestAddGroup.h>
#include <CQMsgCode.h>
#include <CQEVE_PrivateMsg.h>
#include <CQMsgSend.h>
#include<set>
#include <INItool.h>
using namespace std;
using namespace CQ;
bool debugx = true;//debug模式开关
long long nowqq;
long long debugqq; // debugqq号
set<long long>groupList;
long long targetgroup;
long long protectqq;
string protectname;
#define ainb(x,y) (((string(y)).find((string(x)))!=-1))

#define DEBUG(x) if(debugx==true)CQ::sendPrivateMsg(debugqq, ((string(x)).c_str()));

MUST_AppInfo_RETURN(CQAPPID)							  //appinfo公开


EVE_Enable(Enable) {
	string debuglog;
	nowqq = getLoginQQ();
	debugqq = nowqq;
	debuglog += "登陆qq" + to_string(nowqq);
	string filePath = getAppDirectory();
	filePath += "option.ini";
	fstream f(filePath, ios::in);
	if (f)DEBUG("找不到ini配置文件");
	{
		string s;
		while (f >> s) {
			if (s == "debugqq") {
				f >> debugqq;
			}
			else if (s == "grouplist") {
				getline(f, s);
				stringstream ss(s);
				long long groupid;
				while (ss >> groupid) {
					groupList.insert(groupid);
				}
			}
			else if (s == "debug") {
				f >> debugx;
			}
			else if (s == "targetgroup") {
				f >> targetgroup;
			}
			else if (s == "protectqq") {
				f >> protectqq;
			} 
			else if (s == "protectname") {
				f >> protectname;
			}
		}
	}
	debuglog += " debugqq=" + to_string(debugqq) + "\n";
	for (auto x : groupList) {
		debuglog += " q群:" + to_string(x) + "\n";
	}
	debuglog += " protectqq=" + to_string(protectqq) + "\n";
	debuglog += " targetgroup=" + to_string(targetgroup) + "\n";

	DEBUG("start");
	DEBUG(debuglog);
	return 0;

}
EVE_Disable(Disable) {
	DEBUG("end");
	return 0;
}
EVE_GroupMsg_EX(Group) {

}
future<int>protectasync;
EVE_PrivateMsg_EX(Speak) {
	if (eve.fromQQ == debugqq) {
		if (eve.message == "启动昵称保护") {
			protectasync = std::async(std::launch::async, [](string nickname, long long targetgroup, long long qqid) {
				while (TRUE) {
					Sleep(2 * 1000);
					setGroupCard(targetgroup, qqid, nickname);
				}
				return 0;
			}, protectname, targetgroup, protectqq);
		}
		else if (eve.message == "关闭昵称保护") {
			protectasync.~future();//强行释放
			DEBUG("关闭完成");
		}

	}

}