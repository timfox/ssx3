#ifndef PLATFORM_HOST_OBJECT_H
#define PLATFORM_HOST_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HostWorldMarker {
    float px;
    float py;
    float pz;
    float extent;
    int section;
    int type_tag;
} HostWorldMarker;

void host_object_interface_init(void);
void host_instance_man_on_track_load(int track_id);
void host_instance_man_on_section_activate(int section_id, int map_id);
int host_instance_man_active_count(void);
int host_instance_man_get_markers(HostWorldMarker* out, int max_out, int map_id);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_HOST_OBJECT_H */
