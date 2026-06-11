#pragma once
#include <glad/glad.h>
#include <corecrt_math.h>

class ViewGrid
{
public:
	ViewGrid();
	~ViewGrid();

	static void DrawGrid(GLfloat verticesForLines[], bool newLine, int verticesCount, int currentIndicesPos, bool endoflinemarker, GLuint IndicesForLines[], int indicesCount) {

		for (float i = 0.0f; i <= 10; i = i + 1)
		{
			for (float j = 0.0f; j <= 10; j = j + 1)
			{

				verticesForLines[verticesCount] = j / 10.0f;//x
				verticesCount++;
				verticesForLines[verticesCount] = 0.0f;		//y;
				verticesCount++;
				verticesForLines[verticesCount] = i / 10.0f;	//z
				verticesCount++;

				if (currentIndicesPos != 0 && fmod(currentIndicesPos - i, 10) == 0)
				{
					if (endoflinemarker) {
						endoflinemarker = false;
						newLine = true;
					}
					else {
						endoflinemarker = true;
					}
				}
				else {
					endoflinemarker = false;
				}

				if (currentIndicesPos + 11 <= 120) {
					IndicesForLines[indicesCount] = currentIndicesPos;
					indicesCount++;
					IndicesForLines[indicesCount] = currentIndicesPos + 11;
					indicesCount++;
				}

				if (currentIndicesPos + 1 <= 120 && !endoflinemarker)
				{
					IndicesForLines[indicesCount] = currentIndicesPos;
					indicesCount++;
					IndicesForLines[indicesCount] = currentIndicesPos + 1;
					indicesCount++;
				}

				currentIndicesPos++;

			}
		}
	}

private:

};

ViewGrid::ViewGrid()
{
}

ViewGrid::~ViewGrid()
{
}