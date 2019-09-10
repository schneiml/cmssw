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
ROOTOPTIONS = "&opt=colz"

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
      mes.append(filepath + rootpath + "/" + key)
  return (dirs, mes)


@bottle.route('/')
def index():
  return browse(DATAPATH)

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

  return (
  """
  <html><head><title>Mini-DQMGUI | %s</title></head>
  <body>
  <ul class="nav">
    <li>PD: %s</li>
    <li>Run: %s</li>
    <li>Dataset: %s</li>
  </ul>
  """ % (path, pd, run, processing) +
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
    "\n".join('<li><iframe src="/jsroot/?nobrowser&file=/root/%s&item=me;1%s" width="50%%" height="50%%"></iframe></li>' % (path, ROOTOPTIONS) for path in mes) +
  '</ul>' +
  """
  </body></html>
  """
  )

@bottle.route('/jsroot/')
def jsroot():
  return """
  <!DOCTYPE html>
  <html lang="en">
     <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>Read a ROOT file</title>
        <link rel="shortcut icon" href="img/RootIcon.ico"/>
        <!-- To make use of RequireJS, replace following line with commented one -->
        <script type="text/javascript" src="https://root.cern/js/latest/scripts/JSRootCore.min.js?gui"></script>
        <!--script type="text/javascript" src="//cdnjs.cloudflare.com/ajax/libs/require.js/2.2.0/require.min.js" data-main="scripts/JSRootCore.js"></script-->
     </head>
     <body>
        <div id="simpleGUI" path="../files/" files="ct.root;exclusion.root;fillrandom.root;glbox.root;graph.root;hsimple.root;legends.root;rf107.root;stacks.root;zdemo.root">
           loading scripts ...
        </div>
     </body>
  </html>
  """ 


@bottle.route('/root/<path:path>')
def rootdata(path):
  m = PATHPATTERN.match(path)
  rootfile = m.groups()[0]
  rootpath = m.groups()[4].strip("/")
  print("rootdata: %s, %s" % (rootfile, rootpath))
  f = uproot.open(rootfile)
  obj = f[rootpath]
  with tempfile.NamedTemporaryFile() as t:
    with uproot.recreate(t.name, compression=None) as f:
      f["me"] = obj
    with open(t.name, "rb") as f:
      return f.read()

if __name__ == '__main__':
  bottle.run(port=8086)
