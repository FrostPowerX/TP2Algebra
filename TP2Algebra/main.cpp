#include <iostream>
#include <vector>
#include <string>

#include "raylib.h"
#include "raymath.h"

using namespace std;

//Rectas
struct Rect
{
	//Rect
	//positions
	Vector3 startPos;
	Vector3 finishPos;

	//Vector
	//direction
	Vector3 direction;
	//length
	float magnitude;
};

struct Cube
{
	//un cubo se compone de 12 rectas

	//vector random
	Rect vecA;
	//vector a 90 grados del primero
	Rect vecB;
	//vector a 90 grados de los dos anteriores
	Rect vecC;

	Rect vecA2;
	Rect vecB2;
	Rect vecC2;

	Rect vecA3;
	Rect vecB3;
	Rect vecC3;

	Rect vecA4;
	Rect vecB4;
	Rect vecC4;
};

vector<Cube> pyramidParts;

//-

float n = 0.0f;
float totalPerimeter = 0;
float totalArea = 0;
float totalVolume = 0;

bool cubeUpdated = false;

Camera3D camera;

void Init();
void Update();
void Draw();
void BuildCube(Cube& cube);
void BuildPyramid();
void DrawPyramid();
void DrawCube(Cube cube);
void InitVectors(Vector3 offSet, Vector3 rotationAngles, Cube& cube, float magnitude, float baseMagnitude);
void InitCamera();

void GetFinishPosition(Rect& vector);
Vector3 GetCrossProduct(Vector3 rotationA, Vector3 rotationB);

//-
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

//-
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
	int num = 0;

	if (aux >= '1' && aux <= '9')
	{
		num = aux - '0';
		n = num;

		totalArea = 0;
		totalPerimeter = 0;
		totalVolume = 0;

		pyramidParts.clear();
		BuildPyramid();
	}

	//-
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
			0.0f                                                  // Rotation: roll
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

	const int maxDegreesT = 360;
	const int maxDegreesD = 180;
	const int theta = rand() % maxDegreesT;
	const int phi =	rand() % maxDegreesD;

	Vector3 startRotation = 
	{ 
		(float)theta, //x
		(float)theta, //y
		(float)phi //z
	};

	Vector3 offSet = { 0.0f, 0.0f, 0.0f };

	const float baseMagnitude = GetRandomValue(20, 100);
	float startMagnitude = baseMagnitude;

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

			Rect midPoint = myCube.vecA4;
			midPoint.magnitude = myCube.vecC.magnitude;
			GetFinishPosition(midPoint);

			midPoint.startPos = midPoint.finishPos;
			midPoint.direction = myCube.vecB4.direction;
			GetFinishPosition(midPoint);

			offSet = midPoint.finishPos;

			startMagnitude -= myCube.vecC.magnitude * 2;

			//perimetro de un cuboide (suma de alto, ancho y profundidad)
			perimeter = ((myCube.vecA.magnitude * 8.0f) + (myCube.vecC.magnitude * 4.0f));
			//area de las 6 caras
			area = ((myCube.vecA.magnitude * 2.0f) + (myCube.vecC.magnitude * 2.0f)) * 6.0f;
			//formula de volumen para el cuboide. (largo * profundidad * alto)
			volume = ((myCube.vecA.magnitude * myCube.vecB.magnitude) * myCube.vecC.magnitude);

			totalPerimeter += perimeter;
			totalArea += area;
			totalVolume += volume;
		}
		else
			pyramidFinished = true;

	} while (!pyramidFinished);
}

void DrawPyramid()
{
	for (int i = 0; i < pyramidParts.size(); i++)
		DrawCube(pyramidParts[i]);
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
	
	int fontSize = 30;

	DrawText(area.c_str(), GetScreenWidth() - MeasureText(area.c_str(), fontSize) - 5, 0, fontSize, RED);
	DrawText(volume.c_str(), GetScreenWidth() - MeasureText(volume.c_str(), fontSize) - 5, 30, fontSize, RED);
	DrawText(perimeter.c_str(), GetScreenWidth() - MeasureText(perimeter.c_str(), fontSize) - 5, 60, fontSize, RED);

	EndDrawing();
}

void DrawCube(Cube cube)
{
	DrawLine3D(cube.vecA.startPos, cube.vecA.finishPos, RED);
	DrawLine3D(cube.vecB.startPos, cube.vecB.finishPos, BLUE);
	DrawLine3D(cube.vecC.startPos, cube.vecC.finishPos, GREEN);

	DrawLine3D(cube.vecA2.startPos, cube.vecA2.finishPos, RED);
	DrawLine3D(cube.vecB2.startPos, cube.vecB2.finishPos, BLUE);
	DrawLine3D(cube.vecC2.startPos, cube.vecC2.finishPos, GREEN);

	DrawLine3D(cube.vecA3.startPos, cube.vecA3.finishPos, RED);
	DrawLine3D(cube.vecB3.startPos, cube.vecB3.finishPos, BLUE);
	DrawLine3D(cube.vecC3.startPos, cube.vecC3.finishPos, GREEN);

	DrawLine3D(cube.vecA4.startPos, cube.vecA4.finishPos, RED);
	DrawLine3D(cube.vecB4.startPos, cube.vecB4.finishPos, BLUE);
	DrawLine3D(cube.vecC4.startPos, cube.vecC4.finishPos, GREEN);

}

void InitVectors(Vector3 offSet, Vector3 rotationAngles, Cube& cube, float magnitude, float baseMagnitude)
{
	//Vector A

	cube.vecA.startPos = offSet;
	cube.vecA.direction = rotationAngles;
	cube.vecA.magnitude = magnitude;

	GetFinishPosition(cube.vecA);

	//Vector B	

	Vector3 zDir = { 0,0,1 };
	Rect aux;

	aux = cube.vecA;
	aux.direction = zDir;

	cube.vecB.direction = GetCrossProduct(cube.vecA.direction, aux.direction);
	cube.vecB.startPos = offSet;
	cube.vecB.magnitude = cube.vecA.magnitude;
	GetFinishPosition(cube.vecB);


	//Vector C

	float fraction = 1 / n;

	cube.vecC.magnitude = fraction * baseMagnitude;
	cube.vecC.startPos = offSet;

	cube.vecC.direction = GetCrossProduct(cube.vecA.direction, cube.vecB.direction);
	GetFinishPosition(cube.vecC);
}

void InitCamera()
{
	camera.position = { 0.0f, 0.0f, 10.0f };	// Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };		// Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };			// Camera up vector (rotation towards target)
	camera.fovy = 90.0f;                        // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;     // Camera mode type

}

//Coordenadas esfericas
void GetFinishPosition(Rect& vector)
{
	// Convert degrees to radians
	double radiansX = vector.direction.x * (PI / 180.0);
	double radiansY = vector.direction.y * (PI / 180.0);

	//matrices de rototraslacion
																//vertical		//horizontal
	vector.finishPos.x = vector.startPos.x + vector.magnitude * sin(radiansX) * cos(radiansY);
	vector.finishPos.y = vector.startPos.y + vector.magnitude * sin(radiansX) * sin(radiansY);
	vector.finishPos.z = vector.startPos.z + vector.magnitude * cos(radiansX);
}

Vector3 GetCrossProduct(Vector3 rotationA, Vector3 rotationB) // Producto cruz
{
	Vector3 rotation;

	rotation.x = rotationA.y * rotationB.z - rotationA.z * rotationB.y;
	rotation.y = rotationA.z * rotationB.x - rotationA.x * rotationB.z;
	rotation.z = rotationA.x * rotationB.y - rotationA.y * rotationB.x;

	return rotation;
}