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
#include<random>
using namespace std;
using namespace CQ;
bool debugx = true;//debug模式开关
long long debugqq = 295087430;//debugqq号
long long groupid = 248639786;//群号码
#define DEBUG(x) if(debugx==true)CQ::sendPrivateMsg(debugqq, ((string(x)).c_str()));
map<long long, int>mapp;
map<long long, int>ban;
std::mt19937 rng;
std::uniform_int_distribution<int> rand4(2, 4);
std::uniform_int_distribution<int> rand100(0, 100);
class users {
public: users(long long uid, int uspeak) { id = uid; speak = uspeak; }
		long long id; int speak; string nickname;
		bool operator < (const users& b)const {
			return speak < b.speak;
		}
};
EVE_Enable(Enable) {
	rng.seed(std::random_device()());
	fstream f("db.txt",ios::in);
	if (!f) {
		f.close();
		fstream f1("db.txt", ios::out);

		f1.close();
		f.open("db.txt", ios::in);
	}
	else {
		while (!f.eof()) {
			long long id;
			int times;
			f >> id >> times;
			if (times > 999999)continue;
			mapp.insert(make_pair(id, times));
		}
	}
	f.close();
	DEBUG("start");
	return 0;
}

EVE_Disable(Disable)
{
	fstream f("db.txt", ios::out);
	for (auto x : mapp) 	f << x.first << " " << x.second << "\n";
	f.close();
	DEBUG("save");
	mapp.clear();
	return 0;
}
class banmoretalk {
public:long long qqid;
	   int times;
	   banmoretalk() { qqid = 0; times = 0; }
};
map<long long, string> lastmessage;
map<long long, banmoretalk>bantalk;
string atinfo = "[CQ:at,qq=1442766687";
long long lastmsgid = 0;
long long getlastmsgid() {
	return lastmsgid;
}

EVE_GroupMsg_EX(Group1) {
	bool fudu = false;
	string msg = eve.message;
	long long fromgroup = eve.fromGroup;
	long long qqid = eve.fromQQ;
	long long msgid = eve.msgId;
	/*if (fromgroup == 725516089) {
		lastmsgid = msgid;
		std::future<int> f1 = std::async(std::launch::async, [](long long di, long long groupid) {
			//DEBUG(string("开始撤回"));
			Sleep(60 * 1000 * 5 + 3);
			long long msgid2 = getlastmsgid();
			if (di == msgid2) {
				DEBUG("ok");
				sendGroupMsg(groupid, "已经五分钟没人理你了恭喜楼上成为冷群王");
			}

			return 8;
		}, lastmsgid, 725516089);

	}*/
	if (msg == lastmessage[fromgroup]&&!fudu)
	{ eve.sendMsg(msg);
	fudu = true;
	lastmessage[fromgroup] = "";
	}
	else {
		lastmessage[fromgroup] = msg;
	}
	if ((msg.find("？")!=-1||msg.find("?") != -1)&& rand4(rng) == 3&&!fudu) {
		fudu = true;
		sendGroupMsg(eve.fromGroup, msg);
	}
	if (msg.find("稽气人设置@信息") != -1) {
		//atinfo = msg.substr(0, 20);
		DEBUG("设置@信息为" + atinfo);
	}
	if (msg.find("嘤") != -1 && rand4(rng) == 3&&!fudu) {
		fudu = true;
		sendGroupMsg(eve.fromGroup, "嘤嘤嘤");
	}
	string msg1 = eve.messageRAW;
	if (msg1.find(atinfo) != -1) {
		DEBUG("收到@" + msg1);
		DEBUG(eve.messageRAW);
	}
	/*
	if (rand100(rng) == 60) { 
		auto msg = eve.sendMsg();
		msg<<code::image("窥屏.jpg")<<send;
		DEBUG("表情");
	}*/
	if (eve.fromGroup != groupid)return;
	if (qqid == bantalk[fromgroup].qqid) {
		bantalk[fromgroup].times++;
		if (bantalk[fromgroup].times >= 5) {
			if (getGroupMemberInfo(groupid, qqid, true).permissions < 2) {
				setGroupBan(groupid, qqid, 60);
				eve.sendMsg("检测到过度水群");
			}
		}
	}
	else {
		bantalk[fromgroup].qqid = qqid;
		bantalk[fromgroup].times = 1;
	}
	
	
	mapp[qqid]++;

	if ((msg.find("小可爱") != -1) && (msg.find("水群") != -1)) {
		ban[qqid]++;
		if (ban[qqid] >= rand4(rng) ){
			GroupMemberInfo i = getGroupMemberInfo(groupid, qqid, true);
			
			string resmsg;
			if (i.permissions >= 2) {		
				resmsg="不告诉你";
			}
			else {
				setGroupBan(groupid, qqid, 60);
				resmsg = "就你话多";
			}
			string nickname = i.名片;
			string sx = "@" + nickname + resmsg;
			sendGroupMsg(groupid,sx);
			ban[qqid] = 0;
			return;
		}
		DEBUG("被@" + eve.message);
		priority_queue<users>que;
		for (auto x : mapp)que.push(users(x.first, x.second));
		int times = 0;
		string strs = "今日水群排行";
		while (!que.empty()) {
			if (times++ >= 5)break;
			auto x = que.top();
			GroupMemberInfo i = getGroupMemberInfo(groupid, x.id, true);
			string nickname = i.名片;
			que.pop();
			strs += "\n"+nickname + "  " + to_string(x.speak) + "条";
		}
		long long mesid = sendGroupMsg(groupid, strs);
	}
	if (msg.find(atinfo) != -1&&(msg.find("水")!=-1)&&(msg.find("我") != -1)&& (msg.find("记录")!=-1||msg.find("多少") != -1)) {
		//setGroupBan(groupid, qqid, 120);
		sendGroupMsg(groupid, string(to_string(mapp[qqid])+"条").c_str());
		
	
	}
	

}