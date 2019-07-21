#include <helpers.hpp>
#include <pwd.h>        // geteuid,getpwuid
#include <sys/types.h>  // umask, getpid
#include <sys/stat.h>   // umask


char *getUserName(void)
{
    char * username;
    // - getpwuid() function shall search the user database
    //    for an entry with a matching uid
    // - geteuid() function shall return the effective user
    //    ID of the calling process - this is used as the search criteria
    //    for the getpwuid() function
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
