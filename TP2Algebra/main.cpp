#include <iostream>
#include <vector>
#include "raylib.h"
using namespace std;

struct VecRect
{
	Vector3 startPos;
	Vector3 finishPos;
	Vector3 rotationAngles;
	float magnitude;
};

struct Cube
{
	VecRect vecA;
	VecRect vecB;
	VecRect vecC;

	VecRect vecA2;
	VecRect vecA3;
	VecRect vecA4;

	VecRect vecB2;
	VecRect vecB3;
	VecRect vecB4;

	VecRect vecC2;
	VecRect vecC3;
	VecRect vecC4;
};

Camera3D camera;

vector<Cube> pyramidParts;

float n = 5;
float totalPerimeter = 0;
float totalArea = 0;
float totalVolume = 0;

void Init();
void Update();
void Draw();
void BuildCube(Cube& cube);
void BuildPyramid();
void DrawPyramid();
void DrawCube(Cube cube);
void InitVectors(Vector3 offSet, Vector3 rotationAngles, Cube& cube, float magnitude, float baseMagnitude);
void InitCamera();

void GetFinishPosition(VecRect& vector);
Vector3 GetCrossProduct(Vector3 rotationA, Vector3 rotationB);

int main(void)
{
	srand(time(NULL));

	Init();

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	CloseWindow();

	return 0;
}

void Init()
{
	const int screenWidth = 800;
	const int screenHeight = 500;
	InitWindow(screenWidth, screenHeight, "TP2 Algebra");

	InitCamera();
	BuildPyramid();

	DisableCursor();
}

