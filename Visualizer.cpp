#include "Visualizer.h"
GLuint sim_vbo_izo, sim_vao_izo;
GLuint vbo_izo_col;

//rainbow colormap for scalar value
void rainbow(float value, float& R, float& G, float& B)
{
	const float dx = 0.8;
	value = value < 0 ? 0 : value > 1 ? 1 : value;
	float g = (6 - 2 * dx) * value + dx;
	R = (fmaxf(0.0f, (3.0f - fabsf(g - 4.0f) - fabsf(g - 5.0f)) / 2.0f));
	G = (fmaxf(0.0f, (4.0f - fabsf(g - 2.0f) - fabsf(g - 4.0f)) / 2.0f));
	B = (fmaxf(0.0f, (3.0f - fabsf(g - 1.0f) - fabsf(g - 2.0f)) / 2.0f));
}

void random(float value, float& R, float& G, float& B) {
	double temp = 1 / (1 + exp(-value));
	srand((unsigned)time(NULL));
	R = (float)rand() / RAND_MAX; G = temp; B = (float)rand() / RAND_MAX;
}

void pinkCian(float value, float& R, float& G, float& B) {
	double temp = 1 / (1 + exp(-value));
	R = temp; G = 0.6; B = 1;
}

void ocean(float value, float& R, float& G, float& B) {
	float a = exp(value) + 41 / value;
	value = 1 / (1 + exp(-value));
	a = 1 / (1 + exp(-a));
	R = value; G = a; B = 1;
}

void Visualizer::Colormap(float val, float& R, float& G, float& B)
{
	if (colormap_scalar == WHITE)
		R = G = B = val;
	else if (colormap_scalar == RAINBOW)
		rainbow(val, R, G, B);
	else if (colormap_scalar == BANDED_RAINBOW)
	{
		const int NLEVELS = 7;
		val *= NLEVELS;
		val = ceilf(val) / NLEVELS;
		rainbow(val, R, G, B);
	}
	else if (colormap_scalar == RANDOM) {
		random(val, R, G, B);
	}
	else if (colormap_scalar == PINK_CIAN) {
		pinkCian(val, R, G, B);
	}
	else if (colormap_scalar == OCEAN) {
		ocean(val, R, G, B);
	}
}
void Visualizer::Direction2DToColor(float x, float y, int method, float& r, float& g, float& b)
{
	float f;
	if (method == RAINBOW)
	{
		f = atan2f(y, x) / 3.1415927f + 1.0f;
		r = f;

		if (r > 1)
			r = 2 - r;

		g = f + .66667f;
		if (g > 2)
			g -= 2;

		if (g > 1) g = 2 - g;

		b = f + 2 * .66667f;
		if (b > 2)
			b -= 2;

		if (b > 1)
			b = 2 - b;
	}
	else
	{
		r = g = b = 1;
	}
}

void Visualizer::ScalarToColor(float val, int colormap_scalar, float& R, float& G, float& B)
{
	double colorize = 0.8;
	if (colormap_scalar == WHITE)
		R = G = B = val;
	else if (colormap_scalar == RAINBOW)
		rainbow(val, R, G, B);
	else if (colormap_scalar == BANDED_RAINBOW)
	{
		const int NLEVELS = 7;
		val *= NLEVELS;
		val = ceilf(val) / NLEVELS;
		rainbow(val, R, G, B);
	}
	else if (colormap_scalar == RANDOM) {
		random(val, R, G, B);
	}
	else if (colormap_scalar == PINK_CIAN) {
		pinkCian(val, R, G, B);
	}
	else if (colormap_scalar == OCEAN) {
		ocean(val, R, G, B);
	}
	R *= colorize; G *= colorize; B *= colorize;
}

