/*
  NOTE: You do not need to edit this file for this assignment but may do so

  This file defines the following:
	SceneObjectType Enumeration
	SceneBackground Class
	SceneLight Class
	SceneMaterial Class
	SceneObject Class -> SceneSphere, SceneTriangle, SceneModel
	Scene Class

  Scene Usage: Several Functions
	Scene::Load (sceneFile) - load a scene descriptor file

	Scene::GetDescription () - get the scene description string
	Scene::GetAuthor () - get the scene author string
	Scene::GetBackground () - get the scene background information
	Scene::GetNumLights () - get the number of lights in the scene
	Scene::GetLight (lightIndex) - get one of the lights in the scene
	Scene::GetNumMaterials () - get the number of materials in the scene
	Scene::GetMaterial (materialIndex OR materialName) - get a material's description
	Scene::GetNumObjects () - get the number of objects in the scene
	Scene::GetObject (objectIndex) - get an object's description
	Scene::GetCamera - get the current camera description

	The trickiest one of these is the GetObject () function
	It is used like this:
	[The object is type-casted to its corresponding type]

	SceneObject tempObject = m_Scene.GetObject (0);
	if (tempObject.IsTriangle ())
	{
		Vector vertices[3];
		
		for (int n = 0; n < 3; n++)
		{
			vertices[n] = ((SceneTriangle)tempObject).vertex[n];
		}
	}
*/

#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>

#include "Utils.h"

// XML Parser by Frank Vanden Berghen
// Available: http://iridia.ulb.ac.be/~fvandenb/tools/xmlParser.html
#include "xmlParser.h"

// 3DS File by bkenwright@xbdev.net
//    Updated by Raphael Mun
#include "3ds.h"

// Max Line Length for OBJ File Loading
#define MAX_LINE_LEN 1000

#define CHECK_ATTR(a) (a == NULL ? "" : a)
#define CHECK_ATTR2(a,b) (a == NULL ? b : a)

/*
	SceneObjectType Namespace - Holds the ObjectType Enumeration
*/
namespace SceneObjectType
{
	enum ObjectType
	{
		Sphere = 0,
		Triangle = 1,
		Model = 2,
	};
};

/*
	SceneBackground Class - The Background properties of a ray-trace scene

	This class defines the background in the Scene
*/
class SceneBackground
{
public:
	Vector color;
	Vector ambientLight;
};

/*
	SceneLight Class - The light properties of a single light-source in a ray-trace scene

	The Scene class holds a list of these
*/
class SceneLight
{
public:
	float attenuationConstant, attenuationLinear, attenuationQuadratic;
	Vector color;
	Vector position;
};

/*
	SceneMaterial Class - The material properties used in a ray-trace scene

	The Scene class holds a list of material
*/
class SceneMaterial
{
public:
	std::string name;
	std::string texture;
	Vector diffuse;
	Vector specular;
	float shininess;
	Vector transparent;
	Vector reflective;
	Vector refraction_index;

	// -- Constructors & Destructors --
	SceneMaterial (void)
	{}

	~SceneMaterial (void)
	{
	}
};

/*
	SceneObject Class - A base object class that defines the common features of all objects

	This is the base object class that the various scene object types derive from
*/
class SceneObject
{
public:
	std::string name;
	SceneObjectType::ObjectType type;
	Vector scale, rotation, position;

	// -- Constructors & Destructors --
	SceneObject (void) { scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f; }
	SceneObject (SceneObjectType::ObjectType tp) : type (tp) { scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f; }
	SceneObject (std::string nm, SceneObjectType::ObjectType tp) : name(nm), type (tp) { scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f; }

	// -- Object Type Checking Functions --
	bool IsSphere (void) { return (type == SceneObjectType::Sphere); }
	bool IsTriangle (void) { return (type == SceneObjectType::Triangle); }
	bool IsModel (void) { return (type == SceneObjectType::Model); }
};

/*
	SceneSphere Class - The sphere scene object

	Sphere object derived from the SceneObject
*/
class SceneSphere : public SceneObject
{
public:
	std::string material;
	Vector center;
	float radius;

	// -- Constructors & Destructors --
	SceneSphere (void) : SceneObject ("Sphere", SceneObjectType::Sphere) {}
	SceneSphere (std::string nm) : SceneObject (nm, SceneObjectType::Sphere) {}
};

/*
	SceneTriangle Class - The triangle scene object

	Single triangle object derived from the SceneObject
*/
class SceneTriangle : public SceneObject
{
public:
	std::string material[3];
	Vector vertex[3];
	Vector normal[3];
	float u[3], v[3];

