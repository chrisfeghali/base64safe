project(
  'base64safe',
  'cpp',
  version: '1.0.0',
  license: '0BSD',
  meson_version: '>=1.1',
  default_options: ['cpp_std=c++17'],
)

base64safe_dep = declare_dependency(include_directories: include_directories('include'))

if get_option('enable_tests')
  subdir('tests')
endif
