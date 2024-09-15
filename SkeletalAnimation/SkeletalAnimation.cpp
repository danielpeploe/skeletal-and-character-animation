//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury (2023)
//
//  FILE NAME: Skeleton Animation.cpp
//  See Exer04_SkeletalAnimation.pdf for details
//  ========================================================================

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"
#define GL_CLAMP_TO_EDGE 0x812F

//----------Globals----------------------------
const aiScene* scene = NULL;
aiAnimation* anim;
aiVector3D scene_min, scene_max, scene_center;
float scene_scale;


//timer globals
int tDuration;
int currTick = 0; 
float fps;
int timeStep;

// SECOND SKELETON
const aiScene* scene2 = NULL;
aiAnimation* anim2;
int tDuration2;
int currTick2 = 0;
float fps2;
int timeStep2;
float scene_scale2;
aiVector3D scene_min2, scene_max2, scene_center2;

float lightPosn[4] = { -5, 10, 10, 1 };
float shadow[16] = { lightPosn[1], 0, 0, 0, -lightPosn[0], 0.01, -lightPosn[2], -1, 0, 0, lightPosn[1], 0, 0, 0, 0, lightPosn[1] };
GLuint textures[6];

float y_cam = 2;
float x_cam = 0;
float z_cam = 25;

float angle = 0;


// ------A recursive function to traverse scene graph and render each mesh----------
// Simplified version for rendering a skeleton mesh
void render(const aiNode* node)
{
    aiMatrix4x4 m = node->mTransformation;
    aiMesh* mesh;
    aiFace* face;
    int meshIndex;

    m.Transpose();   //Convert to column-major order
    glPushMatrix();
    glMultMatrixf((float*)&m);   //Multiply by the transformation matrix for this node

    if (node->mNumMeshes > 0)
        {
            if ((node->mName) == aiString("Hips"))
            {
                glPushMatrix();
                    glTranslatef(0, 15, 0);
                    glScalef(32, 32, 6);
                    
                    glutSolidCube(1);
                glPopMatrix();
            } else if ((node->mName) == aiString("Chest")){
                glPushMatrix();
                    glTranslatef(0, 10, 0);
                    glScalef(35, 20, 6);
                    glutSolidCube(1);
                glPopMatrix();
            } else if ((node->mName) == aiString("Head")) {
                glPushMatrix();
                    glTranslatef(0, 4, 0);
                    glutSolidSphere(10, 10, 10);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("LeftCollar"))
            {
                glPushMatrix();
                    glTranslatef(10, 0, 0);
                    glScalef(20, 4, 6);
                    glutSolidCube(1);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("RightCollar"))
            {
                glPushMatrix();
                    glTranslatef(-10, 0, 0);
                    glScalef(20, 4, 6);
                    glutSolidCube(1);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("LeftShoulder"))
            {
                glPushMatrix();
                    glTranslatef(12, 0, 0);
                    glScalef(22, 6, 6);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("RightShoulder"))
            {
                glPushMatrix();
                    glTranslatef(-12, 0, 0);
                    glScalef(22, 6, 6);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("LeftElbow"))
            {
                glPushMatrix();
                    glTranslatef(11, 0, 0);
                    glScalef(22, 6, 6);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("RightElbow"))
            {
                glPushMatrix();
                    glTranslatef(-11, 0, 0);
                    glScalef(22, 6, 6);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("LeftWrist") || (node->mName) == aiString("RightWrist"))
            {
                glPushMatrix();
                    glTranslatef(0, -6, 0);
                    glScalef(6, 15, 6);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();

                // Boxing Glove
                glPushMatrix();
                    glTranslatef(0, -15, 0);
                    glutSolidSphere(8, 8, 8);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("LeftKnee") || (node->mName) == aiString("RightKnee"))
            {
                glPushMatrix();
                    glTranslatef(0, -20, 0);
                    glScalef(6, 40, 6);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("LeftHip") || (node->mName) == aiString("RightHip"))
            {
                glPushMatrix();
                    glTranslatef(0, -22, 0);
                    glScalef(6, 40, 6);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(7, 7, 7);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("LeftAnkle") || (node->mName) == aiString("RightAnkle"))
            {
                glPushMatrix();
                    glTranslatef(0, 0, 8);
                    glScalef(6, 6, 20);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, 0, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();
            }
            else if ((node->mName) == aiString("Neck"))
            {
                glPushMatrix();
                    glTranslatef(0, 5, 0);
                    glScalef(5, 6, 5);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0, -1, 0);
                    glutSolidSphere(6, 6, 6);
                glPopMatrix();
            }
        }

    // Recursively draw all children of the current node
    for (int i = 0; i < node->mNumChildren; i++)
        render(node->mChildren[i]);

    glPopMatrix();
}

