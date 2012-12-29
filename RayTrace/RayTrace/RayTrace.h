/*
  NOTE: This is the file you will need to begin working with
		You will need to implement the RayTrace::CalculatePixel () function

  This file defines the following:
	RayTrace Class
*/

#ifndef RAYTRACE_H
#define RAYTRACE_H

using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "Utils.h"

/*
	RayTrace Class - The class containing the function you will need to implement

	This is the class with the function you need to implement
*/

struct Ray{
	Vector startPt;
	Vector depth;
	Vector ray;
};

Vector pixelColor = Vector (0.0f, 0.0f, 0.0f);

class RayTrace
{
public:
	/* - Scene Variable for the Scene Definition - */
	Scene m_Scene;
	Ray rayPtr; 
	float t;
	//Vector normTri;
	Vector normSphere; 
	Vector normModel;
	SceneTriangle* globalTri;

	// -- Constructors & Destructors --
	RayTrace (void) {}
	~RayTrace (void) {}

	// -- Main Functions --
	// - CalculatePixel - Returns the Computed Pixel for that screen coordinate
	Vector CalculatePixel (int screenX, int screenY)
	{
		// Get the Camera
		Camera sceneCam = m_Scene.GetCamera ();

		//calculating height/width/starting point
		rayPtr.startPt = Vector(sceneCam.position.x, sceneCam.position.y, sceneCam.position.z);
		float PI = 3.14159265;
		//float height = 2 * sceneCam.GetNearClip() * tan((sceneCam.GetFOV()*(PI/180))/2);
		//float width = height * (WINDOW_WIDTH/WINDOW_HEIGHT);
		sceneCam.SetNearClip(.8);
		float fov = (GLfloat)WINDOW_HEIGHT / ((GLfloat)WINDOW_WIDTH * (PI/4));
		float height = ((2.0f * (GLfloat)screenY) - (GLfloat)WINDOW_HEIGHT) /((GLfloat)WINDOW_HEIGHT * tan(fov));
		float width =((2.0f * (GLfloat)screenX) - (GLfloat)WINDOW_WIDTH) / ((GLfloat)WINDOW_WIDTH * tan(PI/4));

		Vector pD = Vector (width - 0.25, height - 0.25, -1.5);
		rayPtr.depth = pD.Normalize();

		rayPtr.ray = rayPtr.depth-(rayPtr.startPt);
		rayPtr.ray.Normalize();
		
		//bool flag = intersection();
		//return pixelColor;	
	

		SceneBackground sceneBg = m_Scene.GetBackground ();
		pixelColor = sceneBg.color;
		bool didInter = false;
		float disValue = 1000;
		Vector intersectRay;
		SceneObject *objHit;

		unsigned int numObjs = m_Scene.GetNumObjects ();
		for (unsigned int n = 0; n < numObjs; n++)
		{
				SceneObject *sceneObj = m_Scene.GetObject (n);
				SceneMaterial sceneMat = m_Scene.GetMaterial (((SceneSphere *)sceneObj)->material);
				SceneModel* modelType = (SceneModel*)sceneObj;

				// Is is a Sphere?
				if (sceneObj->IsSphere ())
				{
					Vector cSphere = ((SceneSphere *)sceneObj)->center;
					float rSphere = ((SceneSphere *)sceneObj)->radius;

					if(interSphere(cSphere,  rSphere) == true)
					{
						didInter = true;
						//pixelColor = Vector(1,0,0);
						Vector tempRay = rayPtr.startPt + rayPtr.depth * t;
						float newDistValue = CalcDistance(rayPtr.startPt, tempRay); 
						
						if(newDistValue < disValue)
						{
							disValue = newDistValue;
							intersectRay = tempRay;
							objHit = sceneObj;
						}
					}

				}
				else if(sceneObj->IsTriangle ())
				{
					Vector v0 = (Vector (((SceneTriangle *)sceneObj)->vertex[0].x, ((SceneTriangle *)sceneObj)->vertex[0].y, ((SceneTriangle *)sceneObj)->vertex[0].z));
					Vector v1 = (Vector (((SceneTriangle *)sceneObj)->vertex[1].x, ((SceneTriangle *)sceneObj)->vertex[1].y, ((SceneTriangle *)sceneObj)->vertex[1].z));
					Vector v2 = (Vector (((SceneTriangle *)sceneObj)->vertex[2].x, ((SceneTriangle *)sceneObj)->vertex[2].y, ((SceneTriangle *)sceneObj)->vertex[2].z));
			
					if(interTri(v0, v1, v2))
					{
						didInter = true;
						//pixelColor = Vector(1,0,0);
						Vector tempRay = rayPtr.startPt + rayPtr.depth * t;
						float newDistValue = CalcDistance(rayPtr.startPt, tempRay); 
						
						if(newDistValue < disValue)
						{
							disValue = newDistValue;
							intersectRay = tempRay;
							objHit = sceneObj;
						}
					}
				}
				else if(sceneObj->IsModel ())
				{
					if(interModel((SceneModel*)sceneObj))
					{
						didInter = true;
						//pixelColor = Vector(1,0,0);
						Vector tempRay = rayPtr.startPt + rayPtr.depth * t;
						float newDistValue = CalcDistance(rayPtr.startPt, tempRay); 
						
						if(newDistValue < disValue)
						{
							disValue = newDistValue;
							intersectRay = tempRay;
							objHit = sceneObj;
						}
					}
				}
			}
			
		if(didInter == false)
		{
			return sceneBg.color;
		}

		//return pixelColor;

		//reset pixel color for intersection
		pixelColor = (Vector(0.0,0.0,0.0));

		unsigned int numLights = m_Scene.GetNumLights ();
		for (unsigned int l = 0; l < numLights; l++)
		{
			SceneLight sceneLight = m_Scene.GetLight (l);
			Vector cSphere = ((SceneSphere *)objHit)->center;
			float rSphere = ((SceneSphere *)objHit)->radius;
			Vector v0 = (Vector (((SceneTriangle *)objHit)->vertex[0].x, ((SceneTriangle *)objHit)->vertex[0].y, ((SceneTriangle *)objHit)->vertex[0].z));
					Vector v1 = (Vector (((SceneTriangle *)objHit)->vertex[1].x, ((SceneTriangle *)objHit)->vertex[1].y, ((SceneTriangle *)objHit)->vertex[1].z));
					Vector v2 = (Vector (((SceneTriangle *)objHit)->vertex[2].x, ((SceneTriangle *)objHit)->vertex[2].y, ((SceneTriangle *)objHit)->vertex[2].z));
			
			
			if(objHit->IsSphere())
			{
				SceneMaterial sceneMat = m_Scene.GetMaterial (((SceneSphere *)objHit)->material);
				
			//	Vector cSphere = ((SceneSphere *)objHit)->center;
			//	float rSphere = ((SceneSphere *)objHit)->radius;
				
				Vector normSphere = (intersectRay - cSphere)/rSphere;

				Vector shadow = (Vector (sceneLight.position - intersectRay));
				shadow.Normalize();
				float mag = sqrt((shadow.x)*(shadow.x) + (shadow.y)*(shadow.y) + (shadow.z)*(shadow.z));

				Vector unitVec = shadow/mag;
				float f =  normSphere.Dot(shadow);
				Vector reflection = (normSphere * 2 * sceneLight.color.Dot(normSphere)) - sceneLight.color;

				//shadow ray
				rayPtr.startPt = intersectRay;
				rayPtr.depth = sceneLight.position - rayPtr.startPt;
				if(intersection() == true)
				{
					pixelColor = (sceneMat.diffuse * max(0,f) + sceneMat.specular * pow((sceneLight.color.Dot(reflection)),sceneMat.shininess))*.5;// * powf(dot,2)*0.35;
				}
				else
				{
					pixelColor = sceneMat.diffuse * max(0,f) + sceneMat.specular * pow((sceneLight.color.Dot(reflection)),sceneMat.shininess);// * powf(dot,2)*0.35;
				}
						
			}
			else if(objHit->IsTriangle())
			{
				SceneMaterial sceneMat = m_Scene.GetMaterial (((SceneTriangle *)objHit)->material[0]);

				SceneTriangle* tri = (SceneTriangle *) objHit;

				Vector shadow = sceneLight.position - intersectRay;
				shadow.Normalize();

				Vector normTri = GetNormalTriangle(tri ,intersectRay);
				normTri.Normalize();
				//Vector unitVec = shadRay/mag;
				float f = normTri.Dot(shadow);

				if(f > 0)
				{
					pixelColor = pixelColor + ((sceneMat.diffuse)*sceneLight.color)*f;
					//Vector reflection = (normTri * 2 * sceneLight.color.Dot(norm)) - sceneLight.color;
					Vector s4 = sceneLight.position - normTri*2.0*sceneLight.position.Dot(normTri);
					float dotP = rayPtr.depth.Dot(s4);

					//shadow ray
					rayPtr.startPt = intersectRay;
					rayPtr.depth = sceneLight.position - rayPtr.startPt;
						
					if(intersection() == true)
					{
						pixelColor = (pixelColor + sceneMat.diffuse*powf(dotP,.4)*.25)*.5;
					}
					else
					{
						pixelColor = pixelColor + sceneMat.diffuse*powf(dotP,.4)*.25;
					}
				}
			}
			else if(objHit->IsModel())
			{
				SceneMaterial sceneMat = m_Scene.GetMaterial (((SceneModel *)objHit)->GetTriangle (0)->material[0]);

				SceneTriangle* model = (SceneTriangle *) globalTri;

				Vector shadow = sceneLight.position - intersectRay;
				shadow.Normalize();

				Vector normModel = GetNormalTriangle(model,intersectRay);
				normModel.Normalize();
				//Vector unitVec = shadRay/mag;
				float f = normModel.Dot(shadow);

				if(f > 0)
				{
					pixelColor = pixelColor + (Vector(.5,0,.5)*sceneLight.color);
					//Vector reflection = (normTri * 2 * sceneLight.color.Dot(norm)) - sceneLight.color;
					Vector s4 = sceneLight.position - normModel*2.0*sceneLight.position.Dot(normModel);
					float dotP = rayPtr.depth.Dot(s4);
						
					//shadow ray
					rayPtr.startPt = intersectRay;
					rayPtr.depth = sceneLight.position - rayPtr.startPt;
						
					if(intersection() == true)
					{
						pixelColor = (pixelColor + sceneMat.diffuse*powf(dotP,.4)*.25)*.5;
					}
					else
					{
						pixelColor = (pixelColor + sceneMat.diffuse*powf(dotP,.4)*.25);
					}

					pixelColor = (Vector(0,0,.5));
				}
			}
		}

		return pixelColor;

		/*
			-- How to Implement a Ray Tracer --

			This computed pixel will take into account the camera and the scene
				and return a Vector of <Red, Green, Blue>, each component ranging from 0.0 to 1.0

			In order to start working on computing the color of this pixel,
				you will need to cast a ray from your current camera position
				to the image-plane at the given screenX and screenY
				coordinates and figure out how/where this ray intersects with 
				the objects in the scene descriptor.
				The Scene Class exposes all of the scene's variables for you 
				through its functions such as m_Scene.GetBackground (), m_Scene.GetNumLights (), 
				etc. so you will need to use those to learn about the World.

			To determine if your ray intersects with an object in the scene, 
				you must calculate where your objects are in 3D-space [using 
				the object's scale, rotation, and position is extra credit]
				and mathematically solving for an intersection point corresponding to that object.
			
			For example, for each of the spheres in the scene, you can use 
				the equation of a sphere/ellipsoid to determine whether or not 
				your ray from the camera's position to the screen-pixel intersects 
				with the object, then from the incident angle to the normal at 
				the contact, you can determine the reflected ray, and recursively 
				repeat this process capped by a number of iterations (e.g. 10).

			Using the lighting equation to calculate the color at every 
				intersection point and adding its fractional amount (determined by the material)
				will get you a final color that returns to the eye at this point.
		*/

		if ((screenX < 0 || screenX > WINDOW_WIDTH - 1) ||
			(screenY < 0 || screenY > WINDOW_HEIGHT - 1))
		{
			// Off the screen, return black
			return Vector (0.0f, 0.0f, 0.0f);
		}
	}

