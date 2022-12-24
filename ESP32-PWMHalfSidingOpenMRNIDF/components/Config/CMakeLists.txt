set(DEPS
    OpenMRNIDF
   )
   

idf_component_register(INCLUDE_DIRS include
                       REQUIRES "${DEPS}"
                      )
                      
