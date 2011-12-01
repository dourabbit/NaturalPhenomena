// set initial state
$('#sidebar .nav .items').hide();
$('#sidebar .nav h2').addClass('collapsed');

// hook up event handlers
$('#sidebar .nav h2')
.each(function(){
	this.tabIndex = '0';
	})
.click(function(){
	var el = $(this).next();
	if(el.filter(':hidden').length){
		el.slideDown(200);
		$(this).removeClass('collapsed');
	}else{
		el.slideUp(200);
		$(this).addClass('collapsed');		
	}
});



Date.prototype.addYear = function()
{
    var base = new Date(0);
    var skew = base.getTime();
    if (skew > 0) this.setTime(this.getTime() - skew);
    this.setTime(this.getTime() + 365 * 24 * 60 * 60 * 1000);
}

/* =======================================================
		SNOOK NAMESPACE
		you gotta keep em separated.
*/

function _$(id) {
	return document.getElementById(id);
}

var SNOOK = {};

SNOOK.prepareCommentForm = function()
{
    // ========================================
		// load cookie info
		// sets default values for form fields
	if(_$('comments_form'))
	{
	    this.prepareField({field:'email',text:'E-mail address',record:true,clear:false});
	    this.prepareField({field:'author',text:'Name',record:true,clear:false});
	    this.prepareField({field:'CommentBody',record:false,clear:false});

	    _$('comments_form').onsubmit = function()
		{
		  var els = this.getElementsByTagName("*");
		  for(var i=0,j=els.length;i<j;i++)
		  {
		    if(els[i].noValue)
		    {
		      if(els[i].value == els[i].noValue) els[i].value = '';
		      if(els[i].record) SNOOK.remember(els[i]);
		    }
		  }
		}
	}
}



SNOOK.prepareLivePreview = function () {
      // return; // feature disabled for the time being
		var el = _$('livepreview');
		if (!el) return;
		
		var txt = _$("CommentBody");
		var auth = _$("author");
		var ison = true;

		/* enables live area functionality  */
        txt.onkeyup=setCommentText;
        setCommentText();
        setMetaData();

		el.style.display = 'block'; // bring the livepreview to life!
		
		function setCommentText() {
			var t = (txt.value == 0 ? 'Your comment will be previewed here. Go on and say your piece.': txt.value);
			t = '<p>' + t + '</p>';
			t = t.replace(/\n\n/g, "</p><p>");
			t = t.replace(/\n/g, "<br>");
    		_$('lptext').innerHTML = t;
  		}

		function setMetaData() {
	      	var pre = '', post = '';

      		var dt = new Date();
  			var ms = new Array("January","February","March","April","May","June","July","August","September","October","November","December");
  			var hr = (dt.getHours()==0?12:(dt.getHours()>12?dt.getHours()-12:dt.getHours()));

  			var cpd = $('#lpdate')[0];
  			cpd.innerHTML = ms[dt.getMonth()] + ' ' + dt.getDate() + ', ' + dt.getFullYear();

      		var lp = $("#lpposter")[0];
      		lp.innerHTML = pre + auth.value + post;
		}

}


SNOOK.prepareField = function(options)
{
  var opt = {
      field: '',
      text: '',
      record: false, /* record value to a cookie on submit */
      clear: false, /* clear field on focus */
      restore: false /* restore field on blur */
    };
  $.extend(opt, options);

  var el = _$(opt.field);
  if(el) // make sure field exists
  {
      if(opt.record) el.value = this.getCookie("snook" + opt.field);

      el.noValue = opt.text;
      el.record = opt.record;
//    el.previousSibling.style.display = 'none';
//    if(!el.value) el.value = opt.text;
      if(opt.clear){ el.onfocus = function(){ if(this.value == this.noValue){this.value = '';} }; };
      if(opt.restore){ el.onblur = function(){ if(this.value == ''){this.value = this.noValue;} }; };
  }
}

// Cookie code Copyright (c) 1996-1997 Athenia Associates.
// http://www.webreference.com/js/
// License is granted if and only if this entire
// copyright notice is included. By Tomer Shiran.
// modifed for Snook namespace


SNOOK.setCookie = function (name, value, expires) {
    var domain = 'snook.ca';
    var curCookie = name + "=" + escape(value) + ((expires) ? "; expires=" + expires.toGMTString() : "") + ((domain) ? "; domain=" + domain : "") + "; path=/";
    document.cookie = curCookie;
}

SNOOK.getCookie = function(name) {
    var prefix = name + '=';
    var c = document.cookie;
    var nullstring = '';
    var cookieStartIndex = c.indexOf(prefix);
    if (cookieStartIndex == -1)
        return nullstring;
    var cookieEndIndex = c.indexOf(";", cookieStartIndex + prefix.length);
    if (cookieEndIndex == -1)
        cookieEndIndex = c.length;
    return unescape(c.substring(cookieStartIndex + prefix.length, cookieEndIndex));
}


SNOOK.remember = function(fld) {
    var now = new Date();
    now.addYear();
    this.setCookie('snook' + fld.id, fld.value, now);
}


SNOOK.prepareCommentForm();
SNOOK.prepareLivePreview();
