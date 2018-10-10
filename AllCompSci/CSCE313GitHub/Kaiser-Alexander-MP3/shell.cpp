/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */ 
/* Project: CSCE-313 Machine Problem #4                                              */
/*                                                                           */
/* File: ./shell.cpp                                                         */ 
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Standard Libaray Includes                                                 */
/* ------------------------------------------------------------------------- */

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<unistd.h>
#include<stdlib.h>
#include<regex>
#include<string>
#include<vector>
#include<stdio.h>
#include<algorithm>
#include<string.h>
#include<string>
#include<sstream>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;
/* ------------------------------------------------------------------------- */
/* Global Variables                                                          */
/* ------------------------------------------------------------------------- */
bool bck_items;
char* input_prompt;
bool output_prompt;
vector<pid_t> backup;

// Put any global variables here

/* User Definable Custom Prompt:
 * 
 * Allows users to define custom prompts using a simple command to change the string.  
 * The following set of macros will be replaced as follows:
 *
 *   - DIR: Current directory 
 *   - WHO: User's name
 *   - DATE: Current date
 *   - TIME: Current time
 */
vector<string> back_setter(vector<string> variable){
	if(variable[variable.size() - 1] == "&"){
		bck_items = true;
		
		variable.pop_back();
	}
	else{
		bck_items = false;
		
	}
	return variable;
}

int exec_func(vector<string> variable){
	vector<char*> vector_of_arguments(variable.size() + 1);
	for(unsigned i = 0; i < variable.size(); i++){
		vector_of_arguments[i] = &variable[i][0];
	}
	execvp(vector_of_arguments[0],vector_of_arguments.data());
	cerr << "Incorrect\n";
	exit(0);
	
	return -1;
}
void wait_func(int address){
	if(bck_items){
		backup.push_back(address);
		waitpid(address,0,WNOHANG);
	}
	else{
		waitpid(address,0,0);
	}
}
void check_back_setter(){
	for(int i = 0; i < backup.size(); i++){
		waitpid(backup[0],0,WNOHANG);
	}
}
void run_del(vector<vector<string>> program, vector<string> delimit, int* finalize);
void update_prompt()
{
	input_prompt = get_current_dir_name();
	if(output_prompt){
		cout<<input_prompt<<": ";
	}
}
vector<string> remember_q(vector<string> ram){
	string porary = "";
	vector<string> cess;
	for(int i = 0; i < ram.size(); i++){
		for(int j = 0; j < ram[i].length(); j++){
			if(ram[i][j] != '\"'){
				porary += ram[i][j];
			}
		}
		cess.push_back(porary);
		porary = "";
	}
	return cess;
}
bool do_not_delete(string sti){
	return (sti != "|" && sti != ">" && sti != "<");
}
int special_char(vector<vector<string>> var){
	if(var[0][0] == "exit"){
		return 1;
	}
	else{
		if(var[0][0] == "cd"){
			int error_change = -1;
			error_change = chdir(var[0][1].c_str());
			return error_change;
		}
	}
	return 2;
}
void program_run(vector<string> program){
	int address;
	if((address = fork()) == 0){
		exec_func(program);
	}
	else{
		wait_func(address);
	}
}
bool validity_check(vector<string> variables){
	bool good_validity = true;
	if(variables.size() == 0){
		good_validity = false;
	}
	int j = 0;
	while(j < variables.size() - 1 && good_validity == true){
		good_validity = !((variables[j] == "<" || variables[j] == "|" || variables[j] == ">") && (variables[j + 1] == "<" || variables[j + 1] == "|" || variables[j + 1] == ">"));
		j++;
	}
	if(good_validity){
		good_validity = !((variables[0] == "<" || variables[0] == "|" || variables[0] == ">") && (variables[variables.size() - 1] == "<" || variables[variables.size() - 1] == "|" || variables[variables.size() - 1] == ">"));
	}
	return good_validity;
}
void output_file(vector<vector<string>> program, vector<string> delimit){
	int address;
	int error_exec = 0;
	int output = open(program[1][0].c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666);
	if(output < 0){
		cerr << "File Write Error\n";
	}
	else{
		if((address = fork()) == 0){
			dup2(output,1);
			close(output);
			exec_func(program[0]);
			
		}
		else{
			wait_func(address);
		}
	}
}

vector<string> token_to_delimit(vector<string> variable){
	int j = 0;
	vector<string> delimit;
	
	while(j < variable.size()){
		if(variable[j] == "<" || variable[j] == ">" || variable[j] == "|" ){
			delimit.push_back(variable[j]);
		}
		j++;
	}
	return delimit;
}
bool check_delimit(vector<string> delimit){
	int a = 0;
	int b = 0;
	for(int i = 0; i < delimit.size(); i++){
		a = a + (delimit[i]==">") ? 1:0;
		b = b + (delimit[i]=="<") ? 1:0;
	}
	bool check = (a<=1 && b<=1);
	return check;
}

