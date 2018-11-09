# NTemplate
Nested Template Parser for Node.js (Populate data and add multiple attachment and template in single page)

<b>Example</b>:
Add library:
```js
var tparser = require( './build/release/ntemplate' );
```
Run Javascript:
```js
tparser.runScript( `for(let p=0; p<10; p++)console.log('Script is running...' + p)` );
```
#Parse & Run Nested Template without async
```js
var html = tparser.parse( __dirname, __dirname + "\\test\\nested_template.html.html" );
console.log( html );
```
#Parse & Run Script Template without async
```js
html = tparser.parse( __dirname, __dirname + "\\test\\script_template.html.html", this );
console.log( html );
```
#Performance Check Nested Template without async
```js
for ( let i = 0; i < 10000; i++ ) {
    let _html = tparser.parse( __dirname, __dirname + "\\test\\nested_template.html" );
    console.log( _html );
};
```
#Parse Template with data without or with async
<b>Settings:</b>
```js
let settings = {
    root_dir: "SERVER ROOT DIRECTORY",
    page_path: "REQUIST PAGE PATH",
    data: "array/object/undefined",
    isAsync: "boolean [true/false]",
    callback: function ( rsp ) { }
};
```
<b>Response should be:</b>
```js
resp = {
    is_error: false,
    err_msg: "NOPE",
    result: "HTML RESULT HERE!!!"
};
```
<b>Send async request to VM:</b>
```js
let example = async () =>{
    let resp = await tparser.parseo( {
        root_dir: __dirname,
        page_path: __dirname + "\\test\\script_template.html",
        data: this,
        isAsync: true,
        callback: function ( rsp ) {
            if ( rsp.is_error ) {
                console.log( rsp.err_msg );
            } else {
                console.log( rsp.result.length );
            }
        }
    } );
    console.log( resp );
    for ( let x = 0; x < 100; x++ ) {
        let copy = x;
        await tparser.async( {
            root_dir: __dirname,
            page_path: __dirname + "\\test\\script_template.html",
            data: this,
            callback: function ( rsp ) {
                console.log( copy );
                if ( rsp.is_error ) {
                    console.log( rsp.err_msg );
                } else {
                    console.log( rsp.result.length );
                }
            }
        } );
    }

};
example();
```
