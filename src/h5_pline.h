/* Current version of the Z_class_t struct */
#define 	Z_CLASS_T_VERS 	(1)

/* Return values for filter callback function */
typedef enum Z_cb_return_t {
    Z_CB_ERROR  = -1,
    Z_CB_FAIL   = 0,    /* I/O should fail if filter fails. */
    Z_CB_CONT   = 1,    /* I/O continues if filter fails.   */
    Z_CB_NO     = 2
} Z_cb_return_t;

typedef 		int 		Z_filter_t;
typedef ck_size_t 	(*Z_func_t)(unsigned int flags, ck_size_t cd_nelmts, const unsigned int cd_values[],
			      ck_size_t nbytes, ck_size_t *buf_size, void **buf);

typedef Z_cb_return_t (*Z_filter_func_t)(Z_filter_t filter, void* buf, ck_size_t buf_size, void* op_data);


typedef struct Z_class_t { 
    int 	version;                /* Version number of the H5Z_class_t struct */
    Z_filter_t 	id;            		/* Filter ID number                          */
    Z_func_t 	filter;          	/* The actual filter function                */
} Z_class_t; 

/* Structure for filter callback property */
typedef struct Z_cb_t {
    Z_filter_func_t 	func;
    void*              	op_data;
} Z_cb_t;

/* Deflate filter */
Z_class_t Z_DEFLATE[1];

/* Shuffle filter */
Z_class_t Z_SHUFFLE[1];

#define Z_SHUFFLE_USER_NPARMS    0       /* Number of parameters that users can set */
#define Z_SHUFFLE_TOTAL_NPARMS   1       /* Total number of parameters for filter */
#define Z_SHUFFLE_PARM_SIZE      0       /* "Local" parameter for shuffling size */


/* Fletcher32 filter */
Z_class_t Z_FLETCHER32[1];

#define FLETCHER_LEN       4

/* szip filter */
Z_class_t Z_SZIP[1];

#define Z_SZIP_PARM_MASK      0       /* "User" parameter for option mask */
#define Z_SZIP_PARM_PPB       1       /* "User" parameter for pixels-per-block */
#define Z_SZIP_PARM_BPP       2       /* "Local" parameter for bits-per-pixel */
#define Z_SZIP_PARM_PPS       3       /* "Local" parameter for pixels-per-scanline */



/* nbit filter */
Z_class_t Z_NBIT[1];

#define Z_NBIT_ATOMIC          1     /* Atomic datatype class: integer/floating-point */
#define Z_NBIT_ARRAY           2     /* Array datatype class */
#define Z_NBIT_COMPOUND        3     /* Compound datatype class */
#define Z_NBIT_NOOPTYPE        4     /* Other datatype class: nbit does no compression */

#define Z_NBIT_ORDER_LE        0     /* Little endian for datatype byte order */
#define Z_NBIT_ORDER_BE        1     /* Big endian for datatype byte order */


/* scaleoffset filter */
Z_class_t Z_SCALEOFFSET[1];

#define Z_SCALEOFFSET_TOTAL_NPARMS     20   /* Total number of parameters for filter */
typedef enum Z_SO_scale_type_t {
    Z_SO_FLOAT_DSCALE = 0,
    Z_SO_FLOAT_ESCALE = 1,
    Z_SO_INT          = 2
} Z_SO_scale_type_t;

/* Values to decide if EDC is enabled for reading data */
typedef enum Z_EDC_t {
    Z_ERROR_EDC       = -1,   /* error value */
    Z_DISABLE_EDC     = 0,
    Z_ENABLE_EDC      = 1,
    Z_NO_EDC          = 2     /* must be the last */
} Z_EDC_t;



#define 	Z_FLAG_INVMASK        	0xff00  /*invocation flag mask          */
#define 	Z_FLAG_REVERSE  	0x0100  /*reverse direction; read       */
#define 	Z_FLAG_SKIP_EDC   	0x0200  /*skip EDC filters for read     */

/* Filter IDs */
#define Z_FILTER_ERROR        (-1)    /*no filter                     */
#define Z_FILTER_NONE         0       /*reserved indefinitely         */
#define Z_FILTER_DEFLATE      1       /*deflation like gzip           */
#define Z_FILTER_SHUFFLE      2       /*shuffle the data              */
#define Z_FILTER_FLETCHER32   3       /*fletcher32 checksum of EDC    */
#define Z_FILTER_SZIP         4       /*szip compression              */
#define Z_FILTER_NBIT         5       /*nbit compression              */
#define Z_FILTER_SCALEOFFSET  6       /*scale+offset compression      */
#define Z_FILTER_RESERVED     256     /*filter ids below this value are reserved for library use */
#define Z_FILTER_MAX          65535   /*maximum filter id             */

#define Z_MAX_NFILTERS        32      /* Maximum number of filters allowed in a pipeline */
                                        /* (should probably be allowed to be an
                                         * unlimited amount, but currently each
                                         * filter uses a bit in a 32-bit field,
                                         * so the format would have to be
                                         * changed to accomodate that) */

