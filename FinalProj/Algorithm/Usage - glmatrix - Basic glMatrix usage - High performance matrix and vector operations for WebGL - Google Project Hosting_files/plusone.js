window.___jsl=window.___jsl||{};
window.___jsl.h=window.___jsl.h||'m;\/_\/apps-static\/_\/js\/widget\/__features__\/rt=j\/ver=JJ_UMfXdHS0.en_US.\/sv=1\/am=!7cXxqVkrvlyIEO88iA\/d=1\/';
window.___jsl.l=[];
window.___gpq=[];
window.gapi=window.gapi||{};
window.gapi.plusone=window.gapi.plusone||(function(){
  function f(n){return function(){window.___gpq.push(n,arguments)}}
  return{go:f('go'),render:f('render')}})();
function __bsld(){var p=window.gapi.plusone=window.googleapisv0.plusone;var f;while(f=window.___gpq.shift()){
  p[f]&&p[f].apply(p,window.___gpq.shift())}
p=window.gapi.plus=window.googleapisv0.plus;while(f=window.___gbq.shift()){
  p[f]&&p[f].apply(p,window.___gbq.shift())}
if (gadgets.config.get("gwidget")["parsetags"]!=="explicit"){gapi.plusone.go();}}
window.___gbq=[];
window.gapi.plus=window.gapi.plus||(function(){
  function f(n){return function(){window.___gbq.push(n,arguments)}}
  return{go:f('go'),render:f('render')}})();
