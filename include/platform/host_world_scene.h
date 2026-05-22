#ifndef PLATFORM_HOST_WORLD_SCENE_H
#define PLATFORM_HOST_WORLD_SCENE_H

#include <cstdint>
#include <vector>

namespace host {

struct Mat4 {
    float m[16] = {};
};

struct WorldVertex {
    float px = 0.0f;
    float py = 0.0f;
    float pz = 0.0f;
    float nx = 0.0f;
    float ny = 1.0f;
    float nz = 0.0f;
    float r = 0.5f;
    float g = 0.5f;
    float b = 0.5f;
};

struct WorldMesh {
    std::vector<WorldVertex> vertices;
    std::vector<std::uint32_t> indices;
};

struct WorldCamera {
    float yaw = 0.6f;
    float pitch = 0.35f;
    float distance = 180.0f;
    float focus_x = 0.0f;
    float focus_y = 40.0f;
    float focus_z = 0.0f;
};

struct WorldScene {
    WorldMesh terrain;
    WorldMesh object_markers;
    WorldCamera camera;
};

Mat4 mat4_identity();
Mat4 mat4_perspective(float fovy_rad, float aspect, float near_z, float far_z);
Mat4 mat4_look_at(float eye_x,
                  float eye_y,
                  float eye_z,
                  float center_x,
                  float center_y,
                  float center_z,
                  float up_x,
                  float up_y,
                  float up_z);
Mat4 mat4_mul(const Mat4& a, const Mat4& b);

WorldScene build_default_world_scene();
WorldScene build_world_scene_for_map(int map_id);
void append_instance_object_markers(WorldScene& scene, int map_id);
void update_world_camera_from_pad(WorldCamera& camera, float dt_seconds);

Mat4 world_camera_mvp(const WorldCamera& camera, float aspect);

} // namespace host

#endif /* PLATFORM_HOST_WORLD_SCENE_H */
