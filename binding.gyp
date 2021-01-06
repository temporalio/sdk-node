{
  'targets': [
    {
      'target_name': 'temporalio-workflow-runtime',
      # see: https://github.com/nodejs/node-gyp/issues/17#issuecomment-3917672
      'cflags_cc!': ['-fno-exceptions'],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        '<!(node -e "require(\'isolated-vm/include\')")',
      ],
      'sources': [
        'workflow-runtime.cc',
      ],
    },
  ],
}
