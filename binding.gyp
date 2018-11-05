{
  "targets": [
    {
      "target_name": "NTemplate",
      "sources": [
        "src/stringx.h",
        "src/stder.h",
        "src/dependancy.h",
        "src/narg.h",
        "src/narg.cpp",
        "src/compiler.h",
        "src/compiler.cpp",
        "src/asyncdep.h",
        "src/async.h",
        "src/async.cpp",
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