	// -- Constructors & Destructors --
	SceneTriangle (void) : SceneObject ("Triangle", SceneObjectType::Triangle) {}
	SceneTriangle (std::string nm) : SceneObject (nm, SceneObjectType::Triangle) {}
};

/*
	SceneModel Class - The model scene object

	A model object consisting of a list of triangles derived from the SceneObject
*/
class SceneModel : public SceneObject
{
public:
	std::string filename;
	std::vector<SceneTriangle> triangleList;

	// -- Constructors & Destructors --
	SceneModel (void) : SceneObject ("Model", SceneObjectType::Model) {}
	SceneModel (std::string file) : SceneObject ("Model", SceneObjectType::Model) { filename = file; }
	SceneModel (std::string file, std::string nm) : SceneObject (nm, SceneObjectType::Model) { filename = file; }

	// -- Accessor Functions --
	// - GetNumTriangles - Returns the number of triangles in the model
	unsigned int GetNumTriangles (void) { return (unsigned int)triangleList.size (); }

	// - GetTriangle - Gets the nth SceneTriangle
	SceneTriangle *GetTriangle (int triIndex) { return &triangleList[triIndex]; }
};

/*
	Scene Class - The main scene definition class with the scene information

	This is the base scene class
*/
class Scene
{
	std::string m_Desc, m_Author;
	SceneBackground m_Background;
	std::vector<SceneLight> m_LightList;
	std::vector<SceneMaterial> m_MaterialList;
	std::vector<SceneObject *> m_ObjectList;

	// - Private utility Functions used by Load () -
	Vector ParseColor (XMLNode node)
	{
		if (node.isEmpty ())
			return Vector (0.0f, 0.0f, 0.0f);
		return Vector (atof(node.getAttribute("red")), 
						atof(node.getAttribute("green")),
						atof(node.getAttribute("blue")));
	}

	Vector ParseXYZ (XMLNode node)
	{
		if (node.isEmpty ())
			return Vector (0.0f, 0.0f, 0.0f);
		return Vector (atof(node.getAttribute("x")), 
					   atof(node.getAttribute("y")),
					   atof(node.getAttribute("z")));
	}

	void ParseOBJCommand (char *line, int max, char *command, int &position);
	Vector ParseOBJVector (char *str);
	bool ParseOBJCoords (char *str, int &num, int v_index[3], int n_index[3]);
public:
	Camera m_Camera;


	// -- Constructors & Destructors --
	Scene (void) {}
	~Scene (void)
	{
		// Free the memory allocated from the objects
		unsigned int numObj = GetNumObjects ();
		for (unsigned int n = 0; n < numObj; n++)
		{
			SceneObject *sceneObj = m_ObjectList[n];
			if (sceneObj->IsSphere ())
			{
				delete ((SceneSphere *)sceneObj);
			}
			else if (sceneObj->IsTriangle ())
			{
				delete ((SceneTriangle *)sceneObj);
			}
			else if (sceneObj->IsModel ())
			{
				delete ((SceneModel *)sceneObj);
			}
		}
		m_ObjectList.clear ();
	}

	// -- Main Functions --
	// - Load - Loads the Scene XML file
	bool Load (char *filename);

	// -- Accessor Functions --
	// - GetDescription - Returns the Description String
	const char * GetDescription (void) { return m_Desc.c_str(); }

	// - GetAuthor - Return the Author String
	const char * GetAuthor (void) { return m_Author.c_str(); }

	// - GetBackground - Returns the SceneBackground
	SceneBackground GetBackground (void) { return m_Background; }

	// - GetNumLights - Returns the number of lights in the scene
	unsigned int GetNumLights (void) { return (unsigned int)m_LightList.size (); }

	// - GetLight - Returns the nth SceneLight
	SceneLight GetLight (int lightIndex) { return m_LightList[lightIndex]; }

	// - GetNumMaterials - Returns the number of materials in the scene
	unsigned int GetNumMaterials (void) { return (unsigned int)m_MaterialList.size (); }

	// - GetMaterial - Returns the nth SceneMaterial
	SceneMaterial GetMaterial (int matIndex) { return m_MaterialList[matIndex]; }
	SceneMaterial GetMaterial (std::string matName)
	{
		unsigned int numMats = (unsigned int)m_MaterialList.size ();
		for (unsigned int n = 0; n < numMats; n++)
		{
			if (matName == m_MaterialList[n].name)
				return m_MaterialList[n];
		}

		return SceneMaterial ();
	}

