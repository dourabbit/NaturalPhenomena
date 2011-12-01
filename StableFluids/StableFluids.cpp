// StableFluids.cpp : Defines the entry point for the console application.
//

#include "ScalarField.h"
#include "VectorField.h"
#include "MacGrid2D.h"
#include "imageio.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

/* macros */

#define IX(i,j) ((i)+(N+2)*(j))
#define IXX(i,j) ((i)+(N+3)*(j))
/* global variables */

static int N;
static float dt, diff, visc;
static float force, source;
static int dvel;
static int dump_frames;
static int frame_number;

static VectorField *VelocityField, *PrevVelocityField;
static ScalarField *DensityField, *PrevDensityField;
static MacGrid2D *Grid;
static int win_id;
static int win_x, win_y;
static int mouse_down[3];
static int omx, omy, mx, my;


/*
----------------------------------------------------------------------
free/clear/allocate simulation data
----------------------------------------------------------------------
*/


static void free_data ( void )
{
	if( VelocityField ) delete ( VelocityField );
	if( PrevVelocityField ) delete ( PrevVelocityField );
	if( DensityField ) delete ( DensityField );
	if( PrevDensityField ) delete ( PrevDensityField );
	if( Grid ) delete ( Grid );
}

static void clear_data ( void )
{
	int i, size=(N+2)*(N+2);

	for ( i=0 ; i<size ; i++ ) {
		/*
		(*VelocityField)[i][0] = (*VelocityField)[i][1] = 0.0f;
		(*PrevVelocityField)[i][0] = (*PrevVelocityField)[i][1] = 1.0f;*/
		(*DensityField)[i] = 0.0f;
		(*PrevDensityField)[i] = 0.0f;
	}

	size=(N+3)*(N+3);
	for ( i=0 ; i<size ; i++ ) {
		(*VelocityField)[i].x = (*VelocityField)[i].y = 0.0f;
		(*PrevVelocityField)[i].x = 0.0f;
		(*PrevVelocityField)[i].y =0.0f;
	}
	
	
	
	
}

static int allocate_data ( void )
{
	VelocityField	  = new VectorField(N);
	PrevVelocityField = new VectorField(N);
	DensityField	  = new ScalarField(N);
	PrevDensityField  = new ScalarField(N);

	Grid = new MacGrid2D(N,dt,visc);

	if ( !VelocityField || !PrevVelocityField || !DensityField || !PrevDensityField ) {
		fprintf ( stderr, "cannot allocate data\n" );
		return ( 0 );
	}

	clear_data();
	return ( 1 );
}


/*
----------------------------------------------------------------------
OpenGL specific drawing routines
----------------------------------------------------------------------
*/

static void pre_display ( void )
{
	glViewport ( 0, 0, win_x, win_y );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D ( 0.0, 1.0, 0.0, 1.0 );
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}

static void post_display ( void )
{
	// Write frames if necessary.
	if (dump_frames) {
		const int FRAME_INTERVAL = 4;
		if ((frame_number % FRAME_INTERVAL) == 0) {
			const unsigned int w = glutGet(GLUT_WINDOW_WIDTH);
			const unsigned int h = glutGet(GLUT_WINDOW_HEIGHT);
			unsigned char * buffer = (unsigned char *) malloc(w * h * 4 * sizeof(unsigned char));
			if (!buffer)
				exit(-1);
			// glRasterPos2i(0, 0);
			glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			char filename[13];
			sprintf(filename, "img%.5i.png", frame_number / FRAME_INTERVAL);
			printf("Dumped %s.\n", filename);
			saveImageRGBA(filename, buffer, w, h);
			free(buffer);
		}
	}
	frame_number++;

	glutSwapBuffers ();
}

