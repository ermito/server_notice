/* dgramsrvr.c:
  *
  * Example datagram server:
  */
  #include <stdio.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <errno.h>
  #include <string.h>
  #include <time.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>


 /*
  * This function reports the error and
  * exits back to the shell:
  */
  void displayError(const char *on_what) {
     printf("%s\n",on_what);
     exit(1);
  }

  int main() {
     int z;
     char srvr_addr[8] = "0.0.0.0";
     struct sockaddr_in adr_inet; // AF_INET
     struct sockaddr_in adr_clnt; // AF_INET
     socklen_t len_inet;                // length
     int s;                       // Socket
     char dgram[512];             // Recv buffer
     char dtfmt[512];             // Date/Time Result
     time_t td;                   // Current Time and Date
     struct tm tm;                // Date time values

     s = socket(AF_INET,SOCK_DGRAM,0);
     if ( s == -1 ) {
      displayError("socket()");
     }
      memset(&adr_inet,0,sizeof(adr_inet));
      adr_inet.sin_family = AF_INET;
      adr_inet.sin_port = htons(35876);
      adr_inet.sin_addr.s_addr =inet_addr(srvr_addr);
 
      if ( adr_inet.sin_addr.s_addr == INADDR_NONE ) {
         displayError("bad address.");
      }
      len_inet = sizeof(adr_inet);
      z = bind(s, (struct sockaddr *)&adr_inet, len_inet);
      if ( z == -1 ) {
        displayError("bind()");
     }
      while(1) {
         len_inet = sizeof adr_clnt;
         z = recvfrom(s,            // Socket
                      dgram,        // Receiving buffer
                      sizeof dgram, // Max recv buf size
                      0,            // Flags: no options
                      (struct sockaddr *)&adr_clnt, // Addr
                      &len_inet);  // Addr len, in & out
         if ( z < 0 ) {
            displayError("recvfrom(2)");
         }

//   * Process the request:
         dgram[z] = 0; // null terminate
         if ( !strcasecmp(dgram,"QUIT") ) {
            break;     // Quit server
        }
        printf("%s\n",dgram);
	char *nargv[ ]={"",dgram,(char *)0};
	
	if (fork() == 0) {
                 execl("/usr/bin/notify-send", "echo", dgram, (char *) 0);
         perror("exec one failed");
         exit(1);
	}
//	execv("/usr/bin/notify-send",nargv);
      }

     close(s);
     return 0;
  }