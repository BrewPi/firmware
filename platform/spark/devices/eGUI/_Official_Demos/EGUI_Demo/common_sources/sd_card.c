/*************************************************************************
*
* $FileName: sd_card.c$
* $Version : 3.8.2.0$
* $Date    : Jan-14-2014$
*
* Comments: Provide MFS file system on external SD card.
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <fio.h>
#include <mfs.h>
#include <shell.h>
#include <sdcard.h>
#include <sdcard_spi.h>
#include <spi.h>
#include <part_mgr.h>



#if ! SHELLCFG_USES_MFS
  #error This application requires SHELLCFG_USES_MFS defined non-zero in user_config.h. Please recompile libraries with this option.
#endif

#if defined BSP_SDCARD_ESDHC_CHANNEL
  #if ! BSPCFG_ENABLE_ESDHC
    #error This application requires BSPCFG_ENABLE_ESDHC defined non-zero in user_config.h. Please recompile libraries with this option.
  #endif
#endif

MQX_FILE_PTR filesystem_handle;
static MQX_FILE_PTR com_handle, sdcard_handle, partman_handle;
static char         filesystem_name[] = "A:";
static char         partman_name[] = "PM:";

const SHELL_COMMAND_STRUCT Shell_commands[] = {
    { "cd",        Shell_cd },
    { "copy",      Shell_copy },
    { "create",    Shell_create },
    { "del",       Shell_del },
    { "disect",    Shell_disect},
    { "dir",       Shell_dir },
    { "df",        Shell_df },
    { "exit",      Shell_exit },
    { "format",    Shell_format },
    { "help",      Shell_help },
    { "mkdir",     Shell_mkdir },
    { "pwd",       Shell_pwd },
    { "read",      Shell_read },
    { "ren",       Shell_rename },
    { "rmdir",     Shell_rmdir },
    { "sh",        Shell_sh },
    { "type",      Shell_type },
    { "write",     Shell_write },
    { "?",         Shell_command_list },
    { NULL,        NULL }
};


/*TASK*-----------------------------------------------------------------
*
* Function Name  : shell_task
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/
void shell_task(uint32_t temp)
{
    /* Run the shell on the serial port */
    printf("Shell Task started\n");
    for(;;)
    {
        Shell(Shell_commands, NULL);
        printf("Shell exited, restarting...\n");
    }
}

/*TASK*-----------------------------------------------------------------
*
* Function Name  : sdcard_task
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/
uint32_t InitSdCard(void)
{
    _mqx_int     error_code;
    _mqx_uint    param;

    printf("SdCard Init started\n");
    
    /* Open low level communication device */
    com_handle = fopen (BSP_SDCARD_ESDHC_CHANNEL, NULL);

    if (NULL == com_handle)
    {
        printf("Error installing communication handle.\n");
        _task_block();
    }

    /* Install SD card device */
    error_code = _io_sdcard_install("sdcard:", (void *)&_bsp_sdcard0_init, com_handle);
    if ( error_code != MQX_OK )
    {
        printf("Error installing SD card device (0x%x)\n", error_code);
        return 0;
    }

    _time_delay (200);
    /* Open the device which MFS will be installed on */
    sdcard_handle = fopen("sdcard:", 0);
    if ( sdcard_handle == NULL )
    {
        printf("Unable to open SD card device.\n");
        return 0;
    }

    /* Set read only flag as needed */
    param = 0;

    if (IO_OK != ioctl(sdcard_handle, IO_IOCTL_SET_FLAGS, (char *) &param))
    {
        printf("Setting device read only failed.\n");
        return 0;
    }

    /* Install partition manager over SD card driver */
    error_code = _io_part_mgr_install(sdcard_handle, partman_name, 0);
    if (error_code != MFS_NO_ERROR)
    {
        printf("Error installing partition manager: %s\n", MFS_Error_text((uint32_t)error_code));
        return 0;
    }

    /* Open partition manager */
    partman_handle = fopen(partman_name, NULL);
    if (partman_handle == NULL)
    {
        error_code = ferror(partman_handle);
        printf("Error opening partition manager: %s\n", MFS_Error_text((uint32_t)error_code));
        return 0;
    }

    /* Validate partition 1 */
    param = 1;
    error_code = _io_ioctl(partman_handle, IO_IOCTL_VAL_PART, &param);
    if (error_code == MQX_OK)
    {

        /* Install MFS over partition 1 */
        error_code = _io_mfs_install(partman_handle, filesystem_name, param);
        if (error_code != MFS_NO_ERROR)
        {
            printf("Error initializing MFS over partition: %s\n", MFS_Error_text((uint32_t)error_code));
            return 0;
        }

    } else {

        /* Install MFS over SD card driver */
        error_code = _io_mfs_install(sdcard_handle, filesystem_name, (_file_size)0);
        if (error_code != MFS_NO_ERROR)
        {
            printf("Error initializing MFS: %s\n", MFS_Error_text((uint32_t)error_code));
            return 0;
        }

    }

    /* Open file system */
    filesystem_handle = fopen(filesystem_name, NULL);
    error_code = ferror (filesystem_handle);
    if ((error_code != MFS_NO_ERROR) && (error_code != MFS_NOT_A_DOS_DISK))
    {
        printf("Error opening filesystem: %s\n", MFS_Error_text((uint32_t)error_code));
        return 0;
    }
    if ( error_code == MFS_NOT_A_DOS_DISK )
    {
        printf("NOT A DOS DISK! You must format to continue.\n");
    }

    printf ("SD card installed to %s\n", filesystem_name);
    
    return 1;
}

/* EOF */
