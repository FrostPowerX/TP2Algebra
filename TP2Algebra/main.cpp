#include <iostream>
#include <vector>
#include <string>

#include "raylib.h"
#include "raymath.h"

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

float n = 10;

float totalPerimeter = 0;
float totalArea = 0;
float totalVolume = 0;

bool cubeUpdated = false;

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
	//BuildPyramid();

	DisableCursor();
}

void Update()
{
	char aux = GetCharPressed();
	float num = 0;
	if (aux >= 49 && aux <= 57)
	{
		num = aux - 48;
		n = num;

		totalArea = 0;
		totalPerimeter = 0;
		totalVolume = 0;

		pyramidParts.clear();
		BuildPyramid();
		cubeUpdated = true;
	}

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

	DrawText("Press a number between 1 and 9", 0, 0, 20, RED);
	DrawText("Move with the arrow keys and the mouse wheel", 0, 30, 20, RED);
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
	Vector3 startRotation = { (float)(rand() % maxDegrees), (float)(rand() % maxDegrees), (float)(rand() % maxDegrees) };

	const float baseMagnitude = GetRandomValue(20, 100);
	float startMagnitude = baseMagnitude;
	int numCubes = 5;

	float perimeter = 0;
	float area = 0;
	float volume = 0;

	Cube myCube;

	do
	{
		InitVectors(offSet, startRotation, myCube, startMagnitude, baseMagnitude);

		if (myCube.vecA.magnitude >= myCube.vecC.magnitude)
		{
			BuildCube(myCube);

			pyramidParts.push_back(myCube);

			VecRect midPoint = myCube.vecA4;
			midPoint.magnitude = myCube.vecC.magnitude;
			GetFinishPosition(midPoint);

			midPoint.startPos = midPoint.finishPos;
			midPoint.rotationAngles = myCube.vecB4.rotationAngles;
			GetFinishPosition(midPoint);



			offSet = midPoint.finishPos;

			startMagnitude -= myCube.vecC.magnitude * 2;

			perimeter += 12 * myCube.vecC.magnitude;
			//((myCube.vecA.magnitude * 8.0f) + (myCube.vecC.magnitude * 4.0f));
			area += ((myCube.vecA.magnitude * 2.0f) + (myCube.vecC.magnitude * 2.0f)) * 6.0f;
			volume += ((myCube.vecA.magnitude * myCube.vecB.magnitude) * myCube.vecC.magnitude);

			totalPerimeter += perimeter;
			totalArea += area;
			totalVolume += volume;

			numCubes--;
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

	string area = "Area: " + to_string(totalArea);
	string volume = "Volumen: " + to_string(totalVolume);
	string perimeter = "Perimetro: " + to_string(totalPerimeter);

	DrawText(area.c_str(), GetScreenWidth() - MeasureText(area.c_str(), 15) - 5, 0, 15, RED);
	DrawText(volume.c_str(), GetScreenWidth() - MeasureText(volume.c_str(), 15) - 5, 30, 15, RED);
	DrawText(perimeter.c_str(), GetScreenWidth() - MeasureText(perimeter.c_str(), 15) - 5, 60, 15, RED);

	EndDrawing();
}

void DrawCube(Cube cube)
{
	if (cubeUpdated)
	{
		ClearBackground(WHITE);
		cubeUpdated = false;
	}

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
	Vector3 zDir = { 0,0,1 };

	VecRect aux;
	cube.vecA.startPos = offSet;
	cube.vecA.rotationAngles = rotationAngles;
	cube.vecA.magnitude = magnitude;

	GetFinishPosition(cube.vecA);

	aux = cube.vecA;
	aux.rotationAngles = zDir;

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
	camera.fovy = 85.0f;                        // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;     // Camera mode type

}

void GetFinishPosition(VecRect& vector) // Rotacion de angulos euler
{
	vector.finishPos.x = vector.startPos.x + vector.magnitude * cos(vector.rotationAngles.y) * cos(vector.rotationAngles.z);
	vector.finishPos.y = vector.startPos.y + vector.magnitude * sin(vector.rotationAngles.x) * cos(vector.rotationAngles.y);
	vector.finishPos.z = vector.startPos.z + vector.magnitude * sin(vector.rotationAngles.z);
}

Vector3 GetCrossProduct(Vector3 rotationA, Vector3 rotationB) // Producto cruz
{
	Vector3 rotation;

	rotation.x = rotationA.y * rotationB.z - rotationA.z * rotationB.y;
	rotation.y = rotationA.z * rotationB.x - rotationA.x * rotationB.z;
	rotation.z = rotationA.x * rotationB.y - rotationA.y * rotationB.x;

	return rotation;
}