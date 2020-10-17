
add_executable( autodiff autodiff.cpp )
target_link_libraries( autodiff boost_math_tr1 )

add_custom_target( run_autodiff autodiff )

