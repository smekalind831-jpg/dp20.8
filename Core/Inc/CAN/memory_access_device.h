#ifndef MEMORY_ACCESS_DEVICE_H
#define MEMORY_ACCESS_DEVICE_H

#include "types.h"
#include "canengine.h"

extern void j1939_memory_access_device_init(void);
extern void j1939_memory_access_device_process(void);

/**return size or -1 if not found 0 value undefined*/
int j1939_memory_access_device_write_get_parameter_size_hook(uint32_t spn);
int j1939_memory_access_device_read_get_parameter_hook(uint32_t spn, uint8_t *outbuffer);


int j1939_memory_access_device_write_is_memory_writable_hook(uint32_t pointer, unsigned size);
/**return NULL for error, pointer for ok*/
const uint8_t *j1939_memory_access_device_read_get_memory_hook(uint32_t pointer, unsigned size);


/**lock reads for parameters*/
void j1939_memory_access_device_parameter_write_start_hook(uint32_t spn_start, unsigned size);
/** return 1 for ok, 0 for fault*/
int j1939_memory_access_device_parameter_write_hook(uint32_t spn, uint8_t *data);
/**commit to nonvolatile memory if neccessary and unlock access*/
void j1939_memory_access_device_parameter_write_complete_hook(uint32_t spn_start, unsigned size);

void j1939_memory_access_device_raw_write_start_hook(uint32_t pointer, unsigned size);
uint8_t *j1939_memory_access_device_write_get_memory_hook(uint32_t pointer, unsigned size);
void j1939_memory_access_device_raw_write_complete_hook(uint32_t pointer, unsigned size);


void j1939_memory_access_device_process_bootload_request_hook(can_port_t port, can_instance_t instance, uint8_t address);

#endif /*MEMORY_ACCESS_DEVICE_H*/
