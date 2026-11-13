#ifndef CAN_DEFS_H
#define CAN_DEFS_H

/** Internal */
#define QUEUE_POP(head, count, size) do { head = (head + 1) % size; --(count); } while (0)

#define CAN_OS_MUTEX_DECLARE(mutex)
#define CAN_OS_MUTEX_INIT(mutex)	do{;}while(0)
#define CAN_OS_MUTEX_LOCK(mutex)	do{;}while(0)
#define CAN_OS_MUTEX_UNLOCK(mutex)	do{;}while(0)

#endif /*CAN_DEFS_H*/
