/**** pikeyd.c *****************************/
/* M. Moller   2013-01-16                  */
/*   Universal RPi GPIO keyboard daemon    */
/*******************************************/

/*
   Copyright (C) 2013 Michael Moller.
   This file is part of the Universal Raspberry Pi GPIO keyboard daemon.

   This is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The software is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  
*/

#include <stdio.h>
#include <stdlib.h>
#include "joy_RPi.h"
#include "iic.h"

static void showHelp(void);
static void showVersion(void);

int main(int argc, char *argv[])
{
  int en_daemonize = 0;
  int i;

  for(i=1; i<argc; i++){
    if(!strcmp(argv[i], "-d")){
      en_daemonize = 1;
      //daemonize("/tmp", "/tmp/pikeyd.pid");
    }
    if(!strcmp(argv[i], "-k")){
      daemonKill("/tmp/pikeyd.pid");
      exit(0);
    }
    if(!strcmp(argv[i], "-r")){
      joy_enable_repeat();
    }
    if(!strcmp(argv[i], "-v")){
      showVersion();
      exit(0);
    }
    if(!strcmp(argv[i], "-h")){
      showHelp();
      exit(0);
    }
  }

  if(en_daemonize){
    daemonize("/tmp", "/tmp/pikeyd.pid");
  }

  init_iic();
  init_config();

  //test_config(); exit(0);

  //test_iic(0x20);  close_iic();  exit(0);

  //printf("init uinput\n");

  if(init_uinput() == 0){
    sleep(1);
    //test_uinput();
    if(joy_RPi_init()>=0){

      if(!en_daemonize){
	printf("Press ^C to exit.\n");
      }

      for(;;){
	joy_RPi_poll();
	usleep(4000);
      }

      joy_RPi_exit();
    }

    close_uinput();
  }

  return 0;
}

static void showHelp(void)
{
  printf("Usage: pikeyd [option]\n");
  printf("Options:\n");
  printf("  -d    run as daemon\n");
  printf("  -r    force key repeats\n");
  printf("  -k    try to terminate running daemon\n");
  printf("  -v    version\n");
  printf("  -h    this help\n");
}

static void showVersion(void)
{
  printf("pikeyd 1.3 (May 2013)\n");
  printf("The Universal Raspberry Pi GPIO keyboard daemon.\n");
  printf("Copyright (C) 2013 Michael Moller.\n");
  printf("This is free software; see the source for copying conditions.  There is NO\n");
  printf("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
}
