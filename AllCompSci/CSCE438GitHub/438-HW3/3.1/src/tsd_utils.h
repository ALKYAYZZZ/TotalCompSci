#ifndef TSN_UTILS_H
#define TSN_UTILS_H

#include "utils.h"
#include <unordered_map>
#include <map>
#include <unistd.h>
#include <queue>
#include <sys/wait.h>
#include <string>
#include <thread>
#include <grpc++/grpc++.h>
#include "tsn.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerReaderWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using tsn::TimelineStream;
using tsn::ConnectRequest;
using tsn::ConnectReply;
using tsn::CreateRequest;
using tsn::CreateReply;
using tsn::PersonRequest;
using tsn::PersonReply;
using tsn::ListRequest;
using tsn::ListReply;
using tsn::SNetwork;

typedef ServerReaderWriter<TimelineStream, TimelineStream>* timeline_stream;
typedef std::unique_ptr<SNetwork::Stub> StubPtr;

// Map for quick lookup of a user
std::unordered_map<std::string, timeline_stream> users;

// Folder in which TSN data is stored
const std::string GLOBAL_DIR = "./server_data/";


StubPtr connect_to(const std::string& host, const std::string& port){
	return tsn::SNetwork::NewStub(CreateChannel(host+":"+port,
					grpc::InsecureChannelCredentials()));
}

// Load user accounts on startup; O(n)
void load_users(){
	// Create data folder if not already present
	if(!file_exists(GLOBAL_DIR)) make_dir(GLOBAL_DIR);

	// Read in a list of user account names
	std::string str = load_file(GLOBAL_DIR + "userlist.txt");
	std::vector<std::string> usernames;
	split_str(usernames, str, '\n');
	for(std::string name : usernames) users[name] = nullptr;
}

// Add a new user account
bool add_user(const std::string& user){
	if(file_exists(GLOBAL_DIR + user)) return false;

	// Keep track of this user from now on (officially in the system)
	make_dir(GLOBAL_DIR + user);
	append_file(GLOBAL_DIR + "userlist.txt", user + '\n');
	users[user] = nullptr;
}

// Get a user's follower list from the data file; O(f)
std::vector<std::string> get_followers(const std::string& user){
	std::string file_contents = load_file(
			GLOBAL_DIR + user + "/followers.txt");
	std::vector<std::string> followers;
	split_str(followers, file_contents, '\n');
	return followers;
}

// Check if a user is following another user; O(log(f))
bool is_following(const std::string& follower, const std::string& followed){
	//for(const std::string& f : get_followers(followed))
	//	if(f == follower) return true;
	//return false;
	return binary_search_file_for_line(
			GLOBAL_DIR + followed + "/followers.txt", follower);
}

// TODO: optimize; O(f)
bool add_follower(std::string new_follower, std::string followed){
	//if(is_following(new_follower, followed)) return false;
	std::ostringstream ss;
	std::vector<std::string> fs = get_followers(followed);
	size_t i = 0;
	for(; i<fs.size() && fs[i] <= new_follower; ++i) ss << fs[i] << '\n';
	ss << new_follower;
	for(; i<fs.size(); ++i) ss << '\n' << fs[i];
	overwrite_file(GLOBAL_DIR + followed + "/followers.txt", ss.str());
	return true;
}

// TODO: optimize; O(f)
bool remove_follower(const std::string& follower, const std::string& followed){
	//if(!is_following(follower, followed)) return false;
	std::ostringstream ss;
	for(std::string f : get_followers(followed)){
		if(f != follower) ss << '\n' << f;
	}
	ss.seekp(1);
	overwrite_file(GLOBAL_DIR + followed + "/followers.txt", ss.str());
	return true;
}

// Convert a string into a timeline post
TimelineStream post_from_str(const std::string& str){
	std::vector<std::string> post_data;
	split_str(post_data, str, '|');
	TimelineStream t;
	t.set_username(post_data[0]);
	t.set_post(post_data[1]);
	t.set_time(post_data[2]);
	return t;
}

// Convert a timeline post into a string
std::string post_to_str(const TimelineStream& t){
	std::vector<std::string> post_vec {t.username(), t.post(), t.time()};
	std::set<char> esc {'|', '\\', '\n'};
	return join_str(post_vec.begin(), post_vec.end(), '|', true, esc);
}

// Add a new timeline post from a user; O(f)
bool post_to_timeline(TimelineStream& t){
	// Stamp on a timestamp
	t.set_time(to_time_str(get_current_time()));
	std::string post_str = post_to_str(t);

	// Add this post to the user's timeline
	append_file(GLOBAL_DIR + t.username() + "/timeline.txt", post_str + '\n');

	// Send this post out to all the followers
	for(std::string follower : get_followers(t.username())){
		// Add this post to the follower's timeline
		append_file(GLOBAL_DIR + follower + "/timeline.txt", post_str + '\n');

		// If the follower is online, also stick this in his/her stream
		if(users[follower] != nullptr) users[follower]->Write(t);
	}
}

// Get the last 20 timeline posts from a user
std::vector<std::string> get_timeline(const std::string& user, int num_posts=20){
	return load_file_ending(GLOBAL_DIR + user + "/timeline.txt", num_posts);
}

// Check to see if master server is dead and restart it when needed
void run_slave(int pid, std::string r_host, std::string r_port){
	while(!kill(pid, 0)) usleep(500000);
	while(true){
		char* h = const_cast<char*>(r_host.c_str());
		char* p = const_cast<char*>(r_port.c_str());
		char tsd[] = "./tsd", r[] = "r", t[] = "t";
		char* argv[] = {tsd, r, h,  t, p, NULL};
		pid = fork();
		if(pid == 0){
			execvp(argv[0], argv);
			exit(EXIT_SUCCESS);
		}
		int sts;
		waitpid(pid, &sts, 0);
	}
}

#endif
