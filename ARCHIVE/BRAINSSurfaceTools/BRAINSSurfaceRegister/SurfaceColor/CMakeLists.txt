StandardBRAINSBuildMacro(NAME SurfaceColor TARGET_LIBRARIES BRAINSCommonLib ${VTK_LIBRARIES}
  ${BRAINSSurfaceTools_ITK_LIBRARIES})

if (BUILD_TESTING AND NOT BRAINSTools_DISABLE_TESTING)

ExternalData_add_test( ${BRAINSTools_ExternalData_DATA_MANAGEMENT_TARGET} NAME TEST_SurfaceColor
  COMMAND ${LAUNCH_EXE} $<TARGET_FILE:SurfaceColor>
          --inputSurfaceFile DATA{${TestData_DIR}/0036195_left_49Labels.vtk}
          --labelMapFile DATA{${TestData_DIR}/0036195_left_transformedLabelMap.nii.gz}
          --outputSurfaceFile SurfaceColorTest.vtk
          --numOfLabels 49 )

ExternalData_add_test( ${BRAINSTools_ExternalData_DATA_MANAGEMENT_TARGET} NAME COMPARE_SurfaceColor
  COMMAND ${LAUNCH_EXE} $<TARGET_FILE:CompareSurfaces>
          --inputSurfaceFile SurfaceColorTest.vtk
          --refSurfaceFile DATA{${TestData_DIR}/SurfaceColor.vtk}
          --vertexLocation
          --scalarArray
          --tolerance 1.0e-7 )

set_tests_properties( COMPARE_SurfaceColor PROPERTIES DEPENDS TEST_SurfaceColor )

endif ()
