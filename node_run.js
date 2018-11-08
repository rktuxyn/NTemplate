var tparser = require('./release/ntemplate');
console.clear();
tparser.runScript(`for(let p=0; p<10; p++)console.log('Script is running...' + p)`);
tparser.parse(__dirname, __dirname + "\\test\\nested_template.html.html");
console.log(tparser.parse(__dirname, __dirname + "\\test\\nested_template.html.html"));
console.log(tparser.parse(__dirname, __dirname + "\\test\\script_template.html.html", this));
for( let i=0; i < 10000; i++ ){
	console.log(tparser.parse(__dirname, __dirname + "\\nested_template.html"))
};