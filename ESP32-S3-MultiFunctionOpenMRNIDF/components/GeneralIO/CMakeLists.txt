set(DEPS
    app_update
    OpenMRNIDF
    Config
   )
   
idf_component_register(SRCS VetoBitEventInterface.cpp
                       INCLUDE_DIRS "include"
                       PRIV_INCLUDE_DIRS "private_include"
                       REQUIRES "${DEPS}"
                      )

set_source_files_properties(VetoBitEventInterface.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized -Wno-implicit-fallthrough -Wno-ignored-qualifiers")
