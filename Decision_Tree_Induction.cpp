#include<bits/stdc++.h>
using namespace std;
ifstream fin;
struct dtnode
{
	string data;
	set<string> type;
	vector<dtnode*> child;
};

vector<string> cloneit(vector<string>&arr)			//clones a string vector...
{
	vector<string>dup;
	for(vector<string>::iterator it=arr.begin();it!=arr.end();it++)dup.push_back(*it);
	return dup;
}

vector<string> wordsof(string str)					//returns the string array of str...
{
	vector<string> tmpset;
	string tmp="";
	int i=0;
	while(str[i])
	{
		if(isalnum(str[i]))tmp+=str[i];
		else {
			if(tmp.size()>0)
				tmpset.push_back(tmp);
			tmp="";
		}
		i++;
	}
	if(tmp.size()>0)tmpset.push_back(tmp);
	return tmpset;
}

bool allAreSame(vector<vector<string> >&tuples)		//return whether all the tuples are of same class..
{
	for(int i=0;i<tuples.size()-1;i++)
		if(tuples[i][tuples[i].size()-1]!=tuples[i+1][tuples[i+1].size()-1])
			return false;
	return true;
}
string majorityClassOf(vector<vector<string> >&tuples) //return class of majority tuples..
{
	set<string>classes;
	map<string,int>cnt;
	for(int i=0;i<tuples.size();i++)
	{
		classes.insert(tuples[i][0]);
		cnt[tuples[i][0]]++;
	}
	string res=*(classes.begin());
	set<string>::iterator it=classes.begin();
	it++;
	for(;it!=classes.end();it++)
		if(cnt[*it]>cnt[res])res=*it;
	return res;
}

double cal_info(vector<vector<string> >&tuples)		//returns info of tuples..
{
	map<string,double>p;
	for(int i=0;i<tuples.size();i++)
		p[tuples[i][tuples[i].size()-1]]++;
	for(map<string,double>::iterator it=p.begin();it!=p.end();it++)
		it->second=(it->second)/tuples.size();
	double infoD=0;
	for(map<string,double>::iterator it=p.begin();it!=p.end();it++)
		infoD+=(-(it->second)*log(it->second)/log(2));
	return infoD;
}

int Attribute_selection_method(vector<string> &attr,vector<vector<string> >&tuples) //information gain method for selecting attribute...
{
	
	double infoD=cal_info(tuples);
	
	vector<double> info(attr.size()-1,0);
	for(int i=0;i<attr.size()-1;i++)
	{
		map<string,vector<vector<string> > >mymap;
		for(int j=0;j<tuples.size();j++)
			mymap[tuples[j][i]].push_back(tuples[j]);
		for(map<string,vector<vector<string> > >::iterator it=mymap.begin();it!=mymap.end();it++)
			info[i]+=it->second.size()*cal_info(it->second)/tuples.size();
	}
	
	vector<double> gain;
	for(int i=0;i<info.size();i++)
		gain.push_back(infoD-info[i]);
	
	return max_element(gain.begin(),gain.end())-gain.begin();
}

void levelorder(dtnode *tmp)		//level order printing of decision tree
{
	dtnode *delimiter=new dtnode();
	delimiter->data="-1";
	queue<dtnode*>q;
	q.push(tmp);
	q.push(delimiter);
	while(q.size()!=1)
	{
		tmp=q.front();
		q.pop();
		if(tmp->data.compare("-1")==0)
		{
			cout<<endl;
			q.push(tmp);
			continue;
		}
		if(tmp->child.size())
		{
			cout<<" {"<<tmp->data<<"? -> ";
			for(set<string>::iterator it=tmp->type.begin();it!=tmp->type.end();it++)
				cout<<*it<<", ";cout<<"} ";
		}
		else cout<<" {"<<tmp->data<<"} ";
		for(int i=0;i<tmp->child.size();i++)q.push(tmp->child[i]);
	}
}

dtnode* Generate_Decision_Tree(vector<string> &attr,vector<vector<string> >&tuples) //returns root of the decision tree...
{
	dtnode *node=new dtnode();
	if(allAreSame(tuples))
	{
		node->data=tuples[0][tuples[0].size()-1];
		return node;
	}
	
	if(attr.size()==1)
	{
		node->data=majorityClassOf(tuples);
		return node;
	}
	
	int splitting_criterion_index=Attribute_selection_method(attr,tuples);
	node->data=attr[splitting_criterion_index];
	
	map<string,vector< vector<string> > >divisions;
	for(int i=0;i<tuples.size();i++)
	{
		vector<string> tmp=cloneit(tuples[i]);
		string str=tmp[splitting_criterion_index];
		node->type.insert(str);
		tmp.erase(tmp.begin()+splitting_criterion_index);
		divisions[str].push_back(tmp);
	}
	
	for(set<string>:: iterator it=node->type.begin();it!=node->type.end();it++)
	{
		vector<string>tattr=cloneit(attr);
		tattr.erase(tattr.begin()+splitting_criterion_index);
		node->child.push_back(Generate_Decision_Tree(tattr, divisions[*it]));
	}
	
	return node;
}

int main()
{
	fin.open("dt.in");
	string str;
	
	getline(fin,str);
	vector<string> attr=wordsof(str);
	
	vector<vector<string> >tuples;
	while(!fin.eof())
	{
		getline(fin,str);
		vector<string> tmp=wordsof(str);
		tuples.push_back(tmp);
	}
	fin.close();
	
	dtnode *root=Generate_Decision_Tree(attr,tuples);
	levelorder(root);
}
