all:	
	gcc -g -o shell -w newshell.c pwd.c cd.c jobs.c pinfo.c ls.c history.c display.c echo.c systemcommands.c setenv.c headerfiles.h 