void input_file(vector<vector<string>> program, vector<string> delimit){
	
	int file_in = open(program[1][0].c_str(),O_RDONLY);
	int address;
	int error_exec = 0;
	if(file_in > 0){
		cerr << "Can't open file!\n";
	}
	else{
		if(program.size() > 2 && delimit.size() > 1 && delimit[1] == ">"){
			int out_file = open(program[2][0].c_str(),O_CREAT|O_TRUNC|O_RDWR,0666);
			if(out_file < 0){
				cerr << "File Write Error\n";
			}
			else{
				if((address = fork()) == 0){
					dup2(file_in,0);
					
					dup2(out_file,1);
					close(file_in);
					
					
					close(out_file);
					exec_func(program[0]);
				}
				else{
					wait_func(address);
				}
			}
		}
		else{
			if(delimit.size()>1 && delimit[1]=="|" && program.size()>2){
				
				int free[2];
				pipe(free);
				if((address = fork()) == 0){
					dup2(file_in,0);
					close(file_in);
					
					
					dup2(free[1],1);
					close(free[0]);
					close(free[1]);
					exec_func(program[0]);
				}
				else{
					wait_func(address);
					program.erase(program.begin());
					delimit.erase(delimit.begin());
					run_del(program,delimit,free);
				}
				
			}
			else{
				if((address = fork()) == 0){
					dup2(file_in,0);
					close(file_in);
					exec_func(program[0]);
						
				}
				else{
					wait_func(address);
				}
			}
		}
	}
}

vector<int*> file_descript(vector<string> delimit){
	int temp = 0;
	vector<int*> mine;
	
	while(!delimit.empty()){
		if(delimit.front() == "|"){
			temp++;
		}
		delimit.erase(delimit.begin());
	}
	for(int i = 0; i < temp; i++){
		mine.push_back(new int[2]);
	}
	return mine;
	
}
vector<vector<string>> program_by_token(vector<string> variable){
	vector<vector<string>> progress;
	int p = 0;
	vector<string> temp;
	while(p < variable.size()){
		while(p < variable.size() && do_not_delete(variable[p])){
			temp.push_back(variable[p]);
			p++;
		}
		p++;
		progress.push_back(temp);
		temp.clear();
	}
	return progress;
	
}
void run_pipe_program(vector<vector<string>> progress, vector<string> delimit, int* piper){
	int error_exec = 0;
	int address;
	int pipette[2];
	pipe(pipette);
	if((address = fork()) == 0){
		dup2(pipette[1],1);
		close(pipette[1]);
		close(pipette[0]);
		exec_func(progress[0]);
	}
	else{
		close(pipette[1]);
		dup2(pipette[0],0);
		close(pipette[0]);
		wait_func(address);
		progress.erase(progress.begin());
		delimit.erase(delimit.begin());
		run_del(progress,delimit,pipette);
	}
}
void run_del(vector<vector<string>> program, vector<string> delimit, int* free = NULL){
	int address;
	if(delimit.size() > 0){
		if(delimit[0] == "<"){
			input_file(program,delimit);
		}
		if(delimit[0] == ">"){
			output_file(program,delimit);
		}
		if(delimit[0] == "|"){
			run_pipe_program(program,delimit,free);
		}
	}
	else{
		int error_exec = 0;
		if((address = fork()) == 0){
			if(free != NULL){
				dup2(free[0],0);
				close(free[0]);
				close(free[1]);
			}
			exec_func(program[0]);
		}
		else{
			wait_func(address);
		}
	}
}

int main(int argc, char** argv)
{
	int out_s = dup(1);
	int in_s = dup(0);
	output_prompt = true;
	int q;
	while((q = getopt(argc,argv,"t")) != -1){
		switch(q){
			case 't': output_prompt = false; break;
			case '?': cerr<<"Did not recognize command\n";
		}
	}
	
	int cmd_spec;
	string raw_input = "";
	bool go = false;
	vector<int*> file_description;
	vector<string> tokes;
	vector<string> delimit;
	vector<vector<string>> progress;
	
	update_prompt();
	while(getline(cin,raw_input)){
		check_back_setter();
		go = false;
		unsigned i = 0;
		unsigned b = 0;
		unsigned l = 0;
		vector<string> vec_cmd;
		vector<string> vec_cmd_q;
		while(l < raw_input.length()){
			switch(raw_input.at(l)){
				case '<': case '>': case ' ': case '&': case '|':
					if(raw_input.substr(i,b) != "" && raw_input.substr(i,b) != " "){
						vec_cmd.push_back(raw_input.substr(i,b));
					}
					if(raw_input.substr(l,1) != " "){
						vec_cmd.push_back(raw_input.substr(l,1));
					}
					b = 0;
					i = ++l;
				default:
					l++;
					b++;
			}
		}	
			if(raw_input.substr(i,b) != ""){
				vec_cmd.push_back(raw_input.substr(i,b));
			}
			vec_cmd_q = remember_q(vec_cmd);
			tokes = vec_cmd_q;
		
		if(tokes.size() > 0){
			tokes = back_setter(tokes);
			progress = program_by_token(tokes);
			delimit = token_to_delimit(tokes);
			file_description = file_descript(delimit);
			cmd_spec = special_char(progress);
			
			switch(cmd_spec){
				case 0: go = true; break;
				case 1: return 0; break;
				case -1: cerr << "Error!\n";
			}
			if(progress.size() > 0 && check_delimit(delimit) && !go && validity_check(tokes)){
				if(delimit.size() > 0){
					run_del(progress,delimit,NULL);
				}
				else{
					program_run(progress[0]);
				}
			}
		}
		dup2(in_s,0);
		dup2(out_s,1);
		update_prompt();
	}
	
	/* Parse command line arguments */

	/* Initialize prompt to default (current directory followed by a colon */

	/* Vector to maintain background processes */

	/* Tokenize input command */

	// The tokenizer may make empty tokens, eliminate them

	// Account for special commands

	// Check to see if the process is to run in the background or foreground

	/* Remove quotes from beginning and end of tokens */

	/* Replace the string \" with the character '\"' */

	/* Detect command pipes */

	/* Set up communication pipes */

	/* Detect Redirection */

	/* Execute commands in order */

	return 0;
}
