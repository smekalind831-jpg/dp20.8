#ifndef CAN_MEMORY_ACCESS_COMMON_H
#define CAN_MEMORY_ACCESS_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#define MAC_ERASE		0
#define MAC_READ		1
#define MAC_WRITE		2
#define MAC_STATUS_REQUEST	3
#define MAC_OPERATION_COMPLETED	4
#define MAC_OPERATION_FAILED	5
#define MAC_BOOTLOAD		6
#define MAC_EDCP_GENERATION	7

#define MAS_PROCEED		0
#define MAS_BUSY		1
#define MAS_OPERATION_COMPLETED	4
#define MAS_OPERATION_FAILED	5

//custom error codes
#define MAE_NOT_SUPPORTED_COMMAND	0
#define MAE_INVALID_VALUE		1

#define MAT_STATUS_OK				0
#define MAT_STATUS_NOT_SUPPORTED_COMMAND	1
#define MAT_STATUS_NACK				2
#define MAT_STATUS_TIMEOUT			3
#define MAT_STATUS_MALFORMED			4
#define MAT_STATUS_REFLASH_ERROR		5
#define MAT_STATUS_INVALID_VALUE		6
#define MAT_STATUS_GETDATA_ERROR		7
#define MAT_STATUS_NO_BOOTLOAD_DATA		8	// нет данных прошивки (обрыв данных файла)

#ifdef __cplusplus
}
#endif

#endif /*CAN_MEMORY_ACCESS_COMMON_H*/
