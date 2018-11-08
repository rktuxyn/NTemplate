{
  "targets": [
    {
      "target_name": "NTemplate",
      "sources": [
		"src/narg.cpp",
		"src/compiler.cpp",
		"src/tr.cpp",
		"src/jsp.cpp",
		"src/tp.cpp",
		"src/async.cpp",
		"src/core.cpp",
		"ntemplate.cc"
      ],
	  "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        }
	 },
	  "direct_dependent_settings":{
        "include_dirs": [ ]
      },
	 "conditions": [
        ["OS=='win'", {
          "defines": [
		"_HAS_EXCEPTIONS=1",
		"_WRITE_OUTPUT=0"
          ]
        }]
      ]
    }
  ]
}
