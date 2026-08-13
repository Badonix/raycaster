#include <vector>
class Texture{
  private:
    std::vector<uint32_t> texture[8];
    void generate_random_textures();
  public:
    int text_count = 8;
    int tex_width = 64;
    int tex_height = 64;
    Texture();
    const std::vector<uint32_t>& get_texture(int i) const;
    uint32_t get_texture_pixel(int i, int x, int y) const;
};
