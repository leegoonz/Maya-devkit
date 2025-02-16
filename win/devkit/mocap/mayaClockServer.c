/*
//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#ifndef _WIN32
# include <sys/time.h>
# include <sys/socket.h>
# include <getopt.h>
# include <unistd.h>
#else
# include <winsock2.h>
# include <windows.h>
# include <io.h>
#endif //wIN32

#ifdef	LINUX
#define	GETOPTHUH '?'
#endif	/* LINUX */

#ifdef _WIN32
char *   optarg = NULL;
int	    optind = 1;
int	    opterr = 0;
int     optlast = 0;
#define GETOPTHUH 0

int getopt(int argc, char **argv, char *pargs)
{
	if (optind >= argc) return EOF;

	if (optarg==NULL || optlast==':')
	{
		optarg = argv[optind];
		if (*optarg!='-' && *optarg!='/')
			return EOF;
	}

	if (*optarg=='-' || *optarg=='/') optarg++;
	pargs = strchr(pargs, *optarg);
	if (*optarg) optarg++;
	if (*optarg=='\0')
	{
		optind++;
		optarg = NULL;
	}
	if (pargs == NULL) return 0;  //error
	if (*(pargs+1)==':')
	{
		if (optarg==NULL)
		{
			if (optind >= argc) return EOF;
			// we want a second paramter
			optarg = argv[optind];
		}
		optind++;
	}
	optlast = *(pargs+1);

	return *pargs;
}
#else
#define closesocket close
#endif

/*
 * Missing prototypes
 */
#ifndef _WIN32

#include <maya/mocapserver.h>
#include <maya/mocapserial.h>
#else

#define bzero  ZeroMemory

#include <maya/mocapserver.h>
#include <maya/mocapserial.h>
#endif 

#ifndef lengthof
#define lengthof(array)	(sizeof(array) / sizeof(array[0]))
#endif /* lengthof */

static int daemon_mode = 0;
static int inetd_mode  = 0;
static int show_usage  = 0;
static int verbose     = 0;
static int debug_mode  = 0;

static char program[80];

static char *server_name = program;

static CapChannel	hours, minutes, seconds;

static int handle_client(int client_fd);
static void get_data(int client_fd);
static int create_channels(int client_fd);

int main(int argc, char **argv)
{
  int opt;
#ifndef _WIN32
  char *cptr;
#endif
  int status;
  int client_fd = -1;


  /*
   * Grab a copy of the program name
   */
#ifdef _WIN32
  _splitpath (argv[0], NULL, NULL, program, NULL);
#else
  cptr = strrchr(argv[0], '/');
  if (cptr)
  {
    strcpy(program, (cptr + 1));
  }
  else
  {
    strcpy(program, argv[0]);
  }
#endif //WIN32

  /*
   * Parse the options
   */
#ifdef DEVEL
  while ((opt = getopt(argc, argv, "hdvDin:")) != -1)
#else /* DEVEL */
  while ((opt = getopt(argc, argv, "hdn:")) != -1)
#endif /* DEVEL */
  {
    switch (opt)
    {
      case 'h':
	show_usage++;
	break;

      case 'd':
	daemon_mode++;
	break;

      case 'D':
	debug_mode++;
	break;

      case 'i':
	inetd_mode++;
	break;

      case 'n':
	server_name = optarg;
	break;

      case 'v':
	verbose++;
	break;

      case GETOPTHUH:
	show_usage++;
    }
  }

  /*
   * Check for errors
   */
  if (daemon_mode && inetd_mode) show_usage++;
  if (optind < argc) show_usage++;

  if (show_usage)
  {
    fprintf(stderr, "Usage:\n");
#ifdef DEVEL
    fprintf(stderr, "    %s [-hdvDi] [-n name]\n", program);
#else /* DEVEL */
    fprintf(stderr, "    %s [-hd] [-n name]\n", program);
#endif /* DEVEL */
    fprintf(stderr, "\n");
    fprintf(stderr, "        -h        Print this help message\n");
    fprintf(stderr, "        -d        Run as a daemon in the background\n");
#ifdef DEVEL
    fprintf(stderr, "        -i        Run from inetd\n");
    fprintf(stderr, "        -D        Set the debug flag\n");
    fprintf(stderr, "        -v        Set the vebose flag\n");
#endif /* DEVEL */
    fprintf(stderr, "        -n name   Set the UNIX socket name to name\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Defaults:\n");
    fprintf(stderr, "    %s -n %s\n", program, program);
    fprintf(stderr, "\n");

    exit(1);
  }

  if (inetd_mode)
  {
    /*
     * The socket to the client is open on file descriptor 0.
     * Handle one client and then exit.
     */
    client_fd = 0;
  }

  if (daemon_mode)
  {
    /*
     * Convert this process into a standard unix type daemon process.
     * It will be running in the background with no controlling terminal
     * and using syslog to report error messages.
     */
    status = CapDaemonize();
    if (status < 0)
    {
      CapError(-1, CAP_SEV_FATAL, program, NULL);
      exit(1);
    }
  }

  if (inetd_mode)
  {
    /*
     * The socket to the client is already open on file descriptor 0.
     * Handle one client and then exit.
     */
    status = handle_client(client_fd);
    exit(status);
  }
  else
  {
    while (1)
    {
      /*
       * Set up the server socket and wait for a connection.
       */
      client_fd = CapServe(server_name);
      if (client_fd < 0)
      {
		CapError(-1, CAP_SEV_FATAL, program, NULL);
		exit(1);
      }
      
      /* Handle client requests */
      status = handle_client(client_fd);

      if (status < 0)
      {
		CapError(-1, CAP_SEV_FATAL, program, NULL);
      }

      /* Shutdown the client */
      closesocket(client_fd);
      client_fd = -1;

      /* Go back and wait for another connection request */
      continue;
    }
  }
  return 0;
}

