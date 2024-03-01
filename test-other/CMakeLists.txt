
add_executable( oa a.cpp )
target_link_libraries( oa Boost::date_time )

add_test( test_other_oa oa )

