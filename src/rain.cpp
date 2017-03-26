#include "../include/rain.hpp"
#include <random>

#undef min
#undef max

/* 
 * RainDrop
 */
RainDrop::RainDrop(vec3f& position)
{
	m_position = position;
}

RainDrop::~RainDrop() {}

void RainDrop::draw(vec3f& direction)
{
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.f, 1.f, 1.f);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_LINE_STRIP);	
	glVertex3f(m_position[0], m_position[1], m_position[2]);
	glVertex3f(m_position[0] - direction[0],
		m_position[1] - direction[1],
		m_position[2] - direction[2]);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

/*
 * Rain
 */

Rain::Rain(Camera * camera, HeightMapLoader* heightmap)
{
	m_pCamera = camera;
	m_pHeightmap = heightmap;

	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_real_distribution<float> distrX(m_pCamera->getX() - m_fRainSquareSize, m_pCamera->getX() + m_fRainSquareSize);
	std::chi_squared_distribution<float> distrY(2);
	std::uniform_real_distribution<float> distrZ(m_pCamera->getZ() - m_fRainSquareSize, m_pCamera->getZ() + m_fRainSquareSize);
	m_vRaindrops.reserve(m_iNumOfDrops);
	for (int i = 0; i < m_iNumOfDrops; i++)
	{
		float x = distrX(generator);
		float y = distrY(generator)*25.0f + 15.f;
		float z = distrZ(generator);
		vec3f position(x, y, z);
		RainDrop rd(position);
		m_vRaindrops.push_back(rd);
	}
}

Rain::~Rain(){}

void Rain::update()
{
	if (m_bRaining)
	{
		float step = m_fRainSpeed * m_pCamera->getElapsedTime();
		for (auto &drop : m_vRaindrops)
		{
			drop.m_position -= m_direction * step;
			checkPosition(drop);
			drop.draw(m_direction);
		}
	}
}

void Rain::toggleRain()
{
	if (m_bRaining)
	{
		m_bRaining = false;
	}
	else
	{
		m_bRaining = true;
	}
}

void Rain::checkPosition(RainDrop& drop)
{
	float dropX = drop.m_position.x();
	float dropY = drop.m_position.y();
	float dropZ = drop.m_position.z();
	// check if drop is in the square
	if (dropX < (m_pCamera->getX() - m_fRainSquareSize))
	{
		drop.m_position[0] += m_fRainSquareSize*2;
	}
	else if (dropX > (m_pCamera->getX() + m_fRainSquareSize))
	{
		drop.m_position[0] -= m_fRainSquareSize * 2;
	}
	if (dropZ < (m_pCamera->getZ() - m_fRainSquareSize))
	{
		drop.m_position[2] += m_fRainSquareSize * 2;
	}
	else if (dropZ >(m_pCamera->getZ() + m_fRainSquareSize))
	{
		drop.m_position[2] -= m_fRainSquareSize * 2;
	}
	// check if drop is not under the terrain
	float terrainY = m_pHeightmap->getHeight(drop.m_position.x(), drop.m_position.z());
	if (dropY < terrainY)
	{
		drop.m_position[1] = terrainY + m_fRainMaxHeight;
	}
}

