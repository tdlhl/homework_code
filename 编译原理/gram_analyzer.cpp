#include<bits/stdc++.h>
using namespace std;

int index=0,line=1;

vector<string> dyd_list;
vector<int> dyd_id;

int begin_code=1,end_code=2,integer_code=3,if_code = 4,then_code = 5;
int else_code = 6,function_code = 7,read_code = 8,write_code = 9;
int identifier_code = 10,constant_code = 11,eoln = 24,eof = 25;
int var_define = 0,var_use = 1,formal_parameter = 1,var_count = 0;
int procedure_count = 0,procedure_level = 0,num_xc=0;

vector<vector<string>> var_table;
vector<vector<string>> pro_table;

string main_procedure_text = "main";
string now_Procedure = main_procedure_text;
string last_Procedure;
fstream error;

void main_procedure();
void branch_procedure();
void advance();
void declare_statement_table();
void declare_statement_tableA();
void declare_statement();
void var_declare(); 
void var(int define_or_use,int is_formal);
string identifier();
void relation_operator();
void condition_expression();
void condition_statement();
void unsigned_integer();
void constant();
void func_all();
void factor();
void itemA();
void item();
void math_expression();
void math_expressionA();
void assign_statement();
void write_statement();
void read_statement();
void exec_statement();
void exec_statement_tableA();
void exec_statement_table();
void func_body();
void parameter();
void func_declare();
int now_more_one(int next);
int now();
string now_word();
string removekg(string hang,int &tid);