void Visualizer::Init(int n, uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;
	m_grid_dim = n;

	vec_pos.resize(n * n * 4);
	vec_col.resize(n * n * 6);

	glGenBuffers(1, &sim_vbo_vec);                                                        GL_CHECK_ERRORS;
	glGenBuffers(1, &vbo_vec_col);                                                        GL_CHECK_ERRORS;
	glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_vec);                                           GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, n * n * 4 * sizeof(GLfloat), vec_pos.data(), GL_STREAM_DRAW); GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vec_col);                                           GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, n * n * 6 * sizeof(GLfloat), vec_col.data(), GL_STREAM_DRAW); GL_CHECK_ERRORS;

	glGenVertexArrays(1, &sim_vao_vec);                                                   GL_CHECK_ERRORS;
	glBindVertexArray(sim_vao_vec);                                                       GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_vec);                                           GL_CHECK_ERRORS;
	glEnableVertexAttribArray(vertexLocation);                                            GL_CHECK_ERRORS;
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);                   GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vec_col);                                           GL_CHECK_ERRORS;
	glEnableVertexAttribArray(colorLocation);                                            GL_CHECK_ERRORS;
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);                   GL_CHECK_ERRORS;

	glBindVertexArray(0);

	//////////////////////////////////////////////////////
	int n_verts_grid = n * n;
	smoke_pos.resize(n_verts_grid * 2);
	smoke_col.resize(n_verts_grid * 3);

	glGenBuffers(1, &sim_vbo_smoke);                                                        GL_CHECK_ERRORS;
	glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_smoke);                                           GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, n_verts_grid * 2 * sizeof(GLfloat), smoke_pos.data(), GL_STREAM_DRAW); GL_CHECK_ERRORS;

	glGenBuffers(1, &vbo_smoke_col);                                                        GL_CHECK_ERRORS;
	glBindBuffer(GL_ARRAY_BUFFER, vbo_smoke_col);                                           GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, n_verts_grid * 3 * sizeof(GLfloat), smoke_col.data(), GL_STREAM_DRAW); GL_CHECK_ERRORS;

	glGenVertexArrays(1, &sim_vao_smoke);                                                   GL_CHECK_ERRORS;
	glBindVertexArray(sim_vao_smoke);                                                       GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_smoke);                                           GL_CHECK_ERRORS;
	glEnableVertexAttribArray(vertexLocation);                                            GL_CHECK_ERRORS;
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);                   GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_smoke_col);                                           GL_CHECK_ERRORS;
	glEnableVertexAttribArray(colorLocation);                                            GL_CHECK_ERRORS;
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);                   GL_CHECK_ERRORS;

	int numIndices = 2 * n * (n - 1) + n - 1;

	indices.resize(numIndices);

	int primRestart = n_verts_grid;
	int k = 0;
	for (int x = 0; x < n - 1; ++x)
	{
		for (int y = 0; y < n - 1; ++y)
		{
			int offset = x * n + y;

			if (y == 0)
			{
				indices[k] = offset + 0;     k++;
				indices[k] = offset + n;     k++;
				indices[k] = offset + 1;     k++;
				indices[k] = offset + n + 1; k++;
			}
			else
			{
				indices[k] = offset + 1;       k++;
				indices[k] = offset + n + 1; k++;

				if (y == n - 2)
				{
					indices[k] = primRestart; k++;
				}
			}
		}
	}

	GLuint vboIndices;
	glGenBuffers(1, &vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices); GL_CHECK_ERRORS;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices.data(), GL_STREAM_DRAW); GL_CHECK_ERRORS;

	glBindVertexArray(0);

	//////////////////////////////////////
	position = (float*)malloc(n * n * 4 * sizeof(float));
	column = (float*)malloc(n * n * 6 * sizeof(float));
	glGenBuffers(1, &sim_vbo_izo); GL_CHECK_ERRORS;
	glGenBuffers(1, &vbo_izo_col); GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_izo); GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, n * n * 2 * sizeof(GLfloat), position, GL_STREAM_DRAW); GL_CHECK_ERRORS;
	glBindBuffer(GL_ARRAY_BUFFER, vbo_izo_col); GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, n * n * 3 * sizeof(GLfloat), column, GL_STREAM_DRAW); GL_CHECK_ERRORS;

	glGenVertexArrays(1, &sim_vao_izo); GL_CHECK_ERRORS;
	glBindVertexArray(sim_vao_izo); GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_izo); GL_CHECK_ERRORS;
	glEnableVertexAttribArray(vertexLocation); GL_CHECK_ERRORS;
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); GL_CHECK_ERRORS;
	glBindBuffer(GL_ARRAY_BUFFER, vbo_izo_col); GL_CHECK_ERRORS;

	glEnableVertexAttribArray(colorLocation); GL_CHECK_ERRORS;
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); GL_CHECK_ERRORS;

	glBindVertexArray(0);
	//////////////////////////////////////
}