	bool intersection()
	{
		bool isInter;
		unsigned int numObjs = m_Scene.GetNumObjects ();
		for (unsigned int n = 0; n < numObjs; n++)
		{
				SceneObject *sceneObj = m_Scene.GetObject (n);
				SceneMaterial sceneMat = m_Scene.GetMaterial (((SceneSphere *)sceneObj)->material);
				SceneModel* modelType = (SceneModel*)sceneObj;
				SceneBackground sceneBg = m_Scene.GetBackground ();
				t = 2000;

				// Is is a Sphere?
				if (sceneObj->IsSphere ())
				{
					Vector cSphere = ((SceneSphere *)sceneObj)->center;
					float rSphere = ((SceneSphere *)sceneObj)->radius;
			
					float A = (rayPtr.depth.x)*(rayPtr.depth.x) + (rayPtr.depth.y)*(rayPtr.depth.y) + (rayPtr.depth.z)*(rayPtr.depth.z);
					float B = 2*((rayPtr.depth.x)*(rayPtr.startPt.x - cSphere.x) + (rayPtr.depth.y)*(rayPtr.startPt.y - cSphere.y) + (rayPtr.depth.z)*(rayPtr.startPt.z - cSphere.z));
					float C = (rayPtr.startPt.x - cSphere.x)*(rayPtr.startPt.x - cSphere.x) + (rayPtr.startPt.y - cSphere.y)*(rayPtr.startPt.y - cSphere.y) + (rayPtr.startPt.z - cSphere.z)*(rayPtr.startPt.z - cSphere.z) - (rSphere*rSphere);
		
					//Finding the discriminant
					float discrim = (B*B) - (4*A*C);
					float discrimSqrt = sqrt(discrim);
					//positive and negative t values
					float tPos = (-B + discrimSqrt)/(2*A);
					float tNeg = (-B - discrimSqrt)/(2*A);
	
					//no intersection
					if(discrim < 0)
					{
						continue;
					}

					//Check if tPos & tNeg are positive
					if((tPos < 0) || (tNeg < 0))
					{
						continue;
					}

					//Check to see if tPos is greater than tNeg
					if(tPos > tNeg)
					{
						if(tNeg > 0)
						{
							t = tNeg;
							isInter = true;
						}
					}
					//check to see if tmninus is greater than tplus
					else if(tNeg > tPos)
					{
						if(tPos > 0)
						{
							t = tPos;
							isInter = true;
						}
					}

					return isInter;

				}
				else if(sceneObj->IsTriangle ())
				{
					Vector v0 = (Vector (((SceneTriangle *)sceneObj)->vertex[0].x, ((SceneTriangle *)sceneObj)->vertex[0].y, ((SceneTriangle *)sceneObj)->vertex[0].z));
					Vector v1 = (Vector (((SceneTriangle *)sceneObj)->vertex[1].x, ((SceneTriangle *)sceneObj)->vertex[1].y, ((SceneTriangle *)sceneObj)->vertex[1].z));
					Vector v2 = (Vector (((SceneTriangle *)sceneObj)->vertex[2].x, ((SceneTriangle *)sceneObj)->vertex[2].y, ((SceneTriangle *)sceneObj)->vertex[2].z));
			
					Vector e1 = v1 - v0;
					Vector e2 = v2 - v0;
					Vector p = (Vector(rayPtr.depth.Cross(e2)));
					float a = e1.Dot(p);
					float e = 0.00001;
					float f = 1/a;
					Vector s = rayPtr.startPt - v0;
					float u = f*(s.Dot(p));
					Vector q = s.Cross(e1);
					float v = f*(rayPtr.depth.Dot(q));
		
					if(a > -e && a < e)
					{
						continue;
					}
		
					if(u < 0.0 || u > 1.0)
					{
						continue;
					}

					if(v < 0.0 || u + v > 1.0)
					{
						continue;
					}
			
					t = f*(e2.Dot(q));
					if(t > e)
					{
						isInter = true;
					}
					else
					{
						continue;
					}
					
					return isInter;
				}
				else if(sceneObj->IsModel ())
				{
					float e = 0.00001;
					SceneTriangle* tri = new SceneTriangle();

					for(int i = 0; i < ((SceneModel *)sceneObj)->GetNumTriangles() ; i ++)
					{
						isInter = false;
						tri = ((SceneModel *)sceneObj)->GetTriangle(i);
						Vector v0 = tri->vertex[0];
						Vector v1 = tri->vertex[1];
						Vector v2 = tri->vertex[2];

						Vector e1 = v1 - v0;
						Vector e2 = v2 - v0;

						Vector p = (Vector(rayPtr.depth.Cross(e2)));
						float a = e1.Dot(p);

						if( a > -e && a < e)
						{
							continue;
						}

						float f = 1/a;
						Vector s = rayPtr.startPt - v0;
						float u = f*(s.Dot(p));

						if (u < 0.0 || u > 1.0)
						{
							continue;
						}

						Vector q = s.Cross(e1);
						float v = f*(rayPtr.depth.Dot(q));

						if (v < 0.0 || u + v > 1.0)
						{
							continue;
						}

						t = f*(e2.Dot(q));
						if(t > e)
						{
							isInter = true;
						}
						else
						{
							continue;
						}

					return isInter;
				}
			}
		}
	}

