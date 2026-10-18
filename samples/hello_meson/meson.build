project('hello_meson', 'c', version : '0.1')

custom_target('hello_meson.self',
  input: executable('hello_meson.elf', 'main.c'),
  output: 'hello_meson.self',
  command: [find_program('elf2self'), '@INPUT@', '@OUTPUT@'],
  build_by_default: true
)