static int handle_client(int client_fd)
{
  int status;
  CapCommand cmd;
  char ruser[64], rhost[64], realhost[64];
  fd_set rd_fds;
  struct timeval base_timeout, timeout;
  int recording = 0;

  static int channels_created = 0;

  base_timeout.tv_sec  = 1;
  base_timeout.tv_usec = 0;

  while (1)
  {
    FD_ZERO(&rd_fds);
    FD_SET((unsigned int)client_fd, &rd_fds);
    timeout.tv_sec  = base_timeout.tv_sec;
    timeout.tv_usec = base_timeout.tv_usec;
    
	status = select(FD_SETSIZE, &rd_fds, NULL, NULL, &timeout);
    if (status < 0)
    {
#ifndef _WIN32
      if (errno == EINTR)
      {
	/* Ignore signals and try again */
	continue;
      }
#endif /*WIN32*/

      /* Otherwise, give a fatal error message */
      CapError(client_fd, CAP_SEV_FATAL, program, "select failed");
      CapError(client_fd, CAP_SEV_FATAL, "select", NULL);
      exit(1);
    }
    else if (status == 0)
    {
      /* We got a timeout */
      if (recording) get_data(client_fd);

      /* Try again */
      continue;
    }
    else
    {
      /* There is data on the client file descriptor */
      cmd = CapGetCommand(client_fd);
      switch (cmd)
      {
	case CAP_CMD_QUIT:
	  return 0;

#ifdef _WIN32
	case CAP_CMD_ERROR:
		return -1;
#endif

	case CAP_CMD_AUTHORIZE:
	  status = CapGetAuthInfo(client_fd, ruser, rhost, realhost);
	  if (status < 0)
	  {
	    return -1;
	  }

	  /*
	   * If user@host is not authorized to use this server then:
	   *
	   * status = CapAuthorize(client_fd, 0);
	   */
	  status = CapAuthorize(client_fd, 1);
	  break;

	case CAP_CMD_INIT:	/* Initial client/server handshake */
	  status = CapInitialize(client_fd, program);
	  break;

	case CAP_CMD_VERSION:	/* Send version information */
	  status = CapVersion(client_fd, program, "1.0",
				  "Clock capture server - v1.0");
	  break;

	case CAP_CMD_INFO:
	  if (!channels_created)
	  {
	    /* Only create the channel data once */
	    status = create_channels(client_fd);
	    if (status < 0)
	    {
	      break;
	    }
	    channels_created = 1;
	  }

#ifdef SERVER_SIDE_RECORDING
	/* The server side recording returns incorrect time in
	 * the current version
	 */
	  status = CapInfo(client_fd, 1.0, 100.0, 10.0, 512 * 1024, 1);
#else
	  status = CapInfo(client_fd, 0.0, 0.0, 0.0, 512 * 1024, 1);
#endif

	  break;
	  
	case CAP_CMD_DATA:	/* Send frame data */
	  get_data(client_fd);
	  status = CapData(client_fd);
	  break;

#ifdef SERVER_SIDE_RECORDING
	/* The server side recording returns incorrect time in
	 * the current version
	 */
	case CAP_CMD_START_RECORD:	/* Start recording */
    {
      int size = 0;
      float rate = 0.0f, time = 0.0f;
	  rate = CapGetRequestedRecordRate(client_fd);
	  size = CapGetRequestedRecordSize(client_fd);

	  /* Convert rate in Hz to a timeout value in a timeval structure */
	  if (rate <   1.0) rate =   1.0;
	  if (rate > 100.0) rate = 100.0;

	  time = 1.0 / rate;
	  base_timeout.tv_sec  = time;
	  base_timeout.tv_usec = (time - base_timeout.tv_sec) * 1000000;

	  status = CapStartRecord(client_fd, rate, size);
	  if (status != -1)
	  {
	    recording = 1;
	  }
	  break;
    }

	case CAP_CMD_STOP_RECORD:		/* Stop recording */
	  status = CapStopRecord(client_fd);
	  recording = 0;
	  base_timeout.tv_sec  = 1;
	  base_timeout.tv_usec = 0;
	  break;
#endif /* server side recording */

	default:			/* Ignore unknown commands */
	  status = CapError(client_fd, CAP_SEV_ERROR, program,
			    "Unknown server command.");
	  break;
      }

      if (status < 0)
      {
	return -1;
      }
    }
  }

  /* return 0; */
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ANGLE(t, a) (((t) % (a)) * 2 * M_PI / (a))
static void get_data(int client_fd)
{
  time_t now, midnight;
  struct tm *local;
  time_t elapsed;
  float hangle, mangle, sangle;

  now = time(0);

  local = localtime(&now);

  local->tm_hour = 0;
  local->tm_min  = 0;
  local->tm_sec  = 0;

  midnight = mktime(local);

  elapsed = now - midnight;

  hangle = (float) -ANGLE(elapsed, 12 * 60 * 60);
  mangle = (float) -ANGLE(elapsed, 60 * 60);
  sangle = (float) -ANGLE(elapsed, 60);

  CapSetData(hours, &hangle);
  CapSetData(minutes, &mangle);
  CapSetData(seconds, &sangle);

  return;
}


static int create_channels(int client_fd)
{

  hours   = CapCreateChannel("hours",   CAP_USAGE_ZROT, 1);
  minutes = CapCreateChannel("minutes", CAP_USAGE_ZROT, 1);
  seconds = CapCreateChannel("seconds", CAP_USAGE_ZROT, 1);

  return 0;
}

