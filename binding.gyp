{
  "targets": [
    {
      "target_name": "NTemplate",
      "sources": [
        "src/narg.cpp",
        "src/result.cpp",
        "src/compiler.cpp",
        "src/ntemplate.cpp",
        "src/async.cpp",
        "src/ntemplateimp.cpp",
        "TemplateParser.cc"
      ],
	  "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        }
	 },
	  "direct_dependent_settings":{
        "include_dirs": [ "F:/C++/Packeges/re2-master/re2/*" ]
      },
	 "conditions": [
        ["OS=='win'", {
          "defines": [
            "_HAS_EXCEPTIONS=1"
          ]
        }]
      ]
    }
  ]
}