	// - GetNumObjects - Returns the number of objects in the scene
	unsigned int GetNumObjects (void) { return (unsigned int)m_ObjectList.size (); }

	// - GetObject - Returns the nth object [NOTE: The Object will need to be type-casted afterwards]
	SceneObject *GetObject (int objIndex) { return m_ObjectList[objIndex]; }

	// - GetCamera - Returns the camera class
	Camera GetCamera (void) { return m_Camera; }
};




bool Scene::Load (char *filename)
{
	XMLNode tempNode;

	// Open the Scene XML File
	printf ("Loading Scenefile %s...\n", filename);
	XMLNode sceneXML = XMLNode::openFileHelper(filename, "scene");
	if (sceneXML.isEmpty ())
		return false;
	m_Desc = CHECK_ATTR(sceneXML.getAttribute("desc"));
	m_Author = CHECK_ATTR(sceneXML.getAttribute("author"));

	printf ("Description: \n\t%s\n", m_Desc.c_str ());
	printf ("Author: %s\n", m_Author.c_str ());

	// Load the Background
	printf ("Loading Background...\n");
	tempNode = sceneXML.getChildNode("background");
	if (tempNode.isEmpty ())
		return false;
	m_Background.color = ParseColor (tempNode.getChildNode("color"));
	m_Background.ambientLight = ParseColor (tempNode.getChildNode("ambientLight"));

	// Load the Lights
	printf ("Loading Lights...\n");
	tempNode = sceneXML.getChildNode("light_list");
	if (!tempNode.isEmpty ())
	{
		unsigned int numLights = tempNode.nChildNode ("light");
		for (unsigned int n = 0; n < numLights; n++)
		{
			XMLNode tempLightNode = tempNode.getChildNode("light", n);
			if (tempLightNode.isEmpty ())
				return false;
			SceneLight tempLight;
			tempLight.color = ParseColor (tempLightNode.getChildNode("color"));
			tempLight.attenuationConstant = atof(CHECK_ATTR(tempLightNode.getChildNode("attenuation").getAttribute ("constant")));
			tempLight.attenuationLinear = atof(CHECK_ATTR(tempLightNode.getChildNode("attenuation").getAttribute ("linear")));
			tempLight.attenuationQuadratic = atof(CHECK_ATTR(tempLightNode.getChildNode("attenuation").getAttribute ("quadratic")));
			tempLight.position = ParseXYZ (tempLightNode.getChildNode("position"));
			m_LightList.push_back (tempLight);
		}
	}

	// Load the Materials
	printf ("Loading Materials...\n");
	tempNode = sceneXML.getChildNode("material_list");
	if (!tempNode.isEmpty ())
	{
		unsigned int numMaterials = tempNode.nChildNode ("material");
		for (unsigned int n = 0; n < numMaterials; n++)
		{
			XMLNode tempMaterialNode = tempNode.getChildNode("material", n);
			if (tempMaterialNode.isEmpty ())
				return false;
			SceneMaterial tempMaterial;
			tempMaterial.name = CHECK_ATTR(tempMaterialNode.getAttribute("name"));
			tempMaterial.texture = CHECK_ATTR(tempMaterialNode.getChildNode("texture").getAttribute("filename"));
			tempMaterial.diffuse = ParseColor (tempMaterialNode.getChildNode("diffuse"));
			tempMaterial.specular = ParseColor (tempMaterialNode.getChildNode("specular"));
			tempMaterial.shininess = atof(CHECK_ATTR(tempMaterialNode.getChildNode("specular").getAttribute("shininess")));
			tempMaterial.transparent = ParseColor (tempMaterialNode.getChildNode("transparent"));
			tempMaterial.reflective = ParseColor (tempMaterialNode.getChildNode("reflective"));
			tempMaterial.refraction_index = ParseColor (tempMaterialNode.getChildNode("refraction_index"));

			m_MaterialList.push_back (tempMaterial);
		}
	}

	// Load the Objects
	printf ("Loading Objects...\n");
	tempNode = sceneXML.getChildNode("object_list");
	if (!tempNode.isEmpty ())
	{
		unsigned int numObjects = tempNode.nChildNode ();
		for (unsigned int n = 0; n < numObjects; n++)
		{
			XMLNode tempObjectNode = tempNode.getChildNode(n);
			if (tempObjectNode.isEmpty ())
				return false;
			if (!strcasecmp(tempObjectNode.getName (), "sphere"))
			{
				// Load a Sphere
				SceneSphere *tempSphere = new SceneSphere ();
				tempSphere->name = CHECK_ATTR(tempObjectNode.getAttribute("name"));
				tempSphere->material = CHECK_ATTR(tempObjectNode.getAttribute("material"));
				tempSphere->radius = atof(CHECK_ATTR(tempObjectNode.getAttribute("radius")));
				tempSphere->scale = ParseXYZ (tempObjectNode.getChildNode("scale"));
				tempSphere->rotation = ParseXYZ (tempObjectNode.getChildNode("rotation"));
				tempSphere->position = ParseXYZ (tempObjectNode.getChildNode("position"));
				tempSphere->center = ParseXYZ (tempObjectNode.getChildNode("center"));
				m_ObjectList.push_back (tempSphere);
			}
			else if (!strcasecmp(tempObjectNode.getName (), "triangle"))
			{
				// Load a Triangle
				XMLNode vertexNode;
				SceneTriangle *tempTriangle = new SceneTriangle ();
				tempTriangle->name = CHECK_ATTR(tempObjectNode.getAttribute("name"));
				tempTriangle->scale = ParseXYZ (tempObjectNode.getChildNode("scale"));
				tempTriangle->rotation = ParseXYZ (tempObjectNode.getChildNode("rotation"));
				tempTriangle->position = ParseXYZ (tempObjectNode.getChildNode("position"));
				
				// Load Vertex 0
				vertexNode = tempObjectNode.getChildNodeWithAttribute ("vertex", "index", "0");
				tempTriangle->material[0] = CHECK_ATTR(vertexNode.getAttribute("material"));
				tempTriangle->vertex[0] = ParseXYZ (vertexNode.getChildNode("position"));
				tempTriangle->normal[0] = ParseXYZ (vertexNode.getChildNode("normal"));
				tempTriangle->u[0] = atof (CHECK_ATTR(vertexNode.getChildNode("texture").getAttribute("u")));
				tempTriangle->v[0] = atof (CHECK_ATTR(vertexNode.getChildNode("texture").getAttribute("v")));

				// Load Vertex 1
				vertexNode = tempObjectNode.getChildNodeWithAttribute ("vertex", "index", "1");
				tempTriangle->material[1] = CHECK_ATTR(vertexNode.getAttribute("material"));
				tempTriangle->vertex[1] = ParseXYZ (vertexNode.getChildNode("position"));
				tempTriangle->normal[1] = ParseXYZ (vertexNode.getChildNode("normal"));
				tempTriangle->u[1] = atof (CHECK_ATTR(vertexNode.getChildNode("texture").getAttribute("u")));
				tempTriangle->v[1] = atof (CHECK_ATTR(vertexNode.getChildNode("texture").getAttribute("v")));

				// Load Vertex 2
				vertexNode = tempObjectNode.getChildNodeWithAttribute ("vertex", "index", "2");
				tempTriangle->material[2] = CHECK_ATTR(vertexNode.getAttribute("material"));
				tempTriangle->vertex[2] = ParseXYZ (vertexNode.getChildNode("position"));
				tempTriangle->normal[2] = ParseXYZ (vertexNode.getChildNode("normal"));
				tempTriangle->u[2] = atof (CHECK_ATTR(vertexNode.getChildNode("texture").getAttribute("u")));
				tempTriangle->v[2] = atof (CHECK_ATTR(vertexNode.getChildNode("texture").getAttribute("v")));

				m_ObjectList.push_back (tempTriangle);
			}
			else if (!strcasecmp(tempObjectNode.getName (), "model"))
			{
				// Load a Model
				SceneModel *tempModel = new SceneModel ();
				tempModel->filename = CHECK_ATTR(tempObjectNode.getAttribute("filename"));
				if (tempModel->filename == "")
				{
					printf ("No Specified Model filename\n");
					return false;
				}
				tempModel->name = CHECK_ATTR(tempObjectNode.getAttribute("name"));
				std::string material = CHECK_ATTR(tempObjectNode.getAttribute("material"));
				//tempModel->material = tempObjectNode.getAttribute("material");
				tempModel->scale = ParseXYZ (tempObjectNode.getChildNode("scale"));
				tempModel->rotation = ParseXYZ (tempObjectNode.getChildNode("rotation"));
				tempModel->position = ParseXYZ (tempObjectNode.getChildNode("position"));
				
				// Check the file format
				if (tempModel->filename.substr (tempModel->filename.length() - 4, 4) == ".3ds")
				{
					// Load the list of triangles from the .3ds
					C3DS sceneObj;
					if (!sceneObj.Create((char *)tempModel->filename.c_str()))
					{
						printf ("Error loading .3ds file\n");
						return false;
					}

					for (unsigned int obj = 0; obj < (unsigned int)sceneObj.m_iNumMeshs; obj++)
					{
						for (unsigned int n = 0; n < (unsigned int)sceneObj.m_pMeshs[obj].iNumFaces; n++)
						{
							SceneTriangle tempTriangle;

							Vector v1, v2, v3;
							// Vert 1
							v1.x = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[0]].x;
							v1.y = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[0]].y;
							v1.z = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[0]].z;
							// Vert 2
							v2.x = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[1]].x;
							v2.y = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[1]].y;
							v2.z = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[1]].z;
							// Vert 3
							v3.x = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[2]].x;
							v3.y = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[2]].y;
							v3.z = sceneObj.m_pMeshs[obj].pVerts[sceneObj.m_pMeshs[obj].pFaces[n].corner[2]].z;

							// Normal
							Vector normal = (v1 - v2).Cross(v3 - v2).Normalize();

							// Load Vertex 0
							tempTriangle.material[0] = material;
							tempTriangle.vertex[0] = v1;
							tempTriangle.normal[0] = normal;
							// Texture Coords
							if (sceneObj.m_pMeshs[obj].bTextCoords)
							{
								tempTriangle.u[0] = sceneObj.m_pMeshs[obj].pTexs[sceneObj.m_pMeshs[obj].pFaces[n].corner[0]].tu;
								tempTriangle.v[0] = sceneObj.m_pMeshs[obj].pTexs[sceneObj.m_pMeshs[obj].pFaces[n].corner[0]].tv;
							}
							else
							{
								tempTriangle.u[0] = 0.0f;
								tempTriangle.v[0] = 0.0f;
							}

							// Load Vertex 1
							tempTriangle.material[1] = material;
							tempTriangle.vertex[1] = v2;
							tempTriangle.normal[1] = normal;
							// Texture Coords
							if (sceneObj.m_pMeshs[obj].bTextCoords)
							{
								tempTriangle.u[1] = sceneObj.m_pMeshs[obj].pTexs[sceneObj.m_pMeshs[obj].pFaces[n].corner[1]].tu;
								tempTriangle.v[1] = sceneObj.m_pMeshs[obj].pTexs[sceneObj.m_pMeshs[obj].pFaces[n].corner[1]].tv;
							}
							else
							{
								tempTriangle.u[1] = 0.0f;
								tempTriangle.v[1] = 0.0f;
							}

							// Load Vertex 2
							tempTriangle.material[2] = material;
							tempTriangle.vertex[2] = v3;
							tempTriangle.normal[2] = normal;
							// Texture Coords
							if (sceneObj.m_pMeshs[obj].bTextCoords)
							{
								tempTriangle.u[2] = sceneObj.m_pMeshs[obj].pTexs[sceneObj.m_pMeshs[obj].pFaces[n].corner[2]].tu;
								tempTriangle.v[2] = sceneObj.m_pMeshs[obj].pTexs[sceneObj.m_pMeshs[obj].pFaces[n].corner[2]].tv;
							}
							else
							{
								tempTriangle.u[2] = 0.0f;
								tempTriangle.v[2] = 0.0f;
							}

							tempModel->triangleList.push_back (tempTriangle);
						}
					}

					sceneObj.Release();
				}
				else if (tempModel->filename.substr (tempModel->filename.length() - 4, 4) == ".obj")
				{
					// The following code is a modified version of code from the old RayTracer Code rt_trimesh.cpp
					char line[MAX_LINE_LEN];
					char command[MAX_LINE_LEN];
					int position;
					vector<Vector> vertices;
					vector<Vector> normals;

					std::ifstream infile (tempModel->filename.c_str());

					if (infile == NULL)
					{
						printf ("Error loading .obj file\n");
						return false;
					}

					while (infile.good ())
					{
						infile.getline (line, MAX_LINE_LEN);
						ParseOBJCommand (line, MAX_LINE_LEN, command, position);

						if (strcmp (command,"v")==0)
						{
							Vector pos = ParseOBJVector (&(line[position]));
							vertices.push_back (pos);
						}
						else if (strcmp (command,"vn")==0)
						{
							Vector norm = ParseOBJVector (&(line[position]));
							normals.push_back (norm);
						}
						else if (strcmp (command,"f")==0)
						{
							int num = 0; // number of edges
							int v_index[3]; // vertex index
							int n_index[3]; // normal index

							if (!ParseOBJCoords (&(line[position]), num, v_index, n_index))
							{
								printf ("Error parsing faces in .obj file\n");
								return false;
							}

							SceneTriangle tempTriangle;

							// Load Vertex 0
							tempTriangle.material[0] = material;
							tempTriangle.vertex[0] = vertices[v_index[0]];
							tempTriangle.normal[0] = normals[n_index[0]];
							// Texture Coords
							tempTriangle.u[0] = 0.0f;
							tempTriangle.v[0] = 0.0f;

							// Load Vertex 1
							tempTriangle.material[1] = material;
							tempTriangle.vertex[1] = vertices[v_index[1]];
							tempTriangle.normal[1] = normals[n_index[1]];
							// Texture Coords
							tempTriangle.u[1] = 0.0f;
							tempTriangle.v[1] = 0.0f;

							// Load Vertex 2
							tempTriangle.material[2] = material;
							tempTriangle.vertex[2] = vertices[v_index[2]];
							tempTriangle.normal[2] = normals[n_index[2]];
							// Texture Coords
							tempTriangle.u[2] = 0.0f;
							tempTriangle.v[2] = 0.0f;

							tempModel->triangleList.push_back (tempTriangle);
						}
						else
						{
							//printf ("Ignoring command <%s> in obj file\n", command);
						}
					}
					infile.close ();
				}
				else
				{
					printf ("Unsupported file format\n");
					return false;
				}

				m_ObjectList.push_back (tempModel);
			}
			else
			{
				printf ("Unrecognized Node <%s> in <object_list>\n", tempObjectNode.getName ());
				exit (255);
			}
		}
	}

	// Load the Camera
	printf ("Loading Camera...\n");
	tempNode = sceneXML.getChildNode("camera");
	if (tempNode.isEmpty ())
		return false;
	m_Camera.SetFOV (atof(CHECK_ATTR2(tempNode.getAttribute("fieldOfView"), "45.0")));
	m_Camera.SetNearClip (atof(CHECK_ATTR2(tempNode.getAttribute("nearClip"), "0.1")));
	m_Camera.SetFarClip (atof(CHECK_ATTR2(tempNode.getAttribute("farClip"), "100.0")));
	m_Camera.SetPosition (ParseXYZ (tempNode.getChildNode("position")));
	m_Camera.SetTarget (ParseXYZ (tempNode.getChildNode("target")));
	m_Camera.SetUp (ParseXYZ (tempNode.getChildNode("up")));

	printf ("Scene Loaded!\n");

	return true;
}