void main_procedure()
{
	//<����>��<�ֳ���>
	branch_procedure();
	return;
}
void branch_procedure()
{
	//<�ֳ���>��begin <˵������>��<ִ������> end
	if(now()==begin_code)
	{
		advance();
		declare_statement_table();
		if(now()==23)
		{
			advance();
			exec_statement_table();
			if(now()==end_code)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ� ȱ��end"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ� ȱ��;"<<endl;	
		} 
	}
	else
	{
		error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ� ȱ��begin"<<endl;
	}
	return;
}
void advance()
{
	int h1=now();
	index++;
}
void declare_statement_table()
{
//     <˵������>��<˵�����><˵������A>
//    <˵������A>��;<˵�����><˵������A>����
	declare_statement();
	declare_statement_tableA();
}
void declare_statement_tableA()
{
//	#<˵������A>��;<˵�����><˵������A>����
	if(now()==23&&now_more_one((index+1))==integer_code)
	{
		advance();
		declare_statement();
		declare_statement_tableA();
	}
	else
	{
		return;
	}
}
void declare_statement()
{
	//<˵�����>��<����˵��>��<����˵��>
	if(now()==integer_code)
	{
		if(now_more_one(index+1)==7)
		{
			func_declare();
		}
		else
		{
			var_declare();
		}
	}
	else
	{
		//cout<<now()<<endl;
		error<<"LINE"<<line<<": "<<"˵������ ȱ��integer"<<endl;
	}
}
void var_declare()
{
	//<����˵��>��integer <����>
	if(now()==3)
	{
		advance();
		var(var_define,0);
	}
	else
	{
		error<<"LINE"<<line<<": "<<"���������� ȱ��integer"<<endl;
	}
}
void var(int define_or_use,int is_formal)
{
	//<����>��<��ʶ��>
	vector<string> temp;
	string word; 
	//���ࣺ�����Ƕ��廹��ʹ�� 
	if(define_or_use==var_define)
	{
		word=identifier();
		bool flag=true;
		for(int i=0;i<var_table.size();i++)
		{
			if(word==var_table[i][0])
			{
				flag=false;
				if(var_table[i][2]=="1"&&num_xc>0)
				{
					num_xc--;
				}
				else
				{
					error<<"LINE"<<line<<": "<<"����"<<word<<"�ض���"<<endl;
					break;
				}			
			}
		} 
		if(flag)
		{
			temp.push_back(word);
			temp.push_back(now_Procedure);
			temp.push_back(to_string(is_formal));
			temp.push_back("integer");
			temp.push_back(to_string(procedure_level));
			temp.push_back(to_string(var_count));
			var_count++;
			var_table.push_back(temp);
			
			//���¹��̱��е�fadr��ladr 
			for(int i=0;i<pro_table.size();i++)
			{
				if(pro_table[i][0]==now_Procedure)
				{
					if(pro_table[i][3]=="-1")
					{
						pro_table[i][3]=to_string(var_count-1);
					}
					if(pro_table[i][4]<to_string(var_count-1))
					{
						pro_table[i][4]=to_string(var_count-1);
					}
					
				}
			}
		}
	}
	else if(define_or_use==var_use)
	{
		word=identifier();
		bool use_word=false;
		for(int i=0;i<var_table.size();i++)
		{
			if(word==var_table[i][0])
			{
				use_word=true;
			}
		} 
		for(int j=0;j<pro_table.size();j++)
		{
			if(word==pro_table[j][0])
			{
				use_word=true;
			}
		}
		if(!use_word)
		{
			error<<"LINE"<<line<<": "<<"����"<<word<<"�޶���"<<endl;
		}
	}
} 
string identifier()
{
	//<��ʶ��>��<��ĸ>��<��ʶ��><��ĸ>�� <��ʶ��><����>
	string temp_word;
	if(now()==identifier_code)
	{
		temp_word=now_word();
		advance();
		return temp_word;
	}
	else
	{
		error<<"LINE"<<line<<": "<<"�޷�ƥ���ʶ��"<<endl;
	}
}
void relation_operator()
{
	//<��ϵ�����> ��<��<=��>��>=��=��<>
	int cht=now();
	if(cht==15||cht==14||cht==17||cht==16||cht==12||cht==13)
	{
		advance();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"�޷�ƥ���ϵ�����"<<endl;
	}
}
void condition_expression()
{
	//<�������ʽ>��<�������ʽ><��ϵ�����><�������ʽ>
	math_expression();
    relation_operator();
    math_expression();
}
void condition_statement()
{
	//<�������>��if<�������ʽ>then<ִ�����>else <ִ�����>
	if(now()==4)
	{
		advance();
		condition_expression();
		if(now()==5)
		{
			advance();
			exec_statement();
			if(now()==6)
			{
				advance();
				exec_statement();
			} 
			else
			{
				error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�ȱ��else"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�ȱ��then"<<endl;
		}
	} 
	else
	{
		error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�ȱ��if"<<endl;	
	}
}
void unsigned_integer()
{
	if(now()==11)
	{
		advance();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"����ƥ�������"<<endl;
	}
}
void constant()
{
	//<����>��<�޷�������>
	unsigned_integer();
}
void func_all()
{
	//<��������>��<��ʶ��>(<�������ʽ>)
	identifier();
	if(now()==21)
	{
		advance();
		math_expression();
		if(now()==22)
		{
			advance();
		}
		else
		{
			error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��)"<<endl;
		}	
	} 
	else
	{
		error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��("<<endl;
	}
}
void factor()
{
	//<����>��<����>��<����>��<��������>
	if(now()==11)
	{
		constant();
	}
	else if(now_more_one(index+1) ==21)
	{
		func_all();
		
	}
	else
	{
		var(var_use,0);
	}
}
void itemA()
{
	//<��A>��*<����><��A>����
	if(now()==19)
	{
		advance();
		factor();
		itemA();
	}
	else
	{
		return;
	}
}
void item()
{
//    <��>��<����><��A>
//    <��A>��*<����><��A>����
	factor();
	itemA();
}
void math_expressionA()
{
	//<�������ʽA>��-<��><�������ʽA>|��
	if(now()==18)
	{
		advance();
		item();
		math_expressionA();
	} 
	else
	{
		return;
	}
}
void math_expression()
{
//    <�������ʽ>��<��><�������ʽA>
//    <�������ʽA>��-<��><�������ʽA>|��
	item();
	math_expressionA();
}
void assign_statement()
{
	//<��ֵ���>��<����>:=<�������ʽ>
	var(var_use,0);
	if(now()==20)
	{
		advance();
		math_expression();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"�޷�ʶ��ֵ���"<<endl;
	}
}
void write_statement()
{
	//<д���>��write(<����>)
	if(now()==9)
	{
		advance();
		if(now()==21)
		{
			advance();
			var(var_use,0);
			if(now()==22)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��)"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��("<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"�޷�ʶ��write���"<<endl;
	}
}
void read_statement()
{
	//<�����>��read(<����>)
	if(now()==8)
	{
		advance();
		if(now()==21)
		{
			advance();
			var(var_use,0);
			if(now()==22)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��)"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��("<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"�޷�ʶ��read���"<<endl;
	}
}
void exec_statement()
{
	//<ִ�����>��<�����>��<д���>��<��ֵ���>��<�������>
	int state=now();
	if(state==read_code)
	{
		read_statement();
	} 
	else if(state==write_code)
	{
		write_statement();
	}
	else if(state==if_code)
	{
		condition_statement();
	}
	else if(state==identifier_code)
	{
		assign_statement();
	}
	else
	{
		error<<"LINE"<<line<<": "<<"fatal error ִ���������޷���һ������"<<endl;
		//exit(1);
	}
}
void exec_statement_tableA()
{
	//<ִ������A>��;<ִ�����><ִ������A>����
	int check=now_more_one(index+1);
	if(now()==23&&(check==8||check==9||check==4||check==10))
	{
		advance();
		exec_statement();
		exec_statement_tableA();
	}
	else
	{
		return;
	}
}
void exec_statement_table()
{
//    # <ִ������>��<ִ�����><ִ������A>
//    # <ִ������A>��;<ִ�����><ִ������A>����
	exec_statement();
	exec_statement_tableA();
	return;
}
void func_body()
{
	//<������>��begin <˵������>��<ִ������> end
	if(now()==begin_code)
	{
		advance();
		declare_statement_table();
		if(now()==23)
		{
			advance();
			exec_statement_table();
			if(now()==end_code)
			{
				advance();
			}
			else
			{
				error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��end"<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��;"<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��begin"<<endl;
	}
}
void parameter()
{
	//<����>��<����>
	var(var_define,formal_parameter);
	num_xc++;
	return;
}
void func_declare()
{
	//<����˵��>��integer function <��ʶ��>��<����>����<������>
	procedure_level++;
	bool flag;
	vector<string> temp_p;
	last_Procedure = now_Procedure;
	if(now()==integer_code)
	{
		advance();
		if(now()==function_code)
		{
			advance();
			now_Procedure = identifier();
			flag=true;
			for(int i=0;i<pro_table.size();i++)
			{
				if(now_Procedure==pro_table[i][0])
				{
					flag=false;
				}
			}
			if(flag)
			{
				temp_p.push_back(now_Procedure);
				temp_p.push_back("integer");
				temp_p.push_back(to_string(procedure_level));
				temp_p.push_back("-1");
				temp_p.push_back("-1");
				pro_table.push_back(temp_p);
			}
			if(now()==21)
			{//������ 
				advance();
				parameter();
				if(now()==22)
				{
					advance();
					if(now()==23)
					{
						advance();
						func_body();
					}
					else
					{
						error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,����˵��ȱ��;"<<endl;
					}
				}
				else
				{
					error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��)"<<endl;
				} 
			}
			else
			{
				error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��("<<endl;
			}
		}
		else
		{
			error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��function"<<endl;
		}
	}
	else
	{
		error<<"LINE"<<line<<": "<<"ȱ�ٷ��Ŵ�,ȱ��integer"<<endl;
	}
	now_Procedure = last_Procedure;
	procedure_level--;
}
int now_more_one(int next)
{
	if(dyd_id[next]==eoln)
	{
		return now_more_one(next+1);
	}
	else
	{
		return dyd_id[next];
	}
}
int now()
{//���ص�ǰ�ֱ�� 
	if(dyd_id[index]==eoln)
	{
		line++;
		index++;
		return now();
	}
	else
	{
		return dyd_id[index];
	}
}
string now_word()
{//���ص�ǰ���ʷ��� 
	if(dyd_id[index]==eoln)
	{
		line++;
		index++;
		return now_word();
	}
	else
	{
		return dyd_list[index];
	}
}

string removekg(string hang,int &tid)
{
	string temp;
//	char num,all=0;
	int i=0;
	while(i<hang.size()&&hang[i]==' ')
	{
		i++;
	}
	while(i<hang.size()&&hang[i]!=' ')
	{
		temp.push_back(hang[i]);
		i++;
	}
	i++;
	string rest(hang,i);
	int num=atoi(rest.c_str());
	tid=num;
	return temp;
}

int main()
{	
	ifstream infile;
	fstream outdys;
	fstream outvar;
	fstream outpro;
	infile.open("sample.dyd",ios::in);
//	error.open("sample.err",ios::out|ios::in|ios::app);
//	outdys.open("sample.dys",ios::out|ios::app);
//	outvar.open("sample.var",ios::out|ios::app);
//	outpro.open("sample.pro",ios::out|ios::app);
	
	error.open("sample.err",ios::out|ios::in);
	outdys.open("sample.dys",ios::out);
	outvar.open("sample.var",ios::out);
	outpro.open("sample.pro",ios::out);
	
	string hang,whang;
	int tid;
	if(infile)
	{
		while(getline(infile,hang))
		{
			outdys<<hang<<endl;
			whang=removekg(hang,tid);
			dyd_list.push_back(whang);
			dyd_id.push_back(tid);
		}
	}
	else
	{
		error<<"fatal error: �޷���ȡdyd�ļ�"<<endl;
		return -1;
	}
	
//	for(int i=0;i<dyd_list.size();i++)
//	{
//		cout<<dyd_list[i]<<" "<<dyd_id[i]<<endl;
//	}

	main_procedure();
	//cout<<"var_table.size()"<<var_table.size()<<endl; 
	//cout<<"pro_table.size()"<<pro_table.size()<<endl; 
	for(int i=0;i<var_table.size();i++)
	{
		for(int j=0;j<var_table[i].size();j++)
		{
			outvar<<var_table[i][j]<<' ';
		}
		outvar<<endl;
	}
	for(int i=0;i<pro_table.size();i++)
	{
		for(int j=0;j<pro_table[i].size();j++)
		{
			outpro<<pro_table[i][j]<<' ';
		}
		outpro<<endl;
	}
	
	if(now()!=25)
	{
		error<<"LINE"<<line<<": "<<"�﷨������ɣ�������δ��Լ���"<<endl;
	}
	
	infile.close();
	error.close();
	outvar.close();
	outpro.close();
	return 0;
}
