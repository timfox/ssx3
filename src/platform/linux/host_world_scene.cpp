#include "platform/host_world_scene.h"

#include "platform/host_object.h"
#include "platform/host_pad.h"

#include <cstdint>
#include <cmath>

namespace host {
namespace {

constexpr float kPi = 3.14159265f;

float clampf(float v, float lo, float hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

float terrain_height(float x, float z, int map_id) {
    const float phase = static_cast<float>(map_id) * 0.41f;
    const float ridge = 18.0f * std::sin(x * 0.018f + phase) * std::cos(z * 0.014f - phase * 0.5f) +
                        12.0f * std::sin((x + z) * 0.009f + phase * 0.25f);
    const float slope = -0.035f * z + 35.0f + static_cast<float>(map_id) * 2.0f;
    return ridge + slope;
}

} // namespace

Mat4 mat4_identity() {
    Mat4 out{};
    out.m[0] = out.m[5] = out.m[10] = out.m[15] = 1.0f;
    return out;
}

Mat4 mat4_perspective(float fovy_rad, float aspect, float near_z, float far_z) {
    const float f = 1.0f / std::tan(fovy_rad * 0.5f);
    Mat4 out{};
    out.m[0] = f / aspect;
    out.m[5] = f;
    out.m[10] = far_z / (near_z - far_z);
    out.m[11] = -1.0f;
    out.m[14] = (near_z * far_z) / (near_z - far_z);
    return out;
}

Mat4 mat4_look_at(float eye_x,
                  float eye_y,
                  float eye_z,
                  float center_x,
                  float center_y,
                  float center_z,
                  float up_x,
                  float up_y,
                  float up_z) {
    float fx = center_x - eye_x;
    float fy = center_y - eye_y;
    float fz = center_z - eye_z;
    const float flen = std::sqrt(fx * fx + fy * fy + fz * fz);
    if (flen > 1e-6f) {
        fx /= flen;
        fy /= flen;
        fz /= flen;
    }

    float sx = fy * up_z - fz * up_y;
    float sy = fz * up_x - fx * up_z;
    float sz = fx * up_y - fy * up_x;
    const float slen = std::sqrt(sx * sx + sy * sy + sz * sz);
    if (slen > 1e-6f) {
        sx /= slen;
        sy /= slen;
        sz /= slen;
    }

    const float ux = sy * fz - sz * fy;
    const float uy = sz * fx - sx * fz;
    const float uz = sx * fy - sy * fx;

    Mat4 out = mat4_identity();
    out.m[0] = sx;
    out.m[4] = sy;
    out.m[8] = sz;
    out.m[1] = ux;
    out.m[5] = uy;
    out.m[9] = uz;
    out.m[2] = -fx;
    out.m[6] = -fy;
    out.m[10] = -fz;
    out.m[12] = -(sx * eye_x + sy * eye_y + sz * eye_z);
    out.m[13] = -(ux * eye_x + uy * eye_y + uz * eye_z);
    out.m[14] = fx * eye_x + fy * eye_y + fz * eye_z;
    return out;
}

Mat4 mat4_mul(const Mat4& a, const Mat4& b) {
    Mat4 out{};
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            out.m[col * 4 + row] = sum;
        }
    }
    return out;
}

static void append_cube(WorldMesh& mesh,
                      float cx,
                      float cy,
                      float cz,
                      float half_extent,
                      float r,
                      float g,
                      float b) {
    const std::uint32_t base = static_cast<std::uint32_t>(mesh.vertices.size());
    const float h = half_extent;
    const float positions[8][3] = {
        {-h, -h, -h}, {h, -h, -h}, {h, h, -h}, {-h, h, -h},
        {-h, -h, h},  {h, -h, h},  {h, h, h},  {-h, h, h},
    };
    for (int i = 0; i < 8; i++) {
        WorldVertex vtx{};
        vtx.px = cx + positions[i][0];
        vtx.py = cy + positions[i][1];
        vtx.pz = cz + positions[i][2];
        vtx.nx = 0.0f;
        vtx.ny = 1.0f;
        vtx.nz = 0.0f;
        vtx.r = r;
        vtx.g = g;
        vtx.b = b;
        mesh.vertices.push_back(vtx);
    }
    const std::uint32_t faces[12][3] = {
        {0, 1, 2}, {0, 2, 3}, {4, 6, 5}, {4, 7, 6}, {0, 4, 5}, {0, 5, 1},
        {2, 6, 7}, {2, 7, 3}, {0, 3, 7}, {0, 7, 4}, {1, 5, 6}, {1, 6, 2},
    };
    for (const auto& face : faces) {
        mesh.indices.push_back(base + face[0]);
        mesh.indices.push_back(base + face[1]);
        mesh.indices.push_back(base + face[2]);
    }
}

void append_instance_object_markers(WorldScene& scene, int map_id) {
    scene.object_markers.vertices.clear();
    scene.object_markers.indices.clear();

    HostWorldMarker markers[24];
    const int count = host_instance_man_get_markers(markers, 24, map_id);
    for (int i = 0; i < count; i++) {
        const HostWorldMarker& marker = markers[i];
        const float size = marker.extent * 2.5f;
        const float hue = static_cast<float>((marker.type_tag & 7)) / 7.0f;
        append_cube(scene.object_markers,
                  marker.px,
                  marker.py,
                  marker.pz,
                  size,
                  0.85f + 0.1f * hue,
                  0.35f + 0.2f * (1.0f - hue),
                  0.15f);
    }
}

