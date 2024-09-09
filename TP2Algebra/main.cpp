#include <iostream>
#include "raylib.h"

using namespace std;

struct VecRect
{
	Vector3 pos;
	Vector3 rotationAngles;
	float magnitude;
};

void Update();
void Draw();
void calculateMagnitude(VecRect& vector);

int main(void)
{
	srand(time(NULL));

	const int screenWidth = 1280;
	const int screenHeight = 720;
	int maxDegrees = 360;

	Vector3 startPos = { 0.0f, 0.0f, 0.0f, };

	VecRect vectorA;
	vectorA.pos = startPos;
	vectorA.rotationAngles = { static_cast<float>(rand() % maxDegrees), static_cast<float>(rand() % maxDegrees), static_cast<float>(rand() % maxDegrees) };
	vectorA.magnitude = 1;

	VecRect vectorB;
	vectorB.pos = startPos;
	vectorB.rotationAngles = { 0.0f, 0.0f, 0.0f };
	vectorB.magnitude = 1;

	VecRect vectorC;
	vectorC.pos = startPos;
	vectorC.rotationAngles = { 0.0f, 0.0f, 0.0f };
	vectorC.magnitude = 1;

	InitWindow(screenWidth, screenHeight, "TP2 Algebra");

	SetTargetFPS(60);

	calculateMagnitude(vectorA);

	while (!WindowShouldClose())
	{
		Update();
		cout << vectorA.magnitude << endl;
		Draw();
	}

	CloseWindow();

	return 0;
}

void Update()
{
	
}

void Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);


	EndDrawing();
}

void calculateMagnitude(VecRect& vector) 
{
	vector.magnitude = sqrt(pow(vector.rotationAngles.x, 2.0f) + pow(vector.rotationAngles.y, 2.0f) + pow(vector.rotationAngles.z, 2.0f));
}