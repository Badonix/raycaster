#include <vector>
#include <string>
#include <cstdint>

class Texture{
  private:
    std::vector<std::vector<uint32_t>> textures;
    void generate_random_textures();
  public:
    int tex_width = 64;
    int tex_height = 64;
    Texture();
    int load_texture(const std::string& filepath);
    bool load_from_files(const std::vector<std::string>& filepaths);
    int get_count() const;
    const std::vector<uint32_t>& get_texture(int i) const;
    uint32_t get_texture_pixel(int i, int x, int y) const;
};