static void draw_velocity ( void )
{
	int i, j;
	float x, y, h;

	h = 1.0f/N;

	glColor3f ( 1.0f, 1.0f, 1.0f );
	glLineWidth ( 1.0f );

	glBegin ( GL_LINES );

	for ( i=1 ; i<=N+1; i++ ) {
		x = (i-0.5f)*h;
		for ( j=1 ; j<=N+1; j++ ) {
			y = (j-0.5f)*h;
			
			DATA xi = ((*VelocityField)[IXX(i+1,j)].x+(*VelocityField)[IXX(i,j)].x)*0.5f;
			DATA yj = ((*VelocityField)[IXX(i+1,j)].y+(*VelocityField)[IXX(i,j)].y)*0.5f;

			glVertex2f ( x, y );
		
			glVertex2f (x+ xi,y+yj);
		}
	}
	
	

	glEnd ();
}

static void draw_density ( void )
{
	int i, j;
	float x, y, h, d00, d01, d10, d11;

	h = 1.0f/N;

	glBegin ( GL_QUADS );

	for ( i=0 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		for ( j=0 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;

			d00 = (*DensityField)[IX(i,j)];
			d01 = (*DensityField)[IX(i,j+1)];
			d10 = (*DensityField)[IX(i+1,j)];
			d11 = (*DensityField)[IX(i+1,j+1)];

			glColor3f ( d00, d00, d00 ); glVertex2f ( x, y );
			glColor3f ( d10, d10, d10 ); glVertex2f ( x+h, y );
			glColor3f ( d11, d11, d11 ); glVertex2f ( x+h, y+h );
			glColor3f ( d01, d01, d01 ); glVertex2f ( x, y+h );
		}
	}

	glEnd ();
}

/*
----------------------------------------------------------------------
relates mouse movements to forces sources
----------------------------------------------------------------------
*/

static void get_from_UI( ScalarField * d, VectorField * u_v )
{
	int i, j, size = (N+2)*(N+2);

	for ( i=0 ; i<size ; i++ ) {
		(*d)[i] = 0.0f;
	}
	
	for ( i=0 ; i<(N+3)*(N+3) ; i++ ) {
	
		(*u_v)[i][0] = (*u_v)[i][1] = 0.0f;
	}
	if ( !mouse_down[0] && !mouse_down[2] ) return;

	i = (int)((       mx /(float)win_x)*N+1);
	j = (int)(((win_y-my)/(float)win_y)*N+1);

	if ( i<1 || i>N || j<1 || j>N ) return;

	if ( mouse_down[0] ) {
		

		(*u_v)[IXX(i,j)].x = force * (mx-omx);
		(*u_v)[IXX(i,j)].y = force * (omy-my);

		//printf("\n?????????????????????%f:%f",(*u_v)[IXX(i,j)].x,(*u_v)[IXX(i,j)].y);
	}

	if ( mouse_down[2] ) {
		(*d)[IX(i,j)] = source;
	}

	omx = mx;
	omy = my;

	return;
}

/*
----------------------------------------------------------------------
GLUT callback routines
----------------------------------------------------------------------
*/

static void key_func ( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case 'c':
	case 'C':
		clear_data ();
		break;

	case 'd':
	case 'D':
		dump_frames = !dump_frames;
		break;

	case 'q':
	case 'Q':
		free_data ();
		exit ( 0 );
		break;

	case 'v':
	case 'V':
		dvel = !dvel;
		break;
	}
}

static void mouse_func ( int button, int state, int x, int y )
{
	omx = mx = x;
	omx = my = y;

	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
		button = 2;
	}

	mouse_down[button] = state == GLUT_DOWN;
}

static void motion_func ( int x, int y )
{
	mx = x;
	my = y;
}

