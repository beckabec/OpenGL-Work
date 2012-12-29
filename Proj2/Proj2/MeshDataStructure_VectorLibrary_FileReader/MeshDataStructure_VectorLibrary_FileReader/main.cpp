#include <stdlib.h>
#include <stdio.h>
#include "glut.h"

#include <fstream>
using namespace std;

#include "loaders.h"
#include "mesh.h"
#include "stb_image.c"

#define MIN2(a,b) (((a) < (b))?(a):(b))
#define MAX2(a,b) (((a) > (b))?(a):(b))

// window parameters
GLint winId;
int position_x		= 0;
int position_y		= 0;
int width			= 800;
int height			= 800;

// controling parameters
int mouse_button;
int mouse_x		= 0;
int mouse_y		= 0;
float scale		= 1.0;
float x_angle	= 0.0;
float y_angle	= 0.0;

// perspective projection parameters
GLfloat fieldofview = 45.0;
GLfloat aspect      = 1.0;
GLfloat nearPlane   = 0.1;
GLfloat farPlane    = 100.0;

// lighting parameters
GLfloat AmbientLight[]		= { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat DiffuseLight[]		= { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat SpecularLight[]		= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPos[]			= {-50.0f,50.0f,100.0f,1.0f};

// scene parameters
Vec3f bbxMin, bbxMax, bbxCenter;

// mesh model
Mesh* model = 0;
Mesh* cube;

// models
static const char* modelfile[] = {
	"../Models/Required/temple.obj",
	"../Models/Required/lady.obj",
	"../Models/Required/venusm-2000.obj",
	"../Models/Required/temple.obj",
	"../Models/Required/flowers.obj"
};

void drawCylinderTB();
int outlineCube = GL_TRIANGLES; //used for outline/filled option
int outlineCyl = GL_TRIANGLE_FAN; //used for outline/filled option
int fnormal_flag = 0;
int draw_fnormals = 0;
int vnormal_flag = 0;
int draw_vnormals = 0;
int draw_wnormals = 0;
int weighted = 0;
int w = 0, nav1 = 0, nav2 = 0, texOn = 1, smooth = 0;
float eyeY = 0.0, eyeX = 0.0, eyeZ = 0.0, centerX = 0.0, centerY =0.0, centerZ = 0.0;
float rotation = 0.0;
unsigned char *image;
GLsizei imageWidth = 128;
GLsizei imageHeight = 128;
GLfloat texEnv = GL_MODULATE;

//////////////////////////////////////////////////////////

void drawCube()
{
	// data references for convenience purpose
	vector<int> vi; //index array
	vector<int> ni;
	vector<int> ti;
	vector<int> vni;
	vector<Vec3f> v; //vertex array
	vector<Vec3f> n;
	vector<Vec2f> t;
	vector<Vec3f> vn;

	//left
	//cube->m_v.push_back(Vec3f(-0.5f,-0.5f,0.5f));
	v.push_back(Vec3f(-0.5f,-0.5f,0.5f));
	v.push_back(Vec3f(-0.5f,-0.5f,-0.5f));
	v.push_back(Vec3f(-0.5f,0.5f,-0.5f));
	v.push_back(Vec3f(-0.5f,0.5f,0.5f));
	//right
	v.push_back(Vec3f(0.5f,-0.5f,-0.5f));
	v.push_back(Vec3f(0.5f,-0.5f,0.5f));
	v.push_back(Vec3f(0.5f,0.5f,0.5f));
	v.push_back(Vec3f(0.5f,0.5f,-0.5f));
	//top
	v.push_back(Vec3f(0.5f,0.5f,0.5f));
	v.push_back(Vec3f(-0.5f,0.5f,0.5f));
	v.push_back(Vec3f(-0.5f,0.5f,-0.5f));
	v.push_back(Vec3f(0.5f,0.5f,-0.5f));
	//front
	v.push_back(Vec3f(0.5f,-0.5f,0.5f));
	v.push_back(Vec3f(-0.5f,-0.5f,0.5f));
	v.push_back(Vec3f(-0.5f,0.5f,0.5f));
	v.push_back(Vec3f(0.5f,0.5f,0.5f));
	//back
	v.push_back(Vec3f(-0.5f,-0.5f,-0.5f));
	v.push_back(Vec3f(0.5f,-0.5f,-0.5f));
	v.push_back(Vec3f(0.5f,0.5f,-0.5f));
	v.push_back(Vec3f(-0.5f,0.5f,-0.5f));
	//bottom
	v.push_back(Vec3f(-0.5f,-0.5f,0.5f));
	v.push_back(Vec3f(0.5f,-0.5f,0.5f));
	v.push_back(Vec3f(0.5f,-0.5f,-0.5f));
	v.push_back(Vec3f(-0.5f,-0.5f,-0.5f));

	// Left
	vi.push_back(0);
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(0);
	vi.push_back(2);
	vi.push_back(3);
	// Right
	vi.push_back(4);
	vi.push_back(5);
	vi.push_back(6);
	vi.push_back(4);
	vi.push_back(6);
	vi.push_back(7);
	// Top
	vi.push_back(8);
	vi.push_back(9);
	vi.push_back(10);
	vi.push_back(8);
	vi.push_back(10);
	vi.push_back(11);
	// Front
	vi.push_back(12);
	vi.push_back(13);
	vi.push_back(14);
	vi.push_back(12);
	vi.push_back(14);
	vi.push_back(15);
	// Back
	vi.push_back(16);
	vi.push_back(17);
	vi.push_back(18);
	vi.push_back(16);
	vi.push_back(18);
	vi.push_back(19);
	// Bottom
	vi.push_back(20);
	vi.push_back(21);
	vi.push_back(22);
	vi.push_back(20);
	vi.push_back(22);
	vi.push_back(23);

	t.push_back(Vec2f(0,0));
	t.push_back(Vec2f(1,0));
	t.push_back(Vec2f(0,1));
	t.push_back(Vec2f(1,1));

	for(int i=0; i<vi.size()/6;i++)
	{
		ti.push_back(0);
		ti.push_back(1);
		ti.push_back(2);
		ti.push_back(1);
		ti.push_back(2);
		ti.push_back(3);
	}
	
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0, 0.0, 1.0);
	// NOTE index i has to be adjusted
	for(int i = 0; i < vi.size()/6; i+=6)
	{
		for(int n = 0; n < 36; n++)
		{
			if(texOn == 1)
			{
				glTexCoord2f(t[ti[i+n]].x,t[ti[i+n]].y);
			}

			if(smooth == 1)
			{
				Vec3f offset = (v[vi[i]] - bbxCenter);
				glColor3f(fabs(cos(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
			}
			
			glVertex3f( v[vi[i+n]].x, v[vi[i+n]].y, v[vi[i+n]].z);

		}
	}
	glEnd();

	if(fnormal_flag == 1)
	{
		int k = 0;
		int v_1 = 0;
		int v_2 = 1;
		int v_3 = 2;
		int v_4 = 5;
		int index = 0;
		Vec3f v0, v1, norm, mid, triMid;
		float normalize;

		for(k = 0; k < 12; k+=2)
		{
			v0 = v[vi[v_1]] - v[vi[v_2]];
			v1 = v[vi[v_3]] - v[vi[v_2]];
			norm = v0%v1;
			norm.normalize();
			vn.push_back(Vec3f(norm.x,norm.y,norm.z));
			vni.push_back(index);
			index++;

			mid = (v[vi[v_1]] + v[vi[v_3]])/2;
			triMid = (v[vi[v_2]]+mid)/2;
			n.push_back(triMid);
			ni.push_back(k);

			triMid = (v[vi[v_4]]+mid)/2;
			n.push_back(triMid);
			ni.push_back(k+1);

			if(draw_fnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 255, 0);
					glVertex3f(n[ni[k]].x, n[ni[k]].y, n[ni[k]].z);
					glVertex3f(n[ni[k]].x+norm.x, n[ni[k]].y+norm.y, n[ni[k]].z+norm.z);
					glVertex3f(n[ni[k+1]].x, n[ni[k+1]].y, n[ni[k+1]].z);
					glVertex3f(n[ni[k+1]].x+norm.x, n[ni[k+1]].y+norm.y, n[ni[k+1]].z+norm.z);
				glEnd();
			}

			v_1 = v_1+6;
			v_2 = v_2+6;
			v_3 = v_3+6;
			v_4 = v_4+6;
		}

	///////create normal vectors//////	
		if(vnormal_flag == 1)
		{
			Vec3f sum;
	
			sum = Vec3f(vn[vni[0]].x +vn[vni[0]].x +vn[vni[3]].x +vn[vni[5]].x +vn[vni[5]].x, 
						vn[vni[0]].y +vn[vni[0]].y +vn[vni[3]].y +vn[vni[5]].y +vn[vni[5]].y,
						vn[vni[0]].z +vn[vni[0]].z +vn[vni[3]].z +vn[vni[5]].z +vn[vni[5]].z);
			Vec3f average = Vec3f(sum.x/5,sum.y/5,sum.z/5);

			sum = Vec3f(vn[vni[0]].x +vn[vni[5]].x +vn[vni[4]].x +vn[vni[4]].x, 
							vn[vni[0]].y +vn[vni[5]].y +vn[vni[4]].y +vn[vni[4]].y,
							vn[vni[0]].z +vn[vni[5]].z +vn[vni[4]].z +vn[vni[4]].z);
			Vec3f average1 = Vec3f(sum.x/4,sum.y/4,sum.z/4);

			sum = Vec3f(vn[vni[0]].x +vn[vni[0]].x +vn[vni[2]].x +vn[vni[2]].x +vn[vni[4]].x, 
							vn[vni[0]].y +vn[vni[0]].y +vn[vni[2]].y +vn[vni[2]].y +vn[vni[4]].y,
							vn[vni[0]].z +vn[vni[0]].z +vn[vni[2]].z +vn[vni[2]].z +vn[vni[4]].z);
			Vec3f average2 = Vec3f(sum.x/5,sum.y/5,sum.z/5);

			sum = Vec3f(vn[vni[0]].x +vn[vni[3]].x +vn[vni[3]].x +vn[vni[2]].x, 
							vn[vni[0]].y +vn[vni[3]].y +vn[vni[3]].y +vn[vni[2]].y,
							vn[vni[0]].z +vn[vni[3]].z +vn[vni[3]].z +vn[vni[2]].z);
			Vec3f average3 = Vec3f(sum.x/4,sum.y/4,sum.z/4);

			sum = Vec3f(vn[vni[1]].x +vn[vni[1]].x +vn[vni[5]].x +vn[vni[5]].x+vn[vni[4]].x, 
							vn[vni[1]].y +vn[vni[1]].y +vn[vni[5]].y +vn[vni[4]].y+vn[vni[4]].y,
							vn[vni[1]].z +vn[vni[1]].z +vn[vni[5]].z +vn[vni[4]].z+vn[vni[4]].z);
			Vec3f average4 = Vec3f(sum.x/5,sum.y/5,sum.z/5);

			sum = Vec3f(vn[vni[1]].x +vn[vni[5]].x +vn[vni[3]].x +vn[vni[3]].x, 
							vn[vni[1]].y +vn[vni[5]].y +vn[vni[3]].y +vn[vni[3]].y,
							vn[vni[1]].z +vn[vni[5]].z +vn[vni[3]].z +vn[vni[3]].z);
			Vec3f average5 = Vec3f(sum.x/4,sum.y/4,sum.z/4);

			sum = Vec3f(vn[vni[1]].x +vn[vni[1]].x +vn[vni[2]].x +vn[vni[2]].x+vn[vni[3]].x, 
							vn[vni[1]].y +vn[vni[1]].y +vn[vni[2]].y +vn[vni[2]].y +vn[vni[3]].y,
							vn[vni[1]].z +vn[vni[1]].z +vn[vni[2]].z +vn[vni[2]].z+vn[vni[3]].z);
			Vec3f average6 = Vec3f(sum.x/5,sum.y/5,sum.z/5);

			sum = Vec3f(vn[vni[1]].x +vn[vni[4]].x +vn[vni[4]].x +vn[vni[2]].x, 
							vn[vni[1]].y +vn[vni[4]].y +vn[vni[4]].y +vn[vni[2]].y,
							vn[vni[1]].z +vn[vni[4]].z +vn[vni[4]].z +vn[vni[2]].z);
			Vec3f average7 = Vec3f(sum.x/4,sum.y/4,sum.z/4);

			if(draw_vnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 0, 0);
					glVertex3f(v[vi[0]].x, v[vi[0]].y, v[vi[0]].z);
					glVertex3f(v[vi[0]].x+average.x, v[vi[0]].y+average.y, v[vi[0]].z+average.z);
					glVertex3f(v[vi[1]].x, v[vi[1]].y, v[vi[1]].z);
					glVertex3f(v[vi[1]].x+average1.x, v[vi[1]].y+average1.y, v[vi[1]].z+average1.z);
					glVertex3f(v[vi[2]].x, v[vi[2]].y, v[vi[2]].z);
					glVertex3f(v[vi[2]].x+average2.x, v[vi[2]].y+average2.y, v[vi[2]].z+average2.z);
					glVertex3f(v[vi[5]].x, v[vi[5]].y, v[vi[5]].z);
					glVertex3f(v[vi[5]].x+average3.x, v[vi[5]].y+average3.y, v[vi[5]].z+average3.z);
					glVertex3f(v[vi[6]].x, v[vi[6]].y, v[vi[6]].z);
					glVertex3f(v[vi[6]].x+average4.x, v[vi[6]].y+average4.y, v[vi[6]].z+average4.z);
					glVertex3f(v[vi[7]].x, v[vi[7]].y, v[vi[7]].z);
					glVertex3f(v[vi[7]].x+average5.x, v[vi[7]].y+average5.y, v[vi[7]].z+average5.z);
					glVertex3f(v[vi[8]].x, v[vi[8]].y, v[vi[8]].z);
					glVertex3f(v[vi[8]].x+average6.x, v[vi[8]].y+average6.y, v[vi[8]].z+average6.z);
					glVertex3f(v[vi[11]].x, v[vi[11]].y, v[vi[11]].z);
					glVertex3f(v[vi[11]].x+average7.x, v[vi[11]].y+average7.y, v[vi[11]].z+average7.z);
				glEnd();
			}
			if(draw_wnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(0, 0, 255);
					glVertex3f(v[vi[0]].x, v[vi[0]].y, v[vi[0]].z);
					glVertex3f(v[vi[0]].x+average.x, v[vi[0]].y+average.y, v[vi[0]].z+average.z);
					glVertex3f(v[vi[1]].x, v[vi[1]].y, v[vi[1]].z);
					glVertex3f(v[vi[1]].x+average1.x, v[vi[1]].y+average1.y, v[vi[1]].z+average1.z);
					glVertex3f(v[vi[2]].x, v[vi[2]].y, v[vi[2]].z);
					glVertex3f(v[vi[2]].x+average2.x, v[vi[2]].y+average2.y, v[vi[2]].z+average2.z);
					glVertex3f(v[vi[5]].x, v[vi[5]].y, v[vi[5]].z);
					glVertex3f(v[vi[5]].x+average3.x, v[vi[5]].y+average3.y, v[vi[5]].z+average3.z);
					glVertex3f(v[vi[6]].x, v[vi[6]].y, v[vi[6]].z);
					glVertex3f(v[vi[6]].x+average4.x, v[vi[6]].y+average4.y, v[vi[6]].z+average4.z);
					glVertex3f(v[vi[7]].x, v[vi[7]].y, v[vi[7]].z);
					glVertex3f(v[vi[7]].x+average5.x, v[vi[7]].y+average5.y, v[vi[7]].z+average5.z);
					glVertex3f(v[vi[8]].x, v[vi[8]].y, v[vi[8]].z);
					glVertex3f(v[vi[8]].x+average6.x, v[vi[8]].y+average6.y, v[vi[8]].z+average6.z);
					glVertex3f(v[vi[11]].x, v[vi[11]].y, v[vi[11]].z);
					glVertex3f(v[vi[11]].x+average7.x, v[vi[11]].y+average7.y, v[vi[11]].z+average7.z);
				glEnd();
			}			
		}
	}
}

void drawPlane()
{
	// data references for convenience purpose
	vector<int> vi; //index array
	vector<int> ni;
	vector<int> ti;
	vector<int> vni;
	vector<Vec3f> v; //vertex array
	vector<Vec3f> n;
	vector<Vec2f> t;
	vector<Vec3f> vn;

	v.push_back(Vec3f(-5.0f,-0.5f,5.0f));
	v.push_back(Vec3f(5.0f,-0.5f,5.0f));
	v.push_back(Vec3f(5.0f,-0.5f,-5.0f));
	v.push_back(Vec3f(-5.0f,-0.5f,-5.0f));

	v.push_back(Vec3f(-5.0f,-0.5f,-5.0f));
	v.push_back(Vec3f(5.0f,-0.5f,5.0f));
	v.push_back(Vec3f(5.0f,-0.5f,-5.0f));
	v.push_back(Vec3f(-5.0f,-0.5f,5.0f));
	
	vi.push_back(0);
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(0);
	vi.push_back(2);
	vi.push_back(3);

	vi.push_back(4);
	vi.push_back(5);
	vi.push_back(6);
	vi.push_back(4);
	vi.push_back(6);
	vi.push_back(7);

	t.push_back(Vec2f(0,0));
	t.push_back(Vec2f(1,0));
	t.push_back(Vec2f(0,1));
	t.push_back(Vec2f(1,1));

	for(int i=0; i<vi.size()/6;i++)
	{
		ti.push_back(0);
		ti.push_back(1);
		ti.push_back(2);
		ti.push_back(1);
		ti.push_back(2);
		ti.push_back(3);
	}

	glBindTexture(GL_TEXTURE_2D, 2);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0, 0.0, 1.0);
	// NOTE index i has to be adjusted
	for(int i = 0; i < vi.size()/6; i+=6)
	{
		for(int n = 0; n < 12; n++)
		{
			if(texOn == 1)
			{
				glTexCoord2f(t[ti[i+n]].x,t[ti[i+n]].y);
			}

			if(smooth == 1)
			{
				Vec3f offset = (v[vi[i]] - bbxCenter);
				glColor3f(fabs(cos(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
			}

			glVertex3f(v[vi[i+n]].x, v[vi[i+n]].y, v[vi[i+n]].z);
		}
	}
	glEnd();

	if(fnormal_flag == 1)
	{
		int j = 0;
		/////////////////////Face Normals/////////////////////////////
		Vec3f v0 = v[vi[4]] - v[vi[7]];
		Vec3f v1 = v[vi[6]] - v[vi[7]];
		Vec3f norm = v0%v1;
		norm.normalize();
		vn.push_back(Vec3f(norm.x,norm.y,norm.z));
		vni.push_back(j);
		j++;

		Vec3f mid = (v[vi[6]] + v[vi[8]])/2;
		Vec3f triMid = (v[vi[7]]+mid)/2;
		n.push_back(triMid);
		ni.push_back(0);

		triMid = (v[vi[11]]+mid)/2;
		n.push_back(triMid);
		ni.push_back(1);
		/////////////////////////////////////// 
		v0 = v[vi[6]] - v[vi[7]];
		v1 = v[vi[8]] - v[vi[7]];
		Vec3f norm1 = v0%v1;
		norm1.normalize();
		vn.push_back(Vec3f(norm1.x,norm1.y,norm1.z));
		vni.push_back(j);
		j++;

		mid = (v[vi[6]] + v[vi[8]])/2;
		triMid = (v[vi[7]]+mid)/2;
		n.push_back(triMid);
		ni.push_back(2);

		triMid = (v[vi[11]]+mid)/2;
		n.push_back(triMid);
		ni.push_back(3);
		/////////////////////////////////////// 
		v0 = v[vi[0]] - v[vi[1]];
		v1 = v[vi[2]] - v[vi[1]];
		Vec3f norm2 = v0%v1;
		norm2.normalize();
		vn.push_back(Vec3f(norm2.x,norm2.y,norm2.z));
		vni.push_back(j);
		j++;

		mid = (v[vi[0]] + v[vi[2]])/2;
		Vec3f mid2 = (v[vi[1]] + v[vi[3]])/2;
		triMid = (mid2+mid)/2;
		n.push_back(triMid);
		ni.push_back(4);

		mid2 = (v[vi[4]] + v[vi[6]])/2;
		triMid = (mid2+mid)/2;
		n.push_back(triMid);
		ni.push_back(5);
		/////////////////////////////////////// 
		v0 = v[vi[2]] - v[vi[1]];
		v1 = v[vi[0]] - v[vi[1]];
		Vec3f norm3 = v0%v1;
		norm3.normalize();
		vn.push_back(Vec3f(norm3.x,norm3.y,norm3.z));
		vni.push_back(j);
		j++;

		mid = (v[vi[0]] + v[vi[2]])/2;
		mid2 = (v[vi[1]] + v[vi[3]])/2;
		triMid = (mid2+mid)/2;
		n.push_back(triMid);
		ni.push_back(6);

		mid2 = (v[vi[4]] + v[vi[6]])/2;
		triMid = (mid2+mid)/2;
		n.push_back(triMid);
		ni.push_back(7);
	
		if(draw_fnormals == 1)
		{
			glBegin(GL_LINES);
				glColor3ub(255, 255, 0);
				glVertex3f(n[ni[0]].x, n[ni[0]].y, n[ni[0]].z);
				glVertex3f(n[ni[0]].x+norm.x, n[ni[0]].y+norm.y, n[ni[0]].z+norm.z);
				glVertex3f(n[ni[1]].x, n[ni[1]].y, n[ni[1]].z);
				glVertex3f(n[ni[1]].x+norm.x, n[ni[1]].y+norm.y, n[ni[1]].z+norm.z);
				glVertex3f(n[ni[2]].x, n[ni[2]].y, n[ni[2]].z);
				glVertex3f(n[ni[2]].x+norm1.x, n[ni[2]].y+norm1.y, n[ni[2]].z+norm1.z);
				glVertex3f(n[ni[3]].x, n[ni[3]].y, n[ni[3]].z);
				glVertex3f(n[ni[3]].x+norm1.x, n[ni[3]].y+norm1.y, n[ni[3]].z+norm1.z);
				glVertex3f(n[ni[4]].x, n[ni[4]].y, n[ni[4]].z);
				glVertex3f(n[ni[4]].x+norm2.x, n[ni[4]].y+norm2.y, n[ni[4]].z+norm2.z);
				glVertex3f(n[ni[5]].x, n[ni[5]].y, n[ni[5]].z);
				glVertex3f(n[ni[5]].x+norm2.x, n[ni[5]].y+norm2.y, n[ni[5]].z+norm2.z);
				glVertex3f(n[ni[6]].x, n[ni[6]].y, n[ni[6]].z);
				glVertex3f(n[ni[6]].x+norm3.x, n[ni[6]].y+norm3.y, n[ni[6]].z+norm3.z);
				glVertex3f(n[ni[7]].x, n[ni[7]].y, n[ni[7]].z);
				glVertex3f(n[ni[7]].x+norm3.x, n[ni[7]].y+norm3.y, n[ni[7]].z+norm3.z);
			glEnd();
		}

		/////////////////////////Per Vertex Normals///////////////////
		if(vnormal_flag == 1)
		{
			///////create normal vectors//////	
			Vec3f sum;

			sum = Vec3f(vn[vni[0]].x +vn[vni[1]].x, 
							vn[vni[0]].y +vn[vni[1]].y,
							vn[vni[0]].z +vn[vni[1]].z);
			Vec3f average = Vec3f(sum.x/2,sum.y/2,sum.z/2);

			if(draw_vnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 0, 0);
					glVertex3f(v[vi[0]].x, v[vi[0]].y, v[vi[0]].z);
					glVertex3f(v[vi[0]].x+average.x, v[vi[0]].y+average.y, v[vi[0]].z+average.z);
				glEnd();
			}
			else
			{
				glBegin(GL_LINES);
					glColor3ub(0, 0, 255);
					glVertex3f(v[vi[0]].x, v[vi[0]].y, v[vi[0]].z);
					glVertex3f(v[vi[0]].x+average.x, v[vi[0]].y+average.y, v[vi[0]].z+average.z);
				glEnd();
			}

			sum = Vec3f(vn[vni[1]].x +vn[vni[2]].x, 
							vn[vni[1]].y +vn[vni[2]].y,
							vn[vni[1]].z +vn[vni[2]].z);
			average = Vec3f(sum.x/2,sum.y/2,sum.z/2);

			if(draw_vnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 0, 0);
					glVertex3f(v[vi[1]].x, v[vi[1]].y, v[vi[1]].z);
					glVertex3f(v[vi[1]].x+average.x, v[vi[1]].y+average.y, v[vi[1]].z+average.z);
				glEnd();
			}
			else
			{
				glBegin(GL_LINES);
					glColor3ub(0, 0, 255);
					glVertex3f(v[vi[1]].x, v[vi[1]].y, v[vi[1]].z);
					glVertex3f(v[vi[1]].x+average.x, v[vi[1]].y+average.y, v[vi[1]].z+average.z);
				glEnd();
			}

			sum = Vec3f(vn[vni[2]].x +vn[vni[3]].x, 
							vn[vni[2]].y +vn[vni[3]].y,
							vn[vni[2]].z +vn[vni[3]].z);
			average = Vec3f(sum.x/2,sum.y/2,sum.z/2);

			if(draw_vnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 0, 0);
					glVertex3f(v[vi[2]].x, v[vi[2]].y, v[vi[2]].z);
					glVertex3f(v[vi[2]].x+average.x, v[vi[2]].y+average.y, v[vi[2]].z+average.z);
				glEnd();
			}
			else
			{
				glBegin(GL_LINES);
					glColor3ub(0, 0, 255);
					glVertex3f(v[vi[2]].x, v[vi[2]].y, v[vi[2]].z);
					glVertex3f(v[vi[2]].x+average.x, v[vi[2]].y+average.y, v[vi[2]].z+average.z);
				glEnd();
			}

			sum = Vec3f(vn[vni[3]].x +vn[vni[0]].x, 
							vn[vni[3]].y +vn[vni[0]].y,
							vn[vni[3]].z +vn[vni[0]].z);
			average = Vec3f(sum.x/2,sum.y/2,sum.z/2);

			if(draw_vnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 0, 0);
					glVertex3f(v[vi[5]].x, v[vi[5]].y, v[vi[5]].z);
					glVertex3f(v[vi[5]].x+average.x, v[vi[5]].y+average.y, v[vi[5]].z+average.z);
				glEnd();
			}
			else
			{
				glBegin(GL_LINES);
					glColor3ub(0, 0, 255);
					glVertex3f(v[vi[5]].x, v[vi[5]].y, v[vi[5]].z);
					glVertex3f(v[vi[5]].x+average.x, v[vi[5]].y+average.y, v[vi[5]].z+average.z);
				glEnd();
			}
		}
	}
}

