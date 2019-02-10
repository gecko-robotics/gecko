#include "node.hpp"
#include <thread>
#include <pwd.h>  // geteuid,getpwuid
#include <sys/types.h>  // umask
#include <sys/stat.h>  // umask
#include <iostream>

// using namespace gecko;
using namespace std;

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0


// FIXME: pass other args
// void Threaded::run(void(*f)(void* args)){
//     thread t(f, &ok);
//     t.join();
// }

////////////////////////////////////////////////////////
Node::~Node(){
    cout << ">> Stopping thread " << the_thread.get_id() << endl;
    the_thread.join();
    printf(">> Node bye ...\n");
}

void Node::run(void(*f)(bool*)){
    the_thread = thread(f, &ok);
    cout << ">> Started thread " << the_thread.get_id() << endl;
}

bool Node::initialized = false;
std::mutex Node::g_mutex;
// Directory Node::db;
std::string Node::host_name;
std::string Node::host_addr;
std::string Node::mc_addr = {"224.3.29.110"};
int Node::mc_port = 11311;

// Node::Node(){
//     ;
// }

//////////////////////////////////////////////////////
char *getUserName(void)
{
	char * username;
	// the getpwuid() function shall search the user database for an entry with a matching uid
	// the geteuid() function shall return the effective user ID of the calling process - this is used as the search criteria for the getpwuid() function
	uid_t euid = geteuid();
	struct passwd *pw = getpwuid(euid);
	if(pw)
	{
		username = strdup(pw->pw_name);
	}
	else
	{
		if(asprintf(&username, "%i", euid) < 0)
			return NULL;
	}

	return username;
}

//////////////////////////////////////////////////////////


void Process::go_daemon(void)
{
	pid_t process_id = 0;

	// Create child process
	process_id = fork();

	// Indication of fork() failure
	if (process_id < 0)
	{
		printf("fork failed!\n");
		// Return failure in exit status
		exit(EXIT_FAILURE);
	}

	// PARENT PROCESS. Need to kill it.
	if (process_id > 0)
	{
		printf("started!\n");
		// return success in exit status
		exit(EXIT_SUCCESS);
	}

	//unmask the file mode
	umask(0);

	// set new session
	// creates a session and sets the process group ID
	// pid_t sid = 0;
	sid = setsid();
	if(sid < 0)
	{
		// Return failure
		printf("setsid failed!\n");
		exit(EXIT_FAILURE);
	}

	// Create grandchild process
	// Fork a second child and exit immediately to prevent zombies.  This
	// causes the second child process to be orphaned, making the init
	// process responsible for its cleanup.  And, since the first child is
	// a session leader without a controlling terminal, it's possible for
	// it to acquire one by opening a terminal in the future (System V-
	// based systems).  This second fork guarantees that the child is no
	// longer a session leader, preventing the daemon from ever acquiring
	// a controlling terminal.
	process_id = fork();

	// Indication of fork() failure
	if (process_id < 0)
	{
		printf("fork failed!\n");
		// Return failure in exit status
		exit(EXIT_FAILURE);
	}

	// PARENT PROCESS. Need to kill it.
	if (process_id > 0)
	{
		// return success in exit status
		exit(EXIT_SUCCESS);
	}

	// savepid();

	// Closing stdin, stdout and stderr is handled by dnsmasq
}

void Process::savepid(const string& fname)
{
	FILE *f;
    pid_t pid = sid;
	// pid_t pid = getpid();
	if((f = fopen(fname.c_str(), "w+")) == NULL)
	{
		// logg("WARNING: Unable to write PID to file.");
		// logg("         Continuing anyway...");
	}
	else
	{
		fprintf(f, "%i", (int)pid);
		fclose(f);
	}
	printf("PID of FTL process: %i", (int)pid);
}
