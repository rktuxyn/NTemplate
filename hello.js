//var addon = require('bindings')('hello');
//console.log("running")
 var addon = require('./build/Release/NTemplate');
 var fs = require('fs');
 
 
 //console.log(addon);
/*console.log(addon.add(3, 5));
console.log(addon.hello()); // 'world'
var resultA = 0;
var resultB = 0;
addon.loop(function(a, b){
    //console.log(arguments);
    resultA+= a;
    resultB+= b;
});*/
//console.log("Result A=>"+resultA + " Result B=>"+resultB);
//console.log(addon)
/**addon.test_function = function(){
    console.log("Running Form Caller.");
    console.log(arguments);
}*/

console.log('TAP Version 13')

/**process.on('uncaughtException', function (er) {
  if (er) { throw er }
  console.log('not ok - Failed checking mock registry dep. Expect much fail!')
  console.log('1..1')
  process.exit(1)
})*/

var assert = require('assert')
//var semver = require('semver')
//var mock = require('npm-registry-mock/package.json').version
//var req = require('../../package.json').devDependencies['npm-registry-mock']

//assert(semver.satisfies(mock, req))
console.log('ok')
console.log('1..1')

/**var px = addon.parseScript(
    `
    function test_function(i) { 
        if(typeof(i)==='function'){
            return i.call(test_function, 100);
        }
        let rs = i;
        let l = i + 100;
        for(; i < l; i++) {
            //rs += i;
            rs++;
        }\
        return rs;
    }\
    `
    , (a, b)=>{
        console.log(a);
        console.log(b);
       
}, 'test_function', 1000
);*/
addon.runScript(`console.log('Script is running...')`);

//console.log(JSON.stringify(addon));
//console.log(px)
//px(1);
//console.log(addon.processTemplate(__dirname + "\\hello.js"));
//console.log(__dirname);
var data ="";

try{
	//console.log(typeof(addon.processTemplate));
    var dataObj= [];
    for(let x =0; x<100; x++){
        dataObj.push({a:x, b:x*2, c:x*5});
    }
this.NOPX=function() {
    this.a("A");
    this.b("B");
    this.c("C");
    this.d("D");
    this.e("E");
    return dataObj;
};
var objs= {
    a:function name(params) {
            console.log(params);
    },b:function name(params) {
        console.log(params);
    },c:function name(params) {
        console.log(params);
    },d:function name(params) {
        console.log(params);
    },e:function name(params) {
        console.log(params);
    }
};

for(var x in objs){

    this[x]= objs[x];
}
//var that = this;
//data = addon.processTemplate(__dirname, __dirname + "\\index.html", this);
//console.log(data);

//data = addon.global(__dirname, __dirname + "\\index.html", this);
//console.log(data);

var that = this;
var count = 0;
//addon.async_init();
 
/*try{
console.log(addon.global({
	root_dir: __dirname,
	page_path:  __dirname + "\\index.html",
	data: this
}) + '____________NON', "===>>>>", 1);							
}catch(e){
		console.log(e.message);
}*/

function ___Template(x){
	/*setTimeout(() => {
		var sp = addon.global({
			root_dir: __dirname,
			page_path:  __dirname + "\\index.html",
			data: this
		});
		x++;
		console.log(sp.length + "=====>>" +x+'============>', new Date());	
		___Template(x); 
	}, 0);
	return;*/
	console.log(addon);
	setTimeout(() => {
		addon.global({
				root_dir: __dirname,
				page_path:  __dirname + "\\index.html",
				data: this,
				//isAsync:true,
				callback:function(rsp){
					//rsp(that);
					if(rsp.is_error){
						console.log(rsp.err_msg + "=====>>" +x+'============>', new Date());
						//return;
					}else{
						fs.writeFile(__dirname + "/output/______"+x+".html", rsp.result, 'utf8', function (er) {
							//console.log(er);
						});
						console.log(rsp.result.length + "=====>>" +x+'============>', new Date());	
					}
					x++;
					___Template(x);
			}
		});
	}, 0);
}
___Template(0);
console.log('START_____________________________Awaken!', new Date());

for(var x = 0, len = 10; x <len; x++){
	addon.async({
			root_dir: __dirname,
			page_path:  __dirname + "\\index.html",
			data: this,
			isAsync:true,
			callback:function(rsp){
				//rsp(that);
				if(rsp.is_error){
					console.log(rsp.err_msg + "=====>>" +count+'============>', new Date());
					//return;
				}else{
					console.log(rsp.result.length + "=====>>" +count+'============>', new Date());	
				}
				
				count++;
				if(count===len){
					 console.log('END________________________________ Async!', new Date());
					 setTimeout(() => {
					 console.log('STARTNON_ASYNC!', new Date());
					for(let o = 0; o < len; o++){
						try{
						console.log(addon.global({
							root_dir: __dirname,
							page_path:  __dirname + "\\index.html",
							data: this
						}).length + '____________NON', "===>>>>", o);							
						}catch(e){
								console.log(e.message);
						}
					/**/
					}
					console.log('ENDNON_ASYNC!', new Date());
					 },10);
				}
			}
	});
}

}catch(e){
	console.log(e.message);
}
console.log("AFTERFN");
setTimeout(() => {
    console.log(count);
	/*for(var t = 0; t<100; t++){
		console.log(t,'------------------------>>>>>>>>>>>>>>>>>>');
	}*/
	//console.log(___NTEMPLATE__SCRIPT__RUNNER(that))
	//console.log(data);
}, 1000);

data = "";

/*setTimeout(() => {
    console.log(___NTEMPLATE__SCRIPT__RUNNER(this));
}, 0);
//console.log("Running Async task");
setTimeout(() => {
    console.log("==>>>Timeout...");
	console.log(data);
}, 0);*/
//___NTEMPLATE__SCRIPT__RUNNER(this);
//eval(data + " ___NTEMPLATE__SCRIPT__RUNNER('')");
//console.log(___NTEMPLATE__SCRIPT__RUNNER(""));