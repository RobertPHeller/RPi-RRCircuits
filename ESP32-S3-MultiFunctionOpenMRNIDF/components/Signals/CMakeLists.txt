set(DEPS
    app_update
    OpenMRNIDF
    GeneralIO
    Config
    Logic
   )
   
idf_component_register(SRCS Mast.cpp Rule.cpp
                       INCLUDE_DIRS "include"
                       PRIV_INCLUDE_DIRS "private_include"
                       REQUIRES "${DEPS}"
                      )
set_source_files_properties(Mast.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized -Wno-implicit-fallthrough -Wno-ignored-qualifiers")
set_source_files_properties(Rule.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized -Wno-implicit-fallthrough -Wno-ignored-qualifiers")
