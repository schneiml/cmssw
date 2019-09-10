#!/usr/bin/env python
from __future__ import print_function
import re
import bottle
import uproot
import tempfile

from glob import glob

DATAPATH = "/eos/cms/store/group/comm_dqm/DQMGUI_data/"
# split path into filepath, filename and path inside ROOT file
PATHPATTERN = re.compile("^(([^./]*/)*([^./]*|([^/.]*[.][^/.]*)))(/.*)?$")
NAVPATTERN = re.compile("(.*DQM_V0.*_R)([0-9]+)__(.+)__(.+)__(.*[.]root.*)")
ROOTOPTIONS = "colz"

def listdirectory(filepath):
  print("listdirectory('%s')" % filepath)
  return (glob(filepath + "/*/"), glob(filepath + "/*.root"))

def listrootfile(filepath, rootpath):
  print("listrootfile('%s', '%s')" % (filepath, rootpath))
  f = uproot.open(filepath)
  d = f
  if rootpath != "":
    d = f[rootpath]
  keys = d.keys()
  dirs = []
  mes = []
  rootpath =  "/" + rootpath.strip("/")
  if rootpath == "/": rootpath = ""
  for key in keys:
    if "/" in key:
      continue # uproot lists these, but can't access them (with [] notation at least)
    if isinstance(d[key], uproot.rootio.ROOTDirectory):
      dirs.append(filepath + rootpath + "/" + key)
    else:
      mes.append((filepath + rootpath + "/" + key, key.split(";")[0]))
  return (dirs, mes)


@bottle.route('/')
def index():
  return browse(DATAPATH)

ctr_count = 0
def ctr():
  global ctr_count
  ctr_count += 1
  return ctr_count

@bottle.route('/browse/<path:path>')
def browse(path):
  m = PATHPATTERN.match(path)
  print("browse: %s" % m.groups().__repr__())
  dirs, files, mes = [], [], []
  if m.group(4): # have a filename
    dirs, mes = listrootfile(m.group(1), (m.group(5) or "").strip("/"))
  else:
    dirs, files = listdirectory(m.group(1))

  n = NAVPATTERN.match(path) 
  if n:
    run = int(n.group(2))
    pd = n.group(3)
    processing = n.group(4)
  else:
    run, pd, processing = "", "", ""

  breadcrumbs = []
  cur = ""
  for part in path.split("/"):
    cur = cur + "/" + part
    breadcrumbs.append((cur, part))

  mefile = "/root/%s" % path

  return (
  """
  <html>
    <head><title>Mini-DQMGUI | %s</title>
    <script type="text/javascript" src="https://root.cern/js/latest/scripts/JSRootCore.min.js"></script>
    <script type="text/javascript">
    JSROOT.OpenFile("%s", function(file) {
      document.querySelectorAll(".plot").forEach(function(el, ix) { 
        let name = el.getAttribute("data-name")
        let id = el.getAttribute("id")
        console.log(ix, name)
        file.ReadObject(name, function(obj) {
          JSROOT.draw(id, obj, "%s");
        });
      });
    });
    
    </script>
    </head>
  <body>
  <ul class="nav">
    <li>PD: %s</li>
    <li>Run: %s</li>
    <li>Dataset: %s</li>
  </ul>
  """ % (path, mefile, ROOTOPTIONS, pd, run, processing) +
  "<div>" +
    "/".join('<a href="/browse%s">%s</a>' % (path, tail) for path, tail in breadcrumbs) +
  "</div>" +
  '<ul class="dirs">' +
    "\n".join('<li><a href="/browse/%s">%s</a></li>' % (path, path.rstrip("/").split("/")[-1]) for path in dirs) +
  '</ul>' +
  '<ul class="files">' +
    "\n".join('<li><a href="/browse/%s">%s</a></li>' % (path, path.rstrip("/").split("/")[-1]) for path in files) +
  '</ul>' +
  '<ul class="mes">' +
    "\n".join('<li><div class="plot" id="plot%d" data-name="%s" style="width: 500px; height: 300px;"></div></li>' % (ctr(), name) for path, name in mes) +
  '</ul>' +
  """
  </body></html>
  """
  )

@bottle.route('/root/<path:path>')
def rootdata(path):
  m = PATHPATTERN.match(path)
  rootfile = m.groups()[0]
  rootpath = m.groups()[4].strip("/")
  print("rootdata: %s, %s" % (rootfile, rootpath))
  f = uproot.open(rootfile)
  d = f[rootpath]
  items = []
  if  isinstance(d, uproot.rootio.ROOTDirectory):
    for key in d.keys():
      if not isinstance(d[key], uproot.rootio.ROOTDirectory):
        items.append((d[key], key.split(";")[0]))
  else:
    items.append((d, rootpath.split("/")[-1].split(";")[0])) 

  with tempfile.NamedTemporaryFile() as t:
    with uproot.recreate(t.name, compression=None) as f:
      for value, name in items:
        try:
          f[name] = value
        except:
          pass # this is fine for now, just continue
    with open(t.name, "rb") as f:
      return f.read()

if __name__ == '__main__':
  bottle.run(port=8086)