void renderTwo(const aiNode* node, bool isRoot = false)
{
    aiMatrix4x4 m = node->mTransformation;
    aiMesh* mesh;
    aiFace* face;
    int meshIndex;

    // If root node zero out translation to keep trendmill users in place
    if (isRoot)
    {
        m.a4 = 0.0f;
        m.b4 = 0.0f;
        m.c4 = 0.0f;
    }

    m.Transpose();   // Convert to column-major order
    glPushMatrix();
    glMultMatrixf((float*)&m);   // Multiply by the transformation matrix for this node

    // The scene graph for a skeleton contains at most one mesh per node
    // Skeleton meshes are always triangle meshes
    if(node->mNumMeshes > 0)
    {
        meshIndex = node->mMeshes[0];          // Get the mesh indices from the current node
        mesh = scene2->mMeshes[meshIndex];    // Using mesh index, get the mesh object

		if ((node->mName) == aiString("Hips")) {
            glPushMatrix();
                glTranslatef(0, 1, -2);
                glScalef(27, 16, 6);
                glutSolidCube(1);
            glPopMatrix();
		} else if ((node->mName) == aiString("Spine")) {
			glPushMatrix();
                glTranslatef(0, 2.5, 3);
                glScalef(30, 15, 6);
                glutSolidCube(1);
            glPopMatrix();
		} else if ((node->mName) == aiString("Spine1")) {
			glPushMatrix();
                glTranslatef(0, 5, 3);
                glScalef(30, 20, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 16, 3);
                glScalef(20, 4, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 20, 3);
                glScalef(10, 10, 6);
                glutSolidCube(1);
            glPopMatrix();
		} else if ((node->mName) == aiString("Neck")) {
			glPushMatrix();
                glTranslatef(0, 2.5, 0);
                glScalef(5, 12, 4);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, -3, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();

		} else if ((node->mName) == aiString("Head")) {
			glPushMatrix();
                glTranslatef(0, 6, 0);
                glutSolidSphere(10, 10, 10);
            glPopMatrix();
		} else if ((node->mName) == aiString("LeftShoulder") || (node->mName) == aiString("RightShoulder")) {
			glPushMatrix();
                glTranslatef(0, 0, 0);
                glScalef(20, 5, 6);
                glutSolidCube(1);
            glPopMatrix();
		} else if ((node->mName) == aiString("LeftArm")) {
			glPushMatrix();
                glTranslatef(10, 0, 0);
                glScalef(20, 6, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();
		} else if ((node->mName) == aiString("RightArm")) {
			glPushMatrix();
                glTranslatef(-10, 0, 0);
                glScalef(20, 6, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();

		} else if ((node->mName) == aiString("LeftForeArm")) {
			glPushMatrix();
                glTranslatef(12, 0, 0);
                glScalef(22, 6, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();
		} else if ((node->mName) == aiString("RightForeArm")) {
			glPushMatrix();
                glTranslatef(-12, 0, 0);
                glScalef(22, 6, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();

		} else if ((node->mName) == aiString("LeftHand")) {
			glPushMatrix();
                glTranslatef(3, 0, 0);
                glScalef(12, 6, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();
		} else if ((node->mName) == aiString("RightHand")) {
			glPushMatrix();
                glTranslatef(-3, 0, 0);
                glScalef(12, 6, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();
		} else if ((node->mName) == aiString("LeftUpLeg") || (node->mName) == aiString("RightUpLeg")) {
			glPushMatrix();
                glTranslatef(0, -22, 0);
                glScalef(6, 40, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(7, 7, 7);
            glPopMatrix();

		} else if ((node->mName) == aiString("LeftLeg") || (node->mName) == aiString("RightLeg")) {
			glPushMatrix();
                glTranslatef(0, -16, 0);
                glScalef(6, 34, 6);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0, 0);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();
		} else if ((node->mName) == aiString("LeftFoot") || (node->mName) == aiString("RightFoot")) {
			glPushMatrix();
                glTranslatef(0, 0, 6);
                glScalef(6, 6, 18);
                glutSolidCube(1);
            glPopMatrix();

			glPushMatrix();
                glTranslatef(0, 0.2, 1);
                glutSolidSphere(6, 6, 6);
            glPopMatrix();
        } 
        // Draw the mesh in the current node
        
    }

    // Recursively draw all children of the current node
    for (int i = 0; i < node->mNumChildren; i++)
        renderTwo(node->mChildren[i], false);

    glPopMatrix();
}

void loadTextures()
{

	glGenTextures(6, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	loadTGA("treadmill.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	loadTGA("brick.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	loadTGA("ring.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[3]);
	loadTGA("wall.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[5]);
	loadTGA("gym.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//----- Function to update node matrices for each tick ------
// Complete this function
void updateNodeMatrices(int tick) 
{
    aiMatrix4x4 matPos, matRot, matProd;  //Position, rotation, product matrices
    aiMatrix3x3 matRot3;
    aiNode* node;

    aiVector3D posn;
    aiQuaternion rotn;


    anim = scene->mAnimations[0];
    for (int i = 0; i < anim->mNumChannels; i++)
    {
        matPos = aiMatrix4x4();   //Identity
        matRot = aiMatrix4x4();
        aiNodeAnim* chnl = anim->mChannels[i];      //Channel

        //See Lec04:Slides 31, 32  and complete this section
        chnl = anim->mChannels[i];
        if (chnl->mNumPositionKeys > 1) {
            posn = chnl->mPositionKeys[tick].mValue;
        }
        else {
            posn = chnl->mPositionKeys[0].mValue;
        }
        if (chnl->mNumRotationKeys > 1) {
            rotn = chnl->mRotationKeys[tick].mValue;
        }
        else {
            rotn = chnl->mRotationKeys[0].mValue;
        }

        matPos.Translation(posn, matPos);
        matRot3 = rotn.GetMatrix();
        matRot = aiMatrix4x4(matRot3);
        matProd = matPos * matRot;

        node = scene->mRootNode->FindNode(chnl->mNodeName);
        node->mTransformation = matProd;
    }
}

void updateNodeMatricesTwo(int tick) 
{
    aiMatrix4x4 matPos, matRot, matProd;  //Position, rotation, product matrices
    aiMatrix3x3 matRot3;
    aiNode* node;

    aiVector3D posn;
    aiQuaternion rotn;


    anim2 = scene2->mAnimations[0];
    for (int i = 0; i < anim2->mNumChannels; i++)
    {
        matPos = aiMatrix4x4();   //Identity
        matRot = aiMatrix4x4();
        aiNodeAnim* chnl = anim2->mChannels[i];      //Channel

        //See Lec04:Slides 31, 32  and complete this section
        chnl = anim2->mChannels[i];
        if (chnl->mNumPositionKeys > 1) {
            posn = chnl->mPositionKeys[tick].mValue;
        }
        else {
            posn = chnl->mPositionKeys[0].mValue;
        }
        if (chnl->mNumRotationKeys > 1) {
            rotn = chnl->mRotationKeys[tick].mValue;
        }
        else {
            rotn = chnl->mRotationKeys[0].mValue;
        }

        matPos.Translation(posn, matPos);
        matRot3 = rotn.GetMatrix();
        matRot = aiMatrix4x4(matRot3);
        matProd = matPos * matRot;

        node = scene2->mRootNode->FindNode(chnl->mNodeName);
        node->mTransformation = matProd;
    }
}

void update(int tick) {
	if (tick > tDuration) {
		tick = 0;
	} else {
		updateNodeMatrices(tick);
		tick++;
	}
	glutTimerFunc(timeStep, update, tick);

	glutPostRedisplay();
}

void updateTwo(int value) {
	if (currTick2 > tDuration2) {
        currTick2 = 45;
    }
    else {
        updateNodeMatricesTwo(currTick2);
        currTick2++;
    }
    glutTimerFunc(timeStep2, updateTwo, currTick2);
	glutPostRedisplay();
}


//--------------------OpenGL initialization------------------------
void initialise()
{
	float ambient[4] = { 0.2, 0.2, 0.2, 1.0 };  //Ambient light
	float white[4] = { 1, 1, 1, 1 };		    //Light's colour
	float black[4] = { 0, 0, 0, 1 };
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, black);   //Disable specular light
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 1.0, 500.0);

	//---- Load the model ------
	scene = aiImportFile("Boxing.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone);
	anim = scene->mAnimations[0];
	fps = anim->mTicksPerSecond;
	tDuration = anim->mDuration / 2;
	timeStep = int(1000 / fps);

	// Second animation
	scene2 = aiImportFile("Walk.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone);
    anim2 = scene2->mAnimations[0];
    fps2 = anim2->mTicksPerSecond;
    tDuration2 = 155;
    timeStep2 = int(1000 / fps2);


	if (scene == NULL || scene2 == NULL) exit(1);

	get_bounding_box(scene, &scene_min, &scene_max);
	scene_center = (scene_min + scene_max) * 0.5f;
	aiVector3D scene_diag = scene_max - scene_center;
	scene_scale = 1.0 / scene_diag.Length();

	get_bounding_box(scene2, &scene_min2, &scene_max2);
    scene_center2 = (scene_min2 + scene_max2) * 0.5f;
    aiVector3D scene_diag2 = scene_max2 - scene_center2;
    scene_scale2 = 1.0 / scene_diag2.Length();

	loadTextures();

}

void drawTreadmill() {
	glNormal3f(0, 1, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-6.0, 0.0, -3.5);
		glTexCoord2f(1.0, 0.0); glVertex3f(-6.0, 0.0, -6.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-5.0, 0.0, -6.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 0.0, -3.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
    	glTranslatef(-5.5, -0.101, -4.75);
        glScalef(1.0, 0.2, 2.5);
        glutSolidCube(1);
    glPopMatrix();

	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
    	glTranslatef(-6, 0.3, -5.89);
        glScalef(0.2, 1.0, 0.2);
        glutSolidCube(1);
    glPopMatrix();

	glPushMatrix();
    	glTranslatef(-5, 0.3, -5.89);
        glScalef(0.2, 1.0, 0.2);
        glutSolidCube(1);
    glPopMatrix();

	glPushMatrix();
    	glTranslatef(-5.5, 0.7, -5.89);
        glScalef(0.8, 0.2, 0.2);
        glutSolidCube(1);
    glPopMatrix();
}

void drawRing() {
	glNormal3f(0, 1, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		// Boxing ring floor
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, 0.0, -6.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(3.0, 0.0, -6.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(3.0, 0.0, 0.0);

		// Sides
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.2, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(3.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(3.0, -0.2, 0.0);

		glTexCoord2f(0.0, 0.0); glVertex3f(3.0, -0.2, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(3.0, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(3.0, 0.0, -6.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(3.0, -0.2, -6.0);


		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.2, -6.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, 0.0, -6.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-2.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, -0.2, 0.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawBackWall()
{
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-10, 3.5, -6);
		glTexCoord2f(0.0, 0.0); glVertex3f(-10, -0.2, -6);
		glTexCoord2f(1.0, 0.0); glVertex3f(5, -0.2, -6);
		glTexCoord2f(1.0, 1.0); glVertex3f(5, 3.5, -6);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

// CHANGE THE VALUES OF BUILDING (CHANGE HEIGHT SLIGHTLY AND STUFF) AND METHOD NAME
void drawRightWall() 
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
		glColor3f(1, 1, 1);

		glNormal3f(-1, 0, 0);
		glTexCoord2f(6.0, 4.0); glVertex3f(5, 3.5, 3.5);
		glTexCoord2f(6.0, 0.0); glVertex3f(5, -0.2, 3.5);
		glTexCoord2f(0.0, 0.0); glVertex3f(5, -0.2, -6);
		glTexCoord2f(0.0, 4.0); glVertex3f(5, 3.5, -6);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawGymSign() 
{
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0); glVertex3f(1, 3, -5.9);
		glTexCoord2f(0.0, 0.0); glVertex3f(1, 1, -5.9);
		glTexCoord2f(0.0, 1.0); glVertex3f(3, 1, -5.9);
		glTexCoord2f(1.0, 1.0); glVertex3f(3, 3, -5.9);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawFloor() 
{
	glNormal3f(0, 0, 1);
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -0.2, 3.5);
		glTexCoord2f(1.0, 0.0); glVertex3f(-10.0, -0.2, -6.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(5.0, -0.2, -6.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(5.0, -0.2, 3.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -0.21, 100);
		glTexCoord2f(1.0, 0.0); glVertex3f(-100.0, -0.21, -100);
		glTexCoord2f(1.0, 1.0); glVertex3f(100.0, -0.21, -100);
		glTexCoord2f(0.0, 1.0); glVertex3f(100.0, -0.21, 100);
	glEnd();

}

void makePlant() {
	glColor3f(0.3, 0.3, 0.3);
	glPushMatrix();
    	glTranslatef(-3.5, 0.05, -5.5);
        glScalef(0.5, 0.5, 0.5);
        glutSolidCube(1);
    glPopMatrix();

	glPushMatrix();
    	glTranslatef(-3.5, 0.35, -5.5);
        glScalef(0.6, 0.1, 0.6);
        glutSolidCube(1);
    glPopMatrix();

	 glColor3f(0.4, 0.22, 0.039);
	glPushMatrix();
    	glTranslatef(-3.5, 0.8, -5.5);
        glScalef(0.1, 1.0, 0.1);
        glutSolidCube(1);
    glPopMatrix();

	glColor3f(0.07, 0.56, 0.1);
	glPushMatrix();
    	glTranslatef(-3.5, 1.2, -5.5);
        glScalef(0.5, 1.0, 0.5);
        glutSolidCube(1);
    glPopMatrix();

}

void drawBuilding()
{
	drawRing();
	drawTreadmill();

	glPushMatrix();
    	glTranslatef(-3, 0.0, 0.0);
		drawTreadmill();
    glPopMatrix();

	drawBackWall();
	drawRightWall();
	drawFloor();
	drawGymSign();
	makePlant();

}

//------The main display function---------
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x_cam, y_cam, z_cam, 0, y_cam, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosn);

  	aiMatrix4x4 m = scene->mRootNode->mTransformation;
    float xpos = m.a4;   //Root joint's position in world space
    float ypos = m.b4;
    float zpos = m.c4;

	glPushMatrix();
		glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(scene_center.x, scene_center.y, scene_center.z);
		

		glPushMatrix();
			glTranslatef(0, 0, 2);
			drawBuilding();
		glPopMatrix();
		
		
		// Blue boxer
		glPushMatrix();
			glTranslatef(0, -0.1, 0);
			glScalef(scene_scale, scene_scale, scene_scale);
			glColor3f(0.0, 0.0, 1.0);
			render(scene->mRootNode);
		glPopMatrix();

        // shadow
		glPushMatrix();
			glColor4f(0.15, 0.15, 0.15, 1);
			glTranslatef(0, 0.01, 0);
			glMultMatrixf(shadow);
			glScalef(scene_scale, scene_scale, scene_scale);
			render(scene->mRootNode);
		glPopMatrix();
        
        
		// Red boxer
        glPushMatrix();
			glTranslatef(0.6, -0.1, -1);
            glRotatef(180.0, 0.0, 1.0, 0.0); 
			glScalef(scene_scale, scene_scale, scene_scale);
			glColor3f(1.0, 0.0, 0.0);
			render(scene->mRootNode);
		glPopMatrix();

		// Red boxer shadow
		glPushMatrix();
			glColor4f(0.15, 0.15, 0.15, 1);
			glTranslatef(0.6, 0.01, -1);
			glMultMatrixf(shadow);
			glScalef(-1.0, 1.0, -1.0);      
			glScalef(scene_scale, scene_scale, scene_scale);
			render(scene->mRootNode);              
		glPopMatrix();


		// Yellow walker
		glPushMatrix();
			glColor3f(0.5, 0.5, 0.0);
			glTranslatef(-5.5, 0.7, -2);
			glScalef(scene_scale, scene_scale, scene_scale);
			glRotatef(45.0, 0.0, 1.0f, 0.0);
			renderTwo(scene2->mRootNode, true);
    	glPopMatrix();

		// Cyan walker
		glPushMatrix();
			glColor3f(0.0, 0.5, 0.5);
			glTranslatef(-8.5, 0.7, -2.5);
			glScalef(scene_scale, scene_scale, scene_scale);
			glRotatef(45.0, 0.0f, 1.0, 0.0);
			renderTwo(scene2->mRootNode, true);
    	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void rotate(int dir) {
	angle += dir * 5;
	
	if (angle <= 0) {
		angle = 360;
	} else if (angle >= 360) {
		angle = 0;
	}
}

void zoom(int dir) {
	float x_move = -x_cam;
	float z_move = -z_cam;

	if (abs(x_move) > abs(z_move)) {
		z_move /= abs(x_move);
		x_move /= abs(x_move);
	}
	else {
		x_move /= abs(z_move);
		z_move /= abs(z_move);
	}
	float x_dist = scene_center.x - x_cam;
	float y_dist = scene_center.y - y_cam;
	float z_dist = scene_center.z - z_cam;

	float dist = sqrt(pow(x_dist, 2) + pow(y_dist, 2) + pow(z_dist, 2));
	if (dist > 7 || dir < 0)
	{
		x_cam += x_move * dir * 0.5;
		z_cam += z_move * dir * 0.5;
	}
}

void special(int key, int x, int y) {
	switch (key){
	case GLUT_KEY_LEFT:
		rotate(1);
		break;
	case GLUT_KEY_RIGHT:
		rotate(-1);
		break;
	case GLUT_KEY_UP:
		zoom(1);
		break;
	case GLUT_KEY_DOWN:
		zoom(-1);
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("GYM SCENE");

	initialise();
	glutDisplayFunc(display);
	glutTimerFunc(timeStep, update, 0);
	glutTimerFunc(timeStep2, updateTwo, 0);
	glutSpecialFunc(special);
	glutMainLoop();
}

