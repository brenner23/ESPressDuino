#pragma once
#include <pgmspace.h>

// ESPressDuino-Dashboard (Stil wie ESPressMiner32). Holt alle 5 s /api/stats.
static const char ED_WEB_PAGE[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en"><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESPressDuino</title>
<script>try{if((localStorage.getItem('cmTheme')||'dark')==='light')document.documentElement.classList.add('light')}catch(e){}</script>
<style>
:root{--bg:#161821;--card:#222530;--line:#333644;--txt:#e8eaf0;--dim:#9aa0b0;
--accent:#f0a020;--accent2:#d48a10;--green:#3fb950;--red:#e5534b;--orange:#f0883e;
--modal:#232733;--field:#191c26;--ibd:#3a3f50;--shadow:rgba(0,0,0,.5)}
:root.light{--bg:#eef1f6;--card:#ffffff;--line:#e2e5ee;--txt:#1a1d26;--dim:#5c6472;
--modal:#ffffff;--field:#f3f5fa;--ibd:#cdd3e0;--shadow:rgba(30,40,80,.15)}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--txt);
font:15px/1.45 system-ui,Segoe UI,Roboto,sans-serif;padding:18px;transition:background .2s,color .2s}
.top{max-width:1200px;margin:0 auto 20px;display:flex;align-items:center;justify-content:center;position:relative}
h1{font-weight:600;text-align:center;margin:6px 0;font-size:26px;letter-spacing:.02em}
.gear{position:absolute;right:0;top:50%;transform:translateY(-50%);background:var(--card);color:var(--txt);
border:1px solid var(--line);border-radius:9px;padding:8px 13px;font-size:14px;cursor:pointer;transition:.15s}
.gear:hover{border-color:var(--accent);color:var(--accent)}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:16px;max-width:1200px;margin:0 auto}
.card{background:var(--card);border-radius:12px;padding:18px;text-align:center;border-top:4px solid var(--accent);
box-shadow:0 1px 3px var(--shadow)}
.lbl{color:var(--dim);letter-spacing:.09em;font-size:11.5px;text-transform:uppercase}
.val{font-size:30px;font-weight:700;margin:7px 0 3px;font-variant-numeric:tabular-nums;word-break:break-all;overflow:hidden}
.sub{color:var(--dim);font-size:12px}
.wide{max-width:1200px;margin:16px auto 0;background:var(--card);border-radius:12px;padding:16px;box-shadow:0 1px 3px var(--shadow)}
canvas{width:100%;height:160px;display:block}
table{width:100%;border-collapse:collapse}
td,th{padding:8px 4px;border-bottom:1px solid var(--line)}
th{color:var(--dim);font-weight:500;font-size:12px;text-align:left}
tr:last-child td{border-bottom:0}
#info td:first-child{color:var(--dim)}
#info td:last-child{text-align:right;font-variant-numeric:tabular-nums;word-break:break-all}
#rigs td{font-variant-numeric:tabular-nums;font-size:13.5px}
.dot{display:inline-block;width:9px;height:9px;border-radius:50%;margin-right:7px;background:var(--red)}
.dot.on{background:var(--green)}
.swbtn{background:var(--card);color:var(--accent);border:1px solid var(--line);border-radius:7px;
padding:3px 9px;font-size:12px;font-weight:600;cursor:pointer;margin-left:8px;transition:.12s}
.swbtn:hover{border-color:var(--accent)}.swbtn:disabled{opacity:.6;cursor:default}
.ov{position:fixed;inset:0;background:rgba(10,12,20,.55);backdrop-filter:blur(2px);display:none;
align-items:flex-start;justify-content:center;overflow:auto;padding:26px 16px;z-index:9}
.ov.open{display:flex}
.modal{background:var(--modal);border-radius:16px;width:100%;max-width:520px;padding:0;overflow:hidden;
box-shadow:0 20px 60px var(--shadow);animation:pop .18s ease-out}
@keyframes pop{from{transform:translateY(10px);opacity:0}to{transform:none;opacity:1}}
.mhead{display:flex;align-items:center;justify-content:space-between;gap:12px;padding:18px 22px;border-bottom:1px solid var(--line)}
.mhead h2{margin:0;font-weight:600;font-size:19px}
.mbody{padding:18px 22px;max-height:70vh;overflow:auto}
.seg{display:inline-flex;background:var(--field);border:1px solid var(--ibd);border-radius:9px;padding:3px}
.seg button{border:0;background:none;color:var(--dim);border-radius:7px;padding:6px 12px;font-size:13px;cursor:pointer;font-weight:600}
.seg button.act{background:var(--accent);color:#fff}
.intro{color:var(--dim);font-size:13.5px;margin:0 0 14px;line-height:1.5}
.sec{border:1px solid var(--line);border-radius:12px;padding:6px 14px 14px;margin:0 0 16px}
.sec-h{color:var(--accent);font-weight:700;font-size:13px;letter-spacing:.04em;text-transform:uppercase;padding:12px 2px 4px}
.fld{margin:10px 0}
.fld label{display:block;color:var(--dim);font-size:12.5px;margin:0 0 5px;font-weight:500}
.fld input{width:100%;background:var(--field);color:var(--txt);border:1px solid var(--ibd);border-radius:8px;
padding:10px 12px;font-size:14px;transition:.12s}
.fld input:focus{outline:none;border-color:var(--accent);box-shadow:0 0 0 3px rgba(240,160,32,.2)}
.scanrow{display:flex;gap:8px}.scanrow input{flex:1}
.scanbtn{background:var(--field);color:var(--accent);border:1px solid var(--ibd);border-radius:8px;
padding:0 14px;cursor:pointer;font-size:13px;font-weight:600;white-space:nowrap;transition:.12s}
.scanbtn:hover{border-color:var(--accent)}.scanbtn:disabled{opacity:.6;cursor:default}
.ssidsel{width:100%;margin-top:8px;background:var(--field);color:var(--txt);border:1px solid var(--ibd);
border-radius:8px;padding:9px 10px;font-size:14px}
.mfoot{display:flex;gap:10px;justify-content:flex-end;align-items:center;padding:16px 22px;border-top:1px solid var(--line)}
.btn{border:0;border-radius:9px;padding:10px 18px;font-size:14px;font-weight:600;cursor:pointer;transition:.12s}
.btn.save{background:var(--accent);color:#fff}.btn.save:hover{background:var(--accent2)}
.btn.save:disabled{opacity:.6;cursor:default}
.btn.cancel{background:var(--field);color:var(--txt);border:1px solid var(--ibd)}
.msg{flex:1;font-size:13.5px;min-height:1.2em}.msg.err{color:var(--red)}.msg.ok{color:var(--green)}
.rng{display:flex;align-items:center;gap:10px}.rng input{flex:1;accent-color:var(--accent)}
.rng b{min-width:44px;text-align:right;font-variant-numeric:tabular-nums}
.fld label.tog{display:flex;align-items:center;justify-content:space-between;color:var(--txt);font-size:14px;cursor:pointer;margin:0}
.fld .tog input{position:absolute;opacity:0;width:0;height:0}
.sw2{position:relative;width:44px;height:24px;border-radius:12px;background:var(--ibd);transition:.15s;flex:none}
.sw2::after{content:"";position:absolute;top:3px;left:3px;width:18px;height:18px;border-radius:50%;background:#fff;transition:.15s}
.tog input:checked+.sw2{background:var(--accent)}.tog input:checked+.sw2::after{left:23px}
.dstat{color:var(--dim);font-size:12.5px;margin-top:8px}
.wbal{font-size:30px;font-weight:700;color:var(--green);margin:6px 0 2px;overflow:hidden;font-variant-numeric:tabular-nums}
.mini3{display:grid;grid-template-columns:repeat(3,1fr);gap:10px;margin:14px 0 8px}
.mini3 div{border:1px solid var(--line);border-radius:10px;padding:10px;text-align:center}
.mini3 b{display:block;font-size:19px;font-variant-numeric:tabular-nums}.mini3 span{color:var(--dim);font-size:11px;text-transform:uppercase;letter-spacing:.08em}
.wrow{display:flex;gap:10px;align-items:flex-end;margin-top:12px;flex-wrap:wrap}
.foot{max-width:1200px;margin:22px auto 4px;text-align:center;color:var(--dim);font-size:13px}.foot a{color:var(--dim)}.foot a:hover{color:var(--accent)}
.xbtn{background:none;border:0;color:var(--dim);font-size:26px;line-height:1;cursor:pointer;padding:0 2px}.xbtn:hover{color:var(--accent)}
</style></head><body>
<div class="top">
 <h1 id="mn">ESPressDuino</h1>
 <button class="gear" onclick="openCfg()">&#9881; Settings</button>
</div>
<div class="grid">
 <div class="card"><div class="lbl">Hashrate</div><div class="val" id="hr">-</div><div class="sub">kH/s &middot; &Oslash; <span id="avg">-</span></div></div>
 <div class="card"><div class="lbl">Shares (R/A)</div><div class="val" id="sh">-</div><div class="sub" id="shsub">-</div></div>
 <div class="card"><div class="lbl">Balance</div><div class="val" id="bal">-</div><div class="sub" id="balsub">DUCO</div></div>
 <div class="card"><div class="lbl">Uptime</div><div class="val" id="up">-</div><div class="sub" id="jobs">-</div></div>
</div>
<div class="wide"><div class="lbl">Hashrate history (10 min)</div><canvas id="cv"></canvas></div>
<div class="wide"><table id="info">
 <tr><td>Pool (<span id="pname">-</span>)</td><td><span class="dot" id="pd"></span><span id="user">-</span> <button id="swbtn" class="swbtn" style="display:none" onclick="switchPool()">&#8646; Switch</button></td></tr>
 <tr><td>Mining node</td><td id="node">-</td></tr>
 <tr><td>Difficulty</td><td id="diff">-</td></tr>
 <tr><td>Wi-Fi</td><td id="wifi">-</td></tr>
 <tr><td>Chip temperature (rough)</td><td id="ctemp">-</td></tr>
 <tr><td>Free memory</td><td id="heap">-</td></tr>
 <tr><td>Firmware</td><td id="ver">-</td></tr>
 <tr><td>Project</td><td><a href="https://github.com/brenner23/ESPressDuino" target="_blank" rel="noopener" style="color:inherit">GitHub</a></td></tr>
</table></div>
<div class="wide" id="walw">
 <div class="lbl" style="display:flex;justify-content:space-between;gap:10px">Wallet <span id="wuser" style="text-transform:none;letter-spacing:0;flex:1"></span><a id="wlink" href="#" target="_blank" rel="noopener" style="color:var(--accent);text-transform:none;letter-spacing:0;font-weight:600;display:none">Open public wallet &#8599;</a></div>
 <div class="wbal" id="wbal">-</div><div id="wusd" style="font-size:16px;color:var(--txt)">-</div><div class="sub" id="wrate" style="margin-top:2px">-</div>
 <div class="mini3"><div><b id="wdev">-</b><span>Devices</span></div><div><b id="wthr">-</b><span>Threads</span></div><div><b id="wtot">-</b><span>Total</span></div></div>
 <div class="sub" id="wsum" style="margin:4px 0 8px">-</div>
 <div style="overflow-x:auto"><table id="rigs"><thead><tr><th>Miner</th><th>IP</th><th>Hashrate</th><th>Diff</th><th>A / R</th><th>Sharetime</th><th>Node</th></tr></thead><tbody id="rigb"></tbody></table></div>
 <div class="wrow"><div class="fld" style="flex:1;margin:0"><label for="wint">Wallet refresh interval (seconds, 60&ndash;86400)</label><input id="wint" type="number" min="60" max="86400"></div>
  <button class="btn cancel" onclick="wSave()">Save</button><button class="btn save" id="wref" onclick="wRefresh()">Refresh now</button></div>
 <div class="sub" id="wmsg" style="margin-top:6px"></div>
</div>

<div class="foot"><a href="https://github.com/brenner23/ESPressDuino" target="_blank" rel="noopener">&#9749; ESPressDuino on GitHub</a> &middot; <a href="https://github.com/brenner23/ESPressMiner32" target="_blank" rel="noopener">ESPressMiner32</a> &middot; <a href="https://duinocoin.com" target="_blank" rel="noopener">Duino-Coin</a></div>

<div class="ov" id="ov" onclick="if(event.target===this)closeCfg()"><div class="modal">
 <div class="mhead">
  <h2 id="ctitle">Settings</h2>
  <div style="display:flex;align-items:center;gap:10px"><div class="seg"><button type="button" id="thDark" onclick="setTheme('dark')">Dark</button><button type="button" id="thLight" onclick="setTheme('light')">Light</button></div><button type="button" class="xbtn" id="cx" onclick="closeCfg()" title="Close">&times;</button></div>
 </div>
 <div class="mbody">
  <p class="intro" id="cintro" style="display:none">Welcome! Enter a name, your Wi-Fi and your Duino-Coin account. The miner then restarts and connects to your Wi-Fi.</p>
  <div class="sec">
   <div class="sec-h">General</div>
   <div class="fld"><label for="c_rig">Miner name (shown in the Duino wallet)</label><input id="c_rig" maxlength="24" placeholder="ESPressDuino"></div>
   <div class="fld"><label for="c_host">Network name (hostname)</label><input id="c_host" maxlength="31"></div>
   <div class="fld"><label for="c_ssid">Wi-Fi name (SSID)</label>
    <div class="scanrow"><input id="c_ssid" maxlength="32"><button type="button" class="scanbtn" id="scanbtn" onclick="scanWifi()">&#128246; Scan</button></div>
    <select id="ssidsel" class="ssidsel" style="display:none" onchange="if(this.value)$('c_ssid').value=this.value"></select>
   </div>
   <div class="fld"><label for="c_wpass">Wi-Fi password</label><input id="c_wpass" type="password" maxlength="64"></div>
  </div>
  <div class="sec">
   <div class="sec-h">Pool 1</div>
   <div class="fld"><label for="p0user">Duino-Coin username</label><input id="p0user" maxlength="40" autocapitalize="off"></div>
   <div class="fld"><label for="p0key">Mining key (if set in the wallet)</label><input id="p0key" type="password" maxlength="40"></div>
  </div>
  <div class="sec">
   <div class="sec-h">Pool 2 (optional)</div>
   <div class="fld"><label for="p1user">Duino-Coin username</label><input id="p1user" maxlength="40" autocapitalize="off" placeholder="empty = no second pool"></div>
   <div class="fld"><label for="p1key">Mining key (if set in the wallet)</label><input id="p1key" type="password" maxlength="40"></div>
  </div>
  <div class="sec">
   <div class="sec-h">Pool switching</div>
   <div class="fld"><label for="c_altmin">Alternate automatically every N minutes &nbsp;(0 = off, switch by button only)</label><input id="c_altmin" type="number" min="0" max="1440" value="0"></div>
  </div>
  <div class="sec" id="dsec" style="display:none">
   <div class="sec-h">Display <span id="dboard" style="color:var(--dim);font-weight:500;text-transform:none"></span></div>
   <p class="dstat" style="margin:2px 0 6px">Changes here apply immediately (no restart).</p>
   <div class="fld" id="d_rotf" style="display:none"><label>Rotation</label><div class="seg" id="d_rot"></div></div>
   <div class="fld"><label for="d_day">Brightness</label>
    <div class="rng"><input id="d_day" type="range" min="1" max="100"><b id="d_dayv"></b></div></div>
   <div class="fld" id="d_ldrf" style="display:none"><label class="tog">Auto light (light sensor)<input type="checkbox" id="d_ldr" onchange="dSave({autoLight:this.checked})"><span class="sw2"></span></label></div>
   <div class="dstat" id="dstat"></div>
  </div>
 </div>
 <div class="mfoot" id="cfoot">
  <span class="msg" id="cmsg"></span>
  <button class="btn cancel" id="ccancel" onclick="openCfg()">Undo</button>
  <button class="btn save" id="csave" onclick="saveCfg()">Save &amp; restart</button>
 </div>
</div></div>

<script>
const $=id=>document.getElementById(id);
let portal=false,lastHist=[],keySet=[false,false],cfgSnap='',fromSecrets=false;
const CFG_IDS=['c_rig','c_host','c_ssid','c_wpass','p0user','p0key','p1user','p1key','c_altmin'];
function cfgNow(){return JSON.stringify(CFG_IDS.map(i=>$(i).value))}
function cfgBtn(){const ch=portal||cfgNow()!==cfgSnap;$('csave').textContent=ch?'Save & restart':'Restart';$('ccancel').style.display=ch&&!portal?'':'none';$('csave').disabled=fromSecrets&&ch}
CFG_IDS.forEach(i=>$(i).addEventListener('input',cfgBtn));
function setTheme(t){document.documentElement.classList.toggle('light',t==='light');
 try{localStorage.setItem('cmTheme',t)}catch(e){}
 $('thDark').className=t==='light'?'':'act';$('thLight').className=t==='light'?'act':'';if(lastHist.length)draw(lastHist)}
(function(){let t='dark';try{t=localStorage.getItem('cmTheme')||'dark'}catch(e){}setTheme(t)})();
async function openCfg(){const m=$('cmsg');m.textContent='';m.className='msg';
 $('cx').style.display=portal?'none':'';$('cintro').style.display=portal?'':'none';
 $('ctitle').textContent=portal?'Set up ESPressDuino':'Settings';
 try{const c=await(await fetch('/api/config')).json();
 $('c_rig').value=c.rigName||'';$('c_host').value=c.hostname||'';$('c_ssid').value=c.wifiSsid||'';$('c_wpass').value='';
 $('c_wpass').placeholder=c.wifiPassSet?'•••••• (unchanged)':'';
 for(let i=0;i<2;i++){const p=c.pools[i]||{};$('p'+i+'user').value=p.user||'';$('p'+i+'key').value='';
  keySet[i]=!!p.keySet;$('p'+i+'key').placeholder=p.keySet?'•••••• (unchanged)':''}
 $('c_altmin').value=c.poolAltMin||0;
 if(c.fromSecrets){$('cintro').style.display='';$('cintro').textContent='These settings come from secrets.ini (use_secrets = 1) and can only be changed there. Display settings below work as usual.';}else{$('cintro').textContent='Welcome! Enter a name, your Wi-Fi and your Duino-Coin account. The miner then restarts and connects to your Wi-Fi.'}
 fromSecrets=!!c.fromSecrets;cfgSnap=cfgNow();cfgBtn();dLoad();$('ov').classList.add('open')}catch(e){alert('Could not load settings')}}
function closeCfg(){if(!portal)$('ov').classList.remove('open')}
let dPrevBusy=false,dPrevNext=null,dPrevT=0;
function dPrev(v){if(dPrevBusy){dPrevNext=v;return}dPrevBusy=true;
 fetch('/api/display/preview?value='+v).catch(()=>{}).finally(()=>{dPrevBusy=false;if(dPrevNext!==null){const n=dPrevNext;dPrevNext=null;dPrev(n)}})}
async function dSave(o){const st=$('dstat');
 try{const r=await fetch('/api/display',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(o)});
 const t=await r.text();if(!r.ok){st.textContent=t;st.style.color='var(--red)';return}st.style.color='';dLoad()}catch(e){st.textContent='Saving failed'}}
{const el=$('d_day'),v=$('d_dayv');el.oninput=()=>{v.textContent=el.value+' %';clearTimeout(dPrevT);dPrevT=setTimeout(()=>dPrev(el.value),120)};
 el.onchange=()=>dSave({brightness:parseInt(el.value)})}
async function dLoad(){let d;try{const r=await fetch('/api/display');if(!r.ok)return;d=await r.json()}catch(e){return}
 $('dsec').style.display='';$('dboard').textContent='· '+d.board;$('dstat').textContent=d.status||'';
 $('d_day').value=d.brightness;$('d_dayv').textContent=d.brightness+' %';
 $('d_ldrf').style.display=d.hasLdr?'':'none';$('d_ldr').checked=!!d.autoLight;
 $('d_rotf').style.display=d.hasRotation?'':'none';
 $('d_rot').innerHTML=[0,90,180,270].map(r=>'<button type="button" class="'+(r===d.rotation?'act':'')+'" onclick="dSave({rotation:'+r+'})">'+r+'&deg;</button>').join('')}
async function saveCfg(){const m=$('cmsg'),b=$('csave');
 if(!portal&&cfgNow()===cfgSnap){b.disabled=true;m.className='msg';m.textContent='Restarting (about 20 s) ...';
  try{await fetch('/restart',{method:'POST'})}catch(e){}waitBack();return}  // nichts geaendert: nur Neustart
 const body={rigName:$('c_rig').value,hostname:$('c_host').value,wifiSsid:$('c_ssid').value,wifiPass:$('c_wpass').value,
 poolAltMin:parseInt($('c_altmin').value)||0,
 pools:[0,1].map(i=>{const k=$('p'+i+'key').value;return{user:$('p'+i+'user').value,key:k,keyKeep:!k&&keySet[i]}})};
 b.disabled=true;m.className='msg';m.textContent='Saving ...';
 try{const r=await fetch('/api/config',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(body)});
 const t=await r.text();if(!r.ok){m.className='msg err';m.textContent=t;b.disabled=false;return}
 m.className='msg ok';
 if(portal){m.textContent='Saved! Restarting and connecting to "'+body.wifiSsid+'". Then open: http://'+body.hostname+'.local/';return}
 m.textContent='Saved. Miner is restarting (about 20 s) ...';waitBack()}
 catch(e){m.className='msg err';m.textContent='Saving failed';b.disabled=false}}
function waitBack(){setTimeout(async function p(){try{await fetch('/api/stats',{cache:'no-store'});location.reload()}catch(e){setTimeout(p,2000)}},10000)}
async function switchPool(){const b=$('swbtn');b.disabled=true;const old=b.innerHTML;b.textContent='switching ...';
 try{await(await fetch('/api/switchpool',{method:'POST'})).text()}catch(e){}
 setTimeout(()=>{b.disabled=false;b.innerHTML=old;tick()},1500)}
async function scanWifi(){const b=$('scanbtn'),sel=$('ssidsel');b.disabled=true;const old=b.innerHTML;b.textContent='Scanning ...';
 try{const j=await(await fetch('/api/scan')).json();const ns=j.nets||[];ns.sort((a,c)=>c.rssi-a.rssi);
 sel.innerHTML='<option value="">'+(ns.length?('-- '+ns.length+' networks found --'):'-- no networks found --')+'</option>';
 ns.forEach(n=>{const o=document.createElement('option');o.value=n.ssid;
  const q=n.rssi>-60?'++++':n.rssi>-70?'+++':n.rssi>-80?'++':'+';o.textContent=n.ssid+'   ['+q+']'+(n.lock?' 🔒':'');sel.appendChild(o)});
 sel.style.display=''}catch(e){alert('Wi-Fi scan failed')}
 b.disabled=false;b.innerHTML=old}
async function wSave(){const m=$('wmsg');try{const r=await fetch('/api/wallet?interval='+parseInt($('wint').value),{method:'POST'});m.textContent=await r.text();m.style.color=r.ok?'var(--green)':'var(--red)'}catch(e){m.textContent='Saving failed'}}
async function wRefresh(){const m=$('wmsg'),b=$('wref');b.disabled=true;m.style.color='';m.textContent='Refreshing ...';
 try{const r=await fetch('/api/wallet/refresh',{method:'POST'});m.textContent=r.ok?'Updated':await r.text();m.style.color=r.ok?'var(--green)':'var(--red)';tick()}catch(e){m.textContent='Refresh failed'}b.disabled=false}
// Lange Zahlen: Schrift so weit verkleinern, dass sie in eine Zeile passt (min. 14 px)
function fitText(el,txt,max){el.textContent=txt;el.style.whiteSpace='nowrap';let f=max;el.style.fontSize=f+'px';
 while(el.scrollWidth>el.clientWidth&&f>14){f--;el.style.fontSize=f+'px'}}
function fu(s){const d=Math.floor(s/86400);s%=86400;const p=n=>String(n).padStart(2,'0');
 return(d?d+'d ':'')+p(Math.floor(s/3600))+':'+p(Math.floor(s/60)%60)+':'+p(s%60)}
function esc(t){return String(t).replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]))}
function draw(h){lastHist=h;const c=$('cv'),r=devicePixelRatio||1,w=c.clientWidth,H=c.clientHeight;
 c.width=w*r;c.height=H*r;const x=c.getContext('2d');x.scale(r,r);x.clearRect(0,0,w,H);
 if(h.length<2)return;const mx=Math.max(...h)*1.15||1;
 const cs=getComputedStyle(document.documentElement);
 const grid=cs.getPropertyValue('--line').trim(),lab=cs.getPropertyValue('--dim').trim(),acc=cs.getPropertyValue('--accent').trim();
 x.strokeStyle=grid;x.fillStyle=lab;x.font='11px sans-serif';x.lineWidth=1;
 for(let g=0;g<=4;g++){const y=H-18-(H-26)*g/4;x.beginPath();x.moveTo(40,y);x.lineTo(w,y);x.stroke();x.fillText(Math.round(mx*g/4),2,y+4)}
 x.beginPath();h.forEach((v,i)=>{const px=40+(w-40)*i/(h.length-1),py=H-18-(H-26)*v/mx;i?x.lineTo(px,py):x.moveTo(px,py)});
 x.strokeStyle=acc;x.lineWidth=2;x.stroke();x.lineTo(w,H-18);x.lineTo(40,H-18);x.closePath();x.fillStyle=acc+'22';x.fill()}
