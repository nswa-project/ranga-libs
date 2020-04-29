#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <ftw.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int ranga_lsdir(lua_State * L)
{
	const char *path2 = luaL_checkstring(L, 1);
	char path[4096];

	if (strstr(path2, "..") != NULL) {
		return 0;
	}

	snprintf(path, 2096, "/extensions/%s", path2);

	struct dirent *dent;
	DIR* dir;

	if ((dir = opendir(path)) == NULL)
		return 0;

	//int i = 1;
	lua_newtable(L);

	while ((dent = readdir(dir)) != NULL) {
		if (dent->d_name[0] == '.')
			continue;

		lua_pushstring(L, dent->d_name);
		switch (dent->d_type) {
		case DT_DIR:
			lua_pushnumber(L, 0);
			break;
		case DT_REG:
			lua_pushnumber(L, 1);
			break;
		defaut:
			lua_pushnumber(L, -1);
			break;
		}
		lua_settable(L, -3);
	}

	return 1;
}

static int ranga_unlink(lua_State * L)
{
	const char *name = luaL_checkstring(L, 1);
	lua_pushnumber(L, unlink(name));
	return 1;
}


static int nswa_openlog(lua_State * L)
{
	const char *name = luaL_checkstring(L, 1);
	openlog(name, 0, LOG_USER);
	return 0;
}

static int nswa_syslog(lua_State * L)
{
	const char *msg = luaL_checkstring(L, 1);
	syslog(LOG_DEBUG, "%s\n", msg);
	return 0;
}

//	{ "wget", wget },
//	{ "wget_close", wget_close },
static luaL_Reg mylibs[] = {
	{ "lsdir", ranga_lsdir },
	{ "unlink", ranga_unlink },
	{ "openlog", nswa_openlog },
	{ "syslog", nswa_syslog },
	{ NULL, NULL }
};

int luaopen_nswa(lua_State * L)
{
	const char *libname = "ranga";
	luaL_register(L, libname, mylibs);
	//lua_newtable(L);  
	//luaL_setfuncs(L, mylibs, 0);   
	return 1;
}
