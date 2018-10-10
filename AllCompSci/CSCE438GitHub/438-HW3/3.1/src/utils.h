#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <set>
#include <fstream>
#include <iterator>
#include <sys/stat.h>
#include <algorithm>
//#include <memory>
#include <iostream>

// Escape a character or set of characters in a string with backslash
void escape_str(std::string& str, const std::set<char>& esc){
	std::ostringstream ss;
	for(char c : str){
		if(esc.count(c) || c == '\\') ss << '\\';
		ss << c;
	}
	str = ss.str();
}

// Remove all unescaped backslashes from a string
void unescape_str(std::string& str){
	std::ostringstream ss;
	bool no_esc = true;
	for(char c : str){
		if(c == '\\' && no_esc) no_esc = false;
		else no_esc = true, ss << c;
	}
	str = ss.str();
}

// Check if the character at position i in the string is escaped
bool is_escaped(const std::string& host, size_t i){
	bool esc = false;
	while(i && host[--i] == '\\') esc = !esc;
	return esc;
}

// Split a string based on a deliminator and store the results in a referenced vector
// Added feature of not splitting on character if it is escaped
void split_str(std::vector<std::string>& res, const std::string& str, char delim,
		bool ignore_empty=true, bool skip_escape=true, bool unescape=true){
	size_t i = 0, j = 0, len = str.size();
	while(i != len){
		if(str[i] == delim){
			if(j == i && ignore_empty) ++j;
			else if(!skip_escape || !is_escaped(str, i)){
				res.push_back(str.substr(j, i-j));
				if(unescape) unescape_str(res.back());
				j = i + 1;
			}
		}
		++i;
	}
	if(j != i || !ignore_empty){
		res.push_back(str.substr(j, i));
		if(unescape) unescape_str(res.back());
	}
}

// Basically equivalent to boost's join functionality,
// but with better handling of escaped characters
template<class Iterator>
std::string join_str(Iterator a, Iterator b, char delim, bool escape=true){
	join_str(a, b, delim, escape, {'\\', delim});
}
template<class Iterator>
std::string join_str(Iterator a, Iterator b, char delim, bool escape=true, const std::set<char>& esc={'\\'}){
	std::ostringstream ss;
	if(escape) escape_str(*a, esc);
	ss << *a; ++a;
	std::for_each(a, b, [&](std::string s){
		if(escape) escape_str(s, esc);
		ss << delim << s;
	});
	return ss.str();
}


// Very fast way of checking if a file/directory exists
bool file_exists(const std::string& name){
	struct stat buffer;
	return stat(name.c_str(), &buffer) == 0;
}

// Load an entire file into a string. Whoop
std::string load_file(const std::string& filename){
	std::ostringstream ss;
	ss << std::ifstream(filename).rdbuf();
	return ss.str();
}

// Creates a directory with as many permissions as permitted
// Returns true if successful, false otherwise
bool make_dir(const std::string& dirname){
	return mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
}

// Unnecessarily optimized function to read the last n lines of a file!
// I owe a lot of internet sources for this one. Primary info source:
// http://en.cppreference.com/w/cpp/io/basic_fstream
std::vector<std::string> load_file_ending(const std::string& filename, int n=20){
	std::ifstream file(filename);
	file.seekg(-1, std::ifstream::end);
	const std::streamoff len = file.tellg();

	std::vector<std::string> lines;
	lines.reserve(n);
	std::ostringstream ss;
	char c;
	int num_read = 0;
	for(int i=0; i<=len; ++i){
		c = file.get();
		if(c == '\n'){
			if(ss.tellp()){
				lines.push_back(ss.str());
				std::reverse(lines[num_read].begin(), lines[num_read].end());
				//ss.clear();
				ss = std::ostringstream();
				if(++num_read == n) break;
			}
		}
		else ss << c;
		file.seekg(-2, std::ifstream::cur);
	}
	if(num_read < n && ss.tellp()){
		lines.push_back(ss.str());
		std::reverse(lines[num_read].begin(), lines[num_read].end());
	}
	std::reverse(lines.begin(), lines.end());
	return lines;
}

// Performs binary search on a file to see if it contains a specified line
bool binary_search_file_for_line(const std::string& filename, const std::string& target){
	std::ifstream file(filename);
	std::streamoff srt = file.tellg();
	file.seekg(0, std::ifstream::end);
	std::streamoff end = file.tellg();
	std::streamoff mid;

	std::string line;
	while(srt < end){
		mid = (end+srt)/2;
		file.seekg(mid);
		while(mid && file.get() != '\n') file.seekg(--mid);
		file >> line;
		if(line < target){
			srt = file.tellg();
			if(++srt == 0) return false;
		}
		else if(line > target) end = mid;
		else return true;
	}
	return false;
}

// Utility to append content to a file.  This is great for keeping track
// of timeline posts, since we can take the last lines 20 (by default)
void append_file(const std::string& filename, const std::string& str){
	std::ofstream(filename, std::fstream::app) << str;
}

// Useful for overwriting outdated files
void overwrite_file(const std::string& filename, const std::string& str, bool delete_empty=true){
	if(str.empty() && delete_empty) remove(filename.c_str());
	else std::ofstream(filename, std::fstream::out) << str;
}

// Get current time in milliseconds
#include <sys/time.h>
unsigned long long get_current_time(){
	timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec*(unsigned long long)1000 + t.tv_usec/1000;
//	return time(0);
//	return std::chrono::duration_cast<std::chrono::milliseconds>(
//			std::chrono::system_clock::now().time_since_epoch()).count();
}

// Format a timestamp into a nice string
std::string to_time_str(long timestamp){
	//std::time_t t = std::chrono::system_clock::to_time_t(timestamp);;
	std::ostringstream ss;
	ss << std::put_time(localtime(&timestamp), "%d-%m-%Y %H-%M-%S");
	return ss.str();
}
#endif
