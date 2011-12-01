// ------------------------------------------------------------
// object.js
//
// Copyright, 2003-2006 by Mike Koss (mike06@mckoss.com)
//
// History:
//
// 12-2-05: Object orientation stuff only from util.js
// ------------------------------------------------------------
// Copy all base class methods that do not have a definition in the current
// constructor prototype.  Also add a prototype variable that references to
// base class's constructor by name
Function.prototype.DeriveFrom = function(fnSuper)
{
	var prop;

	if (this == fnSuper)
		{
		alert("Error - cannot derive from self");
		return;
		}

	for (prop in fnSuper.prototype)
		{
		if (typeof(fnSuper.prototype[prop]) == "function" && !this.prototype[prop])
			{
			this.prototype[prop] = fnSuper.prototype[prop];
			}
		}

	this.prototype[fnSuper.StName()] = fnSuper;
}

Function.prototype.StName = function()
{
	var st;

	st = this.toString();
	st = st.substring(st.indexOf(" ")+1, st.indexOf("("));
	if (st.charAt(0) == "(")
		st = "function ...";

	return st;
}

// Give subclass access to parent's method, via Parent_Method() like call.
Function.prototype.Override = function(fnSuper, stMethod)
{
	this.prototype[fnSuper.StName() + "_" + stMethod] = fnSuper.prototype[stMethod];
}

Function.prototype.StParams = function()
{
	var st;

	st = this.toString();
	st = st.substring(st.indexOf("("), st.indexOf(")")+1);
	return st;
}

// ------------------------------------------------------------
// Named - Base class for javascript objects that need scriptable names
//
// Derive from the Named object for any object that you want to have a script-evalable name
// (these are often needed in attributes added to browser elements for click events, timer callbacks etc.)
//
// e.g.
// MyObj.DeriveFrom(Named);
// function MyObj()
// {
//     this.Named();
//     ...
// }
// "<IMG onclick=" + StAttrQuote(this.StNamed() + ".Callback();") + ">"
// -----------------------------------------------------mck----

Named.idNext = 0;
Named.rg = new Array;

function Named()
{
	this.idNamed = Named.idNext++;
	Named.rg[this.idNamed] = this;
}

// Name for the JS object
Named.prototype.StNamed = function()
{
	return "Named.rg[" + this.idNamed + "]";
}

// Produce DHTML name for web component associated with this JS object
Named.prototype.StNamedPart = function(stPart, iPart)
{
	var st;

	st = "NM_" + this.idNamed + "_" + stPart;
	if (iPart != undefined)
		st += "_" + iPart;
	return st;
}

Named.prototype.StPartID = function(stPart, iPart)
{
	return "ID=" + StAttrQuote(this.StNamedPart(stPart, iPart));
}

Named.prototype.BoundPart = function(stPart, iPart)
{
	return eval(this.StNamedPart(stPart, iPart));
}

Named.prototype.FnCallBack = function(stFunc)
{
	return new Function(this.StNamed() + "." + stFunc + "();");
}
