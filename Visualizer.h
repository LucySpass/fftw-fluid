#ifndef MAIN_VISUALIZER_H
#define MAIN_VISUALIZER_H
#include "common.h"
#include "Simulator.h"
#include <memory>
#include <vector>
#include <math.h> 
#include <ctime>
#include <iostream>

struct Visualizer
{
	static constexpr int ColorMapKfVelocity = 100;
	static constexpr int ColorMapKfForce = 100;

	enum Colormaps {
		WHITE,
		RAINBOW,
		BANDED_RAINBOW,
		RANDOM,
		PINK_CIAN,
		OCEAN,
		CountColormaps
	};

	enum ScalarObjects {
		VELOCITY,
		FORCE,
		DENSITY,
		CountScalarObjects,
	};

	enum GradientScalarObjects {
		GRADIENT_NONE,
		GRADIENT_VELOCITY,
		GRADIENT_DENSITY,
		CountGradientScalarObjects,
	};

	enum GlyphTypes {
		POINTS,
		LINE_LOOP,
		TRIANGLES,
		PATCHES,
		CountGlyphType,
	};

	enum IsolineTypes {
		ISOLINE_NONE,
		BASE,
		INTERVAL,
		CountIsolineType,
	};

	static constexpr uint32_t n_dir_colormaps = 2;
	static constexpr uint32_t n_scalar_colormaps = 6;

	void ToggleDrawGlyphs() { draw_vecs = !draw_vecs; }
	void ToggleDrawSmoke() { draw_smoke = !draw_smoke; }
	void NextDirectionColormap()
	{
		colormap_dir = (colormap_dir + 1) % ((int)Colormaps::CountColormaps);
		
		std::string colorName = "";
		switch (colormap_dir)
		{
		case 0:
			colorName = "WHITE";
			break;
		case 1:
			colorName = "RAINBOW";
			break;
		case 2:
			colorName = "BANDED_RAINBOW";
			break;
		case 3:
			colorName = "RANDOM";
			break;
		case 4:
			colorName = "PINK_CIAN";
			break;
		case 5:
			colorName = "OCEAN";
			break;
		default:
			break;
		}
		std::cout << "Current colormap: " << colorName << std::endl;
	}
	void NextScalarObject() {
		object_scalar = (object_scalar + 1) % ((int)ScalarObjects::CountScalarObjects);

		std::string scalarName = "";
		switch (object_scalar)
		{
		case 0:
			scalarName = "VELOCITY";
			break;
		case 1:
			scalarName = "FORCE";
			break;
		case 2:
			scalarName = "DENSITY";
			break;
		default:
			break;
		}
		std::cout << "Current scalar object: " << scalarName << std::endl;
	}
	void NextGlyphType() {
		glyph_type = (glyph_type + 1) % ((int)GlyphTypes::CountGlyphType);

		std::string glyphName = "";
		switch (glyph_type)
		{
		case 0:
			glyphName = "VELOCITY";
			break;
		case 1:
			glyphName = "FORCE";
			break;
		case 2:
			glyphName = "DENSITY";
			break;
		default:
			break;
		}
		std::cout << "Current glyph type: " << glyphName << std::endl;
	}
	void NextIsolineType() {
		isoline_type = (isoline_type + 1) % ((int)IsolineTypes::CountIsolineType);

		std::string isolineName = "";
		switch (isoline_type)
		{
		case 0:
			isolineName = "NONE";
			break;
		case 1:
			isolineName = "BASE";
			break;
		case 2:
			isolineName = "INTERVAL";
			break;
		default:
			break;
		}
		std::cout << "Current isoline type: " << isolineName << std::endl;
	}
	void NextGradientObject() {
		gradient_object = (gradient_object + 1) % ((int)GradientScalarObjects::CountGradientScalarObjects);

		std::string gradientName = "";
		switch (gradient_object)
		{
		case 0:
			gradientName = "NONE";
			break;
		case 1:
			gradientName = "GRADIENT_VELOCITY";
			break;
		case 2:
			gradientName = "GRADIENT_DENSITY";
			break;
		default:
			break;
		}
		std::cout << "Current gradient type: " << gradientName << std::endl;
	}
	void NextScalarColormap() { 
		colormap_scalar = (colormap_scalar + 1) % n_scalar_colormaps; 
	
		std::string scalarColormapName = "";
		switch (colormap_scalar)
		{
		case 0:
			scalarColormapName = "GRADIENT_VELOCITY";
			break;
		case 1:
			scalarColormapName = "GRADIENT_DENSITY";
			break;
		default:
			break;
		}
		std::cout << "Current colormap scalar type: " << scalarColormapName << std::endl;
	}
	void MultiplyGlyphLength(double mul) { vec_scale *= mul; }

	//Initialize and bind empty buffers; generate mesh grid 
	// n - grid size
	void Init(int n, uint32_t width, uint32_t height);

	//update buffers with values from Simulator
	void UpdateBuffers(const std::shared_ptr<Simulator>& pSim);
	void Draw();
	void DrawIsoline(const std::shared_ptr<Simulator>& pSim);

private:
	//Colormap selection func
	void Colormap(float val, float& R, float& G, float& B);
	//Rainbow colormap for vectors
	static void Direction2DToColor(float x, float y, int method, float& r, float& g, float& b);

	void ScalarToColor(float value, int method, float& r, float& g, float& b);

	float vec_scale = 2000;
	bool  draw_smoke = false;
	bool  draw_vecs = true;
	bool  draw_isoline = true;

	uint32_t colormap_dir = WHITE;
	uint32_t colormap_scalar = WHITE;
	uint32_t object_scalar = VELOCITY;
	uint32_t glyph_type = POINTS;
	uint32_t isoline_type = ISOLINE_NONE;
	uint32_t gradient_object = GRADIENT_VELOCITY;

	int m_grid_dim;
	uint32_t m_width;
	uint32_t m_height;

	std::vector<float> vec_pos;
	std::vector<float> smoke_pos;
	std::vector<float> vec_col;
	std::vector<float> smoke_col;
	std::vector<uint32_t> indices;

	///isoline
	float* iso_pos, * iso_col;

	GLuint vertexLocation = 0;
	GLuint colorLocation = 1;
	GLuint sim_vao_vec;
	GLuint sim_vbo_vec;
	GLuint sim_vao_smoke;
	GLuint sim_vbo_smoke;
	GLuint vbo_vec_col;
	GLuint vbo_smoke_col;
};


#endif //MAIN_VISUALIZER_H
