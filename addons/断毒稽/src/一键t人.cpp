/*
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
#include<ctime>
#include<random>
#include <CQEVE.h>
#include "APPINFO.h"
#include <CQEVE_RequestAddGroup.h>
#include <CQMsgCode.h>
#include <CQEVE_PrivateMsg.h>
#include <CQMsgSend.h>
#include <INItool.h>
using namespace std;
using namespace CQ;
bool debugx = true;//debug模式开关
long long debugqq = 1442766687;//debugqq号
long long targetGroupid = 389333429;//群号码
long long nowqq;
#define DEBUG(x) if(debugx==true)CQ::sendPrivateMsg(debugqq, ((string(x)).c_str()));
#define ainb(x,y) (((string(y)).find((string(x)))!=-1))
//appinfo公开
MUST_AppInfo_RETURN(CQAPPID)





EVE_Enable(Enable)
{
	nowqq = getLoginQQ();
	string filePath = getAppDirectory();
	filePath += "option.ini";
	fstream(filePath, ios::out);

	DEBUG("start");


	return 0;
}

EVE_Disable(Disable)
{
	DEBUG("end");

	return 0;
}

EVE_PrivateMsg_EX(Speak) {
	if (eve.fromQQ != debugqq)return;
	if ainb("启动", eve.message) {
		vector<future<int> >fut;
		//在爆破瞬间更改自己的群名片
		//setGroupCard(targetGroupid, nowqq, "这是骗人的群");
		DEBUG("开始t人");
		vector<GroupMemberInfo>infoList;
		infoList = getGroupMemberList(targetGroupid);
		DEBUG("群总人数="+to_string(infoList.size()));
		clock_t starttime, endtime;
		starttime =clock();
		int syncNum = 0;
		for (auto mem : infoList) {
			//吃瓜保护将含关键字的人
			//if (ainb("淦", mem.名片)) continue;
			if (mem.permissions>=2||mem.QQID == nowqq)continue;
			syncNum++;
			//DEBUG("开启并发" + to_string(syncNum));
			fut.push_back( std::async(std::launch::async,
				[](long long qqid, long long groupid) {
				setGroupKick(groupid, qqid, true);

				return 0;
			}, mem.QQID, targetGroupid));
			
			if (syncNum >= 20) {
				
					fut[syncNum-20].get();
					//DEBUG("结束并发" + to_string(syncNum-10));
				
			}

			//DEBUG(("昵称=" + mem.昵称));
			

		}
		if (syncNum >= 20) {
			for (int a = syncNum - 19; a <= fut.size() - 1; ++a) {
				fut[a].get();
				//DEBUG("结束并发" + to_string(a + 1));
			}
		}
		else if (syncNum > 0) {
			for (int a = 0; a <= fut.size() - 1; ++a) {
				fut[a].get();
			}
		}
		fut.clear();
		endtime =clock();
		DEBUG("耗时="+to_string((double)(endtime - starttime) / CLOCKS_PER_SEC));
	}
	return;
}
*/