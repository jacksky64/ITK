if(ITK_USE_MINC2)

mark_as_advanced(NETCDF_INCLUDE_PATH NETCDF_LIB)
mark_as_advanced(HDF5_INCLUDE_PATH HDF5_LIB)
mark_as_advanced(MINC_INCLUDE_PATH)
mark_as_advanced(MINC2_INCLUDE_PATH MINC2_LIB)

set(NETCDF_INC_SEARCHPATH
   /usr/include
)

set(HDF5_INC_SEARCHPATH
  /usr/include
)

set(MINC_INC_SEARCHPATH
  /usr/include
)

set(MINC2_INC_SEARCHPATH
  /usr/include
)

find_path(NETCDF_INCLUDE_PATH netcdf.h ${NETCDF_INC_SEARCHPATH})
find_path(HDF5_INCLUDE_PATH hdf5.h ${HDF5_INC_SEARCHPATH})
find_path(MINC_INCLUDE_PATH minc.h ${MINC_INC_SEARCHPATH})
find_path(MINC2_INCLUDE_PATH minc2.h ${MINC2_INC_SEARCHPATH})

if(NETCDF_INCLUDE_PATH)
  set(NETCDF_INCLUDE  ${NETCDF_INCLUDE_PATH})
endif()

if(HDF5_INCLUDE_PATH)
  set(HDF5_INCLUDE  ${HDF5_INCLUDE_PATH})
endif()

if(MINC_INCLUDE_PATH)
  set(MINC_INCLUDE ${MINC_INCLUDE_PATH})
endif()


if(MINC2_INCLUDE_PATH)
  set(MINC2_INCLUDE ${MINC2_INCLUDE_PATH})
endif()

find_library(NETCDF_LIB netcdf /usr/lib /usr/local/lib )
find_library(HDF5_LIB hdf5 /usr/lib /usr/local/lib )
find_library(MINC2_LIB minc2 /usr/lib /usr/local/lib )


if(NETCDF_LIB)
        get_filename_component(NETCDF_LIBRARY_PATH ${NETCDF_LIB} PATH)
endif()

if(HDF5_LIB)
        get_filename_component(HDF5_LIBRARY_PATH ${HDF5_LIB} PATH)
endif()

if(MINC2_LIB)
        get_filename_component(MINC2_LIBRARY_PATH ${MINC2_LIB} PATH)
endif()

set(MINC2_FOUND 0)

if(MINC_INCLUDE AND MINC2_INCLUDE AND HDF5_INCLUDE AND NETCDF_INCLUDE)
  if(MINC2_LIBRARY_PATH AND HDF5_LIBRARY_PATH AND NETCDF_LIBRARY_PATH)
    set(MINC2_FOUND 1)
  endif()
endif()

if(MINC2_FOUND)
  include_directories( ${NETCDF_INCLUDE} ${HDF5_INCLUDE} ${MINC_INCLUDE} ${MINC2_INCLUDE}  )
  link_directories( ${NETCDF_LIBRARY_PATH} ${HDF5_LIBRARY_PATH} ${MINC2_LIBRARY_PATH})
  add_definitions(-DUSE_MINC2)
endif()

endif()