void Visualizer::UpdateBuffers(const std::shared_ptr<Simulator>& pSim)
{
	int i, j, idx;
	double px, py;

	double wn = (double)m_width / (double)(m_grid_dim + 1);   // Grid cell width
	double hn = (double)m_height / (double)(m_grid_dim + 1);  // Grid cell height

	float coeff_x = 2.0f / m_width;
	float coeff_y = 2.0f / m_height;

	if (isoline_type != ISOLINE_NONE) {
		DrawIsoline(pSim);
	}

	if (draw_smoke)
	{

		std::cout << draw_smoke << std::endl;
		int k = 0;
		int c = 0;
		for (i = 0; i < m_grid_dim; i++)			//draw smoke
		{
			for (j = 0; j < m_grid_dim; j++)
			{
				px = wn + (double)i * wn;
				py = hn + (double)j * hn;

				float r, g, b;

				if (object_scalar == VELOCITY) {
					ScalarToColor(pSim->GetVelocity(i, j) * ColorMapKfVelocity, colormap_dir, r, g, b);
				}
				else if (object_scalar == FORCE) {
					ScalarToColor(pSim->GetForce(i, j) * ColorMapKfForce, colormap_dir, r, g, b);
				}
				else if (object_scalar == DENSITY) {
					ScalarToColor(pSim->GetDensity(i, j), colormap_dir, r, g, b);
				}

				smoke_col[c] = r; c++;
				smoke_col[c] = g; c++;
				smoke_col[c] = b; c++;

				smoke_pos[k] = coeff_x * px - 1.0f; k++;
				smoke_pos[k] = coeff_y * py - 1.0f; k++;

			}
		}

		glBindVertexArray(sim_vao_smoke);                                                        GL_CHECK_ERRORS;
		glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_smoke);                                            GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, m_grid_dim * m_grid_dim * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); GL_CHECK_ERRORS;
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_grid_dim * m_grid_dim * 2 * sizeof(GLfloat), smoke_pos.data());      GL_CHECK_ERRORS;

		glBindBuffer(GL_ARRAY_BUFFER, vbo_smoke_col);                                            GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, m_grid_dim * m_grid_dim * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); GL_CHECK_ERRORS;
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_grid_dim * m_grid_dim * 3 * sizeof(GLfloat), smoke_col.data());      GL_CHECK_ERRORS;

		glBindVertexArray(0);
	}

	if (draw_vecs)
	{
		for (i = 0; i < m_grid_dim; i++)
		{
			for (j = 0; j < m_grid_dim; j++)
			{
				int temp_i = (i - 1) < 0 ? 0 : (i - 0);
				int temp_j = (j - 1) < 0 ? 0 : (j - 1);

				float r, g, b;
				if (object_scalar == VELOCITY) {
					if (gradient_object == GRADIENT_VELOCITY) {
						ScalarToColor((pSim->GetVelocity(i, j) - pSim->GetVelocity(temp_i, temp_j)) * ColorMapKfVelocity, colormap_dir, r, g, b);
					}
					else if (divergence_object == DIVERGENCE_VELOCITY) {
						ScalarToColor((pSim->GetVelocityX(i, j) - pSim->GetVelocityX(temp_i, j)) +
							(pSim->GetVelocityY(i, j) - pSim->GetVelocityY(i, temp_j)) * ColorMapKfVelocity, colormap_dir, r, g, b);
					}
					else {
						ScalarToColor(pSim->GetVelocity(i, j) * ColorMapKfVelocity, colormap_dir, r, g, b);
					}
				}
				else if (object_scalar == FORCE) {
					if (divergence_object == DIVERGENCE_FORCE) {
						ScalarToColor((pSim->GetForceX(i, j) - pSim->GetForceX(temp_i, j)) +
							(pSim->GetForceY(i, j) - pSim->GetForceY(i, temp_j)) * ColorMapKfForce, colormap_dir, r, g, b);
					}
					else {
						ScalarToColor(pSim->GetForce(i, j) * ColorMapKfForce, colormap_dir, r, g, b);
					}
				}
				else if (object_scalar == DENSITY) {
					if (gradient_object == GRADIENT_DENSITY) {
						ScalarToColor((pSim->GetDensity(i, j) - pSim->GetDensity(temp_i, temp_j)), colormap_dir, r, g, b);
					}
					else {
						ScalarToColor(pSim->GetDensity(i, j), colormap_dir, r, g, b);
					}
				}

				double x1, y1, x2, y2;
				int temp4 = (i * m_grid_dim + j) * 4;
				int temp6 = (i * m_grid_dim + j) * 6;

				/// Величина и направление глифа
				x1 = wn + (double)i * wn;
				y1 = hn + (double)j * hn;



				x2 = x1 + vec_scale * pSim->GetVelocityX(i, j);
				y2 = y1 + vec_scale * pSim->GetVelocityY(i, j);


				vec_pos[temp4 + 0] = coeff_x * x1 - 1.0f;
				vec_pos[temp4 + 1] = coeff_y * y1 - 1.0f;
				vec_pos[temp4 + 2] = coeff_x * x2 - 1.0f;
				vec_pos[temp4 + 3] = coeff_x * y2 - 1.0f;

				vec_col[temp6 + 0] = r;
				vec_col[temp6 + 1] = g;
				vec_col[temp6 + 2] = b;

				vec_col[temp6 + 3] = r;
				vec_col[temp6 + 4] = g;
				vec_col[temp6 + 5] = b;
			}
		}

		glBindVertexArray(sim_vao_vec);                                                          GL_CHECK_ERRORS;

		glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_vec);                                              GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, m_grid_dim * m_grid_dim * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); GL_CHECK_ERRORS;
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_grid_dim * m_grid_dim * 4 * sizeof(GLfloat), vec_pos.data());        GL_CHECK_ERRORS;

		glBindBuffer(GL_ARRAY_BUFFER, vbo_vec_col);                                              GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, m_grid_dim * m_grid_dim * 6 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); GL_CHECK_ERRORS;
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_grid_dim * m_grid_dim * 6 * sizeof(GLfloat), vec_col.data());        GL_CHECK_ERRORS;

		glDrawArrays(GL_LINES, 0, m_grid_dim * m_grid_dim * 2);
		glBindVertexArray(0);
	}
}