void Scene::ParseOBJCommand (char *line, int max, char *command, int &position)
{
	int i = 0;
	int j = 0;

	while (i<max && line[i]==' ') i++;
	while (i<max && line[i]!='\0' && line[i]!=' ')
	{
		command[j] = line[i];
		j++; i++;
	}

	command[j] = '\0';
	position = i;
}

Vector Scene::ParseOBJVector (char *str)
{
	int i = 0;
	float x,y,z;
	while (str[i]!='\0' && str[i]==' ') i++;
	x = atof (&(str[i]));
	while (str[i]!='\0' && str[i]!=' ') i++;
	y = atof (&(str[i]));
	while (str[i]!='\0' && str[i]==' ') i++;
	while (str[i]!='\0' && str[i]!=' ') i++;
	z = atof (&(str[i]));
	return Vector (x,y,z);
}

bool Scene::ParseOBJCoords (char *str, int &num, int v_index[3], int n_index[3])
{
	int i = 0;
	num = 0;

	while (str[i]!='\0' && str[i]==' ') i++;
	while (str[i]!='\0')
	{
		while (str[i]!='\0' && str[i]!=' ') i++;
		while (str[i]!='\0' && str[i]==' ') i++;
		num++;
	}
	if (num != 3)
		return false;

	i = 0;
	for (int j=0; j<num; j++)
	{
		while (str[i]==' ') i++;
		v_index[j] = atoi (&(str[i])) - 1;
		while (str[i]!='/' && str[i]!=' ') i++;
		if (str[i] != '/')
			return false;
		i++;
		while (str[i]!='/' && str[i]!=' ') i++;
		if (str[i] != '/')
			return false;
		i++;
		n_index[j] = atoi (&(str[i])) - 1;
		while(str[i]!='\0' && str[i]!=' ') i++;
	}

	return true;
}

#endif // SCENE_H