window.__GOOGLEAPIS=window.__GOOGLEAPIS||{};
window.__GOOGLEAPIS.iframes=window.__GOOGLEAPIS.iframes||{};
window.__GOOGLEAPIS.iframes.plus=window.__GOOGLEAPIS.iframes.plus||{url:':socialhost:/u/:session_index:/_/pages/badge'};window['___jsl'] = window['___jsl'] || {};window['___jsl']['u'] = 'https:\/\/apis.google.com\/js\/plusone.js';window['___jsl']['f'] = ['googleapis.client','plusone','gcm_ppb'];window['___jsl']['ms'] = 'https://plus.google.com';(window['___jsl']['ci'] = (window['___jsl']['ci'] || [])).push({"gwidget":{"parsetags":"onload"},"iframes":{"sharebox":{"params":{"json":"&"},"url":":socialhost:/:session_prefix:_/sharebox/dialog"},"plus":{"url":":socialhost:/:session_prefix:_/pages/badge"},":socialhost:":"https://plusone.google.com","plusone_m":{"url":":socialhost:/:session_prefix:_/+1/fastbutton","params":{"count":"","size":"","url":""}},"card":{"params":{"style":"#","userid":"&"},"url":":socialhost:/u/:session_index:/_/hovercard/card"},"plusone":{"url":":socialhost:/:session_prefix:_/+1/fastbutton","params":{"count":"","size":"","url":""}}},"googleapis.config":{"requestCache":{"enabled":true},"methods":{"chili.people.list":true,"pos.plusones.list":true,"pos.plusones.get":true,"chili.people.get":true,"pos.plusones.insert":true,"chili.activities.list":true,"pos.plusones.delete":true,"chili.activities.get":true,"chili.activities.search":true,"pos.plusones.getSignupState":true},"versions":{"chili":"v1","pos":"v1"},"rpc":"/rpc","transport":{"isProxyShared":true},"sessionCache":{"enabled":true},"root-1p":"https://clients6.google.com","root":"https://www.googleapis.com","xd3":"/static/proxy.html","developerKey":"AIzaSyCKSbrvQasunBoV16zDH9R33D88CeLr9gQ","auth":{"useInterimAuth":false}}});var gapi=window.gapi||{};gapi.client=window.gapi&&window.gapi.client||{};
(function(){var o=void 0,p=void 0,q="___jsl",H="h",m="l",I="m",r="ms",s="cu",J="c",K="cm",L="o",n="p",i="q",k="https://ssl.gstatic.com",M="/webclient/js",N="/webclient/jsx/",t="https://apis.google.com",u=".js",O="gcjs-3p",P=/^(https?:)?\/\/([^/:@]*)(:[0-9]+)?(\/[\w.,:!=/-]*)(\?[^#]*)?(#.*)?$/,v=/^[?#]([^&]*&)*jsh=([^&]*)/,w="d",j="r",Q="f",l="m",x="n",R="sync",S="callback",y="config",z="nodep",A="gapi.load: ",B=function(a,b){o&&o(a,b);throw A+a+(b&&" "+b);},C=function(a){p&&p(a);var b=window.console;
b&&b.warn(A+a)},T=function(a,b,e){a=a[H];if(b=b&&v.exec(b)||e&&v.exec(e))try{a=decodeURIComponent(b[2])}catch(f){C("Invalid hint "+b[2])}return a},D=function(a){a.sort();for(var b=0;b<a.length;)!a[b]||b&&a[b]==a[b-1]?a.splice(b,1):++b},U=function(a,b){for(var e=!0,f=0,c=0,d,g;e&&(d=a[f])&&(g=b[c]);)d==g?++f:d<g&&(e=!1),++c;return e&&!d},V=function(a){if("loading"!=document.readyState)return!1;if("undefined"!=typeof window.___gapisync)return window.___gapisync;if(a&&(a=a[R],"undefined"!=typeof a))return a;
for(var a=document.getElementsByTagName("meta"),b=0,e;e=a[b];++b)if("generator"==e.getAttribute("name")&&"blogger"==e.getAttribute("content"))return!0;return!1},W=function(a){var b=document.createElement("script");b.setAttribute("src",a);a=document.getElementsByTagName("script")[0];a.parentNode.insertBefore(b,a)},X=function(a,b,e,f,c){var d=e.shift(),g;g=d==j?k:d==l?f[r]||t:(g=e.shift())&&g.replace(/\/+$/,"");var h;d==j?(h=e.shift(),h=(h.indexOf(N)?M+"/":"")+h):h=e.shift();var E=d==w,i=E&&e.shift()||
O,e=E&&e.shift();if(d==w)c=h,h=i,a="/"+a.join(":")+(b.length?"!"+b.join(":"):"")+u+"?container="+h+"&c=2&jsload=0",c&&(a+="&r="+c),"d"==e&&(a+="&debug=1");else if(d==j||d==Q)c=h,a=(c.indexOf("/")?"/":"")+c+"/"+a.join("__")+(b.length?"--"+b.join("__"):"")+u;else if(d==l||d==x)b=h,a=a.join(",").replace(/\./g,"_").replace(/-/g,"_"),a=b.replace("__features__",a),a=c[z]?a.replace("/d=1/","/d=0/"):a;else return C("Unknown hint type "+d),"";if(!g)return"";g+=a;a=g;b=f;if(c=f=P.exec(a))if(c=!/\.\.|\/\//.test(f[4]))b:if(c=
a,f=f[2],d==j)c=c.substr(0,k.length)==k;else if(d==l)f=b[r]||t,c=c.substr(0,f.length)==f;else{d=b[I];if(f&&d){d=d.split(",");b=0;for(c=d.length;b<c;++b)if(e=d[b],h=f.lastIndexOf(e),(0==h||"."==e.charAt(0)||"."==f.charAt(h-1))&&f.length-e.length==h){c=!0;break b}}c=!1}c||B("Invalid URI",a);return g},Y=function(a,b,e){(a[i]=a[i]||[]).push([b,e])},G=function(a){a[i]&&0<a[i].length&&(window.gapi.load||F).apply(null,a[i].shift())},F=function(a,b){var e,f={};"function"!==typeof b?(f=b||{},e=f[S]):e=b;var c=
window[q]=window[q]||{};if(c[n])Y(c,a,b);else{var d=T(c,window.location.search,window.location.hash);d||B("No hint present","");var g=d.split(";"),h=g[0]==l||g[0]==x,d=a.split(":");f[z]||D(d);var i=c[m]=c[m]||[];D(i);var j=function(a){if(a)try{a()}catch(b){return b}return null};if(!U(d,i)){var k=h?K:J,g=X(d,i,g,c,f);f[y]&&(c[s]=c[s]||[]).push(f[y]);if(g){c[n]=d;c[L]=1;c[k]=function(){delete c[n];delete c[k];var a=j(e);G(c);if(a)throw a;};[].push.apply(c[m],d);V(f)?document.write('<script src="'+encodeURI(g)+
'"><\/script>'):W(g);return}}f=j(e);G(c);if(f)throw f;}};gapi.loader={load:F}})();
gapi.load=window.___jsl&&window.___jsl.il&&window.gapi&&window.gapi.load||gapi.loader.load;
(window.gapi=window.gapi||{}).load=window.___jsl&&window.___jsl.il&&window.gapi.load||gapi.load;
gapi.load('googleapis.client:plusone:gcm_ppb', {'callback': window['__bsld']  });