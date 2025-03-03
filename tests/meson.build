gtest_dep = dependency('gtest', fallback: ['gtest', 'gtest_dep'], required: true)

base64safe_test = executable(
  'base64safe_test',
  'base64safe_test.cpp',
  dependencies: [gtest_dep, base64safe_dep],
)

test('base64safe_test', base64safe_test)