void Visualizer::DrawIsoline(const std::shared_ptr<Simulator>& pSim) {
	float rho0;
	float interval;
	static const GLsizei WIDTH = 1024, HEIGHT = 1024;
	// isoline toggle (true - interval, false - base)
	bool isoline_m = false;
	// isoline count
	int iso_n = 1;
	// base
	float rho0_iso = 0.2;
	// interval
	float rho1_iso = 0.0;
	float rho2_iso = 0.2;

	double  wn = (double)WIDTH / (double)(m_grid_dim + 1);
	double  hn = (double)HEIGHT / (double)(m_grid_dim + 1);

	float coeff_x = 2.0f / WIDTH;
	float coeff_y = 2.0f / HEIGHT;

	// if interval
	if (isoline_type == INTERVAL) {
		isoline_m = true;
	}

	if (isoline_m)
		interval = (rho2_iso - rho1_iso) / iso_n;

	for (int k = 0; k < iso_n; k++)
	{
		if (isoline_m)
			rho0 = rho2_iso - k * interval;
		else
			rho0 = rho0_iso;

		for (int i = 0; i < m_grid_dim; i++)
		{
			for (int j = 0; j < m_grid_dim; j++)
			{
				int idx = (j * m_grid_dim) + i;

				float r, g, b;

				int temp4 = (i * m_grid_dim + j) * 4;
				int temp6 = (i * m_grid_dim + j) * 6;

				r = g = b = 1;

				column[temp6 + 0] = r;
				column[temp6 + 1] = g;
				column[temp6 + 2] = b;
				column[temp6 + 3] = r;
				column[temp6 + 4] = g;
				column[temp6 + 5] = b;

				int bl = pSim->GetDensityIso(idx) >= rho0;
				int tl = pSim->GetDensityIso(idx + 1) >= rho0;
				int br = pSim->GetDensityIso(idx + 2) >= rho0;
				int tr = pSim->GetDensityIso(idx + 3) >= rho0;

				int config = bl | (br << 1) | (tl << 2) | (tr << 3);

				if (config > 7)
					config = 15 - config;

				float px = wn + (double)i * wn;
				float py = hn + (double)j * hn;


				switch (config)
				{
				case 0:
					break;

				case 1:
					position[temp4 + 0] = px;
					position[temp4 + 1] = py;
					position[temp4 + 2] = px + wn / 2;
					position[temp4 + 3] = py + hn / 2;
					break;

				case 2:
					position[temp4 + 0] = px + wn / 2;
					position[temp4 + 1] = py + hn / 2;
					position[temp4 + 2] = px;
					position[temp4 + 3] = py - hn / 2;
					break;

				case 3:
					position[temp4 + 0] = px + wn;
					position[temp4 + 1] = py;
					position[temp4 + 2] = px + wn / 2;
					position[temp4 + 3] = py + hn / 2;
					break;

				case 4:
					position[temp4 + 0] = px;
					position[temp4 + 1] = py + hn / 2;
					position[temp4 + 2] = px + wn;
					position[temp4 + 3] = py + hn;
					break;

				case 5:
				case 6:
				case 7:
					position[temp4 + 0] = px + wn;
					position[temp4 + 1] = py + hn / 2;
					position[temp4 + 2] = px + wn / 2;
					position[temp4 + 3] = py + hn;
					break;
				}

				position[temp4 + 0] = position[temp4 + 0] * coeff_x - 1.0f;
				position[temp4 + 1] = position[temp4 + 1] * coeff_y - 1.0f;
				position[temp4 + 2] = position[temp4 + 2] * coeff_x - 1.0f;
				position[temp4 + 3] = position[temp4 + 3] * coeff_y - 1.0f;
			}
		}

		glBindVertexArray(sim_vao_izo);  GL_CHECK_ERRORS;
		glBindBuffer(GL_ARRAY_BUFFER, sim_vbo_izo);  GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, m_grid_dim * m_grid_dim * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); GL_CHECK_ERRORS;
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_grid_dim * m_grid_dim * 4 * sizeof(GLfloat), position);   GL_CHECK_ERRORS;
		glBindBuffer(GL_ARRAY_BUFFER, vbo_izo_col);   GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, m_grid_dim * m_grid_dim * 6 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); GL_CHECK_ERRORS;
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_grid_dim * m_grid_dim * 6 * sizeof(GLfloat), column);  GL_CHECK_ERRORS;
		glDrawArrays(GL_LINES, 0, m_grid_dim * m_grid_dim * 2);

		glBindVertexArray(0);
	}
}

