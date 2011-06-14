#ifndef H5CHECK_API_H__
#define H5CHECK_API_H__

#if defined(H5CHECK_BUILT_AS_DYNAMIC_LIB)

#if defined(hdf5check_EXPORTS)
  #if defined (_MSC_VER)  /* MSVC Compiler Case */
    #define H5CHKDLL __declspec(dllexport)
    #define H5CHKDLLVAR extern __declspec(dllexport)
  #elif (__GNU__ >= 4)  /* GCC 4.x has support for visibility options */
    #define H5CHKDLL __attribute__ ((visibility("default")))
    #define H5CHKDLLVAR extern __attribute__ ((visibility("default")))
  #endif
#else
  #if defined (_MSC_VER)  /* MSVC Compiler Case */
    #define H5CHKDLL __declspec(dllimport)
    #define H5CHKDLLVAR __declspec(dllimport)
  #elif (__GNUC__ >= 4)  /* GCC 4.x has support for visibility options */
    #define H5CHKDLL __attribute__ ((visibility("default")))
    #define H5CHKDLLVAR extern __attribute__ ((visibility("default")))
  #endif
#endif /* hdf5check_EXPORTS */

#endif /* H5CHECK_BUILT_AS_DYNAMIC_LIB */

#ifndef H5CHKDLL
  #define H5CHKDLL
  #define H5CHKDLLVAR extern
#endif /* _HDF5DLL_ */

#endif  /* !H5CHECK_API_H__ */
