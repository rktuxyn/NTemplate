# NTemplate
Nested Template Parser for Node.js (Populate data and add multiple attachment and template in single page)

<b>Example</b>:<br/>
<b>#Add library:</b>
```js
var tparser = require( './build/release/ntemplate' );
```
<i>#Run Javascript:</i>
```js
tparser.runScript( `for(let p=0; p<10; p++)console.log('Script is running...' + p)` );
```
<i>#Parse & Run Nested Template without async</i>
```js
var html = tparser.parse( __dirname, __dirname + "\\test\\nested_template.html.html" );
console.log( html );
```
<i>#Parse & Run Script Template without async</i>
```js
html = tparser.parse( __dirname, __dirname + "\\test\\script_template.html.html", this );
console.log( html );
```
<i>#Performance Check Nested Template without async</i>
```js
for ( let i = 0; i < 10000; i++ ) {
    let _html = tparser.parse( __dirname, __dirname + "\\test\\nested_template.html" );
    console.log( _html );
};
```
<i>#Parse Template with data without or with async</i><br/>
<i>Settings:</i>
```js
let settings = {
    root_dir: "SERVER ROOT DIRECTORY",
    page_path: "REQUIST PAGE PATH",
    data: "array/object/undefined",
    isAsync: "boolean [true/false]",
    callback: function ( rsp ) { }
};
```
<i>Response should be:</i>
```js
resp = {
    is_error: false,
    err_msg: "NOPE",
    result: "HTML RESULT HERE!!!"
};
```
<i>Send async request to VM:</i>
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