	bool interModel(SceneModel* sceneModel)
	{
		float e = 0.00001;
		SceneTriangle* tri = new SceneTriangle();

		bool inter = false;

		for(int i = 0; i < sceneModel->GetNumTriangles() ; i ++)
		{
			tri = sceneModel->GetTriangle(i);
			Vector v0 = tri->vertex[0];
			Vector v1 = tri->vertex[1];
			Vector v2 = tri->vertex[2];

			Vector e1 = v1 - v0;
			Vector e2 = v2 - v0;

			Vector p = (Vector(rayPtr.depth.Cross(e2)));
			float a = e1.Dot(p);

			if( a > -e && a < e)
			{
				continue;
			}

			float f = 1/a;
			Vector s = rayPtr.startPt - v0;
			float u = f*(s.Dot(p));

			if (u < 0.0 || u > 1.0)
			{
				continue;
			}

			Vector q = s.Cross(e1);
			float v = f*(rayPtr.depth.Dot(q));

			if (v < 0.0 || u + v > 1.0)
			{
				continue;
			}

			t = f*(e2.Dot(q));
			inter = true;
		}

		globalTri = tri;
		return inter;
	}

	bool interTri(Vector v0, Vector v1, Vector v2)
	{
		Vector e1 = v1 - v0;
		Vector e2 = v2 - v0;
		Vector p = (Vector(rayPtr.depth.Cross(e2)));
		float a = e1.Dot(p);
		float e = 0.00001;
		float f = 1/a;
		Vector s = rayPtr.startPt - v0;
		float u = f*(s.Dot(p));
		Vector q = s.Cross(e1);
		float v = f*(rayPtr.depth.Dot(q)); 
		
		if(a > -e && a < e)
		{
			return false;
		}
		
		if(u < 0.0 || u > 1.0)
		{
			return false;
		}

		if(v < 0.0 || u + v > 1.0)
		{
			return false;
		}
			
		t = f*(e2.Dot(q));
		if(t > 0.00001)
		{
			return true;
		}
		else
		{
			return false;
		}	
	}

