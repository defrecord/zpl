/******************************************************************************

            Copyright (c) 1996 - 2004  --  University of Washington

******************************************************************************/

/*********************************************************************
*  mpi_zlib.c:
*
**********************************************************************/
   

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#include "md_zlib.h"
#include "start.h"
#include "mpi_zlib.h"
#include "grid.h"
#include "distribution.h"
#include "timer.h"
#include "zc_proto.h"
#include "zerror.h"
#include "zlib.h"

int _able_to_print = 1;
int _print_node = 0;

/* tells whether or not the user is running in quiet mode -- ZPL will
   not print out as much extraneous info (such as processor mesh size) */

int _quiet_mode=0;

/* This function is used by the current version of LAM MPI to catch
   signals sent by MPI to processes to make them halt.  Without
   registering such a handler, a non-zero exit code is given, prompting
   an annoying message from mpirun  -- BLC */

#ifdef LAM_MPI
static void signal_handler(int sig) {
  if (_quiet_mode <= 0) {
    fprintf(stderr,"[%d] Halt called by another process\n",_INDEX);
  }
  exit(0);
}
#endif


/* This function goes through the command line and gets rid of the
   equal signs that may have been used in specifying config var
   values. */

static void _NullifyEquals(int argc,char *argv[]) {
  int i;
  char *equals;

  for (i=1;i<argc;i++) {
    equals=strchr(argv[i],'=');
    if (equals!=NULL) {
      *equals='\0';
    }
  }
}

void _MDSetupGrid(_grid_fnc grid) {
  _GRID_SLICE_V_SIZE(grid) = 0;
  _GRID_SLICE_V(grid) = NULL;
  _GRID_WORLD(grid) = MPI_COMM_NULL;
}

void _MDInitGrid(_grid_fnc grid) {
  int i;

  if (grid != _DefaultGrid) {
    if (_GRID_WORLD(grid) != MPI_COMM_NULL) {
      MPI_Comm_free(&_GRID_WORLD(grid));
    }
    MPI_Comm_split(_GRID_WORLD(_DefaultGrid), (_INDEX != _PROCESSORS), _INDEX, &_GRID_WORLD(grid));
  }

  if (_GRID_SLICE_V(grid)) {
    for (i = 0; i < _GRID_SLICE_V_SIZE(grid); i++) {
      if (_GRID_SLICE(grid, i) != MPI_COMM_NULL) {
	MPI_Comm_free(&_GRID_SLICE(grid, i));
      }
    }
    _zfree(_GRID_SLICE_V(grid), "mpi grid slices");
  }
  _GRID_SLICE_V(grid) = (MPI_Comm*)_zmalloc(_GRID_NUMSLICES(grid) * sizeof(MPI_Comm), "mpi grid slices");
  _GRID_SLICE_V_SIZE(grid) = _GRID_NUMSLICES(grid);
  MPI_Comm_split(_GRID_WORLD(_DefaultGrid), (_INDEX != _PROCESSORS), _INDEX, &_GRID_SLICE(grid, 0));
  /* _GRID_SLICE(grid, 0) = _GRID_WORLD(grid);*/

  for (i = 1; i < _GRID_NUMSLICES(grid); i++) {
    _GRID_SLICE(grid, i) = MPI_COMM_NULL;
  }

}


/* This function contains all of the calls which need to be done on
   every processor.*/

static void _PerNode(int argc,char *argv[]) {
  int result = 1;
  MPI_Status status;

  /* Call the procedure _ParseConfigArgs() which is generated by the
     ZPL compiler in order to parse the command-line for
     user-initialized configuration variables. */
  _NullifyEquals(argc,argv);


  /* Initialize our node's state vector. */
  _InitRuntime(argc,argv);

  /* Barrier synch to make sure all threads are ready to go before
     calling main() */
  _BarrierSynch();


  /* Call the user's entry point. */
  _zplmain();

  /* Print out a message so it's clear that the user program has
     completed. */
  _BarrierSynch();
  if (_INDEX==0) {
    if (_quiet_mode <= 0) {
      printf("Your zpl program has completed running.\n");
      fflush(stdout);
    }
  }

  if (!_able_to_print) {
    MPI_Send(&result,1,MPI_INT,_print_node,_DONETAG,MPI_COMM_WORLD);
    MPI_Recv(&result,1,MPI_INT,_print_node,_DONETAG,MPI_COMM_WORLD,&status);
    MPI_Barrier(MPI_COMM_WORLD);
  }
}