let wStamp='';
async function tick(){try{const s=await(await fetch('/api/stats?ws='+wStamp)).json();
 if(s.rigName){$('mn').textContent=s.rigName;document.title=s.rigName}
 $('hr').textContent=s.hashrate.toFixed(1);$('avg').textContent=s.average.toFixed(1);
 const tot=s.accepted+s.rejected;$('sh').textContent=s.rejected+'/'+s.accepted;
 $('shsub').textContent=(tot?(s.accepted*100/tot).toFixed(1):'100.0')+' % accepted';
 fitText($('bal'),s.balance,30);$('balsub').textContent='DUCO'+(s.usd?' · ≈ $'+s.usd:'');
 $('up').textContent=fu(s.uptime);$('jobs').textContent=s.jobs+' jobs';
 $('user').textContent=s.user||'-';$('pd').className='dot'+(s.mining?' on':'');
 $('pname').textContent=s.poolName+(s.poolAltMin?' ⇆ every '+s.poolAltMin+' min':'');
 $('swbtn').style.display=s.hasPool2?'':'none';
 $('node').textContent=s.node;$('diff').textContent=s.difficulty;
 $('wifi').textContent=s.ip+' · '+s.rssi+' dBm'+(s.wifiDrops?' · '+s.wifiDrops+' drops / '+s.wifiReconnects+' reconnects':'');
 $('ctemp').textContent=s.chipTemp?s.chipTemp.toFixed(1)+' °C (max '+s.chipTempMax.toFixed(1)+')':'-';
 $('heap').textContent=(s.heap/1024).toFixed(1)+' KB';$('ver').textContent=s.version;
 $('wuser').textContent=s.user?'· '+s.user:'';$('wlink').style.display=s.user?'':'none';$('wlink').href='https://explorer.duinocoin.com/?search='+encodeURIComponent(s.user||'');fitText($('wbal'),s.balance+' DUCO',30);$('wusd').textContent=s.usd?'≈ $'+s.usd+' USD':'≈ - USD';
 $('wrate').textContent='1 DUCO = $'+(s.usdRate||'-')+(s.usdLive?' · live':' · default')+' · '+String(s.walletAge||'-').slice(0,5);$('wrate').title='Rate from server.duinocoin.com (same as the official wallet), refreshed with the wallet';
 $('wdev').textContent=s.walletDevices;$('wthr').textContent=s.walletThreads;$('wtot').textContent=s.walletHashrate;
 $('wsum').textContent='Total: '+s.walletAccepted+' accepted · '+s.walletRejected+' rejected';
 if(s.rowsHtml!==undefined){$('rigb').innerHTML=s.rowsHtml;wStamp=String(s.walletStamp)}if(document.activeElement!==$('wint'))$('wint').value=s.walletInterval;
 draw(s.history);if(s.portal&&!portal){portal=true;openCfg()}}catch(e){$('pd').className='dot'}}
tick();setInterval(tick,5000);if(location.hash==='#settings')openCfg();addEventListener('resize',()=>{if(lastHist.length)draw(lastHist)});
</script></body></html>)rawliteral";