	bool interSphere(Vector cSphere, float rSphere)
	{
		float A = (rayPtr.depth.x)*(rayPtr.depth.x) + (rayPtr.depth.y)*(rayPtr.depth.y) + (rayPtr.depth.z)*(rayPtr.depth.z);
		float B = 2*((rayPtr.depth.x)*(rayPtr.startPt.x - cSphere.x) + (rayPtr.depth.y)*(rayPtr.startPt.y - cSphere.y) + (rayPtr.depth.z)*(rayPtr.startPt.z - cSphere.z));
		float C = (rayPtr.startPt.x - cSphere.x)*(rayPtr.startPt.x - cSphere.x) + (rayPtr.startPt.y - cSphere.y)*(rayPtr.startPt.y - cSphere.y) + (rayPtr.startPt.z - cSphere.z)*(rayPtr.startPt.z - cSphere.z) - (rSphere*rSphere);
		
		//Finding the discriminant
		float discrim = (B*B) - (4*A*C);
		float discrimSqrt = sqrt(discrim);
		//positive and negative t values
		float tPos = (-B + discrimSqrt)/(2*A);
		float tNeg = (-B - discrimSqrt)/(2*A);
	
		//no intersection
		if(discrim < 0)
		{
			return false;
		}

		//Check if tPos & tNeg are positive
		if((tPos < 0) || (tNeg < 0))
		{
			return false;
		}

		//Check to see if tPos is greater than tNeg
		if(tPos > tNeg)
		{
			if(tNeg > 0)
			{
				t = tNeg;
				return true;
			}
		}
		//check to see if tmninus is greater than tplus
		else if(tNeg > tPos)
		{
			if(tPos > 0)
			{
				t = tPos;
				return true;
			}
		}
		
	}

	float CalcDistance(Vector startPt, Vector tempRay)
	{
		float x = tempRay.x - startPt.x;
		float y = tempRay.y - startPt.y;
		float z = tempRay.z - startPt.z;

		float dist = sqrt(x*x + y*y + z*z);
		return dist;
	}

	float GetTriangleArea(Vector v1, Vector v2, Vector v3)
	{
		Vector p, r, c;
		p = v2 - v1;
		r = v3 - v1;
		c = p.Cross(r);

		return c.Magnitude()*.5;
	}

	Vector GetNormalTriangle(SceneTriangle* triangle, Vector intersect)
	{
		Vector v0 = triangle->vertex[0]; 
		Vector v1 = triangle->vertex[1]; 
		Vector v2 = triangle->vertex[2];
		Vector n0 = triangle->normal[0]; 
		Vector n1 = triangle->normal[1]; 
		Vector n2 = triangle->normal[2];

		float a = GetTriangleArea(v1,v2,intersect);
		float b = GetTriangleArea(v2,v0,intersect);
		float c = GetTriangleArea(intersect,v0,v1);

		t = GetTriangleArea(v0,v1,v2);
		return (n0*a+ n1*b + n2*c)/t;
	}
};

#endif // RAYTRACE_H