void Visualizer::Draw()
{
	int numIndices = 2 * m_grid_dim * (m_grid_dim - 1) + m_grid_dim - 1;
	int primRestart = m_grid_dim * m_grid_dim;

	if (draw_smoke)
	{
		glBindVertexArray(sim_vao_smoke); GL_CHECK_ERRORS;
		glEnable(GL_PRIMITIVE_RESTART); GL_CHECK_ERRORS;
		glPrimitiveRestartIndex(primRestart); GL_CHECK_ERRORS;
		glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, nullptr); GL_CHECK_ERRORS;
		glDisable(GL_PRIMITIVE_RESTART); GL_CHECK_ERRORS;
		glBindVertexArray(0);
	}

	if (draw_vecs)
	{
		glBindVertexArray(sim_vao_vec); GL_CHECK_ERRORS;
		if (glyph_type == POINTS) {
			glDrawArrays(GL_POINTS, 0, m_grid_dim * m_grid_dim * 2); GL_CHECK_ERRORS;
		}
		else if (glyph_type == LINE_LOOP) {
			glDrawArrays(GL_LINE_LOOP, 0, m_grid_dim * m_grid_dim * 2); GL_CHECK_ERRORS;
		}
		else if (glyph_type == TRIANGLES) {
			glDrawArrays(GL_TRIANGLES, 0, m_grid_dim * m_grid_dim * 2); GL_CHECK_ERRORS;
		}
		else if (glyph_type == PATCHES) {
			glDrawArrays(GL_PATCHES, 0, m_grid_dim * m_grid_dim * 2); GL_CHECK_ERRORS;
		}
		glBindVertexArray(0);
	}

	const int DIM = 128;
	glBindVertexArray(sim_vao_izo);  GL_CHECK_ERRORS;
	glDrawArrays(GL_LINES, 0, DIM * DIM * 2);

	glBindVertexArray(0);
}

