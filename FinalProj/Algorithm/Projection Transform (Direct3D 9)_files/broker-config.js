//(MSDN)-/en-us/(TIER 5)-CUSTOM - Make frequency changes here for mdsn.microsoft.com/en-us/…(various paths)
//Array [PATH,SITE_CODE,FREQ]
var _msdn = [ ["default.aspx",'1175',0.3],["library/windows/apps",'1458',0.3],["library",'1176',0.24],["ie",'1177',0.15],["netframework",'1178',0.3],["windowsazure",'1179',0.3],["magazine",'1180',0.3], ["office",'1181',0.3],["sharepoint",'1183',0.3],["sqlserver",'1184',0.3],["subscriptions",'1185',0.3],["vbasic",'1186',0.15], ["vcsharp",'1187',0.15],["visualc",'1188',0.15],["vstudio",'1189',0.15],["windows/apps",'1457',0.0001],["windows",'1190',0.00004]  ];
//(TECHNET)-/en-us/(TIER 5) - CUSTOM - Make frequency changes here for tehnet.microsoft.com/en-us/…(various paths)
//Array [PATH,SITE_CODE,FREQ]
var _technet = [ ["default.aspx",'1219',0.055],["library",'1192',0.055],["windowsserver",'1194',0.055],["forefront",'1011',0.3],["office",'1195',0.055], ["sharepoint",'1196',0.3],["sqlserver",'1197',0.3],["systemcenter",'1198',0.3],["windows",'1199',0.016],["scriptcenter",'1200',0.055],["security",'1202',0.055], ["sysinternals",'1203',0.016],["virtualization",'1205',0.3],["subscriptions",'1206',0.055],["magazine",'623',0.055],["ie",'1220',0.3],["exchange",'1221',0.3],["edge",'1222',0.055]  ];
var SR_url = window.location.toString().toLowerCase();
var SR_url_stripped = SR_url.split("?");
//Freq,Site,Halt reset params for Center survey on (MSDN)-/en-us/(TIER 5)-CUSTOM  and (TECHNET)-/en-us/(TIER 5) mappings
var _Freq=0,_halt=false,_Site="See deployment report. ";
//Raw Params passed to CENTER SURVEY on (MSDN)-/en-us/(TIER 5)-CUSTOM  and (TECHNET)-/en-us/(TIER 5) mappings
var srchMSForumIroot="",_wtsp="", dcSextProduct="";
if(document.getElementsByName('Search.MSForums.Iroot')[0] && document.getElementsByName('Search.MSForums.Iroot')[0].getAttribute('content') != null){
		srchMSForumIroot = document.getElementsByName('Search.MSForums.Iroot')[0].getAttribute('content');
		if(srchMSForumIroot.search('windowsazure') != -1){ srchMSForumIroot='windowsazure'; }
}
if(document.getElementsByName('DCSext.Product')[0] && document.getElementsByName('DCSext.Product')[0].getAttribute('content') != null){
		dcSextProduct = document.getElementsByName('DCSext.Product')[0].getAttribute('content');
}
if(typeof(wtsp)&& typeof(wtsp)!='undefined'){ _wtsp=wtsp.toLowerCase(); if(/_technet_library_windowsserver/i.test(_wtsp)){_wtsp="_technet_library_windowsserver_";} };
var _raw_params = 'Search.MSForums.Iroot='+srchMSForumIroot+"&wtsp="+ _wtsp+"&DCSext.Product="+dcSextProduct;