static void reshape_func ( int width, int height )
{
	glutSetWindow ( win_id );
	glutReshapeWindow ( width, height );

	win_x = width;
	win_y = height;
}
//static bool isIntialized=false;
static void idle_func ( void )
{

/*
	if(!isIntialized)
	{
		Grid->VelTimeStep(*PrevVelocityField, *VelocityField);
		glutSetWindow ( win_id );
		glutPostRedisplay ();
		isIntialized = true;
		return;
	}*/
	get_from_UI( PrevDensityField, PrevVelocityField );

	//VelocityField->TimeStep( PrevVelocityField, VelocityField);
	//DensityField->TimeStep( PrevDensityField, VelocityField);

/*
	printf("\nPrevVelSourceX:\n");
	for(int j=0;j<N+3;j++)
	{
		for(int i=0;i<N+3;i++)
		{
			printf("%f\t", (*PrevVelocityField)[IXX(i,j)].x);
		}
		printf("\n");
	}
	printf("\nPrevVelSourceY:\n");
	for(int j=0;j<N+3;j++)
	{
		for(int i=0;i<N+3;i++)
		{
			printf("%f\t", (*PrevVelocityField)[IXX(i,j)].y);
		}
		printf("\n");
	}*/
	
	Grid->VelTimeStep(*PrevVelocityField, *VelocityField);
	Grid->DensityTimeStep(*PrevDensityField,*DensityField,*VelocityField);
	glutSetWindow ( win_id );
	glutPostRedisplay ();
}

static void display_func ( void )
{
	pre_display ();

	if ( dvel ) 
		draw_velocity ();
	else		
		draw_density ();
	
	//Grid->draw();
	post_display ();
}


/*
----------------------------------------------------------------------
open_glut_window --- open a glut compatible window and set callbacks
----------------------------------------------------------------------
*/

static void open_glut_window ( void )
{
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );

	glutInitWindowPosition ( 0, 0 );
	glutInitWindowSize ( win_x, win_y );
	win_id = glutCreateWindow ( "Stable Fluids!" );

	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();

	pre_display ();

	glutKeyboardFunc ( key_func );
	glutMouseFunc ( mouse_func );
	glutMotionFunc ( motion_func );
	glutReshapeFunc ( reshape_func );
	glutIdleFunc ( idle_func );
	glutDisplayFunc ( display_func );
}


/*
----------------------------------------------------------------------
main --- main routine
----------------------------------------------------------------------
*/

int main ( int argc, char ** argv )
{
	glutInit ( &argc, argv );

	if ( argc != 1 && argc != 6 ) {
		fprintf ( stderr, "usage : %s N dt diff visc force source\n", argv[0] );
		fprintf ( stderr, "where:\n" );\
			fprintf ( stderr, "\t N      : grid resolution\n" );
		fprintf ( stderr, "\t dt     : time step\n" );
		fprintf ( stderr, "\t diff   : diffusion rate of the density\n" );
		fprintf ( stderr, "\t visc   : viscosity of the fluid\n" );
		fprintf ( stderr, "\t force  : scales the mouse movement that generate a force\n" );
		fprintf ( stderr, "\t source : amount of density that will be deposited\n" );
		exit ( 1 );
	}

	if ( argc == 1 ) {
		N = 20;
		dt = 0.1f;
		diff = 0.0f;
		visc = 0.0f;
		force = 0.005f;
		source = 100.0f;
		fprintf ( stderr, "Using defaults : N=%d dt=%g diff=%g visc=%g force = %g source=%g\n",
			N, dt, diff, visc, force, source );
	} else {
		N = atoi(argv[1]);
		dt = atof(argv[2]);
		diff = atof(argv[3]);
		visc = atof(argv[4]);
		force = atof(argv[5]);
		source = atof(argv[6]);
	}

	printf ( "\n\nHow to use this demo:\n\n" );
	printf ( "\t Add densities with the right mouse button\n" );
	printf ( "\t Add velocities with the left mouse button and dragging the mouse\n" );
	printf ( "\t Toggle density/velocity display with the 'v' key\n" );
	printf ( "\t Clear the simulation by pressing the 'c' key\n" );
	printf ( "\t Quit by pressing the 'q' key\n" );

	dvel = 0;

	if ( !allocate_data () ) exit ( 1 );
	clear_data ();

	win_x = 512;
	win_y = 512;
	open_glut_window ();

	glutMainLoop ();

	exit ( 0 );
}


