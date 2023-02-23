#pragma once

namespace My
{
	/*
	Vertex : contient les informations suivantes : 
	
	vec3 position 
	*/

	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(float p_x, float p_y, float p_z);
		~Vertex() = default;

	private:
		//vec3 m_position // TODO : Vector3
	};
}