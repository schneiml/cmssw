import re
import json
import time
import traceback
import http.server

from urllib.parse import parse_qs

import ROOT

from DQMServices.DQMGUI.render import RenderPool
from DQMServices.FwkIO.DQMIO import DQMIOReader
from DQMServices.FwkIO.DQMIO import NotPool

reader = DQMIOReader("server.db", progress=False, nconnections=8, pooltype=NotPool)
renderpool = RenderPool(workers=3)


def samples(args):
    args = parse_qs(args)
    if 'match' in args:
        matchre = re.compile(args['match'][0])
        match = lambda ds, run, lumi: len(matchre.findall(ds)) > 0
    else:
        match = lambda ds, run, lumi: True
    if 'run' in args:
        runre = re.compile(args['run'][0])
        runm = lambda ds, run, lumi: len(runre.findall(str(run))) > 0
    else:
        runm = lambda ds, run, lumi: True

    items = []
    structure = {'samples': [
        {'type':'dqmio_data', 'items': items }]}

    for ds, run, lumi in reader.samples():
        if lumi == 0 and match(ds, run, lumi) and runm(ds, run, lumi):
            item = {'type':'dqmio_data', 'run':run, 'dataset':ds, 'version':''}
            items.append(item)
    return structure

def list(run, dataset, folder):
    run = int(run)
    items = reader.listsample(dataset, run, 0, folder)
    contents = [{ "streamerinfo":"" }]
    structure = {"contents": contents}
    for item in items:
        if item[-1] == "/":
            contents.append({'subdir': item[:-1]})
        else:
            contents.append({"obj": item, "properties": {}})
    return structure

def jsroot(run, dataset, fullname, args):
    run = int(run)
    mes = reader.readsampleme(dataset, run, 0, fullname)
    accu = mes.pop().data
    tlist = ROOT.TList()
    for me in mes:
        tlist.Add(me.data)
    accu.Merge(tlist)
    return str(ROOT.TBufferJSON.ConvertToJSON(accu)).encode("utf-8")

def plotpng(run, dataset, fullname, args):
    run = int(run)
    args = parse_qs(args)
    width = int(args['w'][0]) if 'w' in args else 400
    height = int(args['h'][0]) if 'h' in args else 320
    while width < 300 or height < 200:
        width, height = width * 2, height * 2
    if 'w' in args: del args['w']
    if 'h' in args: del args['h']
    spec = "&".join(k + "=" + v[0] for k, v in args.items())
    mes = reader.readsampleme(dataset, run, 0, fullname)
    me = mes.pop()
    print(me)
    if not hasattr(me.data, 'GetName'): # TODO: we also need the real flags.
        with renderpool.renderer() as r:
            return  r.renderscalar(str(me.data), width=width, height=height)
    else:
        accu = me.data
        tlist = ROOT.TList()
        for me in mes:
            tlist.Add(me.data)
        accu.Merge(tlist)
        with renderpool.renderer() as r:
            return  r.renderhisto(me.data, [], name = me.name, spec=spec, width=width, height=height)

def index():
    datasets = sorted(reader.datasets())
    return (['<ul>'] +
        [f'<li><a href="/runsfordataset/{ds}">{ds}</a></li>' for ds in datasets] +
        ['</ul>'])

def listruns(dataset):
    runs = sorted(set(run for ds, run, lumi in reader.samples() if lumi == 0 and ds == dataset))
    return ([f'<h1>{dataset}</h1><ul>'] +
        [f'<li><a href="/showdata/{run}{dataset}/">{run}</a></li>' for run in runs] +
        ['</ul>'])

def showdata(run, dataset, folder):
    run = int(run)
    items = sorted(reader.listsample(dataset, run, 0, folder))
    out = [f'<h1><a href="/runsfordataset/{dataset}">{dataset}</a> {run}</h1>', '<h2>']
    breadcrumbs = folder.split("/")
    for i in range(len(breadcrumbs)-1):
        part = "/".join(breadcrumbs[:i+1]) + "/"
        out.append(f'<a href="/showdata/{run}{dataset}/{part}"> {breadcrumbs[i]} </a>/')
    out.append("</h2><ul>")
    for item in items:
        if item[-1] == '/':
            out.append(f'<li><a href="/showdata/{run}{dataset}/{folder}{item}">{item}</a></li>')
    out.append("</ul>")
    for item in items:
        if item[-1] != '/':
            out.append(f'<a href="/plotfairy/archive/{run}{dataset}/{folder}{item}?w=1000&h=800"><img src="/plotfairy/archive/{run}{dataset}/{folder}{item}" /></a>')
    return out


ROUTES = [
    (re.compile('/$'), index, "html"),
    (re.compile('/runsfordataset/(.*)$'), listruns, "html"),
    (re.compile('/showdata/([0-9]+)(/[^/]+/[^/]+/[^/]+)/(.*)'), showdata, "html"),
    (re.compile('/data/json/samples[?]?(.+)?'), samples, "json"),
    (re.compile('/data/json/archive/([0-9]+)(/[^/]+/[^/]+/[^/]+)/(.*)'), list, "json"),
    (re.compile('/jsrootfairy/archive/([0-9]+)(/[^/]+/[^/]+/[^/]+)/([^?]*)[?]?(.+)?'), jsroot, "application/json"),
    (re.compile('/plotfairy/archive/([0-9]+)(/[^/]+/[^/]+/[^/]+)/([^?]*)[?]?(.+)?'), plotpng, "image/png"),
]

# the server boilerplate.
class Handler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        try:
            res = None
            for pattern, func, type in ROUTES:
                m = pattern.match(self.path)
                if m:
                    res = func(*m.groups())
                    break

            if res:
                self.send_response(200, "Here you go")
                if type == "html":
                    self.send_header("Content-Type", "text/html; charset=utf-8")
                    self.end_headers()
                    self.wfile.write(b"""<html><style>
                        body {
                            font-family: sans;
                        }
                    </style><body>""")
                    self.wfile.write("\n".join(res).encode("utf-8"))
                    self.wfile.write(b"</body></html>")
                elif type == "json":
                    self.send_header("Content-Type", "application/json; charset=utf-8")
                    self.end_headers()
                    self.wfile.write(json.dumps(res).encode("utf-8"))
                else:
                    self.send_header("Content-Type", f"{type}; charset=utf-8")
                    self.end_headers()
                    self.wfile.write(res)
            else:
                self.send_response(400, "Something went wrong")
                self.send_header("Content-Type", "text/plain; charset=utf-8")
                self.end_headers()
                self.wfile.write(b"I don't understand this request.")
        except:
            trace = traceback.format_exc()
            self.send_response(500, "Things went very wrong")
            self.send_header("Content-Type", "text/plain; charset=utf-8")
            self.end_headers()
            self.wfile.write(trace.encode("utf8"))

PORT = 8889
server_address = ('', PORT)
httpd = http.server.ThreadingHTTPServer(server_address, Handler)
print(f"Serving at http://localhost:{PORT}/ ...")
httpd.serve_forever()


