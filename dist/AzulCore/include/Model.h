#ifndef MODEL_H
#define MODEL_H

class Vect;
struct TriangleIndex;

class Model
{
public:
	Model(const char * const _modelName);
	virtual ~Model();

private:
	void privCreateVAO(const char * const _modelName);

public:
	// Model information
		int numTextures;
		int numVerts;
		int numTris;

	// Bounding Sphere: bounding volume
		Vect *center;
		float radius;

	// AABB: axis aligned bounding box
		// Specifies the minimum extent of this AABB in the world space x, y and z axes.
		Vect *minPointAABB;
		// Specifies the maximum extent of this AABB in the world space x, y and z axes
		Vect *maxPointAABB;

	// Get the texture name
		char *getTextureName(int index);

	// Get access
		Vect *getVectList(void);
		TriangleIndex *getTriangleList(void);

	// Data that should be private below ---------------------------
	Vect *pVect;
	TriangleIndex *pTriList;


	char *textNames;

	unsigned int vao;
	unsigned int vbo[2];
};


#endif