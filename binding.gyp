{
    "targets": [{
        "target_name": "i106_decode_1553f1_js_wrap",
        # "cflags!": [ "-fno-exceptions" ],
        # "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "./src/i106_decode_1553_js_wrapper.c"
        ],
        # 'include_dirs': [
        #     "<!@(node -p \"require('node-addon-api').include\")"
        # ],
        # 'libraries': [],
        # 'dependencies': [
        #     "<!(node -p \"require('node-addon-api').gyp\")"
        # ],
        # 'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}