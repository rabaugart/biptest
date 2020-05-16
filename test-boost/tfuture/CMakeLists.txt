project( tfuture )


add_executable( tfuture tfuture.cpp tiosfut.cpp )
target_link_libraries( tfuture boost_unit_test_framework boost_system )
