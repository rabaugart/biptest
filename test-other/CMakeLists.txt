
add_executable( oa a.cpp )
target_link_libraries( oa Boost::date_time fmt::fmt )

add_test( test_other_oa oa )

add_custom_target( run_test_other oa DEPENDS oa )
