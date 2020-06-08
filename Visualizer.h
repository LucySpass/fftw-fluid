#ifndef MAIN_VISUALIZER_H
#define MAIN_VISUALIZER_H
#include "common.h"
#include "Simulator.h"
#include <memory>
#include <vector>

struct Visualizer
{
    enum Colormaps {
        WHITE   = 0,
        RAINBOW = 1,
        BANDED_RAINBOW = 2
    };

    static constexpr uint32_t n_dir_colormaps = 2;
    static constexpr uint32_t n_scalar_colormaps = 3;

    void ToggleDrawGlyphs() { draw_vecs = !draw_vecs;}
    void ToggleDrawSmoke() { draw_smoke = !draw_smoke;}
    void NextDirectionColormap() { colormap_dir = (colormap_dir + 1) % n_dir_colormaps;}
    void NextScalarColormap()    { colormap_scalar = (colormap_scalar + 1) % n_scalar_colormaps;}
    void MultiplyGlyphLength(double mul) { vec_scale *= mul; }

    //Initialize and bind empty buffers; generate mesh grid 
    void Init(int n, uint32_t width, uint32_t height);

    //update buffers with values from Simulator
    void UpdateBuffers(const std::shared_ptr<Simulator> &pSim);
    void Draw();

private:
    //Colormap selection func
    void Colormap(float val, float &R, float &G, float &B);

    //Rainbow colormap for vectors
    static void Direction2DToColor(float x, float y, int method, float& r, float& g, float& b);

    float vec_scale = 2000;
    bool  draw_smoke = false;
    bool  draw_vecs = true;

    uint32_t colormap_dir    = WHITE;
    uint32_t colormap_scalar = WHITE;

    int m_grid_dim;
    uint32_t m_width;
    uint32_t m_height;

    std::vector<float> vec_pos;
    std::vector<float> smoke_pos;
    std::vector<float> vec_col;
    std::vector<float> smoke_col;
    std::vector<uint32_t> indices;

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