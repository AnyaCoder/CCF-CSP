#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
const int MAXSIZE = 10010;  
 
int n,Tdef,Tmax,Tmin,curTime;
struct IP
{
	int state;//未分配、待分配、占用、过期 0 1 2 3
	int t;/* 过期时间 */
	string owner;//占用主机名  处于未分配状态的 IP 地址没有占用者，而其余三种状态的 IP 地址均有一名占用者
}ip[MAXSIZE];
 
void upadate_ip_state()
{
	for(int i = 1;i<=n; i++)
		if(ip[i].t==0)
			continue;
		else if(ip[i].t<=curTime)
		{
			if(ip[i].state==1)
			{
				ip[i].state = 0;
				ip[i].owner = "";
				ip[i].t = 0;
			}
			else
			{
				ip[i].state = 3;
				ip[i].t = 0;
			}
		}
}
void initial_ip()
{
	for(int i = 1;i<n;i++)
	{
		ip[i].state = 0;
		ip[i].owner = "";
		ip[i].t = 0;
	}
	return ;
}
 
 
void test()
{
	for(int i = 1; i<n; i++)
		cout<<ip[i].state<<"\t";
	cout<<endl;
}
 
int get_suit_time(int t_p)
{
	if(t_p==0)
		return curTime+Tdef;
	else if(t_p<Tmin+curTime)
		return curTime+Tmin;
	else if(t_p>Tmax+curTime)
		return curTime+Tmax;
	else
		return t_p;
}
 
int get_ip_by_state(int s)
{
	for(int i = 1;i<n; i++)
		if(ip[i].state==s)
			return i;
	return 0;
}
 
 
int get_ip_by_owner(string own)
{
	for(int i = 1 ;i<n; i++)
		if(ip[i].owner==own)
			return i;
 
	return 0;
}
 
int main()
{
 
	string H;
	cin>>n>>Tdef>>Tmax>>Tmin>>H;
	n+=1;
	int len;
	cin>>len;
	initial_ip();
	while(len--)
	{
		string client,server,type;
		int target_ip,time;//过期时刻
		cin>>curTime>>client>>server>>type>>target_ip>>time;
		
		if(server!="*"&&server!=H)
			if(type!="REQ")
				continue;
		if(type!="DIS"&&type!="REQ") continue;
		if(server=="*"&&type!="DIS"||server==H&&type=="DIS") continue;
		upadate_ip_state();
		if(type=="DIS")
		{
			int k = get_ip_by_owner(client);
			if(!k) k = get_ip_by_state(0);
			if(!k) k = get_ip_by_state(3);
			if(!k)	continue;
			ip[k].state = 1;
			ip[k].owner = client;
 
			int opt_time = get_suit_time(time);
			ip[k].t = opt_time;
			cout<<"dhcp "<<client<<" OFR "<<k<<" "<<opt_time<<endl;
 
		}
		else if(type=="REQ")
		{
			if(server!=H)
			{
				int k = get_ip_by_owner(client);
				if(!k) continue;
				if(ip[k].state==1)
				{	
					ip[k].state=0;
					ip[k].t = 0;
					ip[k].owner = "";
 
				}
				continue;
			}
			int opt_time = get_suit_time(time);
			if(target_ip>=n||target_ip<=0||ip[target_ip].owner!=client)
			{	
				cout<<"dhcp "<<client<<" NAK "<<target_ip<<" 0"<<endl;
				continue;
			}
			ip[target_ip].state = 2;
			ip[target_ip].t = opt_time;
			cout<<"dhcp "<<client<<" ACK "<<target_ip<<" "<<opt_time<<endl;
 
		}
 
	}
	return 0;
}
 
