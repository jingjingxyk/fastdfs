/**
* Copyright (C) 2008 Happy Fish / YuQing
*
* FastDFS may be copied only under the terms of the GNU General
* Public License V3, which may be found in the FastDFS source kit.
* Please visit the FastDFS Home Page http://www.fastken.com/ for more detail.
**/

//storage_service.h

#ifndef _STORAGE_SERVICE_H_
#define _STORAGE_SERVICE_H_

#define STORAGE_CREATE_FLAG_NONE  0
#define STORAGE_CREATE_FLAG_FILE  1
#define STORAGE_CREATE_FLAG_LINK  2

#define STORAGE_DELETE_FLAG_NONE  0
#define STORAGE_DELETE_FLAG_FILE  1
#define STORAGE_DELETE_FLAG_LINK  2

#include "fastcommon/logger.h"
#include "fastcommon/fast_task_queue.h"
#include "sf/sf_service.h"
#include "fdfs_define.h"
#include "storage_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int storage_service_init();
void storage_service_destroy();

int fdfs_stat_file_sync_func(void *args);

int storage_get_storage_path_index(int *store_path_index);

void storage_get_store_path(const char *filename, const int filename_len,
		int *sub_path_high, int *sub_path_low);

static inline void storage_clear_task(struct fast_task_info *pTask)
{
    StorageClientInfo *pClientInfo;

    pClientInfo = (StorageClientInfo *)pTask->arg;
    if (pClientInfo->clean_func != NULL)
    {
        pClientInfo->clean_func(pTask);
    }
    memset(pTask->arg, 0, sizeof(StorageClientInfo));
}

static inline void storage_release_task(struct fast_task_info *pTask)
{
    if (__sync_sub_and_fetch(&pTask->reffer_count, 0) == 1)
    {
        storage_clear_task(pTask);
    }
    sf_release_task(pTask);
}

#ifdef __cplusplus
}
#endif

#endif