int main(int argc,char *argv[]) {
  int numprocs=0;
  int i;
  int color;
  int flag;

  /* Initialize the ZPL timer so that the time spent preprocessing can
     be measured */

  /* FILL-IN (optional): At this point, any initialization functions
     which need to be executed on every processor can be called.
     Examples might include: initialization of communications
     libraries, joining processor groups, setting up console I/O,
     etc. */

  MPI_Init(&argc, &argv);

  _InitTimer();   /*** for machine-specific timing routines ***/

  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  
  /* FILL-IN (required): Somewhere in here, _INDEX should be set to be
     the logical index number of the processor.  Processors should be
     numbered starting with 0. */
  MPI_Comm_rank(MPI_COMM_WORLD, &_INDEX);

#ifdef LAM_MPI
  signal(SIGUSR2, signal_handler);
#endif
  
  /* This function is called to parse all non-config var information
     out of the command line and into the machine-independent
     libraries */

  _ParseArgs(argc,(char **)argv,0);
    
  /* Query the machine-independent libraries to determine the mesh
     size which will be used, and whether we should run in quiet
     mode. */

  _PROCESSORS=0;
  for (i = 0; i < _MAXRANK; i++) {
    _GRID_SIZE(_DefaultGrid, i) = 0;
  }
  _QueryProcInfo(&_PROCESSORS,_GRID_SIZE_V(_DefaultGrid));

  _QueryQuietMode(&_quiet_mode);
  
  /* find the print node if (numprocs != _PROCESSORS) else all nodes
     can print */
  if(numprocs != _PROCESSORS){ 
    _print_node = _QueryMPIPrintNode();
  } else {
    _print_node = _INDEX;
  }
 
  /* split processors into those that compute and the one that prints */
  color = 0;
  if(numprocs != _PROCESSORS && _INDEX == _print_node) {
    color = MPI_UNDEFINED;
  }
  MPI_Comm_split(MPI_COMM_WORLD,color,_INDEX,&_GRID_WORLD(_DefaultGrid));
  _GRID_SLICE_V_SIZE(_DefaultGrid) = 0;
  _GRID_SLICE_V(_DefaultGrid) = NULL;

  _able_to_print = 0;
  if (numprocs == _PROCESSORS) {
    _able_to_print = 1;
  } else if (numprocs != _PROCESSORS && _INDEX == _print_node) {
    _able_to_print = 1;
  }

  if (_quiet_mode <= 0) {
    _PrintGridConfiguration(_DefaultGrid);
    if (_INDEX == _print_node && !_able_to_print) {
      printf("(also using one extra task for I/O)\n");
    }
  }

  if (numprocs != _PROCESSORS && _INDEX == _print_node) {
    _able_to_print = 1;
    _HandlePrintRequests(_quiet_mode);
  }

  if (numprocs != _PROCESSORS && _INDEX != _print_node) {
    MPI_Comm_rank(_GRID_WORLD(_DefaultGrid), &_INDEX);
  }

  /* Initialize all things which need to be done by multiple
     processors.  The user's entry point is called from within this
     function.  */
  
  _PerNode(argc,argv);

  /* If we get to this point, the user's code must have completed, so
     we can exit without errors. */

  _UnInitRuntime();

  MPI_Finalize();

  exit(0);
}


/* A general barrier synch */

void _BarrierSynch() {
  if (_PROCESSORS != 1) {
    MPI_Barrier(_GRID_WORLD(_DefaultGrid));
  }
}