// TinkerToy.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>

#include <Particle\Particle.h>
#include <Particle\Force\SpringForce.h>
#include <Particle\Force\Gravity.h>
#include <Particle\Constraint\RodConstraint.h>
#include <Particle\Constraint\CircularWireConstraint.h>
#include <Particle\Constraint\Constraint.hpp>
#include "imageio.h"

#include <vector>

//#include <gfx/vec3.h>
#include <Vector\Vector.hpp>
#include <Vector\Matrix.hpp>
#include "Camera.h"
#include <GL/glut.h>
#include <Integrators\Solver.hpp>
#include <Integrators\common.h>
#include <Integrators\ImplicitIntegrator.h>
#include <Integrators\EulerIntegrator.hpp>


/* macros */

/* external definitions (from solver) */

//extern void simulation_step( std::vector<Particle*> pVector, float dt, Integrator* integrator);

/* global variables */

static int N;
static float dt, d;
static int dsim;
static int dump_frames;
static int frame_number;

//static Vec3f CAMPOS;
//static Vec3f CAMTARGET;
CCamera* pCam;

// static Particle *pList;
static std::vector<Particle*> pVector;

static Solver* pSolver;
static int indexOfIntegrator=0;
static int win_id;
static int win_x, win_y;
static int mouse_down[3];
static int mouse_release[3];
static int mouse_shiftclick[3];
static int omx, omy, mx, my;
static int hmx, hmy;

//static SpringForce * delete_this_dummy_spring = NULL;
//static RodConstraint * delete_this_dummy_rod = NULL;
//static CircularWireConstraint * delete_this_dummy_wire = NULL;

static std::vector<Force*> pForces;
static std::vector<Constraint*> pConstraints;
static std::vector<Integrator*> pIntegrators;
/*
----------------------------------------------------------------------
free/clear/allocate simulation data
----------------------------------------------------------------------
*/

static void free_data ( void )
{
	pVector.clear();
	/*if (delete_this_dummy_rod) {
		delete delete_this_dummy_rod;
		delete_this_dummy_rod = NULL;
	}
	if (delete_this_dummy_spring) {
		delete delete_this_dummy_spring;
		delete_this_dummy_spring = NULL;
	}
	if (delete_this_dummy_wire) {
		delete delete_this_dummy_wire;
		delete_this_dummy_wire = NULL;
	}*/


	for(int ii=0; ii<pForces.size();ii++){
		delete pForces[ii];
	}
	for(int ii=0; ii<pConstraints.size();ii++){
		delete pConstraints[ii];
	}
	

}

static void clear_data ( void )
{
	int ii, size = pVector.size();

	for(ii=0; ii<size; ii++){
		pVector[ii]->reset();
	}
}

static void initialize(void)
{
	const float dist = 0.8;
	const Vector<float,3> center=make_vector(0.0f, 0.0f, 0.0f);
	const Vector<float,3> offset=make_vector(dist, 0.0f,0.0f);

	pVector.clear();
	pForces.clear();
	pConstraints.clear();

	pVector.push_back(new Particle(center + offset,1.0f,10,10, 0.1f));
	pVector.push_back(new Particle(center + offset + offset,1.0f,10,10, 0.1f));
	pVector.push_back(new Particle(center + offset + offset + offset,1.0f,10,10, 0.1f));

	double tmp = length(pVector[0]->m_ConstructPos-pVector[1]->m_ConstructPos);

	pForces.push_back(new SpringForce(pVector[0], pVector[1], tmp/2, 1.0, 1.0));
	pForces.push_back(new SpringForce(pVector[1], pVector[2], tmp/2, 1.0, 1.0));
	
	pConstraints.push_back(new CircularWireConstraint(pVector[0], center, dist));


	for(int i=0;i<pVector.size();i++)
	{
		pForces.push_back(new Gravity(pVector[i]));
	}
	pSolver->Initialize(pVector.size(), pVector,pForces,pConstraints);
}

static void init_system(void)
{
	/*const float dist = 0.2;
	const Vector<float,3> center=make_vector(0.0f, 0.0f, 0.0f);
	const Vector<float,3> offset=make_vector(dist, 0.0f,0.0f);*/

	/*CAMPOS= Vec3f(0.0,0.0,5.0);
	CAMTARGET = Vec3f(0.0,0.0,0.0);*/

	pCam = new CCamera();
	// Create three particles, attach them to each other, then add a
	// circular wire constraint to the first.
	pSolver = new Solver();
	
	//pVector.push_back(new Particle(center + offset,1.0f,10,10, 0.1f));
	//pVector.push_back(new Particle(center + offset + offset,1.0f,10,10, 0.1f));
	//pVector.push_back(new Particle(center + offset + offset + offset,10,10, 0.1f));


	
	// You shoud replace these with a vector generalized forces and one of
	// constraints...
	/*delete_this_dummy_spring = new SpringForce(pVector[0], pVector[1], dist, 1.0, 1.0);
	delete_this_dummy_rod = new RodConstraint(pVector[1], pVector[2], dist);
	delete_this_dummy_wire = new CircularWireConstraint(pVector[0], center, dist);*/

	//double tmp = length(pVector[0]->m_ConstructPos-pVector[1]->m_ConstructPos);

	//pForces.push_back(new SpringForce(pVector[0], pVector[1], tmp, 1.0, 1.0));
	//pConstraints.push_back(new RodConstraint(pVector[1], pVector[2], dist));
	//pConstraints.push_back(new CircularWireConstraint(pVector[0], center, dist));
	//pIntegrators.push_back(new ImplicitIntegrator(pSolver));
	pIntegrators.push_back(new EulerIntegrator(pSolver));


	initialize();
	
}