if(SR_url_stripped[0].search('msdn.microsoft.com') != -1){
	setSiteFreq("http://msdn.microsoft.com/en-us/", _msdn);//MSDN center survey check
	checkWTSP();
}else if(SR_url_stripped[0].search('technet.microsoft.com') != -1){
	setSiteFreq("http://technet.microsoft.com/en-us/", _technet);//TechNet center survey check
	checkWTSP();
}
//Function to reset _Site and _Freq for Center Survey usinng ARRAY's above
function setSiteFreq(_url, _array){
	for(i=0; i< _array.length; i++){
		if(SR_url.search(_url + _array[i][0].toString().toLowerCase()) != -1){	
			_Site = _array[i][1];
			_Freq = _array[i][2];
			break;
		}
	}
}
//Function to reset _Freq, _halt based on wtsp param
function checkWTSP(){
	if(_Site == '1176'){
		if(!(_wtsp=="msdnlib_webdev" || _wtsp=="msdnlib_devtools_lang" || _wtsp=="windowsazure" || _wtsp=="_msdn_library_sqlserver_" || _wtsp=="msdnlib_w32_com")){
			_halt=true;
		}
		if(_wtsp=="msdnlib_w32_com"){
			_Freq=0.15;
		}else if(_wtsp=="windowsazure" || _wtsp=="_msdn_library_sqlserver_"){
			_Freq=0.3;
		}else if(_wtsp=="msdnlib_webdev" || _wtsp=="msdnlib_devtools_lang"){
			_Freq=0.089;
		}//Default freq is set in _msdn[] array above
	}else if(_Site=='1192'){
		if(!(_wtsp=="_technet_library_windowsserver_" || _wtsp=="_technet_prodtechnol_office_" || _wtsp=="_technet_library_sqlserver_" || _wtsp=="_sto_technet_systemcenter_" || _wtsp=="_technet_library_win7_" || _wtsp=="scriptcenter_technet" || _wtsp=="_technet_library_security_" || _wtsp=="_technet_library_ie_" || _wtsp=="_technet_library_exchangeserver_")){	
				_halt=true;
		} 
		if(_wtsp=="_technet_library_ie_" || _wtsp=="_technet_library_sqlserver_" || _wtsp=="_technet_library_exchangeserver_"){
			_Freq=0.3;
		}else if(_wtsp=="_technet_library_win7_"){
			_Freq=0.016;
		}else if(_wtsp=="_sto_technet_systemcenter_"){
			_Freq=0.3;
		}//Default is 1232 set in _technet[] array above
	}
}
function readCookie(name)
{
  var ca = document.cookie.split(';');
  var nameEQ = name + "=";
  for(var i=0; i < ca.length; i++) {
    var c = ca[i];
    while (c.charAt(0)==' ') c = c.substring(1, c.length); //delete spaces
    if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length, c.length);
    }
  return null;
}
_SS_TRACK=readCookie("SS_TRACK");
_MSCOM_C=readCookie("Microsoft.com");
_raw_params+='&'+ _MSCOM_C+'&'+_SS_TRACK;
//if(!_halt){alert("Site=" + _Site + "\nFreq: " + _Freq + "\n" +_raw_params);}
COMSCORE.SiteRecruit.Broker.config={version:"4.6.3",testMode:false,cookie:{name:"msresearch",path:"/",domain:".microsoft.com",duration:90,rapidDuration:0,expireDate:""},prefixUrl:"",mapping:[{m:"(code\\.msdn\\.microsoft\\.com|cpapp02)",c:"inv_c_CODE-MSDN_p77737117-661.js",f:0.16,p:0},{m:"(gallery\\.msdn\\.microsoft)[\\w\\.-]+/ScriptJunkie",c:"inv_c_GA-MSDN-p77737117-1210.js",f:0.16,p:0},{m:"(gallery\\.technet\\.microsoft)[\\w\\.-]+/",c:"inv_c_GA-TN-p77737117-1212.js",f:0.13,p:0},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/de-de/",c:"inv_c_MSDN-p15466742-DE-DE.js",f:0.018,p:1},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/en-us/",c:"inv_c_MSDN-p77737117_TIER1.js",f:0.16,p:0},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/en-us/((default\\.aspx)|(windows/)|(library|ie|netframework|windowsazure|magazine|office|sharepoint|sqlserver|subscriptions|vbasic|vcsharp|visualc|vstudio)|(windows/apps)|(library/windows/apps))",c:"inv_c_MSDN-p77596864_TIER5.js",f:_Freq,p:1,halt:_halt},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/en-us/library",c:"inv_c_MSDN-p77596864_ALM.js",f:0.089,p:2,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"ALM_other|ALM_architect|ALM_dev|ALM_data|ALM_debug|ALM_TF_AO|ALM_TF_PM|ALM_TF_Build|ALM_TF_VC|ALM_TF_SDK|ALM_lab|ALM_test|ALM_VFP"}],cookie:[]}},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/en-us/library",c:"inv_c_MSDN-p77596864_NDP.js",f:0.089,p:2,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"NDP_CLR|NDP_CLR_SL|NDP_SL|NDP_WPF_SL|NDP_Other"}],cookie:[]}},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/en-us/library",c:"inv_c_MSDN-p77596864_VS.js",f:0.089,p:2,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"VS_Video|VS_Other|VS_BzApp_SMB|VS_BzApp_Other|VS_Lang_CS|VS_Lang_FS|VS_Lang_VB|VS_Lang_ML|VS_Lang_SL|VS_Lang_Other|VS_NonPro_ML|VS_PCP|VS_VSPE_VSPlat|VS_VSPE_Other|VS_TC_Solver|VS_VSPE_VSSDK|VS_VC|VS_ISVNT_Other|VS_NonPro_VC"}],cookie:[]}},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/en-us/library",c:"inv_c_MSDN-p77596864_Web.js",f:0.089,p:2,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"NDP_ASPNET|NDP_ASPNET_ACT|NDP_ASPNET_MVC|NDP_ASPNET_VWD|NDP_IIS_WinSDK|NDP_IIS|NDP_ASPNET_SL|VS_BzApp_SharePoint|VS_JScript"}],cookie:[]}},{m:"/(sr-msdn|msdnstage|msdntest|msdnlive|msdn\\.microsoft)[\\w\\.-]+/ja-jp/",c:"inv_c_MSDN-p15466742-JA.js",f:0.0022,p:0},{m:"(.*?social\\.msdn\\.microsoft)[\\w\\.-/]+/Forums/en-(us|US)",c:"inv_c_SC-MSDN-p77737117-1207.js",f:0.16,p:0},{m:"(.*?social\\.msdn\\.microsoft)[\\w\\.-/]+/Forums/en-(us|US)",c:"inv_c_SC-MSDN-p77596864-p77737117-Tier1.js",f:0.3,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"netframework"}],cookie:[]}},{m:"(.*?social\\.msdn\\.microsoft)[\\w\\.-/]+/Forums/en-(us|US)",c:"inv_c_SC-MSDN-p77596864-p77737117-Tier2.js",f:0.15,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"ie|vbasic|vcsharp|visualc|vstudio|windows"}],cookie:[]}},{m:"(.*?social\\.msdn\\.microsoft)[\\w\\.-/]+/Forums/en-(us|US)",c:"inv_c_SC-MSDN-p77596864-p77737117-Tier3.js",f:0.3,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"windowsazure|office|sharepoint|sqlserver"}],cookie:[]}},{m:"(.*?social\\.msdn\\.microsoft)[\\w\\.-/]+/forums/en-(us|US)/category/windowsapps",c:"inv_c_SC-MSDN-p77596864-1459.js",f:0.3,p:2},{m:"(.*?social\\.technet\\.microsoft|sr-technet)[\\w\\.-]+/Forums/en/",c:"inv_c_SC-TN-p77737117-1308.js",f:0.13,p:0},{m:"(.*?social\\.technet\\.microsoft|sr-technet)[\\w\\.-]+/Forums/en/",c:"inv_c_SC-TN-p77596864-1308.js",f:0.055,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"scriptcenter|windowsserver"}],cookie:[]}},{m:"(.*?social\\.technet\\.microsoft|sr-technet)[\\w\\.-]+/Forums/en/",c:"inv_c_SC-TN-p77596864-p77737117-1308.js",f:0.016,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"windows"}],cookie:[]}},{m:"(.*?social\\.technet\\.microsoft|sr-technet)[\\w\\.-]+/forums/en-(us|US)",c:"inv_c_SC-TN-p77596864-p77737117-TIER3.js",f:0.3,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"systemcenter"}],cookie:[]}},{m:"(.*?social\\.technet\\.microsoft|sr-technet)[\\w\\.-]+/Forums/en-(us|US)",c:"inv_c_SC-TN-p77737117-1208.js",f:0.13,p:0},{m:"(.*?social\\.technet\\.microsoft|sr-technet)[\\w\\.-]+/forums/en-(us|US)",c:"inv_c_SC-TN-p77596864-p77737117-TIER1.js",f:0.055,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"windowsserver|office|scriptcenter"}],cookie:[]}},{m:"(.*?social\\.technet\\.microsoft|sr-technet)[\\w\\.-]+/forums/en-(us|US)",c:"inv_c_SC-TN-p77596864-p77737117-TIER2.js",f:0.3,p:1,prereqs:{content:[{"element":"meta","attrib":"content","attribValue":"forefront|sharepoint|sqlserver|exchange"}],cookie:[]}},{m:"(.*?social\\.technet\\.microsoft)[\\w\\.-]+/wiki",c:"inv_c_SC-TN-p77737117-1213.js",f:0.13,p:0},{m:"/(sr-technet|tnstage|tnlive|tntest|technet\\.microsoft)[\\w\\.-]+/de-de/",c:"inv_c_TN-p15466742-p81712691-DE.js",f:0.032,p:2},{m:"/(sr-technet|tnstage|tnlive|tntest|technet\\.microsoft)[\\w\\.-]+/en-us/",c:"inv_c_TN-p77737117_74-TIER4.js",f:0.13,p:0},{m:"/(sr-technet|tnstage|tnlive|tntest|technet\\.microsoft)[\\w\\.-]+/en-us/((default\\.aspx)|(windows/)|(library|windowsserver|forefront|office|sharepoint|sqlserver|systemcenter|scriptcenter|security|sysinternals|virtualization|subscriptions|magazine|ie|exchange|edge))",c:"inv_c_TechNet-p77596864.js",f:_Freq,p:1,halt:_halt},{m:"/(sr-technet|tnstage|tnlive|tntest|technet\\.microsoft)[\\w\\.-]+/ja-jp",c:"inv_c_TECHNET-p15466742-p81712691-JA.js",f:0.00192,p:1},{m:"(visualstudiogallery\\.msdn\\.microsoft)[\\w\\.-]+/",c:"inv_c_GA-MSDN-p77737117-1211.js",f:0.16,p:0}]};
COMSCORE.SiteRecruit.Broker.run();