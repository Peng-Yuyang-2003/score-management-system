#include<string>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<time.h>//下面三个用于easyX
#include<conio.h>
#include <graphics.h>
using namespace std;
//(全局变量）
extern int N=0;//人数
extern int M=0;//判断是否显示欢迎界面
extern const int MAX=100;//最大人数，超出后会数组越界
//向前引用说明
void tishi(int flag);
void check();
//四个类
class base{
public:
	int id;
	double c,m,e,a;//分别表示语文、数学、英语、总成绩
	base();
	base(int id1,double c1,double m1,double e1,double a1)
	{id=id1;c=c1;m=m1;e=e1;a=a1;}//构造函数
	~base(){};
	void plus(){a=c+e+m;}//计算总成绩;
	virtual int get_id(){return id;}//在查找学生和删除学生时检索id
};
class info{
public:
	info();
	info(int grade1,int clas1,string name1):
	grade(grade1),clas(clas1),name(name1){}//构造函数
	~info(){};
	int grade;
	int clas;
	string name;
};
class student:public base,public info{//多重继承
friend class clas;
public:
	student(int id1=0,string name1="None",double c1=0.0,double m1=0.0,
		double e1=0.0,double a1=0.0,int grade1=0,int clas1=0):
	base(id1,c1,m1,e1,a1),info(grade1,clas1,name1){}//构造函数
	~student(){};
    friend ostream& operator<<(ostream&,student&);//函数运算符重载
	friend istream& operator>>(istream&,student&);
	void newlist(student *s);//新建文件
	void display(student *s);//显示
	void addnew(student *s);//增加
	void del(student *s);//删除
	void change(student *s);//修改
	void search(student *s);//查找
	void sort(student *s);//排序
	void exchange(student &s1,student &s2);//交换两个对象的内容
};
class clas:public base{
public:
	clas(int id1=0,double c1=0.0,double m1=0.0,double e1=0.0,double a1=0.0):
	base(id1,c1,m1,e1,a1){}
	~clas(){};
	void average(student *s);
	virtual int get_id(){return id;}
};
//*****计算班级平均成绩	
void clas::average(student *s)
{
	ifstream infile("stu.dat",ios::in);//载入数据
	if(!infile)
	{
		cout<<"打开失败"<<endl;
	}
	for(int i=0;i<N;i++)
	{
		infile>>s[i];
	}
    infile.close();
	bool flag=false;
	int gradenum,clasnum;
	while(!flag)
	{
	cout<<"请输入年级序号"<<endl;
	cin>>gradenum;check();
	cout<<"请输入班级序号（输入0代表求该年级所有学生平均成绩）"<<endl;
	cin>>clasnum;check();
	double ch=0,ma=0,en=0,al=0,stunum=0;
	for(int i=0;i<N;i++)
	{
		if((s[i].grade==gradenum&&s[i].clas==clasnum)||(s[i].grade==gradenum&&clasnum==0))
		//判断是否找到对应班级
		{   
		    flag=true;
			ch+=s[i].c;ma+=s[i].m;en+=s[i].e;
			stunum+=1;//该班学生总人数
			ch=ch/stunum;ma=ma/stunum;en=en/stunum;al=ch+en+ma;
		}
	} 
	if(!flag)
	{
		cout<<"未找到该班级，请重新输入"<<endl; 
	}
	else {cout<<"语文、数学、英语、总分平均分如下："<<endl;
		cout<<ch<<' '<<ma<<' '<<' '<<en<<' '<<al<<endl;}
    }
}
//*****函数运算符重载
ostream& operator<<(ostream& out,student& s)
{
	out<<s.id<<"  "<<s.name<<" "<<s.grade<<"   "<<s.clas<<"   "<<s.c<<"    "<<s.m<<"   "<<s.e<<"   "<<s.a;
	return out;
}
istream& operator>>(istream& in,student& s)
{
	in>>s.id>>s.name>>s.grade>>s.clas>>s.c>>s.m>>s.e>>s.a;
	return in;
}
//*****新建文件
void student::newlist(student *s)
{
	int n;
	cout << "请输入学生人数";
	cin >> n;check();
	ofstream outfile("stu.dat", ios::out);  //定义文件流对象，打开磁盘文件"stu.dat"
	if (! outfile)                          //如果打开失败，outfile返回值
	{
		cerr << "打开失败" << endl;
		abort();
	}
	N=n;
	for (int i = 0; i < n; i++)
	{
		cout << "请依次输入学生的学号、姓名、年级、班级、语文、数学、英语成绩（用空格隔开）：" << endl;
		cin>> s[i].id>>s[i].name>>s[i].grade>>s[i].clas>>s[i].c>>s[i].m>>s[i].e;check();
		bool flag=false;
		for(int j=0;j<N&&j!=i;j++)
			if(s[j].id==s[i].id) flag=true;//判断是否输入重复学号
		if(flag) 
		{
			i-=1;//错误输入后，下一次还是输入这一项
			tishi(1);
		}
		else if(s[i].c>100||s[i].c<0||s[i].m>100||s[i].m<0||s[i].e>100||s[i].e<0)//判断是否超出范围
    	{
			i-=1;
    		tishi(2);
		}
		else 
		{
			s[i].plus();//计算总分
		    outfile<<s[i]<<endl;//写入文件
		}
	}
	outfile.close();  //关闭磁盘文件
	cout << "学生信息已经建立"<<endl;
}
//*****显示全部学生信息
void student::display(student *s)
{
	cout<<"学号 姓名 年级 班级 语文 数学 英语 总分"<<endl; 
	ifstream infile("stu.dat",ios::in);
	if(!infile)
	{
		cout<<"打开失败"<<endl;
	}
	for(int i=0;i<N;i++)
	{
		infile>>s[i];
		cout<<s[i]<<endl;
	}
    infile.close();
}
//*****增加学生信息
void student::addnew(student *s)
{
	int n;
	cout << "请输入要添加的学生人数";
	cin >> n;check();
	ifstream infile("stu.dat",ios::in);
	if(!infile)
	{
		cout<<"打开失败"<<endl;
	}
	for(int i=0;i<N;i++)
	{
		infile>>s[i];
	}
    infile.close();//完成载入
	N=N+n;
	for (int i = N-n; i < N; i++)
	{
		cout << "请依次输入学生的学号、姓名、年级、班级、语文、数学、英语成绩（用空格隔开）：" << endl;
		cin>> s[i].id>>s[i].name>>s[i].grade>>s[i].clas>>s[i].c>>s[i].m>>s[i].e;check();
		bool flag=false;
		for(int j=0;j<N&&j!=i;j++)
			if(s[j].id==s[i].id) flag=true;//判断是否输入重复学号
		if(flag) 
		{
			i-=1;//错误输入后，下一次还是输入这一项
			tishi(1);
		}
		else if(s[i].c>100||s[i].c<0||s[i].m>100||s[i].m<0||s[i].e>100||s[i].e<0)
    	{
			i-=1;
    		tishi(2);
		}
		else 
		{
			s[i].plus();//计算总分
		}
	}
	ofstream outfile("stu.dat",ios::out);//最后把数据放入文件
	if(!outfile)
	{
		cout<<"打开失败!"<<endl;
	}
	for(int i=0;i<N;i++)
	{
	outfile<<s[i]<<endl;//写入文件
    }
	outfile.close(); //关闭文件
}
//*****删除学生信息
void student::del(student *s)
{
	ifstream infile("stu.dat",ios::in);//先将数据加载进来
	if(!infile)
	{
		cout<<"打开失败"<<endl;
	}
	for(int i=0;i<N;i++)
	{
		infile>>s[i];
		if(s[i].id==0)break;
	}
	infile.close();
	int id; 
	bool flag=false;
	cout<<"请输入要删除学生的学号："<<endl;
	while(!flag) 
	{
	cin>>id;check();
	for(int i=0;i<N;i++)
	{
		if(s[i].get_id()==id)//判断是否找到要删除学生学号
		{   
		    flag=true;
		    cout<<"该学生学号、姓名、年级、班级、语文、数学、英语、总分依次为："<<endl;
		    cout<<s[i]<<endl;
			for(int j=i;j<N-1;j++)//往前移为，覆盖该学生信息
			s[j]=s[j+1];
			cout<<"删除成功！"<<endl; 
			N=N-1;//减少学生人数
			break;
		}
	} 
	if(!flag)
	{
		cout<<"未找到该学生信息，请重新输入"<<endl; 
	}
    }
	ofstream outfile("stu.dat",ios::out);//最后把数据放入文件
	if(!outfile)
	{
		cout<<"打开失败!"<<endl;
	}
	cout<<"删除后学生成绩表："<<endl; 
	for(int i=0;i<N;i++)
	{
	outfile<<s[i]<<endl;//写入文件
	cout<<s[i]<<endl;//打印终端
    }
	outfile.close(); //关闭文件
}
//*****修改学生信息
void student::change(student *s)
{
	ifstream infile("stu.dat",ios::in);//先将数据加载进来
	if(!infile)
	{
		cout<<"打开失败"<<endl;
	}
	for(int i=0;i<N;i++)
	{
		infile>>s[i];
		if(s[i].id==0)break;
	}
	infile.close();
	int id; 
	bool flag=false;
	cout<<"请输入要修改学生的学号"<<endl;
	while(!flag) 
	{
	cin>>id;check();
	for(int i=0;i<N;i++)
	{
		if(s[i].get_id()==id)//判断是否找到要修改学生学号
		{   
		    flag=true;
			cout<<"该学生成绩信息："<<endl;  //打印到终端
		    cout<<s[i]<<endl;
			for(int j=0;j<1;j++)
			{
			cout<<"请重新输入该学生的学号、姓名、年级、班级、语文、数学、英语成绩（用空格隔开）："<<endl;
			cin>> s[i].id>>s[i].name>>s[i].grade>>s[i].clas>>s[i].c>>s[i].m>>s[i].e;check();
			bool flag1=false;
			for(int k=0;k<N;k++){if(s[k].id==s[i].id&&k!=i) flag1=true;}//判断是否输入重复学号,不能像增加里面那样，否则会无法循环
		    if(flag1) 
		    {
			j-=1;//错误输入后，下一次还是输入这一项
			tishi(1);
		    }
		    else if(s[i].c>100||s[i].c<0||s[i].m>100||s[i].m<0||s[i].e>100||s[i].e<0)
    	    {
			j-=1;
    		tishi(2);
		    }
		    else 
		    {
			s[i].plus();//计算总分
		    }
			}
			break;
		}
	} 
	if(!flag)
	{
		cout<<"未找到该学生信息，请重新输入"<<endl; 
	}
    }
	ofstream outfile("stu.dat",ios::out);//最后把数据放入文件
	if(!outfile)
	{
		cout<<"打开失败!"<<endl;
	} 
	for(int i=0;i<N;i++)
	{
	outfile<<s[i]<<endl;//写入文件
    }
	outfile.close(); //关闭文件
}
//*****查找学生信息
void student::search(student *s)
{
	ifstream infile("stu.dat",ios::in);//先将数据加载进来
	if(!infile)
	{
		cout<<"打开失败"<<endl;
	}
	for(int i=0;i<N;i++)
	{
		infile>>s[i];
		if(s[i].id==0)break;
	}
	infile.close();
	int id; 
	bool flag=false;
	cout<<"请输入要查找学生的学号："<<endl;
	while(!flag)
	{
		cin>>id;check();
	for(int i=0;i<N;i++)
	{
		if(id==s[i].get_id())//判断是否查找到该学生信息
		{
			flag=true;
			cout<<"该学生成绩信息："<<endl;
			cout<<s[i]<<endl;  
			break;
		}
	 } 
	 if(!flag)
    {
    	cout<<"未找到该学生信息，请重新输入学号"<<endl;
	 }
	}
}
//*****排序
void student::sort(student *s)
{
	ifstream infile("stu.dat",ios::in);//先将数据加载进来
	if(!infile)
	{
		cout<<"打开失败"<<endl;
	}
	for(int i=0;i<N;i++)
	{
		infile>>s[i];
		if(s[i].id==0)break;
	}
	int gradenum,clasnum;
	cout<<"请输入年级序号"<<endl;
	cin>>gradenum;check();
	cout<<"请输入班级序号（输入0代表求该年级所有学生排序）"<<endl;
	cin>>clasnum;check();
	student temp;
	cout<<"|=============================================================================================|"<<endl;
	cout<<"|**###################################请选择排序的依据######################################**|"<<endl;
	cout<<"|**                                    ①语文成绩                                           **|"<<endl;
	cout<<"|**                                    ②数学成绩                                           **|"<<endl;
	cout<<"|**                                    ③英语成绩                                           **|"<<endl;
	cout<<"|**                                    ④  总分                                             **|"<<endl;
	cout<<"|**#########################################################################################**|"<<endl;
	cout<<"|=============================================================================================|"<<endl;
	char num;
	do{cout<<"请输入您要选择的服务种类：（1 ~ 4）"<<endl;//对输入进行检查
	num=_getche();
	putchar('\n');
	}while(num!='1'&&num!='2'&&num!='3'&&num!='4');
	if (num=='4'){
	for(int i=0;i<N-1;i++)//按总分冒泡排序从高到低
	{
		for(int j=0;j<N-1-i;j++)
		{
		   if(s[j].a<s[j+1].a)temp.exchange(s[j],s[j+1]);
		}
	}}
	else if(num=='1'){
	for(int i=0;i<N-1;i++)//按语文冒泡排序从高到低
	{
		for(int j=0;j<N-1-i;j++)
		{
		   if(s[j].c<s[j+1].c)temp.exchange(s[j],s[j+1]);
		}
	}}
	else if(num=='2'){
	for(int i=0;i<N-1;i++)//按数学冒泡排序从高到低
	{
		for(int j=0;j<N-1-i;j++)
		{
		   if(s[j].m<s[j+1].m)temp.exchange(s[j],s[j+1]);
		}
	}}
	else {
	for(int i=0;i<N-1;i++)//按英语冒泡排序从高到低
	{
		for(int j=0;j<N-1-i;j++)
		{
		   if(s[j].e<s[j+1].e)temp.exchange(s[j],s[j+1]);
		}
	}}
	ofstream outfile("stu.dat",ios::out);//最后把数据放入文件
	if(!outfile)
	{
		cout<<"打开失败!"<<endl;
	}
	cout<<"按分数从高到低学生成绩表："<<endl; 
	for(int i=0;i<N;i++)
	{
	outfile<<s[i]<<endl;//写入文件
	if((s[i].grade==gradenum&&s[i].clas==clasnum)||(s[i].grade==gradenum&&clasnum==0))
	{cout<<s[i]<<endl;}//打印终端
    }
	outfile.close(); //关闭文件
}
//*****交换两个对象的数据
void student::exchange(student &s1,student &s2)
{
	student temp;//建立中间对象交换两个对象的内容
	temp.id=s2.id;s2.id=s1.id;s1.id=temp.id;
	temp.name=s2.name;s2.name=s1.name;s1.name=temp.name;
	temp.grade=s2.grade;s2.grade=s1.grade;s1.grade=temp.grade;
	temp.clas=s2.clas;s2.clas=s1.clas;s1.clas=temp.clas;
	temp.c=s2.c;s2.c=s1.c;s1.c=temp.c;
	temp.m=s2.m;s2.m=s1.m;s1.m=temp.m;
	temp.e=s2.e;s2.e=s1.e;s1.e=temp.e;
	temp.a=s2.a;s2.a=s1.a;s1.a=temp.a;
}
int main()
{ 
	system("color B4");//设置色彩
	if(M==0) {tishi(0);M+=1;}//第一次开启有提示
	int FLAG3=1;
	while(FLAG3==1)
	{  
	system("CLS");//清屏
	cout<<"|================================================================================================|"<<endl;
	cout<<"|**#################################欢迎使用学生成绩管理系统###################################**|"<<endl;
	cout<<"|**                                    ①新建学生成绩                                          **|"<<endl;
	cout<<"|**                                    ②增加学生成绩                                          **|"<<endl;
	cout<<"|**                                    ③删除学生成绩                                          **|"<<endl;
	cout<<"|**                                    ④修改学生成绩                                          **|"<<endl;
	cout<<"|**                                    ⑤查找学生成绩                                          **|"<<endl;
	cout<<"|**                                    ⑥学生成绩排序                                          **|"<<endl;
	cout<<"|**                                    ⑦所有学生信息                                          **|"<<endl;
	cout<<"|**                                    ⑧班级平均成绩                                          **|"<<endl;
	cout<<"|**                                    ⑨   退出                                               **|"<<endl;
	cout<<"|**############################################################################################**|"<<endl;
	cout<<"|================================================================================================|"<<endl; 
	char num;
	do{cout<<"请输入您要选择的服务种类：（1~ 9）"<<endl;//对输入进行检查
	num=_getche();
	putchar('\n');
	}while(num!='1'&&num!='2'&&num!='3'&&num!='4'&&num!='5'&&num!='6'&&num!='7'&&num!='8'&&num!='9');
	student s[MAX];
	clas c;
	switch(num)
	{
	case '1' :system("CLS");s[0].newlist(&s[0]);system("pause");break;	
	case '2' :system("CLS");s[0].addnew(&s[0]);system("pause");break;
	case '3' :system("CLS");s[0].del(&s[0]);system("pause");break;
	case '4' :system("CLS");s[0].change(&s[0]);system("pause");break;
	case '5' :system("CLS");s[0].search(&s[0]);system("pause");break;
	case '6' :system("CLS");s[0].sort(&s[0]);system("pause");break;
	case '7' :system("CLS");s[0].display(&s[0]);system("pause");break;
	case '8' :system("CLS");c.average(&s[0]);system("pause");break;
	case '9' :system("CLS");FLAG3=0;break;
	default:system("CLS");check();
	}
	}
	tishi(3);
	exit(0);
	return 0;
}
// *****提示动画
void tishi(int flag)
{
    srand((unsigned) time(NULL));
    initgraph(320, 120,EW_SHOWCONSOLE);// 初始化图形模式
	int  x, y;
	char c;
	settextstyle(16, 8, _T("Courier"));	// 设置字体
	// 设置颜色
	settextcolor(RED);
	setlinecolor(CYAN);
	setbkcolor(CYAN);
	for (int i = 0; i <=119; i++)
	{
		// 在随机位置显示三个随机字母
		for (int j = 0; j < 3; j++)
		{
			x = (rand() % 80) * 8;
			y = (rand() % 20) * 24;
			c = (rand() % 26) + 65;
			outtextxy(x, y, c);
		}

		// 画线擦掉一个像素行
		line(0, i, 319, i);
		if(flag==1) {TCHAR s[] = _T("学号重复!请再次输入");outtextxy(60, i, s);}
		else if(flag==2) {TCHAR s[] = _T("分数超出范围！请再次输入");outtextxy(35, i, s);}
		else if(flag==4) {TCHAR s[] = _T("无效输入，请重新输入");outtextxy(60, i, s);}
		else if(flag==0) {TCHAR s[] = _T("欢迎使用学生成绩管理系统");outtextxy(35, i, s);}
		else if(flag==5) {TCHAR s[] = _T("无效输入，即将退回主界面");outtextxy(35, i, s);}
		else if(flag==3) {TCHAR s[] = _T("按任意键退出");outtextxy(80, i, s);}
		else {TCHAR s[] = _T("错误");outtextxy(80, i, s);}
		Sleep(1);					// 延时
		if (i==119)	break;		// 定时退出
	}

	// 关闭图形模式
	closegraph();
}
//*****检查输入是否成功
void check()
{
	if(cin.fail()){
		tishi(5);
		cin.clear();
		while(cin.get()!='\n')continue;
		main();
	}
}
