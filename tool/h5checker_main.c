#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "h5_check.h"
#include "h5_error.h"


int main(int argc, char **argv)
{
    int		ret;
    ck_addr_t	ss;
    ck_addr_t	gheap_addr;
    FILE 		*inputfd;
    driver_t	*thefile;
    global_shared_t	*shared;


    /* command line declarations */
    int		argno;
    const 	char *s = NULL;
    char	*prog_name;
    char	*fname;
    char	*rest;
	
/* NEED TO delete this later */
ck_addr_t     force_fheap_addr;

    if ((prog_name=strrchr(argv[0], '/'))) 
	prog_name++;
    else 
	prog_name = argv[0];

    g_verbose_num = DEFAULT_VERBOSE;
    g_format_num = DEFAULT_FORMAT;
    g_obj_addr = CK_ADDR_UNDEF;

    for (argno=1; argno<argc && argv[argno][0]=='-'; argno++) {
	if (!strcmp(argv[argno], "--help")) {
	    usage(prog_name);
	    leave(EXIT_SUCCESS);

	} else if (!strcmp(argv[argno], "--version")) {
	    print_version(prog_name);
	    leave(EXIT_SUCCESS);

	/* --verbose=n or --verbose or -vn */
	} else if (!strncmp(argv[argno], "--verbose=", 10)) {
	    g_verbose_num = strtol(argv[argno]+10, NULL, 0);
	    printf("VERBOSE is true:verbose # = %d\n", g_verbose_num);
	} else if (!strncmp(argv[argno], "--verbose", 9)) {
	    g_verbose_num = DEFAULT_VERBOSE;
	    printf("VERBOSE is true:no number provided, assume default verbose number.\n");
	} else if (!strncmp(argv[argno], "-v", 2)) {
	    if (argv[argno][2]) {
		g_verbose_num = strtol(argv[argno]+2, NULL, 0);
		printf("VERBOSE is true:verbose # = %d\n", g_verbose_num);
	    } else {
		usage(prog_name);
		leave(EXIT_COMMAND_FAILURE);
	    }

	/* --format=d.d or --format or -fd.d */
	} else if (!strncmp(argv[argno], "--format=", 9)) {
	    g_format_num = strtod(argv[argno]+9, NULL);
	    printf("FORMAT is true:format version = %3.1f\n", g_format_num);
	} else if (!strncmp(argv[argno], "--format", 8)) {
	    g_format_num = DEFAULT_FORMAT;
	    printf("FORMAT is true:no number provided, assume default format version.\n");
	} else if (!strncmp(argv[argno], "-f", 2)) {
	    if (argv[argno][2]) {
		g_format_num = strtod(argv[argno]+2, NULL);
		printf("FORMAT is true:format version = %3.1f\n", g_format_num);
	    } else {
		usage(prog_name);
		leave(EXIT_COMMAND_FAILURE);
	    }

	/* --object=a or --object or -oa */
	} else if (!strncmp(argv[argno], "--object=", 9)) {
	    g_obj_addr = strtoull(argv[argno]+9, NULL, 0);
	    printf("CHECK OBJECT_HEADER is true:object address =%llu\n", g_obj_addr);
	} else if (!strncmp(argv[argno], "--object", 10)) {
	    g_obj_addr = CK_ADDR_UNDEF;
	    printf("CHECK OBJECT_HEADER is true:no address provided, assume default validation\n");
	} else if (!strncmp(argv[argno], "-o", 2)) {
	    if (argv[argno][2]) {
		s = argv[argno]+2;
		g_obj_addr = strtoull(s, NULL, 0);
		printf("CHECK OBJECT_HEADER is true:object address =%llu\n", g_obj_addr);
	    } else {
		usage(prog_name);
		leave(EXIT_COMMAND_FAILURE);
	    }

	} else if (argv[argno][1] != '-') {
	    for (s=argv[argno]+1; *s; s++) {
		switch (*s) {
		    case 'h':  /* --help */
			usage(prog_name);
			leave(EXIT_SUCCESS);
		    case 'V':  /* --version */
			print_version(prog_name);
			leave(EXIT_SUCCESS);
			break;
		    default:
			usage(prog_name);	
			leave(EXIT_COMMAND_FAILURE);
		}  /* end switch */
	    }  /* end for */
	} else {
	    printf("default is true, no option provided...assume default verbose and format\n");
	}
    }

    if (argno >= argc) {
	usage(prog_name);
	leave(EXIT_COMMAND_FAILURE);
    }
    if (g_verbose_num > DEBUG_VERBOSE) {
	printf("Invalid verbose # provided.  Default verbose is assumed.\n");
	g_verbose_num = DEFAULT_VERBOSE;
    }

    if (g_format_num != FORMAT_ONE_SIX && g_format_num != FORMAT_ONE_EIGHT) {
	printf("Invalid library version provided.  Default library version is assumed.\n");
	g_format_num = DEFAULT_FORMAT;
    }

    g_obj_api = FALSE;
    shared = calloc(1, sizeof(global_shared_t));
    ret = SUCCEED;
    fname = strdup(argv[argno]);

    printf("\nVALIDATING %s according to library version %3.1f", fname, g_format_num);

    if (g_obj_addr != CK_ADDR_UNDEF)
	printf("at object header address %llu", g_obj_addr);
    printf("\n\n");

	
    /* Initially, use the SEC2 driver by default */
    thefile = FD_open(fname, shared, SEC2_DRIVER);
#if 0
	printf("Using default file driver...\n");
#endif
    if (thefile == NULL) {
	error_push(ERR_FILE, ERR_NONE_SEC, 
	    "Failure in opening input file using the default driver. Validation discontinued.", -1, NULL);
	error_print(stderr, thefile);
	error_clear();
	goto done;
    }

    ret = check_superblock(thefile);
    /* superblock validation has to be all passed before proceeding further */
    if (ret != SUCCEED) {
	error_push(ERR_LEV_0, ERR_LEV_0A, 
	    "Errors found when checking superblock. Validation stopped.", -1, NULL);
	error_print(stderr, thefile);
	error_clear();
	goto done;
    }

    /* not using the default driver */
    if (thefile->shared->driverid != SEC2_DRIVER) {
	/* still have and need info in shared */
	ret = FD_close(thefile);
	if (ret != SUCCEED) {
	    error_push(ERR_FILE, ERR_NONE_SEC, 
		"Errors in closing input file using the default driver", -1, NULL);
	    error_print(stderr, thefile);
	    error_clear();
	}
	printf("Switching to new file driver...\n");
	thefile = FD_open(fname, shared, shared->driverid);
	if (thefile == NULL) {
	    error_push(ERR_FILE, ERR_NONE_SEC, "Errors in opening input file. Validation stopped.", -1, NULL);
	    error_print(stderr, thefile);
	    error_clear();
	    goto done;
        }
    }

    ss = FD_get_eof(thefile);
    if ((ss==CK_ADDR_UNDEF) || (ss<shared->stored_eoa)) {
	error_push(ERR_FILE, ERR_NONE_SEC, 
	    "Invalid file size or file size less than superblock eoa. Validation stopped.", 
	    -1, NULL);
	error_print(stderr, thefile);
	error_clear();
	goto done;
    }

    if ((g_obj_addr != CK_ADDR_UNDEF) && (g_obj_addr >= shared->stored_eoa)) {
	error_push(ERR_FILE, ERR_NONE_SEC, 
	    "Invalid Object header address provided. Validation stopped.", 
	    -1, NULL);
	error_print(stderr, thefile);
	error_clear();
	goto done;
    }

    ret = table_init(&obj_table);
    if (ret != SUCCEED) {
	error_push(ERR_INTERNAL, ERR_NONE_SEC, "Errors in initializing hard link table", -1, NULL);
	error_print(stderr, thefile);
	error_clear();
	ret = SUCCEED;
    }
 
    ret = pline_init_interface();
    if (ret != SUCCEED) {
	error_push(ERR_LEV_0, ERR_NONE_SEC, "Problems in initializing filters...later validation may be affected", 
	    -1, NULL);
	error_print(stderr, thefile);
	error_clear();
	ret = SUCCEED;
    }

    if (g_obj_addr != CK_ADDR_UNDEF)
	ret = check_obj_header(thefile, g_obj_addr, NULL);
    else 
	ret = check_obj_header(thefile, shared->root_grp->header, NULL);
    if (ret != SUCCEED) {
	error_push(ERR_LEV_0, ERR_NONE_SEC, "Errors found when checking the object header", 
	    g_obj_addr==CK_ADDR_UNDEF?shared->root_grp->header:g_obj_addr, NULL);
	error_print(stderr, thefile);
	error_clear();
	ret = SUCCEED;
    }

done:
    if (thefile != NULL) {
	ret = FD_close(thefile);
	if (ret != SUCCEED) {
	    error_push(ERR_FILE, ERR_NONE_SEC, "Errors in closing input file", -1, NULL);
	    error_print(stderr, thefile);
	    error_clear();
	}
    }
	
    if (shared) free(shared);
    if (found_error()){
	printf("Non-compliance errors found\n");
	leave(EXIT_FORMAT_FAILURE);
    } else {
	printf("No non-compliance errors found\n");
	leave(EXIT_SUCCESS);
    }
}
