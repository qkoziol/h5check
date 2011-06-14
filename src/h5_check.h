#ifndef H5_CHECK_H__
#define H5_CHECK_H__

#include "h5chk_config.h"
#include "h5check_api.h"
#include "h5check_public.h"

/*
 * Include ANSI-C header files.
 */
#ifdef STDC_HEADERS
#   include <assert.h>
#   include <ctype.h>
#   include <errno.h>
#   include <fcntl.h>
#   include <float.h>
#   include <limits.h>
#   include <math.h>
#   include <signal.h>
#   include <stdarg.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#ifndef __cplusplus
# ifdef HAVE_STDINT_H
#   include <stdint.h>      /*for C9x types                  */
# endif
#else
# ifdef HAVE_STDINT_H_CXX
#   include <stdint.h>      /*for C9x types when include from C++        */
# endif
#endif

#ifdef H5_HAVE_STDDEF_H
#   include <stddef.h>
#endif

/*
 * The `struct stat' data type for stat() and fstat(). This is a Posix file
 * but often apears on non-Posix systems also.  The `struct stat' is required
 * for hdf5 to compile, although only a few fields are actually used.
 */
#ifdef HAVE_SYS_STAT_H
#   include <sys/stat.h>
#endif

/*
 * If a program may include both `time.h' and `sys/time.h' then
 * TIME_WITH_SYS_TIME is defined (see AC_HEADER_TIME in configure.in).
 * On some older systems, `sys/time.h' includes `time.h' but `time.h' is not
 * protected against multiple inclusion, so programs should not explicitly
 * include both files. This macro is useful in programs that use, for example,
 * `struct timeval' or `struct timezone' as well as `struct tm'.  It is best
 * used in conjunction with `HAVE_SYS_TIME_H', whose existence is checked
 * by `AC_CHECK_HEADERS(sys/time.h)' in configure.in.
 */
#if defined(TIME_WITH_SYS_TIME)
#   include <sys/time.h>
#   include <time.h>
#elif defined(HAVE_SYS_TIME_H)
#   include <sys/time.h>
#else
#   include <time.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN     /*Exclude rarely-used stuff from Windows headers */

#ifdef HAVE_WINSOCK_H
#include <winsock2.h>
#endif

#include <windows.h>
#include <direct.h>         /* For _getcwd() */

#endif /*_WIN32*/

#include "h5_check_defines.h"

/* command line option */
H5CHKDLLVAR int opt_ind;
H5CHKDLLVAR const char *opt_arg;     

H5CHKDLLVAR int          g_verbose_num;
H5CHKDLLVAR int          g_format_num;
H5CHKDLLVAR ck_addr_t    g_obj_addr;
H5CHKDLLVAR ck_bool_t    g_follow_ext;

H5CHKDLLVAR table_t     *g_ext_tbl;

/* entering via h5checker_obj() API */
H5CHKDLLVAR int          g_obj_api;
H5CHKDLLVAR int          g_obj_api_err;

H5CHKDLLVAR const obj_class_t *const message_type_g[MSG_TYPES];
H5CHKDLLVAR const B2_class_t HF_BT2_INDIR[1];
H5CHKDLLVAR const B2_class_t HF_BT2_FILT_INDIR[1];
H5CHKDLLVAR const B2_class_t HF_BT2_DIR[1];
H5CHKDLLVAR const B2_class_t HF_BT2_FILT_DIR[1];
H5CHKDLLVAR const B2_class_t G_BT2_CORDER[1];
H5CHKDLLVAR const B2_class_t G_BT2_NAME[1];
H5CHKDLLVAR const B2_class_t SM_INDEX[1];
H5CHKDLLVAR const B2_class_t A_BT2_NAME[1];
H5CHKDLLVAR const B2_class_t A_BT2_CORDER[1];

#ifdef __cplusplus
extern "C" {
#endif

/* virtual file drivers */
H5CHKDLL driver_t   *FD_open(const char *, global_shared_t *, int);
H5CHKDLL ck_err_t    FD_close(driver_t *);
H5CHKDLL ck_addr_t   FD_get_eof(driver_t *);
H5CHKDLL char       *FD_get_fname(driver_t *, ck_addr_t);
H5CHKDLL void        free_driver_fa(global_shared_t *shared);

/* command line option */
H5CHKDLL void        print_version(const char *);
H5CHKDLL void        usage(char *);
H5CHKDLL void        leave(int);

/* for handling names */
H5CHKDLL int         name_list_init(name_list_t **name_list);
H5CHKDLL ck_bool_t   name_list_search(name_list_t *nl, char *name);
H5CHKDLL ck_err_t    name_list_insert(name_list_t *nl, char *name);
H5CHKDLL void        name_list_dest(name_list_t *nl);

/* Validation routines */
H5CHKDLL ck_err_t    check_superblock(driver_t *);
H5CHKDLL ck_err_t    check_obj_header(driver_t *, ck_addr_t, OBJ_t **);

/* entering via h5checker_obj() API */
H5CHKDLL void        process_err(ck_errmsg_t *);

H5CHKDLL ck_err_t    check_fheap(driver_t *, ck_addr_t);
H5CHKDLL ck_err_t    check_SOHM(driver_t *, ck_addr_t, unsigned);
H5CHKDLL ck_err_t    check_btree2(driver_t *, ck_addr_t, const B2_class_t *, ck_op_t ck_op, void *ck_udata);
H5CHKDLL HF_hdr_t   *HF_open(driver_t *, ck_addr_t fh_addr);
H5CHKDLL ck_err_t    HF_close(HF_hdr_t *fhdr);
H5CHKDLL ck_err_t    HF_get_obj_info(driver_t *, HF_hdr_t *, const void *, obj_info_t *);
H5CHKDLL ck_err_t    HF_read(driver_t *, HF_hdr_t *, const void *, void */*out*/, obj_info_t *);
H5CHKDLL ck_err_t    SM_get_fheap_addr(driver_t *, unsigned, ck_addr_t *);
H5CHKDLL unsigned    V_log2_gen(uint64_t);

H5CHKDLL ck_err_t    FD_read(driver_t *, ck_addr_t, size_t, void */*out*/);
H5CHKDLL void        addr_decode(global_shared_t *, const uint8_t **, ck_addr_t *);
H5CHKDLL ck_addr_t   get_logical_addr(const uint8_t *, const uint8_t *, ck_addr_t);
H5CHKDLL int         debug_verbose(void);
H5CHKDLL int         object_api(void);

H5CHKDLL uint32_t    checksum_metadata(const void *, ck_size_t, uint32_t);
H5CHKDLL uint32_t    checksum_lookup3(const void *, ck_size_t, uint32_t);

H5CHKDLL driver_t   *file_init(char *fname);
H5CHKDLL void        free_file_shared(driver_t *thefile);

H5CHKDLL ck_err_t    table_init(table_t **tbl, int type);
H5CHKDLL ck_err_t    table_insert(table_t *tbl, void *_id, int type);
H5CHKDLL void        table_free(table_t *tbl);

H5CHKDLL int         get_option(int argc, const char **argv, const char *opts, const struct long_options *l_opts) ;


#ifdef __cplusplus
}
#endif

#endif  /* !H5_CHECK_H__ */

