project('qcd_ml_accel', 'cpp', version:'0.0.1'
  , default_options : ['cpp_std=c++17'])

add_global_arguments('-O3', language:'cpp')
add_global_arguments('-march=native', language:'cpp')

CXX = meson.get_compiler('cpp')


executable('add_bench', 'src/add.cpp')
