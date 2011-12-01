Object.prototype.Dump = function(stName)
{
	document.write(StDumpObj(stName, this));
}

Object.prototype.IsHidden = function(stProp)
{
	if (stProp == "Dump" || stProp == "IsHidden" || stProp == "constructor")
		return true;
	return false;
}

StDumpObj.rgCons = new Array;

function RegisterConstructor(cons)
{
	StDumpObj.rgCons.push(cons);
}

RegisterConstructor(Object);

function StDumpObj(stName, obj)
{
	var st = "";
	var icons;
	var dict = new DictProps;
		
	st += "<TABLE class=doTable>\n";

	st += "<TR><TD class=doTitle colspan=2>" + stName + "</TD></TR>\n";
	st += StPropsInObj(obj, dict);

	for (icons = StDumpObj.rgCons.length-1; icons >= 0; icons--)
		{
		var cons = StDumpObj.rgCons[icons];
		if (obj instanceof cons)
			{
			st += "<TR><TD class=doProto colspan=2><I>" + cons.StName() + ".prototype</I></TD></TR>\n"
			st += StPropsInObj(cons.prototype, dict);
			}
		}

	st += "</TABLE>";

	return st;
}

function DictProps()
{
}

DictProps.prototype.StProp = function(stProp)
{
	// Note: prefix prop with _ so it doesn't match a built-in property - like "constructor"!
	if (this["_" + stProp])
		return "(" + stProp + ")";

	this["_" + stProp] = true;
	return stProp;
}

function StPropsInObj(obj, dict)
{
	var st = "";
	
	if (obj.hasOwnProperty("constructor"))
		{
		st += StDumpVar(dict.StProp("constructor"), obj.constructor);
		}

	for (prop in obj)
		{
		if (obj.IsHidden(prop))
			continue;
		if (obj.hasOwnProperty(prop))
			st += StDumpVar(dict.StProp(prop), obj[prop]);
		}

	return st;
}

function StDumpVar(stVar, value)
{
	var st = "";

	if (value == undefined)
		{
		value = "UNDEFINED";
		return st;
		}

	st += "<TR><TD class=doName>";
	st += stVar;
	st += "</TD><TD class=doValue>";
	if (typeof(value) == "object")
		st += StDumpObj("", value);
	else if (typeof(value) == "function")
		st += value.StName();
	else
		st += StHTML(value);
	st += "</TD></TR>\n";
	return st;
}

function StHTML(st)
{
	st = st.toString();
	st = st.replace(/&/g, '&amp;');
	st = st.replace(/</g, '&lt;');
	st = st.replace(/>/g, '&gt;');
	st = st.replace(/\n/g, '<br>');
	st = st.replace(/ /g, '&nbsp;');
	st = st.replace(/\t/g, '&nbsp;&nbsp;&nbsp;&nbsp;');
	return st;
}

function CodeSample(st, fDisplay, stHidden)
{
	if (!stHidden)
		stHidden = "";

	document.write("<LISTING>"+StHTML(st)+"</LISTING>");
	if (fDisplay)
		document.write("<DIV class=codeOutput>");
	
	eval(st+stHidden);
	
	if (fDisplay)
		document.write("</DIV>");
}
