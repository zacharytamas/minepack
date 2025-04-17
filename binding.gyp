{
  "targets": [
    {
      "target_name": "afc_client",
      "sources": [ "src/native/afc_client.c" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "/opt/homebrew/Cellar/libimobiledevice/1.3.0_3/include",
        "/opt/homebrew/include",
        "/usr/local/include",
        "/usr/include"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "cflags": [
        "-I/opt/homebrew/Cellar/libimobiledevice/1.3.0_3/include"
      ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.15",
        "OTHER_CFLAGS": [
          "-I/opt/homebrew/Cellar/libimobiledevice/1.3.0_3/include",
          "-I/opt/homebrew/include",
          "-I/usr/local/include"
        ],
        "OTHER_LDFLAGS": [
          "-L/opt/homebrew/Cellar/libimobiledevice/1.3.0_3/lib",
          "-L/opt/homebrew/lib",
          "-L/usr/local/lib",
          "-limobiledevice-1.0",
          "-lplist-2.0",
          "-lusbmuxd-2.0",
          "-lssl",
          "-lcrypto"
        ]
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 }
      },
      "conditions": [
        ["OS=='linux'", {
          "libraries": [
            "-limobiledevice-1.0",
            "-lplist-2.0",
            "-lusbmuxd-2.0",
            "-lssl",
            "-lcrypto"
          ]
        }]
      ]
    }
  ]
} 