void Update()
{
	UpdateCameraPro(&camera,
		Vector3{
		(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * 0.1f -      // Move forward-backward
			(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * 0.1f,
			(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * 0.1f -   // Move right-left
			(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * 0.1f,
			0.0f                                                // Move up-down
		},
		Vector3{
		GetMouseDelta().x * 0.05f,                            // Rotation: yaw
			GetMouseDelta().y * 0.05f,                            // Rotation: pitch
			0.0f                                                // Rotation: roll
		},
		GetMouseWheelMove() * 2.0f);                              // Move to target (zoom)
}

void BuildCube(Cube& cube)
{
	cube.vecA2 = cube.vecA;
	cube.vecA2.startPos = cube.vecB.finishPos;
	GetFinishPosition(cube.vecA2);

	cube.vecB2 = cube.vecB;
	cube.vecB2.startPos = cube.vecA.finishPos;
	GetFinishPosition(cube.vecB2);

	cube.vecC2 = cube.vecC;
	cube.vecC2.startPos = cube.vecB.finishPos;
	GetFinishPosition(cube.vecC2);

	cube.vecC3 = cube.vecC;
	cube.vecC3.startPos = cube.vecA.finishPos;
	GetFinishPosition(cube.vecC3);

	cube.vecC4 = cube.vecC;
	cube.vecC4.startPos = cube.vecA2.finishPos;
	GetFinishPosition(cube.vecC4);

	cube.vecA3 = cube.vecA;
	cube.vecA3.startPos = cube.vecC2.finishPos;
	GetFinishPosition(cube.vecA3);

	cube.vecA4 = cube.vecA;
	cube.vecA4.startPos = cube.vecC.finishPos;
	GetFinishPosition(cube.vecA4);

	cube.vecB3 = cube.vecB;
	cube.vecB3.startPos = cube.vecC3.finishPos;
	GetFinishPosition(cube.vecB3);

	cube.vecB4 = cube.vecB;
	cube.vecB4.startPos = cube.vecC.finishPos;
	GetFinishPosition(cube.vecB4);
}



void BuildPyramid()
{
	bool pyramidFinished = false;
	int maxDegrees = 360;
	Vector3 offSet = { 0.0f, 0.0f, 0.0f };
	//Vector3 startRotation = { (float)(rand() % maxDegrees), (float)(rand() % maxDegrees), (float)(rand() % maxDegrees) };
	Vector3 startRotation = { 270,98,28 };

	const float baseMagnitude = n * 10;
	float startMagnitude = baseMagnitude;
	int numCubes = 5;

	float perimeter = 0;
	float area = 0;
	float volume = 0;

	Cube myCube;

	do
	{
		InitVectors(offSet, startRotation, myCube, startMagnitude, baseMagnitude);

		if (myCube.vecA.magnitude >= myCube.vecC.magnitude * 2)
		{
			BuildCube(myCube);

			pyramidParts.push_back(myCube);

			offSet = myCube.vecC.finishPos;
			offSet.y += myCube.vecC.magnitude;

			perimeter += 12 * myCube.vecC.magnitude;
			//((myCube.vecA.magnitude * 8.0f) + (myCube.vecC.magnitude * 4.0f));
			area += ((myCube.vecA.magnitude * 2.0f) + (myCube.vecC.magnitude * 2.0f)) * 6.0f;
			volume += ((myCube.vecA.magnitude * myCube.vecB.magnitude) * myCube.vecC.magnitude);

			cout << "CUBE" << numCubes << ": " << endl;
			cout << "OSX: " << offSet.x << endl;
			cout << "old Y: " << myCube.vecC.finishPos.y << endl;
			cout << "new Y: " << offSet.y << endl;
			cout << "OSZ: " << offSet.z << endl;
			cout << "Perimeter: " << perimeter << endl;
			cout << "Area: " << area << endl;
			cout << "Volume: " << volume << endl;

			DrawCircle(offSet.x, offSet.y, 10, RED);
			numCubes--;
			startMagnitude -= myCube.vecC.magnitude * 2;
			totalPerimeter += perimeter;
			totalArea += area;
			totalVolume += volume;
			cout << "Total perimeter: " << totalPerimeter << endl;
			cout << "Total area: " << totalArea << endl;
			cout << "Total volume: " << totalVolume << endl;
		}
		else
			pyramidFinished = true;

	} while (!pyramidFinished);
}

void DrawPyramid()
{
	for (int i = 0; i < pyramidParts.size(); i++)
	{
		DrawCube(pyramidParts[i]);
	}
}

void Draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	BeginMode3D(camera);
	DrawPyramid();

	EndMode3D();

	EndDrawing();
}

void DrawCube(Cube cube)
{
	DrawLine3D(cube.vecA.startPos, cube.vecA.finishPos, RED);
	DrawLine3D(cube.vecB.startPos, cube.vecB.finishPos, BLUE);
	DrawLine3D(cube.vecC.startPos, cube.vecC.finishPos, GREEN);

	DrawLine3D(cube.vecA2.startPos, cube.vecA2.finishPos, RED);
	DrawLine3D(cube.vecA3.startPos, cube.vecA3.finishPos, RED);
	DrawLine3D(cube.vecA4.startPos, cube.vecA4.finishPos, RED);

	DrawLine3D(cube.vecB2.startPos, cube.vecB2.finishPos, BLUE);
	DrawLine3D(cube.vecB3.startPos, cube.vecB3.finishPos, BLUE);
	DrawLine3D(cube.vecB4.startPos, cube.vecB4.finishPos, BLUE);

	DrawLine3D(cube.vecC2.startPos, cube.vecC2.finishPos, GREEN);
	DrawLine3D(cube.vecC3.startPos, cube.vecC3.finishPos, GREEN);
	DrawLine3D(cube.vecC4.startPos, cube.vecC4.finishPos, GREEN);
}

void InitVectors(Vector3 offSet, Vector3 rotationAngles, Cube& cube, float magnitude, float baseMagnitude)
{
	VecRect aux;
	cube.vecA.startPos = offSet;
	cube.vecA.rotationAngles = rotationAngles;
	cube.vecA.magnitude = magnitude;

	GetFinishPosition(cube.vecA);

	aux = cube.vecA;
	aux.rotationAngles.x *= -1;
	aux.rotationAngles.y *= -1;
	aux.rotationAngles.z *= -1;


	//Vector B
	cube.vecB.rotationAngles = GetCrossProduct(cube.vecA.rotationAngles, aux.rotationAngles);
	cube.vecB.startPos = offSet;
	cube.vecB.magnitude = cube.vecA.magnitude;
	GetFinishPosition(cube.vecB);


	//Vector C
	cube.vecC.magnitude = (1 / n) * baseMagnitude;
	cube.vecC.startPos = offSet;

	cube.vecC.rotationAngles = GetCrossProduct(cube.vecA.rotationAngles, cube.vecB.rotationAngles);
	GetFinishPosition(cube.vecC);

}

void InitCamera()
{
	camera.position = { 0.0f, 0.0f, 10.0f };	// Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };		// Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };			// Camera up vector (rotation towards target)
	camera.fovy = 64.0f;                        // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;     // Camera mode type

}

void GetFinishPosition(VecRect& vector)
{
	vector.finishPos.x = vector.startPos.x + vector.magnitude * cos(vector.rotationAngles.y) * cos(vector.rotationAngles.z);
	vector.finishPos.y = vector.startPos.y + vector.magnitude * sin(vector.rotationAngles.x) * cos(vector.rotationAngles.y);
	vector.finishPos.z = vector.startPos.z + vector.magnitude * sin(vector.rotationAngles.z);
}

Vector3 GetCrossProduct(Vector3 rotationA, Vector3 rotationB)
{
	Vector3 rotation;

	rotation.x = rotationA.x * rotationB.x;
	rotation.y = rotationA.y * rotationB.y;
	rotation.z = rotationA.z * rotationB.z;

	return rotation;
}