WorldScene build_world_scene_for_map(int map_id) {
    WorldScene scene{};
    constexpr int kGrid = 96;
    constexpr float kCell = 4.0f;
    const float origin = -static_cast<float>(kGrid) * kCell * 0.5f;

    scene.terrain.vertices.reserve(static_cast<std::size_t>((kGrid + 1) * (kGrid + 1)));
    for (int z = 0; z <= kGrid; ++z) {
        for (int x = 0; x <= kGrid; ++x) {
            const float wx = origin + static_cast<float>(x) * kCell;
            const float wz = origin + static_cast<float>(z) * kCell;
            const float wy = terrain_height(wx, wz, map_id);

            const float h_xp = terrain_height(wx + kCell, wz, map_id);
            const float h_xm = terrain_height(wx - kCell, wz, map_id);
            const float h_zp = terrain_height(wx, wz + kCell, map_id);
            const float h_zm = terrain_height(wx, wz - kCell, map_id);
            float nx = h_xm - h_xp;
            float nz = h_zm - h_zp;
            const float ny = 2.0f * kCell;
            const float nlen = std::sqrt(nx * nx + ny * ny + nz * nz);
            if (nlen > 1e-6f) {
                nx /= nlen;
                nz /= nlen;
            }
            const float ny_n = ny / nlen;

            WorldVertex vtx{};
            vtx.px = wx;
            vtx.py = wy;
            vtx.pz = wz;
            vtx.nx = nx;
            vtx.ny = ny_n;
            vtx.nz = nz;

            const float snow = clampf((wy - 20.0f) / 55.0f, 0.0f, 1.0f);
            vtx.r = 0.25f + 0.35f * (1.0f - snow);
            vtx.g = 0.45f + 0.25f * (1.0f - snow);
            vtx.b = 0.2f + 0.55f * snow;
            scene.terrain.vertices.push_back(vtx);
        }
    }

    const auto index_at = [kGrid](int x, int z) -> std::uint32_t {
        return static_cast<std::uint32_t>(z * (kGrid + 1) + x);
    };
    for (int z = 0; z < kGrid; ++z) {
        for (int x = 0; x < kGrid; ++x) {
            const std::uint32_t i0 = index_at(x, z);
            const std::uint32_t i1 = index_at(x + 1, z);
            const std::uint32_t i2 = index_at(x, z + 1);
            const std::uint32_t i3 = index_at(x + 1, z + 1);
            scene.terrain.indices.push_back(i0);
            scene.terrain.indices.push_back(i1);
            scene.terrain.indices.push_back(i2);
            scene.terrain.indices.push_back(i1);
            scene.terrain.indices.push_back(i3);
            scene.terrain.indices.push_back(i2);
        }
    }

    scene.camera.focus_y = terrain_height(0.0f, 0.0f, map_id) + 8.0f;
    scene.camera.focus_z = static_cast<float>(map_id) * 12.0f;
    append_instance_object_markers(scene, map_id);
    return scene;
}

WorldScene build_default_world_scene() {
    return build_world_scene_for_map(0);
}

void update_world_camera_from_pad(WorldCamera& camera, float dt_seconds) {
    const PadState& pad = pad_state();
    const float turn = 1.4f * dt_seconds;
    const float zoom = 90.0f * dt_seconds;

    camera.yaw += pad.right_x * turn;
    camera.pitch = clampf(camera.pitch + pad.right_y * turn, 0.08f, 1.35f);

    const float cy = std::cos(camera.yaw);
    const float sy = std::sin(camera.yaw);
    const float cp = std::cos(camera.pitch);

    const float forward_x = -sy * cp;
    const float forward_z = -cy * cp;
    const float right_x = cy;
    const float right_z = -sy;

    const float move = 120.0f * dt_seconds;
    camera.focus_x += (pad.left_x * right_x + pad.left_y * forward_x) * move;
    camera.focus_z += (pad.left_x * right_z + pad.left_y * forward_z) * move;

    if (pad_down(PadButton::R1)) {
        camera.distance = clampf(camera.distance - zoom, 40.0f, 420.0f);
    }
    if (pad_down(PadButton::L1)) {
        camera.distance = clampf(camera.distance + zoom, 40.0f, 420.0f);
    }
    if (pad_pressed(PadButton::R2)) {
        camera.distance = clampf(camera.distance - 25.0f, 40.0f, 420.0f);
    }
    if (pad_pressed(PadButton::L2)) {
        camera.distance = clampf(camera.distance + 25.0f, 40.0f, 420.0f);
    }
}

Mat4 world_camera_mvp(const WorldCamera& camera, float aspect) {
    const float cy = std::cos(camera.yaw);
    const float sy = std::sin(camera.yaw);
    const float cp = std::cos(camera.pitch);
    const float sp = std::sin(camera.pitch);

    const float eye_x = camera.focus_x + sy * cp * camera.distance;
    const float eye_y = camera.focus_y + sp * camera.distance;
    const float eye_z = camera.focus_z + cy * cp * camera.distance;

    const Mat4 proj = mat4_perspective(55.0f * kPi / 180.0f, aspect, 1.0f, 2500.0f);
    const Mat4 view =
        mat4_look_at(eye_x, eye_y, eye_z, camera.focus_x, camera.focus_y, camera.focus_z, 0.0f, 1.0f, 0.0f);
    return mat4_mul(proj, view);
}

} // namespace host
