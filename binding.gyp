{
  "targets": [
    {
      "target_name": "NTemplate",
      "sources": [
        "NTemplate/stringx.h",
        "NTemplate/stder.h",
        "NTemplate/dependancy.h",
        "NTemplate/narg.h",
        "NTemplate/narg.cpp",
        "NTemplate/compiler.h",
        "NTemplate/compiler.cpp",
        "NTemplate/asyncdep.h",
        "NTemplate/async.h",
        "NTemplate/async.cpp",
        "NTemplate.cc"
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
