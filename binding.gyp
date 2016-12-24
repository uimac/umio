{
	"targets": [
	{
		"target_name": "umio",
		"conditions": [
				[	"OS == 'win'", {
					"libraries": [
					"Ws2_32.lib",
					"<(module_root_dir)/lib/fbxsdk2016/x64/Release/libfbxsdk-md.lib",
					],
					"configurations": {
						'Debug': {
							'msvs_settings': {
									'VCCLCompilerTool': {
										'RuntimeLibrary': '3' # /MDd
								},
							},
						},
						'Release': {
							'msvs_settings': {
									'VCCLCompilerTool': {
										'RuntimeLibrary': '2' # /MD
								},
							},
						},
					},
					"msvs_configuration_attributes": {
						'CharacterSet': '1'
					},
					'defines': [ '_HAS_EXCEPTIONS=1', 'WITH_V8', 'FBXSDK_NEW_API', 'WITH_FBX2016', 'WITH_MSGPACK', 'WIN32_LEAN_AND_MEAN' ],
					'msvs_settings': {
						'VCCLCompilerTool': {
						'ExceptionHandling': '1',  # /EHsc
						'AdditionalOptions': [ '/GR' ]
						},
					}
				}
			]
		],
		"sources": [
		"src/UMFbx.cpp",
		"src/UMFbx2016.cpp",
		"src/UMIO.cpp",
		"src/UMObject.cpp",
		"src/UMNode.cpp"
		],
		"include_dirs": [
		"<(module_root_dir)/src",
		"<(module_root_dir)/src/msg",
		"<(module_root_dir)/lib/v8pp",
		"<(module_root_dir)/lib/msgpack/include",
		"<(module_root_dir)/lib/fbxsdk2016/include",
		]
	}]
}
