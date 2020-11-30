#ifndef __DL_METHOD_H__
#define __DL_METHOD_H__


typedef struct __dl_callback
{
	void * pApp;
	void (* SetProgress)(void*, int);
	void (* ShowInfo)(void*, const char*);
}DL_CB;

typedef struct __dl_data
{
	int addr;
	int len;
	unsigned char* data;
}DL_DATA;

/* flags for enumerate and open */
#define DLF_TYPE_SCSI            0x02  /* read and write sector */
#define DLF_TYPE_SCSI_FILE       0x03  /* read and write file */
#define DLF_TYPE_MASK            0x0F
/* only for device type is DLF_TYPE_SCSI */
#define DLF_DRIVE_REMOVABLE      0x02000000 

/* flags for ctrl */

#define DLF_CTL_DWONLOAD       2   /* first write sram then program to eflash */
#define DLF_CTL_DISBOOT        4
#define DLF_CTL_REBOOT         5   /* COS or SECUBOOT to BOOTLOADER */

#define DLF_CTL_HASH           8
#define DLF_CTL_FREE           9
#define DLF_CTL_VERSION        10
#define DLF_CTL_HIGH_SCAN      11

#define DLF_CTL_CHECK_SD_MPFLAG 15 /* Only for SD, User Tool for high scan */


typedef struct __disboot_data
{
	unsigned char* addr_len;     /* big or little endian is depended on the disboot bin */
	int len;
}DISBOOT_DATA;

typedef struct __scan_data
{
	int step;
	int reserved_cap;
	int bUser;
}SCAN_DATA;


typedef struct __dl_method
{
	int (* enumerate)   (char** name, int flags, int vpid);
	int (* open)   (const char* name, int flags, void** ctx);
	int (* ctrl)   (void* ctx, int flags, void* param, DL_CB* cb);
	int (* close)  (void* ctx);
}DL_METHOD;


#ifdef DL_METH_EXPORTS
#	define DL_EXPORT __declspec(dllexport)
#else
#	define DL_EXPORT __declspec(dllimport)
#endif

//SD and rom
DL_EXPORT DL_METHOD*  __cdecl DL_Method();
typedef DL_EXPORT DL_METHOD* (__cdecl * FDL_Method)();


#define FDL_MODE_ROM     0x00000000
#define FDL_MODE_NOR     0x00000001
#define FDL_MODE_MASK    FDL_MODE_NOR
#define FDL_TYPE_SD      0x00000000
#define FDL_TYPE_USB     0x01000000 

#define FDL_TYPE_MASK    0xFF000000 

/* if flags == 0 DL_MethodEx is same as DL_Method, it is for sd and rom mode,  
 * else DL_MethodEx functions are define by flags.
 */
DL_EXPORT DL_METHOD*  __cdecl DL_MethodEx(int flags);
typedef DL_EXPORT DL_METHOD* (__cdecl * FDL_MethodEx)(int);

/* return value of all functions.
 * DR_OK for SUCCESS.
 * >0 GetLastError() or errno return by system, some system errors <0 too.
 * other errors defined flowing 
 */

//#define DR_OK              0
//#define DR_E_BASE         (-1000) /* 0xFFFFFC18 */
//
//#define DR_MEMORY         (DR_E_BASE - 1)  /* 0xFFFFFC17 */
//#define DR_NOT_SUPPORT    (DR_E_BASE - 2)
//#define DR_INVALID_HANDLE (DR_E_BASE - 3)
//#define DR_INVALID_PARAM  (DR_E_BASE - 4)
//#define DR_NO_DEVICE      (DR_E_BASE - 5)

/* SCSI bus state error defined like : SCSI_STATE_BASE + STATE 
 */
// #define SCSI_STATE_BASE   DR_E_BASE

// SCSI bus status codes.
//#define SCSISTAT_GOOD                  0x00
//#define SCSISTAT_CHECK_CONDITION       0x02   /* 0xFFFFFC1A */
//#define SCSISTAT_CONDITION_MET         0x04
//#define SCSISTAT_BUSY                  0x08
//#define SCSISTAT_INTERMEDIATE          0x10
//#define SCSISTAT_INTERMEDIATE_COND_MET 0x14
//#define SCSISTAT_RESERVATION_CONFLICT  0x18
//#define SCSISTAT_COMMAND_TERMINATED    0x22
//#define SCSISTAT_QUEUE_FULL            0x28


#endif