/*
----------------------------------------------------------------------
OpenGL specific drawing routines
----------------------------------------------------------------------
*/

static void pre_display ( void )
{
	//glViewport ( 0, 0, win_x, win_y );
	//glMatrixMode ( GL_PROJECTION );
	//glLoadIdentity ();
	////gluOrtho2D ( -1.0, 1.0, -1.0, 1.0 );
	//gluPerspective(60.0, 1.0, 1.5, 20.0);
	//gluLookAt(	CAMPOS[0],CAMPOS[1],CAMPOS[2], 
	//			CAMTARGET[0],CAMTARGET[1],CAMTARGET[2], 
	//			0.0, 1.0, 0.0);


	//glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	//glClear ( GL_COLOR_BUFFER_BIT );
	pCam->Render(win_x,win_y);
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
		}
	}
	frame_number++;
	
	glutSwapBuffers ();
}

static void draw_particles ( void )
{
	int size = pVector.size();

	for(int ii=0; ii< size; ii++)
	{
		pVector[ii]->draw();
	}
}

static void draw_forces ( void )
{
	// change this to iteration over full set
	/*if (delete_this_dummy_spring)
		delete_this_dummy_spring->draw();*/

	for(int ii=0; ii<pForces.size();ii++){
		pForces[ii]->draw();
	}

}

static void draw_constraints ( void )
{
	// change this to iteration over full set
	/*if (delete_this_dummy_rod)
		delete_this_dummy_rod->draw();
	if (delete_this_dummy_wire)
		delete_this_dummy_wire->draw();*/


	for(int ii=0; ii<pConstraints.size();ii++){
		pConstraints[ii]->draw();
	}


}

/*
----------------------------------------------------------------------
relates mouse movements to tinker toy construction
----------------------------------------------------------------------
*/

static void get_from_UI ()
{
	int i, j;
	// int size, flag;
	int hi, hj;
	// float x, y;
	if ( !mouse_down[0] && !mouse_down[2] && !mouse_release[0] 
	&& !mouse_shiftclick[0] && !mouse_shiftclick[2] ) return;

	i = (int)((       mx /(float)win_x)*N);
	j = (int)(((win_y-my)/(float)win_y)*N);

	if ( i<1 || i>N || j<1 || j>N ) return;

	if ( mouse_down[0] ) {

	}

	if ( mouse_down[2] ) {
	}

	hi = (int)((       hmx /(float)win_x)*N);
	hj = (int)(((win_y-hmy)/(float)win_y)*N);

	if( mouse_release[0] ) {
	}

	omx = mx;
	omy = my;
}

static void remap_GUI()
{
	int ii, size = pVector.size();
	for(ii=0; ii<size; ii++)
	{
		pVector[ii]->m_Position[0] = pVector[ii]->m_ConstructPos[0];
		pVector[ii]->m_Position[1] = pVector[ii]->m_ConstructPos[1];
	}
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
		case 'a':
			pCam->Move(make_vector(-1.0f,0.0f,0.0f));
			break;
		case 'd':
			pCam->Move(make_vector(1.0f,0.0f,0.0f));
			break;
		case 'w':
			pCam->Move(make_vector(0.0f,0.0f,-1.0f));
			break;
		case 's':
			pCam->Move(make_vector(0.0f,0.0f,1.0f));
			break;



/*

	case 'c':
	case 'C':
		clear_data ();
		break;

	case 'd':
	case 'D':
		dump_frames = !dump_frames;
		break;
*/
	case 'q':
	case 'Q':
		free_data ();
		exit ( 0 );
		break;


	case 'o':
		initialize();
		dsim = !dsim;
		break;
	case ' ':
		dsim = !dsim;
		break;
	}
}

static void mouse_func ( int button, int state, int x, int y )
{
	omx = mx = x;
	omx = my = y;

	if(!mouse_down[0]){hmx=x; hmy=y;}
	if(mouse_down[button]) mouse_release[button] = state == GLUT_UP;
	if(mouse_down[button]) mouse_shiftclick[button] = glutGetModifiers()==GLUT_ACTIVE_SHIFT;
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

static void idle_func ( void )
{
	if ( dsim ) //simulation_step( pVector, dt );
	{
		pSolver->update(pVector,dt, pIntegrators[indexOfIntegrator]);
		
	}
	else        {get_from_UI();remap_GUI();}

	glutSetWindow ( win_id );
	glutPostRedisplay ();
}

static void display_func ( void )
{
	pre_display ();

	draw_forces();
	draw_constraints();
	draw_particles();

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
	win_id = glutCreateWindow ( "Tinkertoys!" );

	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

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

	if ( argc == 1 ) {
		N = 64;
		dt = 0.01f;
		d = 5.f;
		fprintf ( stderr, "Using defaults : N=%d dt=%g d=%g\n",
			N, dt, d );
	} else {
		N = atoi(argv[1]);
		dt = atof(argv[2]);
		d = atof(argv[3]);
	}

	printf ( "\n\nHow to use this application:\n\n" );
	printf ( "\t Toggle construction/simulation display with the spacebar key\n" );
	printf ( "\t Dump frames by pressing the 'q' key\n" );
	printf ( "\t Quit by pressing the 'q' key\n" );

	dsim = 0;
	dump_frames = 0;
	frame_number = 0;
	
	init_system();
	//initialize();
	win_x = 512;
	win_y = 512;
	open_glut_window ();

	glutMainLoop ();

	exit ( 0 );
}