void drawCylinder()
{

	// data references for convenience purpose
	vector<int> vi; //index array
	vector<int> ni;
	vector<int> ti;
	vector<int> vni;
	vector<Vec3f> v; //vertex array
	vector<Vec3f> n;
	vector<Vec2f> t;
	vector<Vec3f> vn;
	
	/////vertices//////

	//front
	v.push_back(Vec3f(0.1f,-0.5f,0.5));
	v.push_back(Vec3f(-0.1f,-0.5f,0.5));
	v.push_back(Vec3f(-0.1f,0.5f,0.5));
	v.push_back(Vec3f(0.1f,0.5f,0.5));
	
	//front right 1
	v.push_back(Vec3f(0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(0.1f,-0.5f,0.5));
	v.push_back(Vec3f(0.1f,0.5f,0.5));
	v.push_back(Vec3f(0.3f,0.5f,0.4f));
	
	//front right 2
	v.push_back(Vec3f(0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(0.3f,0.5f,0.4f));
	v.push_back(Vec3f(0.4f,0.5f,0.3f));
	
	//front right 3
	v.push_back(Vec3f(0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(0.4f,0.5f,0.3f));
	v.push_back(Vec3f(0.5f,0.5f,0.1f));
	
	//right
	v.push_back(Vec3f(0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(0.5f,0.5f,0.1f));
	v.push_back(Vec3f(0.5f,0.5f,-0.1f));
	
	//backright1
	v.push_back(Vec3f(0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(0.5f,0.5f,-0.1f));
	v.push_back(Vec3f(0.4f,0.5f,-0.3f));
	
	//backright2
	v.push_back(Vec3f(0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(0.4f,0.5f,-0.3f));
	v.push_back(Vec3f(0.3f,0.5f,-0.4f));
	
	//backright3
	v.push_back(Vec3f(0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(0.3f,0.5f,-0.4f));
	v.push_back(Vec3f(0.1f,0.5f,-0.5));
	
	//back
	v.push_back(Vec3f(-0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(0.1f,0.5f,-0.5));
	v.push_back(Vec3f(-0.1f,0.5f,-0.5));
	
	//backleft1
	v.push_back(Vec3f(-0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(-0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(-0.1f,0.5f,-0.5));
	v.push_back(Vec3f(-0.3f,0.5f,-0.4f));
	
	//backleft2
	v.push_back(Vec3f(-0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(-0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(-0.3f,0.5f,-0.4f));
	v.push_back(Vec3f(-0.4f,0.5f,-0.3f));
	
	//backleft3
	v.push_back(Vec3f(-0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(-0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(-0.4f,0.5f,-0.3f));
	v.push_back(Vec3f(-0.5f,0.5f,-0.1f));

	//left
	v.push_back(Vec3f(-0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(-0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(-0.5f,0.5f,-0.1f));
	v.push_back(Vec3f(-0.5f,0.5f,0.1f));

	//left front 1
	v.push_back(Vec3f(-0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(-0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(-0.5f,0.5f,0.1f));
	v.push_back(Vec3f(-0.4f,0.5f,0.3f));
	
	//left front 2
	v.push_back(Vec3f(-0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(-0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(-0.4f,0.5f,0.3f));
	v.push_back(Vec3f(-0.3f,0.5f,0.4f));
	
	//left front 3
	v.push_back(Vec3f(-0.1f,-0.5f,0.5));
	v.push_back(Vec3f(-0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(-0.3f,0.5f,0.4f));
	v.push_back(Vec3f(-0.1f,0.5f,0.5));

	//////indices//////////

	// Front
	vi.push_back(0);
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(0);
	vi.push_back(2);
	vi.push_back(3);

	//front right 1
	vi.push_back(4);
	vi.push_back(5);
	vi.push_back(6);
	vi.push_back(4);
	vi.push_back(6);
	vi.push_back(7);
	//front right 2
	vi.push_back(8);
	vi.push_back(9);
	vi.push_back(10);
	vi.push_back(8);
	vi.push_back(10);
	vi.push_back(11);
	// front right 3
	vi.push_back(12);
	vi.push_back(13);
	vi.push_back(14);
	vi.push_back(12);
	vi.push_back(14);
	vi.push_back(15);
	// Right
	vi.push_back(16);
	vi.push_back(17);
	vi.push_back(18);
	vi.push_back(16);
	vi.push_back(18);
	vi.push_back(19);
	//back right 1
	vi.push_back(20);
	vi.push_back(21);
	vi.push_back(22);
	vi.push_back(20);
	vi.push_back(22);
	vi.push_back(23);
	//back right 2
	vi.push_back(24);
	vi.push_back(25);
	vi.push_back(26);
	vi.push_back(24);
	vi.push_back(26);
	vi.push_back(27);
	//back right 3
	vi.push_back(28);
	vi.push_back(29);
	vi.push_back(30);
	vi.push_back(28);
	vi.push_back(30);
	vi.push_back(31);

	// Back
	vi.push_back(32);
	vi.push_back(33);
	vi.push_back(34);
	vi.push_back(32);
	vi.push_back(34);
	vi.push_back(35);
	// Back left 1
	vi.push_back(36);
	vi.push_back(37);
	vi.push_back(38);
	vi.push_back(36);
	vi.push_back(38);
	vi.push_back(39);
	// Back left 2
	vi.push_back(40);
	vi.push_back(41);
	vi.push_back(42);
	vi.push_back(40);
	vi.push_back(42);
	vi.push_back(43);
	// Back left 3
	vi.push_back(44);
	vi.push_back(45);
	vi.push_back(46);
	vi.push_back(44);
	vi.push_back(46);
	vi.push_back(47);
	
	//left
	vi.push_back(48);
	vi.push_back(49);
	vi.push_back(50);
	vi.push_back(48);
	vi.push_back(50);
	vi.push_back(51);
	//front left 1
	vi.push_back(52);
	vi.push_back(53);
	vi.push_back(54);
	vi.push_back(52);
	vi.push_back(54);
	vi.push_back(55);
	//front left 2
	vi.push_back(56);
	vi.push_back(57);
	vi.push_back(58);
	vi.push_back(56);
	vi.push_back(58);
	vi.push_back(59);
	//front left 3
	vi.push_back(60);
	vi.push_back(61);
	vi.push_back(62);
	vi.push_back(60);
	vi.push_back(62);
	vi.push_back(63);

	t.push_back(Vec2f(0,0));
	t.push_back(Vec2f(1,0));
	t.push_back(Vec2f(0,1));
	t.push_back(Vec2f(1,1));

	for(int i=0; i<vi.size()/6;i++)
	{
		ti.push_back(0);
		ti.push_back(1);
		ti.push_back(2);
		ti.push_back(1);
		ti.push_back(2);
		ti.push_back(3);
	}

	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0, 0.0, 1.0);
	// NOTE index i has to be adjusted
	for(int i = 0; i < vi.size()/16; i+=16)
	{
		for(int n = 0; n < 96; n++)
		{
			if(texOn == 1)
			{
				glTexCoord2f(t[ti[i+n]].x,t[ti[i+n]].y);
			}

			if(smooth == 1)
			{
				Vec3f offset = (v[vi[i]] - bbxCenter);
				glColor3f(fabs(cos(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
			}

			glVertex3f( v[vi[i+n]].x, v[vi[i+n]].y, v[vi[i+n]].z);
		}
	}
	glEnd();

	if(fnormal_flag == 1)
	{
///////////////////Face Normals///////////////////////////
		int k = 0;
		int index = 0;
		int v_1 = 0;
		int v_2 = 1;
		int v_3 = 2;
		int v_4 = 5;
		Vec3f v0, v1, norm, mid, triMid;
		float normalize;
		for(k = 0; k < 32; k+=2)
		{
			v0 = v[vi[v_1]] - v[vi[v_2]];
			v1 = v[vi[v_3]] - v[vi[v_2]];
			norm = v0%v1;
			norm.normalize();
			vn.push_back(Vec3f(norm.x,norm.y,norm.z));
			vni.push_back(index);
			index++;

			mid = (v[vi[v_1]] + v[vi[v_3]])/2;
			triMid = (v[vi[v_2]]+mid)/2;
			n.push_back(triMid);
			ni.push_back(k);

			triMid = (v[vi[v_4]]+mid)/2;
			n.push_back(triMid);
			ni.push_back(k+1);

			if(draw_fnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 255, 0);
					glVertex3f(n[ni[k]].x, n[ni[k]].y, n[ni[k]].z);
					glVertex3f(n[ni[k]].x+norm.x, n[ni[k]].y+norm.y, n[ni[k]].z+norm.z);
					glVertex3f(n[ni[k+1]].x, n[ni[k+1]].y, n[ni[k+1]].z);
					glVertex3f(n[ni[k+1]].x+norm.x, n[ni[k+1]].y+norm.y, n[ni[k+1]].z+norm.z);
				glEnd();
			}

			v_1 = v_1+6;
			v_2 = v_2+6;
			v_3 = v_3+6;
			v_4 = v_4+6;
		}
	
		if(vnormal_flag == 1)
		{
			///////create normal vectors//////	
			int index = 0;
			Vec3f sum;
			Vec3f posY = (Vec3f(0.0,1.0,0.0));
			Vec3f negY = (Vec3f(0.0,-1.0,0.0));

			for(int j = 0; j < 15; j++)
			{
	
				sum = Vec3f(vn[vni[j]].x +vn[vni[j]].x +vn[vni[j+1]].x +negY.x +negY.x, 
								vn[vni[j]].y +vn[vni[j]].y +vn[vni[j+1]].y +negY.y +negY.y,
								vn[vni[j]].z +vn[vni[j]].z +vn[vni[j+1]].z +negY.z +negY.z);
				Vec3f average = Vec3f(sum.x/5,sum.y/5,sum.z/5);

				if(draw_vnormals == 1)
				{
				glBegin(GL_LINES);
					glColor3ub(255, 0, 0);
					glVertex3f(v[vi[index]].x, v[vi[index]].y, v[vi[index]].z);
					glVertex3f(v[vi[index]].x+average.x, v[vi[index]].y+average.y, v[vi[index]].z+average.z);
				glEnd();
				}

				index += 6;
			}

			//last point, special case
			sum = Vec3f(vn[vni[15]].x +vn[vni[15]].x +vn[vni[0]].x +negY.x +negY.x, 
							vn[vni[15]].y +vn[vni[15]].y +vn[vni[0]].y +negY.y +negY.y,
							vn[vni[15]].z +vn[vni[15]].z +vn[vni[0]].z +negY.z +negY.z);
			Vec3f average = Vec3f(sum.x/5,sum.y/5,sum.z/5);

			if(draw_vnormals == 1)
			{
			glBegin(GL_LINES);
				glColor3ub(255, 0, 0);
				glVertex3f(v[vi[1]].x, v[vi[1]].y, v[vi[1]].z);
				glVertex3f(v[vi[1]].x+average.x, v[vi[1]].y+average.y, v[vi[1]].z+average.z);
			glEnd();
			}

			index = 5; 
			for(int j = 0; j < 15; j++)
			{
	
				sum = Vec3f(vn[vni[j]].x +vn[vni[j]].x +vn[vni[j+1]].x +posY.x +posY.x, 
								vn[vni[j]].y +vn[vni[j]].y +vn[vni[j+1]].y +posY.y +posY.y,
								vn[vni[j]].z +vn[vni[j]].z +vn[vni[j+1]].z +posY.z +posY.z);
				Vec3f average = Vec3f(sum.x/5,sum.y/5,sum.z/5);

				if(draw_vnormals == 1)
				{
				glBegin(GL_LINES);
					glColor3ub(255, 0, 0);
					glVertex3f(v[vi[index]].x, v[vi[index]].y, v[vi[index]].z);
					glVertex3f(v[vi[index]].x+average.x, v[vi[index]].y+average.y, v[vi[index]].z+average.z);
				glEnd();
				}

				index += 6;
			}

			//last point, special case
			sum = Vec3f(vn[vni[15]].x +vn[vni[15]].x +vn[vni[0]].x +posY.x +posY.x, 
							vn[vni[15]].y +vn[vni[15]].y +vn[vni[0]].y +posY.y +posY.y,
							vn[vni[15]].z +vn[vni[15]].z +vn[vni[0]].z +posY.z +posY.z);
			average = Vec3f(sum.x/5,sum.y/5,sum.z/5);

			if(draw_vnormals == 1)
			{
			glBegin(GL_LINES);
				glColor3ub(255, 0, 0);
				glVertex3f(v[vi[2]].x, v[vi[2]].y, v[vi[2]].z);
				glVertex3f(v[vi[2]].x+average.x, v[vi[2]].y+average.y, v[vi[2]].z+average.z);
			glEnd();
			}

			Vec3f base = v[vi[1]] - v[vi[0]];
			Vec3f height = v[vi[2]] - v[vi[0]];

			Vec3f cross = base%height; // cross product
			float sArea = cross.length() / 2;
			float area = (.25)*(sArea)+(.25)*(sArea)+(.25)*(sArea)+(.125)*(.05)+(.125)*(.05);

			Vec3f total = (Vec3f(area,area,area));

			if(draw_wnormals == 1)
			{
				for(int j = 0; j < 91; j+=6)
				{
					glBegin(GL_LINES);
						glColor3ub(0, 0, 255);
						glVertex3f(v[vi[j]].x, v[vi[j]].y, v[vi[j]].z);
						glVertex3f(v[vi[j]].x-total.x, v[vi[j]].y-total.y, v[vi[j]].z-total.z);
					glEnd();
				}
				for(int j = 5; j < 96; j+=6)
				{
					glBegin(GL_LINES);
						glColor3ub(0, 0, 255);
						glVertex3f(v[vi[j]].x, v[vi[j]].y, v[vi[j]].z);
						glVertex3f(v[vi[j]].x+total.x, v[vi[j]].y+total.y, v[vi[j]].z+total.z);
					glEnd();
				}
			}
		}	
	}

	drawCylinderTB();

}

void drawCylinderTB()
{
		// data references for convenience purpose
	vector<int> vi; //index array
	vector<int> ni;
	vector<int> ti;
	vector<Vec3f> v; //vertex array
	vector<Vec3f> n;
	vector<Vec2f> t;
	
	///top/////////////////

	//top front
	v.push_back(Vec3f(0.1f,0.5f,0.5f));
	v.push_back(Vec3f(-0.1f,0.5f,0.5f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//front right 1
	v.push_back(Vec3f(0.3f,0.5f,0.4f));
	v.push_back(Vec3f(0.1f,0.5f,0.5f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//front right 2
	v.push_back(Vec3f(0.4f,0.5f,0.3f));
	v.push_back(Vec3f(0.3f,0.5f,0.4f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//front right 3
	v.push_back(Vec3f(0.5f,0.5f,0.1f));
	v.push_back(Vec3f(0.4f,0.5f,0.3f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	//right
	v.push_back(Vec3f(0.5f,0.5f,-0.1f));
	v.push_back(Vec3f(0.5f,0.5f,0.1f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//backright1
	v.push_back(Vec3f(0.4f,0.5f,-0.3f));
	v.push_back(Vec3f(0.5f,0.5f,-0.1f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//backright2
	v.push_back(Vec3f(0.3f,0.5f,-0.4f));
	v.push_back(Vec3f(0.4f,0.5f,-0.3f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//backright3
	v.push_back(Vec3f(0.1f,0.5f,-0.5));
	v.push_back(Vec3f(0.3f,0.5f,-0.4f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	//back
	v.push_back(Vec3f(-0.1f,0.5f,-0.5));
	v.push_back(Vec3f(0.1f,0.5f,-0.5));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	//backleft1
	v.push_back(Vec3f(-0.3f,0.5f,-0.4f));
	v.push_back(Vec3f(-0.1f,0.5f,-0.5));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	//backleft2
	v.push_back(Vec3f(-0.4f,0.5f,-0.3f));
	v.push_back(Vec3f(-0.3f,0.5f,-0.4f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	//backleft3
	v.push_back(Vec3f(-0.5f,0.5f,-0.1f));
	v.push_back(Vec3f(-0.4f,0.5f,-0.3f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	//left
	v.push_back(Vec3f(-0.5f,0.5f,0.1f));
	v.push_back(Vec3f(-0.5f,0.5f,-0.1f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	//left front 1
	v.push_back(Vec3f(-0.4f,0.5f,0.3f));
	v.push_back(Vec3f(-0.5f,0.5f,0.1f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//left front 2
	v.push_back(Vec3f(-0.3f,0.5f,0.4f));
	v.push_back(Vec3f(-0.4f,0.5f,0.3f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));
	//left front 3
	v.push_back(Vec3f(-0.1f,0.5f,0.5));
	v.push_back(Vec3f(-0.3f,0.5f,0.4f));
	v.push_back(Vec3f(0.0f,0.5f,0.0f));

	/////////bottom////////////////////

	// front
	v.push_back(Vec3f(-0.1f,-0.5f,0.5f));
	v.push_back(Vec3f(0.1f,-0.5f,0.5f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//front right 1
	v.push_back(Vec3f(0.1f,-0.5f,0.5f));
	v.push_back(Vec3f(0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//front right 2
	v.push_back(Vec3f(0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//front right 3
	v.push_back(Vec3f(0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	//right
	v.push_back(Vec3f(0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//backright1
	v.push_back(Vec3f(0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//backright2
	v.push_back(Vec3f(0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//backright3
	v.push_back(Vec3f(0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	//back
	v.push_back(Vec3f(0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(-0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	//backleft1
	v.push_back(Vec3f(-0.1f,-0.5f,-0.5));
	v.push_back(Vec3f(-0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	//backleft2
	v.push_back(Vec3f(-0.3f,-0.5f,-0.4f));
	v.push_back(Vec3f(-0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	//backleft3
	v.push_back(Vec3f(-0.4f,-0.5f,-0.3f));
	v.push_back(Vec3f(-0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	//left
	v.push_back(Vec3f(-0.5f,-0.5f,-0.1f));
	v.push_back(Vec3f(-0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	//left front 1
	v.push_back(Vec3f(-0.5f,-0.5f,0.1f));
	v.push_back(Vec3f(-0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//left front 2
	v.push_back(Vec3f(-0.4f,-0.5f,0.3f));
	v.push_back(Vec3f(-0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));
	//left front 3
	v.push_back(Vec3f(-0.3f,-0.5f,0.4f));
	v.push_back(Vec3f(-0.1f,-0.5f,0.5));
	v.push_back(Vec3f(0.0f,-0.5f,0.0f));

	// top Front
	vi.push_back(0);
	vi.push_back(1);
	vi.push_back(2);

	vi.push_back(3);
	vi.push_back(4);
	vi.push_back(5);

	vi.push_back(6);
	vi.push_back(7);
	vi.push_back(8);

	vi.push_back(9);
	vi.push_back(10);
	vi.push_back(11);

	vi.push_back(12);
	vi.push_back(13);
	vi.push_back(14);

	vi.push_back(15);
	vi.push_back(16);
	vi.push_back(17);

	vi.push_back(18);
	vi.push_back(19);
	vi.push_back(20);

	vi.push_back(21);
	vi.push_back(22);
	vi.push_back(23);

	vi.push_back(24);
	vi.push_back(25);
	vi.push_back(26);

	vi.push_back(27);
	vi.push_back(28);
	vi.push_back(29);

	vi.push_back(30);
	vi.push_back(31);
	vi.push_back(32);

	vi.push_back(33);
	vi.push_back(34);
	vi.push_back(35);

	vi.push_back(36);
	vi.push_back(37);
	vi.push_back(38);

	vi.push_back(39);
	vi.push_back(40);
	vi.push_back(41);

	vi.push_back(42);
	vi.push_back(43);
	vi.push_back(44);

	vi.push_back(45);
	vi.push_back(46);
	vi.push_back(47);

	//bottom
	vi.push_back(48);
	vi.push_back(49);
	vi.push_back(50);

	vi.push_back(51);
	vi.push_back(52);
	vi.push_back(53);

	vi.push_back(54);
	vi.push_back(55);
	vi.push_back(56);

	vi.push_back(57);
	vi.push_back(58);
	vi.push_back(59);

	vi.push_back(60);
	vi.push_back(61);
	vi.push_back(62);

	vi.push_back(63);
	vi.push_back(64);
	vi.push_back(65);

	vi.push_back(66);
	vi.push_back(67);
	vi.push_back(68);

	vi.push_back(69);
	vi.push_back(70);
	vi.push_back(71);

	vi.push_back(72);
	vi.push_back(73);
	vi.push_back(74);

	vi.push_back(75);
	vi.push_back(76);
	vi.push_back(77);

	vi.push_back(78);
	vi.push_back(79);
	vi.push_back(80);

	vi.push_back(81);
	vi.push_back(82);
	vi.push_back(83);

	vi.push_back(84);
	vi.push_back(85);
	vi.push_back(86);

	vi.push_back(87);
	vi.push_back(88);
	vi.push_back(89);

	vi.push_back(90);
	vi.push_back(91);
	vi.push_back(92);

	vi.push_back(93);
	vi.push_back(94);
	vi.push_back(95);

	t.push_back(Vec2f(0,0));
	t.push_back(Vec2f(1,0));
	t.push_back(Vec2f(0,1));

	for(int i=0; i<vi.size()/3;i++)
	{
		ti.push_back(0);
		ti.push_back(1);
		ti.push_back(2);
	}

	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_TRIANGLES);
	glColor3f(.78,.78,.78);
	glNormal3f(0.0, 0.0, 1.0);
	// NOTE index i has to be adjusted
	for(int i = 0; i < vi.size()/32; i+=32)
	{
		for(int n = 0; n < 96; n++)
		{
			if(texOn == 1)
			{
				glTexCoord2f(t[ti[i+n]].x,t[ti[i+n]].y);
			}

			if(smooth == 1)
			{
				Vec3f offset = (v[vi[i]] - bbxCenter);
				glColor3f(fabs(cos(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
			}

			glVertex3f( v[vi[i+n]].x, v[vi[i+n]].y, v[vi[i+n]].z);
		}
	}
	glEnd();

	if(fnormal_flag == 0)
	{
		///////////////////Face Normals///////////////////////////
		int k = 0;
		int v_1 = 0;
		int v_2 = 1;
		int v_3 = 2;
		Vec3f v0, v1, norm, mid, triMid;
		float normalize;
		for(k = 0; k < 32; k++)
		{
			v0 = v[vi[v_1]] - v[vi[v_2]];
			v1 = v[vi[v_3]] - v[vi[v_2]];
			norm = v0%v1;
			normalize = sqrt((norm.x*norm.x)+(norm.y*norm.y)+(norm.z*norm.z));
			norm = norm/normalize;

			mid = (v[vi[v_1]] + v[vi[v_3]])/2;
			triMid = (v[vi[v_2]]+mid)/2;
			n.push_back(triMid);
			ni.push_back(k);

			if(draw_fnormals == 1)
			{
				glBegin(GL_LINES);
					glColor3ub(255, 255, 0);
					glVertex3f(n[ni[k]].x, n[ni[k]].y, n[ni[k]].z);
					glVertex3f(n[ni[k]].x+norm.x, n[ni[k]].y+norm.y, n[ni[k]].z+norm.z);
				glEnd();
			}

			v_1 = v_1+3;
			v_2 = v_2+3;
			v_3 = v_3+3;
		}

		Vec3f base = v[vi[1]] - v[vi[0]];
		Vec3f height = v[vi[2]] - v[vi[0]];

		Vec3f cross = base%height; // cross product
		float sArea = cross.length() / 2;
	}
}

void drawChair()
{
	glPushMatrix();
	glTranslatef(3.0,0.0,4.0);
	glRotatef(180,0,1,0);
	glScalef(3.0,1.0,1.5);
	glColor3f(.8,.7,.7);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0,1.0,4.6);
	glRotatef(180,0,1,0);
	glScalef(3.0,1.0,0.25);
	glColor3f(.8,.7,.7);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(.8,.8,.8);
	glTranslatef(3.0,1.5,4.6);
	glRotatef(90,0.0,0.0,1.0);
	glScalef(0.5,3.0,0.5);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glColor3f(.8,.8,.8);
	glTranslatef(3.0,-0.26,4.6);
	glRotatef(90,0.0,0.0,1.0);
	glScalef(0.5,3.0,0.5);
	drawCylinder();
	glPopMatrix();
}

void drawPillars()
{
	glPushMatrix();
	glTranslatef(-4.0,-0.35,-4.0);
	glScalef(2.0,0.25,2.0);
	glColor3f(.78,.78,.78);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.0,1.9,-4.0);
	glScalef(1.0,4.4,1.0);
	glColor3f(.78,.78,.78);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.0,-0.35,4.0);
	glScalef(2.0,0.25,2.0);
	glColor3f(.78,.78,.78);
	drawCube();
	glPopMatrix();

	//////

	glPushMatrix();
	glTranslatef(-4.0,4.0,-4.0);
	glScalef(2.0,0.25,2.0);
	glColor3f(.78,.78,.78);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.0,1.9,4.0);
	glScalef(1.0,4.4,1.0);
	glColor3f(.78,.78,.78);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.0,4.0,4.0);
	glScalef(2.0,0.25,2.0);
	glColor3f(.78,.78,.78);
	drawCube();
	glPopMatrix();
}
/* 
 * Facilities for drawing models. The functions below show:
 * - How to use ILM(Industrial Light & Magic)'s math library
 * - Mesh data structure
 * - How to draw the mesh in immediate mode
 */

/* 
 * Compute the bounding box of the model
 */

void calc_bbox(Vec3f& bbxmin, Vec3f& bbxmax)
{
	vector<Vec3f>& v = model->m_v;

	if(v.empty()) return;		

	bbxmax = bbxmin= v[0];

	for(unsigned int i = 1;i < v.size();i++)
	{	
		bbxmin.x = MIN2(bbxmin.x, v[i].x);
		bbxmin.y = MIN2(bbxmin.y, v[i].y);
		bbxmin.z = MIN2(bbxmin.z, v[i].z);

		bbxmax.x = MAX2(bbxmax.x, v[i].x);
		bbxmax.y = MAX2(bbxmax.y, v[i].y);
		bbxmax.z = MAX2(bbxmax.z, v[i].z);
	}
}

/* 
 * Load a model from a file
 */

void load_model(const char* name)
{
	if(model) delete model;
	model = ObjLoader::load(name);
	calc_bbox(bbxMin, bbxMax);
	bbxCenter = (bbxMin+bbxMax)*0.5f;
}

/* 
 * Draw the model in immediate mode
 */

void draw_model()
{
	// data references for convenience purpose
	vector<int>& vi = model->m_vi;
	vector<int>& ni = model->m_ni;
	vector<int>& ti = model->m_ti;
	vector<Vec3f>& v = model->m_v;
	vector<Vec3f>& n = model->m_n;
	vector<Vec2f>& t = model->m_texc;

	//glBindTexture(GL_TEXTURE_2D, 2);
	// drawing in immediate mode
	glBegin(GL_TRIANGLES);

	for(unsigned int i = 0;i < vi.size();i++)
	{
		// normals and texture coordiates is not used.
		// to achieve lighting, per vertex normal has to be attached
		// *** Note: normals are sometimes absent from the data.You have to create
		// *** them yourself

		
	/*	if(!n.empty() && !ni.empty())
			glNormal3fv(&n[ni[i]].x);*/

		if(!t.empty() && !ti.empty())
			glTexCoord2fv(&t[ti[i]].x);

		// assign some funny color to each vertex
		Vec3f offset = (v[vi[i]] - bbxCenter);
		glColor3f(fabs(cos(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		//glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));

		glVertex3fv(&v[vi[i]].x);
	}

	glEnd();
}

// ================================================================================
// Main program functions
void renderBitmapString(float x, float y, float z, char *string)
{
   char *c;
   
   glRasterPos3f(x, y, z);   // fonts position
   for(c=string; *c != '\0'; c++)
     glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

void shut()
{
	if(model) delete model;
}

void modelMenu (int menuId)
{
	load_model(modelfile[menuId]);

	// adjust the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	farPlane = nearPlane+4.0f*(bbxMax-bbxMin).length();
	gluPerspective(fieldofview, aspect, nearPlane, farPlane);

	// adjust the viewing matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float cz = 2.0f*(bbxMax.z - bbxMin.z);

	gluLookAt(0.0f, 0.0f, cz, 
		0.0f, 0.0f, cz - 1.0f, 
		0.0f, 1.0f, 0.0f);

	glutPostRedisplay();
}

void navMenu(int menuId)
{
	if(menuId == 13)//nav 1
	{
		eyeY = 15.0;
		eyeX = 0.0;
		eyeZ = 10.0;
		centerX = 0.0;
		centerY = 0.0;
		centerZ = 0.0;
		nav1 = 1;
		nav2 = 0;
	}
	if(menuId == 14)//nav 2
	{
		eyeY = -2.0;
		eyeX = 0.0;
		eyeZ = 7.0;
		centerX = 0.0;
		centerY = -2.0;
		centerZ = 0.0;
		nav1 = 0;
		nav2 = 1;
	}
	if(menuId == 15)
	{
		// adjust the projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	
		//farPlane = nearPlane+4.0f*(bbxMax-bbxMin).length();
		gluPerspective(fieldofview, aspect, nearPlane, farPlane);

		// adjust the viewing matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(eyeX, eyeY, eyeZ, 
			centerX,centerY,centerZ, 
			0.0f, 1.0f, 0.0f);

		glutPostRedisplay();
	}
}

void menu(int menuId)
{
	if(menuId == 24)
	{
		glutDestroyWindow(winId);
		shut();
		exit(0);
	}
	if(menuId == 3)//shading-flat
	{
		smooth = 0;
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glShadeModel(GL_FLAT);
	}
	if(menuId == 4)//shading-smooth
	{
		smooth = 1;
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glShadeModel(GL_SMOOTH);
	}
	if(menuId == 5)//wireframe
	{
		smooth = 0;
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	if(menuId == 6)//face norm
	{
		fnormal_flag = 1;
	}
	if(menuId == 7)//on
	{
		if(fnormal_flag == 1)
		{
			draw_fnormals = 1;
		}
	}
	if(menuId == 8)//off
	{
		draw_fnormals = 0;
		//fnormal_flag = 0;
	}
	if(menuId == 9)
	{
		if(fnormal_flag == 1)
		{
			vnormal_flag = 1;
			w = 0;
		}
	}
	if(menuId == 10)
	{
		if(fnormal_flag == 1)
		{
			vnormal_flag = 1;
			w = 1;
		}
	}
	if(menuId == 11)
	{
		if(vnormal_flag == 1 && w == 1)
		{
			draw_vnormals = 0;
			draw_wnormals = 1;
		}
		else if(vnormal_flag == 1 && w == 0)
		{
			draw_vnormals = 1;
			draw_wnormals = 0;
		}
	}
	if(menuId == 12)
	{
		draw_vnormals = 0;
		draw_wnormals = 0;
		//vnormal_flag = 0;
	}
	if(menuId == 16)//texture on
	{
		texOn = 1;
	}
	if(menuId == 17)//texture off
	{
		texOn = 0;
	}
	if(menuId == 18)//texture replace
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	if(menuId == 19)//texture modulate
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	if(menuId == 20)//min simple
	{
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	if(menuId == 21)//min smooth
	{
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	if(menuId == 22)//mag simple
	{
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	if(menuId == 23)//mag smooth
	{
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	glutPostRedisplay();
}

void glutMenus (void)
{	
	GLint subShading = glutCreateMenu(menu);
	// Add sub menu entry
	glutAddMenuEntry("Shading-Flat", 3);
	glutAddMenuEntry("Shading-Smooth", 4);
	glutAddMenuEntry("No Shading", 5);

	GLint subFaceNormal = glutCreateMenu(menu);
	// Add sub menu entry
	glutAddMenuEntry("Normals - Per Face Normals", 6);
	glutAddMenuEntry("Normal Visualization - On", 7);
	glutAddMenuEntry("Normal Visualization - Off", 8);

	GLint subVertexNormal = glutCreateMenu(menu);
	// Add sub menu entry
	glutAddMenuEntry("Normals - Per Vertex Normals", 9);
	glutAddMenuEntry("Normals - Per Vertex Normals Weighted", 10);
	glutAddMenuEntry("Normal Visualization - On", 11);
	glutAddMenuEntry("Normal Visualization - Off", 12);

	GLint nav = glutCreateMenu(navMenu);
	// Add sub menu entry
	glutAddMenuEntry("View - Overview", 13);
	glutAddMenuEntry("View - First Person", 14);
	glutAddMenuEntry("Navigation - Camera", 15);

	GLint texture = glutCreateMenu(menu);
	// Add sub menu entry
	glutAddMenuEntry("Texturing - On", 16);
	glutAddMenuEntry("Texturing - Off", 17);
	glutAddMenuEntry("Texturing - Replace", 18);
	glutAddMenuEntry("Texturing - Modulate", 19);

	GLint min = glutCreateMenu(menu);
	// Add sub menu entry
	glutAddMenuEntry("Minification - Simple", 20);
	glutAddMenuEntry("Minification - Smooth", 21);

	GLint mag = glutCreateMenu(menu);
	// Add sub menu entry
	glutAddMenuEntry("Magnification - Simple", 22);
	glutAddMenuEntry("Magification - Smooth", 23);

	GLint subMenu_models = glutCreateMenu(modelMenu);
	glutAddMenuEntry("lady", 0);
	glutAddMenuEntry("venus", 1);
	glutAddMenuEntry("temple", 2);

	glutCreateMenu(menu);
	glutAddSubMenu("Shading", subShading);
	glutAddSubMenu("Face Normals", subFaceNormal);
	glutAddSubMenu("Vertex Normals", subVertexNormal);
	glutAddSubMenu("Texturing", texture);
	glutAddSubMenu("Minification", min);
	glutAddSubMenu("Magification", mag);
	glutAddSubMenu("Navigation", nav);
	//glutAddSubMenu("Models", subMenu_models);
	glutAddMenuEntry("Quit", 24);	

	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void key(unsigned char key, int x, int y)
{
	switch (key) 
    {       
        case 'a': 
			if(nav1 == 1)
			{
				scale += 0.1;
				if(scale < 0.1) scale = 0.1;     // too small
				if(scale > 7)	scale = 7;		 // too big
			}
			else if(nav2 == 1)
			{
				//same for nav1 up/down
				eyeY++;
				centerY++;
				navMenu(15);
			}
            break; 
        case 's': 
			if(nav1 == 1)
			{
				scale -= 0.1;
				if(scale < 0.1) scale = 0.1;     // too small
				if(scale > 7)	scale = 7;		 // too big
			}
			else if(nav2 == 1)
			{
				//same for nav1 up/down
				eyeY--;
				centerY--;
				navMenu(15);
			}
            break;   
    } 

    glutPostRedisplay(); 
}

void specialKey(int key, int x, int y) 
{  
	switch(key) 
	{
		case GLUT_KEY_LEFT:
			y_angle -= 1.0;

			break;
		case GLUT_KEY_RIGHT:
			y_angle += 1.0;

			break;
		case GLUT_KEY_UP:
			if(nav1 == 1)
			{
				eyeY++;
				centerY++;
				navMenu(15);
			}
			else if(nav2 == 1)
			{
				eyeZ--;	 
				navMenu(15);
			}
			
			break;
		case GLUT_KEY_DOWN:
			if(nav1 == 1)
			{
				eyeY--;
				centerY--;
				navMenu(15);
			}
			else if(nav2 == 1)
			{
				eyeZ++;
				navMenu(15);
			}

			break;
	}

	glutPostRedisplay(); 
}

void loadPNG(char* filename, int width, int height, int bitdepth)
{
	image = stbi_load(filename, &width, &height, &bitdepth, 0);
}

void init()
{	
	loadPNG("marble.bmp",  imageWidth, imageHeight, 24);
	glBindTexture(GL_TEXTURE_2D, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glEnable (GL_TEXTURE_2D);

	loadPNG("grass.bmp",  imageWidth, imageHeight, 24);
	glBindTexture(GL_TEXTURE_2D, 2);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glEnable (GL_TEXTURE_2D);

	loadPNG("marble1.bmp",  imageWidth, imageHeight, 24);
	glBindTexture(GL_TEXTURE_2D, 3);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glEnable (GL_TEXTURE_2D);
	
	// use black to clean background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// always re-normalize normal (due to scale transform)
	glEnable(GL_NORMALIZE);

	// lighting (disabled)
	// To achieve lighting effect
	//	- enable lighting
	//	- setup lighting parameters
	//	- setup materials
	//	- attach normals to each vertex
	
	glDisable(GL_LIGHTING);
	 
	/*glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glEnable(GL_LIGHT0);*/	

	// smooth shading
	glShadeModel(GL_SMOOTH);

	// load the first model by default
	load_model(modelfile[0]);
	
	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspect = (double)width/(double)height;
	farPlane = nearPlane+4.0f*(bbxMax-bbxMin).length();
	gluPerspective(fieldofview, aspect, nearPlane, farPlane);

	// setup viewing matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float cz = 2.0f*(bbxMax.z - bbxMin.z);

	gluLookAt(0.0f, 0.0f, cz, 
		0.0f, 0.0f, cz - 1.0f, 
		0.0f, 1.0f, 0.0f);

	// set up menus
	glutMenus();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
		glScalef(scale, scale, scale);
		glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
		glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
		glTranslatef(-bbxCenter.x, -bbxCenter.y, -bbxCenter.z);
		//cube = drawCube();
		//draw_model(cube);
		drawChair();
		drawPillars();
		glColor3f(0.56,0.7,0.6);
		drawPlane();
		glPushMatrix();
			glColor3f(.8,.7,.7);
			glTranslatef(4.0, 0.0, -2.0);
			glScalef(2,1,2);
			drawCube();
		glPopMatrix();
		load_model(modelfile[2]);
		glPushMatrix();
			glTranslatef(1.0, 0.3, -4.0);
			glScalef(.001, .001, .001);
			draw_model();
		glPopMatrix();
		load_model(modelfile[1]);
		glPushMatrix();
			glTranslatef(4.0, 3.5, -2.0);
			glRotatef(-90,0,1,0);
			draw_model();
		glPopMatrix();
		load_model(modelfile[4]);
		glPushMatrix();
			glTranslatef(-2.0, 0.8, -4.0);
			glScalef(.1, .1, .1);
			draw_model();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-2.0, 0.8, 4.0);
			glScalef(.1, .1, .1);
			draw_model();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(4.0, 0.8, 0.0);
			glScalef(.1, .1, .1);
			draw_model();
		glPopMatrix();
		load_model(modelfile[3]);
		glPushMatrix();
			glTranslatef(-4.0,-0.6,0.0);
			glRotatef(90,0,1,0);
			glScalef(1, 1, .05);
			draw_model();
		glPopMatrix();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,width, 0, height);	

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, height-13.0f, 0.0f, "Use [Mouse Left Key] to rotate");	
	renderBitmapString(0.0, height-26.0f, 0.0f, "Use [Mouse Right Key] to scale");
	renderBitmapString(0.0, height-39.0f, 0.0f, "Press [Mouse Middle key] to summon out menu");

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	// button -- GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
	// state  -- GLUT_UP, GLUT_DOWN

	mouse_x = x;
	mouse_y = y;

	mouse_button = button;
}

void reshape(int x, int y)
{
    width  = x;
	height = y;

	if (height == 0)		// not divided by zero
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,width,height);

	aspect = (float)width/(float)height;

	gluPerspective(fieldofview, aspect, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);

}

void motion(int x, int y)
{
	if(mouse_button == GLUT_LEFT_BUTTON)
	{
		// compute the angle (0..360) around x axis, and y-axis
		y_angle += (float(x - mouse_x)/width)*360.0;
		x_angle += (float(y - mouse_y)/height)*360.0;

	}

	if(mouse_button == GLUT_RIGHT_BUTTON)
	{
		// scale
		scale += (y-mouse_y)/100.0;
			
		if(scale < 0.1) scale = 0.1;     // too small
		if(scale > 7)	scale = 7;		 // too big

	}

	mouse_x	= x;		// update current mouse position
	mouse_y	= y;
	glutPostRedisplay();

}

void main(int argc, char* argv[])
{
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(position_x,position_y);
	glutInitWindowSize(width,height);
	winId = glutCreateWindow("Mesh Viewer");
	
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutSpecialFunc(specialKey);
	glutKeyboardFunc(key);

	glutDisplayFunc(display);		// display function

	init();

	glutMainLoop();

	shut();
}






